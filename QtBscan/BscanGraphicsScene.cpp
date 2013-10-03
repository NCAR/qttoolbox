/*
 * BscanGraphicsScene.cpp
 *
 *  Created on: Aug 20, 2009
 *      Author: burghart
 */
#include <cmath>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include <QAction>
#include <QDateTime>
#include <QGraphicsView>
#include <QPainter>

#include "BscanGraphicsScene.h"
#include "BscanRay.h"
#include "RayGraphicsItem.h"

BscanGraphicsScene::BscanGraphicsScene(QtConfig &config,
        std::string sceneName) :
    QGraphicsScene(),
    _deleteLaterList(),
    _sceneStartTime(0),
    _isPaused(false),
    _config(config),
    _pointingUp(true),
    _sceneName(sceneName) {
    // Get time span, gate limits, and display var from the config (or use
    // default values)
    int minGate = _config.getInt(_sceneName + "/MinGate", 0);
    int maxGate = _config.getInt(_sceneName + "/MaxGate", 0);
    setGateLimits(minGate, maxGate);

    int timeSpan = _config.getInt(_sceneName + "/TimeSpan", 30);
    setTimeSpan(timeSpan);

    _displayVar = _config.getString(_sceneName + "/VarName", "").c_str();
    _displayVarUnits = "";
    
    // Get pointing direction from config, or default to pointing up.
    bool pointingUp = _config.getBool(_sceneName + "/PointingUp", true);
    setPointingUp(pointingUp);
    
    // Set min and max values to be displayed. Use values stored for
    // _displayVar from the config, or default values.
    float minVal = config.getFloat(_displayVar.toStdString() + "/minValue", -1);
    float maxVal = config.getFloat(_displayVar.toStdString() + "/maxValue", 1);
    // Finally, set up our color table
    std::string ctName =
            _config.getString(_sceneName + "/ColorTable", "eldoraDbz.ct");
    _colorTable = new ColorTable(minVal, maxVal, ctName.c_str());
    // Set up our connections
    initConnections_();
}

BscanGraphicsScene::~BscanGraphicsScene() {
    for (BscanRayMap_t::iterator it = _bscanRayMap.begin(); 
            it != _bscanRayMap.end(); it++) {
        BscanRay *ray = it->second;
        delete(ray);
    }
    _bscanRayMap.clear();
    // Clear up items we were holding for later deletion
    for (unsigned int i = 0; i < _deleteLaterList.size(); i++) {
        delete(_deleteLaterList[i]);
    }
    _deleteLaterList.clear();
}

void
BscanGraphicsScene::initConnections_() {
    // Resend changed() signals from our color table as local
    // colorTableChanged() signals
    connect(_colorTable, SIGNAL(changed()), this, SIGNAL(colorTableChanged()));
}

// set our scene rect to reflect current gate and time limits
void 
BscanGraphicsScene::updateSceneRect_() {
    qreal left = 0;
    qreal width = _timeSpan;
    qreal top = _maxGate + 1;
    qreal height = -int(_maxGate - _minGate + 1);  // height is negative!
    if (! _pointingUp) {
        top = top + height;
        height = -height;
    }
    setSceneRect(left, top, width, height);
}

/**
 * Return the number of gates in the last ray, or 1 if we have no
 * rays yet.
 */
unsigned int
BscanGraphicsScene::nGates() const {
    if (_bscanRayMap.empty())
        return 1;
    
    BscanRayMap_t::const_reverse_iterator rit = _bscanRayMap.rbegin();
    if (rit != _bscanRayMap.rend())
        return rit->second->nGates();
    else
        return 1;
}

unsigned int
BscanGraphicsScene::timeSpan() const {
    return _timeSpan;
}

void
BscanGraphicsScene::addRay(const BscanRay & ray) {
    double time = 1.0e-6 * ray.time();
    unsigned int oldNGates = nGates();	// gate count from our previous ray

    // Add this ray to our map of BscanRay-s
    _bscanRayMap[time] = new BscanRay(ray);
    
    // If we have no start time, use the time of this ray
    if (startTime() == 0) {
        setStartTime_(time);
    }

    // If we have no _displayVar, use the first product from this ray
    if (_displayVar.length() == 0) {
        QString varName(ray.productName(0).c_str());
        setDisplayVar(varName);
    }
    
    // Potentially adjust our scene start time if we're not paused and the
    // new ray would be off the edge of the scene.
    if (! _isPaused && (time > endTime())) {
        // Shift the scene so the new start is (<raytime> - 0.9 * _timeSpan)
        setStartTime_(time - 0.9 * _timeSpan);
    }
    
    // Create a RayGraphicsItem from the BscanRay and add it to our scene
    RayGraphicsItem * newItem = new RayGraphicsItem(ray, _displayVar, *_colorTable);
    addItem(newItem);

    // Our x coordinates run from zero to _timeSpan, so translate the new
    // ray into the correct x location. We do this to keep the x coordinates
    // in the range +/- 2^15, because of limits in Qt's raster paint engine.
    newItem->moveBy(-_sceneStartTime, 0);

    // If the gate count changes w.r.t. the previous ray, change limits
    // to show all gates of the new ray
    if (ray.nGates() != oldNGates) {
      unsigned int nGates = newItem->nGates();
      unsigned int maxGate = std::max(_maxGate, nGates);
      unsigned int minGate = std::min(_minGate, maxGate - 1);
      setGateLimits(minGate, maxGate);
    }
    
    // The units of our latest ray become *our* units
    std::string newUnits = ray.hasProduct(_displayVar.toStdString()) ?
        ray.productUnits(_displayVar.toStdString()) : "";
    if (newUnits != _displayVarUnits.toStdString()) {
        _displayVarUnits = QString(newUnits.c_str());
        emit unitsChanged();
    }
}

void
BscanGraphicsScene::setMinGate(unsigned int minGate) {
    setGateLimits(minGate, _maxGate);
}

void
BscanGraphicsScene::setMaxGate(unsigned int maxGate) {
    setGateLimits(_minGate, maxGate);
}

void
BscanGraphicsScene::setGateLimits(unsigned int minGate, unsigned int maxGate) {
    if (minGate == _minGate && maxGate == _maxGate)
        return;
    
    _config.setInt(_sceneName + "/MinGate", minGate);
    _config.setInt(_sceneName + "/MaxGate", maxGate);
    _minGate = minGate;
    _maxGate = maxGate;

    updateSceneRect_();
    
    emit gateLimitsChanged(_minGate, _maxGate);
}

void
BscanGraphicsScene::setPointingUp(bool pointingUp) {
    if (pointingUp == _pointingUp)
        return;
    
    _config.setBool(_sceneName + "/PointingUp", pointingUp);
    _pointingUp = pointingUp;
    
    updateSceneRect_();
    emit pointingUpChanged(_pointingUp);
}

void
BscanGraphicsScene::setTimeSpan(unsigned int newSpan) {
    if (newSpan == _timeSpan)
        return;
    
    _config.setInt(_sceneName + "/TimeSpan", newSpan);
    // If our time span is shrinking, set the end time of the scene to
    // the last ray time (if we have rays) or leave it unchanged
    double newStartTime = _sceneStartTime;
    if (newSpan < _timeSpan) {
        double newEndTime = (_bscanRayMap.size() > 0) ? 
                _bscanRayMap.rbegin()->first : endTime();
        newStartTime = newEndTime - newSpan;
    }
    
    setTimeLimits(newStartTime, newSpan);
}

void
BscanGraphicsScene::setStartTime_(double newStart) {
    setTimeLimits(newStart, _timeSpan);
}

void
BscanGraphicsScene::setTimeLimits(double newStartTime, 
        unsigned int newTimeSpan) {
    bool startChanged = (_sceneStartTime != newStartTime);
    bool spanChanged = (_timeSpan != newTimeSpan);
    if (! startChanged && ! spanChanged)
        return;

    _sceneStartTime = newStartTime;
    _timeSpan = newTimeSpan;
    updateSceneRect_();
    
    // If our start time changes...
    if (startChanged) {
        // Clean out rays before our new start time
        BscanRayMap_t::iterator startIt = _bscanRayMap.begin();
        BscanRayMap_t::iterator endIt = _bscanRayMap.lower_bound(_sceneStartTime);
        // First delete all of the dynamically allocated BscanRay-s before the
        // start time. After that's done, we can erase the dangling pointers 
        // from _bscanRayMap.
        for (BscanRayMap_t::iterator it = startIt; it != endIt; it++) {
            delete(it->second);
        }
        _bscanRayMap.erase(startIt, endIt);

        // Remove all of our old RayGraphicsItem-s and regenerate new ones
        // based on our current set of BscanRay-s
        updateRayGraphicsItems_();
    }

    emit timeLimitsChanged(_sceneStartTime, _timeSpan);
}

void 
BscanGraphicsScene::setDisplayLimits(double minValue, double maxValue) {
    _colorTable->setValueLimits(minValue, maxValue);
    // save user's display limits for current displayed variable
    std::string minKey = _displayVar.toStdString() + "/minValue";
    std::string maxKey = _displayVar.toStdString() + "/maxValue";
    _config.setFloat(minKey, minValue);
    _config.setFloat(maxKey, maxValue);
    
    updateRayGraphicsItems_();
}

void
BscanGraphicsScene::pause() {
    setPaused(true);
}

void
BscanGraphicsScene::unpause() {
    setPaused(false);
}

void
BscanGraphicsScene::setPaused(bool state) {
    if (state != _isPaused) {
        _isPaused = state;
        emit pauseStateChanged(_isPaused);
    }
}

/*
 * Draw semi-transparent labels and grid in the foreground of the scene.
 */
void
BscanGraphicsScene::drawForeground(QPainter *painter, const QRectF &rect) {
    // Push the painter's current state on to its stack.
    painter->save();

    // Use an identity matrix so we paint in device coordinates, 
    // since drawText() has strange font stretching results when we work 
    // in scene coordinates.
    QMatrix worldToDev = painter->worldMatrix();
    QMatrix devToWorld = worldToDev.inverted();
    painter->setWorldMatrix(QMatrix());
    int devHeight = painter->device()->height();
    int devWidth = painter->device()->width();

    // Exposed rectangle in dev coordinates
    QRectF pixExposedRect = worldToDev.mapRect(rect);

    // Go with 8-point text
    QFont font = painter->font();
    font.setPointSize(8);
    painter->setFont(font);

    // Default pen is semi-transparent black
    painter->setPen(QPen(QColor(0, 0, 0, 160)));
    
    // Fat pen in semi-transparent white
    QPen wideWhitePen(QColor(255, 255, 255, 128));
    wideWhitePen.setWidth(3);

    // Find the y-axis label step value which will give us a good number of 
    // labels, without making things too dense
    const int goodYSteps[] = { 1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000 };
    int labelStep = 100000000;  // default to *huge*
    int maxLabels = devHeight / 75 + 1;  // allow at least 75 pixels per label
    for (unsigned int i = 0; i < (sizeof(goodYSteps) / sizeof(*goodYSteps)); i++) {
        int nlabels = int(fabsf(sceneRect().height()) / goodYSteps[i]) + 1;
        if (nlabels <= maxLabels) {
            labelStep = goodYSteps[i];
            break;
        }
    }
    
    // Find the low and high label values which work with our y-axis step
    int lowLabel = labelStep * int(ceil(float(_minGate) / labelStep));
    int highLabel = labelStep * int(floor(float(_maxGate) / labelStep));
    
    // Draw the y-axis labels and isolines
    for (int i = lowLabel; i <= highLabel; i += labelStep) {
        // Draw the text label, with a surrounding filled box.
        int alignmentFlags = Qt::AlignLeft | Qt::AlignVCenter;
        QString label = QString::number(i);
        QPointF leftPoint = worldToDev.map(QPointF(sceneRect().left(), i));
        QRectF pixTextRect = painter->boundingRect(QRectF(leftPoint, QSizeF(0,0)), 
                alignmentFlags, label);
        // Draw the label if it's in the exposed region
        if (pixExposedRect.intersects(pixTextRect)) {
            // Draw a semi-transparent box big enough for the text, then
            // draw the text.
            painter->fillRect(pixTextRect, QColor(255, 255, 255, 160));
            painter->drawText(pixTextRect, alignmentFlags, label);
        }
        // Now draw a line from the right side of the text box to the right side
        // of the view. Don't bother checking for exposure.
        QRectF sceneTextRect = devToWorld.mapRect(pixTextRect);
        // ...first a fat line in semi-transparent white
        painter->save();
        painter->setPen(wideWhitePen);
        painter->drawLine(worldToDev.map(QPointF(sceneTextRect.right(), i)), 
                worldToDev.map(QPointF(sceneRect().right(), i)));
        painter->restore();
        // ...then a thin line in semi-transparent black
        painter->drawLine(worldToDev.map(QPointF(sceneTextRect.right(), i)), 
                worldToDev.map(QPointF(sceneRect().right(), i)));
    }
    
    // Find the x-axis label step value which will give us a good number of 
    // labels, without making things too dense
    const int goodXSteps[] = { 1, 2, 5, 10, 15, 30, 60, 120, 300, 600, 900, 1800 };
    labelStep = 100000000;  // default to *huge*
    maxLabels = devWidth / 100 + 1;  // allow at least 100 pixels per label
    for (unsigned int i = 0; i < (sizeof(goodXSteps) / sizeof(*goodXSteps)); i++) {
        int nlabels = int(fabsf(sceneRect().width()) / goodXSteps[i]) + 1;
        if (nlabels <= maxLabels) {
            labelStep = goodXSteps[i];
            break;
        }
    }
    
    // Find the low and high label values which work with our x-axis step
    uint lowXLabel = labelStep * uint(ceil(startTime() / labelStep));
    uint highXLabel = labelStep * uint(floor(endTime() / labelStep));
    
    // Draw the x-axis (time) labels and isolines
    for (uint i = lowXLabel; i <= highXLabel; i += labelStep) {
        double sceneX = i - _sceneStartTime;
        
        // Build a test rectangle centered at this x location and 20 pixels
        // wide.  If it doesn't intersect the exposed rectangle, move on.
        QRectF pixTestRect = 
            worldToDev.mapRect(QRectF(sceneX, sceneRect().top(), 
                                      0, sceneRect().height()));
        pixTestRect.adjust(-20, 0, 20, 0);
        if (! pixExposedRect.intersects(pixTestRect))
            continue;
        
        // Draw the text label, with a surrounding filled box.
        int alignmentFlags = Qt::AlignBottom | Qt::AlignHCenter;
        QString label = QDateTime::fromTime_t(i).toUTC().toString("yyyy-MM-dd\nhh:mm:ss");
        QPointF labelLoc = worldToDev.map(QPointF(sceneX, sceneRect().bottom()));
        QRectF pixTextRect = painter->boundingRect(QRectF(labelLoc, QSizeF(0, 0)), 
                alignmentFlags, label);
        if (pixExposedRect.intersects(pixTextRect)) {
            // Draw a semi-transparent box big enough for the text, then
            // draw the text.
            painter->fillRect(pixTextRect, QColor(255, 255, 255, 160));
            painter->drawText(pixTextRect, alignmentFlags, label);
        }
        // Now draw the isoline from the top of the text box to the top
        // of the view.
        QRectF sceneTextRect = devToWorld.mapRect(pixTextRect);
        // ...first a fat line in semi-transparent white
        painter->save();
        painter->setPen(wideWhitePen);
        qreal sceneTextTop = (sceneRect().height() < 0) ? 
                sceneTextRect.bottom() : sceneTextRect.top();
        painter->drawLine(worldToDev.map(QPointF(sceneX, sceneTextTop)), 
                worldToDev.map(QPointF(sceneX, sceneRect().top())));
        painter->restore();
        // ...then a thin line in semi-transparent black
        painter->drawLine(worldToDev.map(QPointF(sceneX, sceneTextTop)), 
                worldToDev.map(QPointF(sceneX, sceneRect().top())));
    }

    // Restore the painter to its original state
    painter->restore();
}

QStringList
BscanGraphicsScene::varNames() const {
    // Return the list of vars from the latest ray we have
    BscanRayMap_t::const_reverse_iterator rit = _bscanRayMap.rbegin();
    if (rit == _bscanRayMap.rend())
        return QStringList();
    else {
        BscanRay * ray = rit->second;
        QStringList list;
        for (unsigned int i = 0; i < ray->nProducts(); i++) {
            list.append(QString(ray->productName(i).c_str()));
        }
        return list;
    }
}

void
BscanGraphicsScene::setDisplayVar(QString varName) {
    if (_displayVar == varName)
        return;
    
    _displayVar = varName;

    std::string minKey = _displayVar.toStdString() + "/minValue";
    std::string maxKey = _displayVar.toStdString() + "/maxValue";
    double minValue = _config.getFloat(minKey, 0.0);
    double maxValue = _config.getFloat(maxKey, 100.0);
    _colorTable->setValueLimits(minValue, maxValue);
    
    // Save our new display var in the config
    _config.setString(_sceneName + "/VarName", varName.toStdString());

    // Now that _displayVar and _colorTable have been updated, recreate all
    // the RayGraphicsItems.
    updateRayGraphicsItems_();
    
    emit displayVarChanged(_displayVar);
}

void
BscanGraphicsScene::setColorTable() {
    QAction *action = qobject_cast<QAction*>(sender());
    if (action) {
    	QString ctName = action->data().toString();
        _config.setString(_sceneName + "/ColorTable", ctName.toStdString());

        ColorTable * oldCt = _colorTable;
    	_colorTable = new ColorTable(oldCt->minimumValue(),
    	        oldCt->maximumValue(), ctName);
    	delete(oldCt);

        // Resend changed() signals from our color table as local
        // colorTableChanged() signals
        connect(_colorTable, SIGNAL(changed()), this, SIGNAL(colorTableChanged()));

    	updateRayGraphicsItems_();
    	emit colorTableChanged();
    }
}

void
BscanGraphicsScene::updateRayGraphicsItems_() {
    // Get rid of all of our old RayGraphicsItem-s
    clear();
    // Recreate RayGraphicsItem-s for each of our BscanRay-s, based on our
    // current _displayVar, _colorTable, etc.
    for (BscanRayMap_t::iterator it = _bscanRayMap.begin(); 
            it != _bscanRayMap.end(); it++) {
        BscanRay * ray = it->second;
        
        // Create a new RayGraphicsItem
        RayGraphicsItem * newItem = new RayGraphicsItem(*ray, _displayVar, 
                *_colorTable);
        // Our x coordinates run from zero to _timeSpan, so translate the new
        // ray into the correct x location. We do this to keep the x coordinates
        // in the range +/- 2^15, because of limits in Qt's raster paint engine.
        newItem->moveBy(-_sceneStartTime, 0);
        // Add the item to our item map, and to the scene
        addItem(newItem);
    }
    // Invalidate everything to force a complete redraw. (Although this doesn't
    // seem to work as expected...)
    invalidate(sceneRect());
}

void
BscanGraphicsScene::setSceneName(std::string newSceneName) {
    if (newSceneName == _sceneName)
        return;
    // Copy the current scene configuration values into the config file under
    // the new scene name.
    _config.setInt(newSceneName + "/TimeSpan",
            _config.getInt(_sceneName + "/TimeSpan", 0));
    _config.setInt(newSceneName + "/minGate",
            _config.getInt(_sceneName + "/minGate", 0));
    _config.setInt(newSceneName + "/maxGate",
            _config.getInt(_sceneName + "/maxGate", 0));
    _config.setString(newSceneName + "/VarName",
            _config.getString(_sceneName + "/VarName", ""));
    _config.setString(newSceneName + "/ColorTable",
            _config.getString(_sceneName + "/ColorTable", ""));
    // Now just change our _sceneName
    _sceneName = newSceneName;
}

void
BscanGraphicsScene::copyRaysFrom(const BscanGraphicsScene & srcScene) {
    // Copy the BscanRay map from the source scene.
    for (BscanRayMap_t::const_iterator it = srcScene._bscanRayMap.begin();
            it != srcScene._bscanRayMap.end(); it++) {
        double time = it->first;
        BscanRay * ray = it->second;
        _bscanRayMap[time] = new BscanRay(*ray);
    }
}
