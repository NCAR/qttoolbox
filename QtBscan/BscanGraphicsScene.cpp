/*
 * BscanGraphicsScene.cpp
 *
 *  Created on: Aug 20, 2009
 *      Author: burghart
 */
#include <cmath>
#include <iostream>

#include <QAction>
#include <QDateTime>
#include <QPainter>

#include "BscanGraphicsScene.h"
#include "BscanRay.h"
#include "RayGraphicsItem.h"

/*
 * Basic constructor
 */
BscanGraphicsScene::BscanGraphicsScene(unsigned int timeSpan, 
        const std::string & varName, const std::string & ctFileName) : 
    _sceneStartTime(0),
    _timeSpan(timeSpan), 
    _minGate(0), 
    _maxGate(0),
    _lastScrubTime(0),
    _isPaused(false),
    _displayVar(varName.c_str()),
    _displayVarUnits(""),
    _colorTable(83, 92, ctFileName.c_str()) {
    // Set up our connections
    initConnections_();
    // update our scene rect
    updateSceneRect_();
}

/*
 * Copy constructor
 */
BscanGraphicsScene::BscanGraphicsScene(const BscanGraphicsScene & srcScene) :
    _sceneStartTime(srcScene._sceneStartTime),
    _timeSpan(srcScene._timeSpan),
    _minGate(srcScene._minGate),
    _maxGate(srcScene._maxGate),
    _lastScrubTime(srcScene._lastScrubTime),
    _isPaused(srcScene._isPaused),
    _displayVarUnits(""),
    _colorTable(srcScene._colorTable) {
    // Copy the rays from the source scene and add them here
    ItemMap_t srcItemMap = srcScene._itemMap;
    for (ItemMap_t::const_iterator it = srcItemMap.begin(); it != srcItemMap.end(); it++) {
        double rayTime = it->first;
        RayGraphicsItem *srcItem = it->second;
        RayGraphicsItem *newItem = new RayGraphicsItem(*srcItem);
        _itemMap[rayTime] = newItem;
        addItem(newItem);
    }
    setDisplayVar(srcScene._displayVar);
    // Set up our connections
    initConnections_();
    // update our scene rect
    updateSceneRect_();
}

BscanGraphicsScene::~BscanGraphicsScene() {
    for (ItemMap_t::iterator it = _itemMap.begin(); it != _itemMap.end(); it++) {
        RayGraphicsItem *rgi = it->second;
        delete(rgi);
    }
}

void
BscanGraphicsScene::initConnections_() {
    // Resend changed() signals from our color table as local
    // colorTableChanged() signals
    connect(&_colorTable, SIGNAL(changed()), this, SIGNAL(colorTableChanged()));
}

// set our scene rect to reflect current gate and time limits
void 
BscanGraphicsScene::updateSceneRect_() {
    qreal left = 0;
    qreal width = _timeSpan;
    qreal top = _maxGate + 1;
    qreal height = -int(_maxGate - _minGate + 1);  // height is negative!
    setSceneRect(left, top, width, height);
}

/**
 * Return the number of gates in the last ray, or 1 if we have no
 * rays yet.
 */
unsigned int
BscanGraphicsScene::nGates() const {
    if (_itemMap.empty())
        return 1;
    
    ItemMap_t::const_reverse_iterator rit = _itemMap.rbegin();
    if (rit != _itemMap.rend())
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

    // If this is our first ray, set our start time and set gate limits to 
    // show the whole ray
    if (startTime() == 0)
        setStartTime_(time);
    
    // Potentially adjust our scene start time if we're not paused and the
    // new ray would be off the edge of the scene.
    if (! _isPaused && (time > endTime())) {
        // Shift the scene so the new start is (<raytime> - 0.9 * _timeSpan)
        setStartTime_(time - 0.9 * _timeSpan);
    }
    
    // Create a RayGraphicsItem from the ProductSet and add it to our list
    RayGraphicsItem *newItem = new RayGraphicsItem(ray, _displayVar);
    _itemMap[time] = newItem;
    // Our x coordinates run from zero to _timeSpan, so translate the new
    // ray into the correct x location. We do this to keep the x coordinates
    // in the range +/- 2^15, because of limits in Qt's raster paint engine.
    newItem->translate(-_sceneStartTime, 0);

    // If the gate count changes w.r.t. the previous ray, change limits
    // to show all gates of the new ray
    if (ray.nGates() != oldNGates) {
        setGateLimits(0, newItem->nGates());
    }
    
    // Now add this item to the scene
    addItem(newItem);
    
    // The units of our latest item become *our* units
    if (newItem->displayVarUnits() != _displayVarUnits) {
        _displayVarUnits = newItem->displayVarUnits();
        emit unitsChanged();
    }
}

void
BscanGraphicsScene::scrubRaysBefore_(double scrubTime) {
    if (_itemMap.empty())
        return;

    // Remove items from the scene which are between the last scrub 
    // time and this scrub time
    ItemMap_t::iterator startIt = _itemMap.upper_bound(_lastScrubTime);
    ItemMap_t::iterator endIt = _itemMap.lower_bound(scrubTime);        
    for (ItemMap_t::iterator it = startIt; it != endIt; it++) {
        removeItem(it->second);
    }

    // XXX SLIGHT KLUGE HERE
    // Now actually delete items which are earlier than our *last* scrub 
    // time. Waiting until now gives a little time to those who get an 
    // item list via our items() methods and are dealing with those items.
    //
    // This seems to be necessary for Qt4.4.3, while just doing the simple
    // removeItem(item) immediately followed by delete(item) in the loop
    // above works for at least Qt4.2.1 and Qt4.5.2.
    endIt = _itemMap.lower_bound(_lastScrubTime);
    for (ItemMap_t::iterator it = _itemMap.begin(); it != endIt; it++) {
        delete(it->second);
    }
    _itemMap.erase(_itemMap.begin(), endIt);

    _lastScrubTime = scrubTime;
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
    
    _minGate = minGate;
    _maxGate = maxGate;

    updateSceneRect_();
    
    emit gateLimitsChanged(_minGate, _maxGate);
}

void
BscanGraphicsScene::setTimeSpan(unsigned int newSpan) {
    if (newSpan == _timeSpan)
        return;
    
    // If our time span is shrinking, set the end time of the scene to
    // the last ray time (if we have rays) or leave it unchanged
    double newStartTime = _sceneStartTime;
    if (newSpan < _timeSpan) {
        double newEndTime = (_itemMap.size() > 0) ? 
                _itemMap.rbegin()->first : endTime();
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
    if (newStartTime == _sceneStartTime && newTimeSpan == _timeSpan)
        return;

    // If our start time changes...
    if (newStartTime != _sceneStartTime) {
        // Clean out rays before our new start time
        scrubRaysBefore_(newStartTime);
        
        // Time shift our items to reflect the new start time.  This is
        // all due to a limitation in Qt's raster paint engine that requires
        // us to keep our coordinate limits in the range +/- 2^15...
        double deltaTime = newStartTime - _sceneStartTime;
        for (ItemMap_t::const_iterator it = _itemMap.begin(); 
            it != _itemMap.end(); it++) {
            RayGraphicsItem *ray = it->second;
            ray->translate(-deltaTime, 0);
        }
    }
    _sceneStartTime = newStartTime;
    _timeSpan = newTimeSpan;
    updateSceneRect_();
    
    emit timeLimitsChanged(_sceneStartTime, _timeSpan);
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
    const int goodXSteps[] = { 1, 2, 5, 10, 30, 60, 120, 300 };
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
            worldToDev.mapRect(QRectF(sceneX, sceneRect().bottom(), 
                                      sceneX, sceneRect().top()));
        pixTestRect.adjust(-20, 0, 20, 0);
        if (! pixExposedRect.intersects(pixTestRect))
            continue;
        
        // Draw the text label, with a surrounding filled box.
        int alignmentFlags = Qt::AlignBottom | Qt::AlignHCenter;
        QString label = QDateTime::fromTime_t(i).toString("hh:mm:ss");
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
        painter->drawLine(worldToDev.map(QPointF(sceneX, sceneTextRect.bottom())), 
                worldToDev.map(QPointF(sceneX, sceneRect().top())));
        painter->restore();
        // ...then a thin line in semi-transparent black
        painter->drawLine(worldToDev.map(QPointF(sceneX, sceneTextRect.bottom())), 
                worldToDev.map(QPointF(sceneX, sceneRect().top())));
    }

    // Restore the painter to its original state
    painter->restore();
}

QStringList
BscanGraphicsScene::varNames() const {
    // Return the list of vars from the latest ray we have
    ItemMap_t::const_reverse_iterator rit = _itemMap.rbegin();
    if (rit == _itemMap.rend())
        return QStringList();
    else
        return rit->second->varNames();
}

void
BscanGraphicsScene::setDisplayVar(QString varName) {
    if (_displayVar == varName)
        return;
    
    _displayVar = varName;
    for (ItemMap_t::iterator it = _itemMap.begin(); it != _itemMap.end(); it++) {
        RayGraphicsItem *rgi = it->second;
        rgi->setDisplayVar(_displayVar);
    }
    emit displayVarChanged(_displayVar);
}

void
BscanGraphicsScene::setColorTable() {
    QAction *action = qobject_cast<QAction*>(sender());
    if (action) {
    	QString ctName = action->data().toString();
    	_colorTable = ColorTable(_colorTable.minimumValue(), 
    			_colorTable.maximumValue(), ctName);
    	emit colorTableChanged();
    }
}
