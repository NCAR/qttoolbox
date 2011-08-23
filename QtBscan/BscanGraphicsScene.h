/*
 * BscanGraphicsScene.h
 *
 *  Created on: Aug 20, 2009
 *      Author: burghart
 */

#ifndef BSCANGRAPHICSSCENE_H_
#define BSCANGRAPHICSSCENE_H_

#include <map>
#include <vector>

#include <QGraphicsScene>
#include "ColorTable.h"

class QPainter;
class RayGraphicsItem;
class BscanRay;

/**
 * BscanGraphicsScene is a subclass of QGraphicsScene which keeps a list of and 
 * displays radar rays within a specified time span in a moving time-height 
 * display. Gate range limits are also supported by the scene.
 */
class BscanGraphicsScene : public QGraphicsScene {
    Q_OBJECT
public:
    /**
     * Construct with the given time span, variable for display, and color
     * table.
     * @param timeSpan the time span of the scene, in seconds
     * @param varName the name of the variable to display
     * @param ctFileName the name of the color table file to use
     */
    BscanGraphicsScene(unsigned int timeSpan = 30,
            const std::string & varName = "DZ", 
            const std::string & ctFileName = "eldoraDbz.ct");
    /**
     * Copy constructor.
     * @param scene the BscanGraphicsScene to be copied
     */
    BscanGraphicsScene(const BscanGraphicsScene & scene);
    virtual ~BscanGraphicsScene();
    /**
     * Return the number of radar gates currently being shown in the scene.
     */
    unsigned int nGates() const;
    /**
     * Return the time span, in seconds, currently covered by the scene.
     */
    unsigned int timeSpan() const;
    /**
     * Return the index of the minimum gate currently being shown.
     */
    unsigned int minGate() const { return _minGate; }
    /**
     * Set the minimum gate to display.
     * @param minGate the minimum gate to display.
     */
    void setMinGate(unsigned int minGate);
    /**
     * Return the index of the maximum gate currently being shown.
     */
    unsigned int maxGate() const { return _maxGate; }
    /**
     * Set the maximum gate to display.
     * @param maxGate the maximum gate to display.
     */
    void setMaxGate(unsigned int maxGate);
    /**
     * Pause the scene.  While paused, the scene's time bounds will be held 
     * fixed.
     * @see unpause()
     * @see setPaused()
     */
    void pause();
    /**
     * Unpause the scene, allowing it to dynamically adjust time bounds
     * again.
     * @see pause()
     * @see setPaused()
     */
    void unpause();
    /**
     * Returns true iff the scene is paused.
     */
    bool isPaused() const { return _isPaused; }
    /**
     * Return the scene's start time.
     */
    double startTime() const { return _sceneStartTime; }
    /**
     * Return the scene's end time.
     */
    double endTime() const { return(_sceneStartTime + _timeSpan); }
    /**
     * Return the name of the currently displayed variable.
     * @return the name of the currently displayed variable
     */
    QString displayVar() const { return _displayVar; }
    /**
     * Return the units string for the currently displayed variable.
     * @return the units string for the currently displayed variable.
     */
    QString displayVarUnits() const { return _displayVarUnits; }
    /**
     * Return the list of available vars. The list is obtained from
     * the latest ray in the scene. If there are no rays in the scene,
     * an empty list is returned.
     * @return the list of var names known to the scene
     */
    QStringList varNames() const;
    /**
     * Return a reference to the scene's color table.
     */
    const ColorTable & colorTable() const { return _colorTable; }
public slots:
    /**
     * Set the scene's gate limits.
     * @param minGate the minimum gate to display
     * @param maxGate the maximum gate to display
     */
    void setGateLimits(unsigned int minGate, unsigned int maxGate);
    /**
     * Set the scene's time span.
     * @param timeSpan the desired time span, in seconds
     */
    void setTimeSpan(unsigned int timeSpan);
    /**
     * Set the paused state of the scene.
     * @param state the new paused state
     * @see pause()
     * @see unpause()
     */
    void setPaused(bool state);
    /**
     * Add the given BscanRay to the scene. If necessary, the end time displayed
     * by the scene will be adjusted to accommodate the new ray and older rays 
     * outside the time span of the scene will be deleted.
     * @param ray the new BscanRay 
     */
    void addRay(const BscanRay & ray);
    /**
     * Set time limits of the scene
     * @param startTime start time of the scene, in seconds since 1970-01-01
     *     00:00:00 UTC
     * @param timeSpan time span of the scene, in seconds
     */
    void setTimeLimits(double startTime, unsigned int timeSpan);
    /**
     * Set the variable to display.
     * @param varName the name of the variable to display
     */
    void setDisplayVar(QString varName);
    /**
     * Set the display limits for our color table.
     * @param minValue the minimum value to be represented by the color table
     * @param maxValue the maximum value to be represented by the color table
     */
    void setDisplayLimits(double minValue, double maxValue) {
        _colorTable.setValueLimits(minValue, maxValue);
    }
    /**
     * Set the scene's color table based on the text of a QAction.
     */
    void setColorTable();
protected:
    void initConnections_();
    void scrubRaysBefore_(double scrubTime);
    void setStartTime_(double startTime);
    // set our scene rect to reflect current gate and time limits
    void updateSceneRect_();
    
    // virtual method from QGraphicsScene
    virtual void drawForeground(QPainter *painter, const QRectF & rect);
signals:
    /**
     * This signal is emitted when the scene's gate limits change.
     * @param minGate the new minimum gate
     * @param maxGate the new maximum gate
     */
    void gateLimitsChanged(unsigned int minGate, unsigned int maxGate);
    /**
     * This signal is emitted when the scene's time limits change
     * @param startTime the new start time of the scene, in seconds since
     *     1-Jan-1970 00:00 UTC
     * @param timeSpan the new time span in seconds
     */
    void timeLimitsChanged(double startTime, unsigned int timeSpan);
    /**
     * This signal is emitted when the scene's pause state changes, with
     * state true if the scene is paused.
     * @param paused true if the scene is now paused
     */
    void pauseStateChanged(bool paused);
    /**
     * This signal is emitted when the displayed variable changes.
     * @param varName the name of the new display variable.
     */
    void displayVarChanged(QString varName);
    /**
     * This signal is emitted when the units of our displayed variable
     * change.
     */
    void unitsChanged();
    /**
     * This signal is emitted when any trait of the color table changes
     */
    void colorTableChanged();
private:
    // Map from time to RayGraphicsItem-s
    typedef std::map<double,RayGraphicsItem*> ItemMap_t;
    ItemMap_t _itemMap;
    double _sceneStartTime;
    unsigned int _timeSpan;
    unsigned int _minGate;
    unsigned int _maxGate;
    double _lastScrubTime;
    bool _isPaused;
    QString _displayVar;
    QString _displayVarUnits;
    ColorTable _colorTable;
};

/**
 * @brief Class which links a set of BscanGraphicsScene instances so that they 
 * share gate limits, time limits, and pause state. When one of these states 
 * changes in one of the  * scenes, it is changed in all of them.
 */
class BscanSceneGroup : QObject {
    Q_OBJECT
public:
    BscanSceneGroup() {};
    /**
     * @brief Add a BscanGraphicsScene to the group.
     * @param scene the BscanGraphicsScene to be added to the group.
     */
    void addScene(BscanGraphicsScene* scene) {
        connect(scene, SIGNAL(gateLimitsChanged(unsigned int, unsigned int)),
                this, SIGNAL(gateLimitsChanged(unsigned int, unsigned int)));
        connect(this, SIGNAL(gateLimitsChanged(unsigned int, unsigned int)),
                scene, SLOT(setGateLimits(unsigned int, unsigned int)));
        connect(scene, SIGNAL(timeLimitsChanged(double, unsigned int)),
                this, SIGNAL(timeLimitsChanged(double, unsigned int)));
        connect(this, SIGNAL(timeLimitsChanged(double, unsigned int)),
                scene, SLOT(setTimeLimits(double, unsigned int)));
        connect(scene, SIGNAL(pauseStateChanged(bool)), 
                this, SIGNAL(pauseStateChanged(bool)));
        connect(this, SIGNAL(pauseStateChanged(bool)), 
                scene, SLOT(setPaused(bool)));
    }
    /**
     * @brief Remove a BscanGraphicsScene to the group.
     * @param scene the BscanGraphicsScene to be removed from the group. If
     * the scene is not in the group, this method has no effect.
     */
    void removeScene(BscanGraphicsScene* scene) {
        // Disconnect signals in both directions between this and the
        // scene being removed.
        disconnect(this, 0, scene, 0);
        disconnect(scene, 0, this, 0);
    }
signals:
    void gateLimitsChanged(unsigned int minGate, unsigned int maxGate);
    void timeLimitsChanged(double startTime, unsigned int timeSpan);
    void pauseStateChanged(bool paused);
};

#endif /* BSCANGRAPHICSSCENE_H_ */
