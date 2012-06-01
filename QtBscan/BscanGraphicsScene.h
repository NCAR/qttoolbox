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

#include <QtConfig.h>

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
     * @brief Construct a scene with the given name. The name is used for looking up and
     * storing information for the scene in the given configuration.
     * @param config - The QtConfig holding information for the scene. Time span,
     * gate limits, variable, display limits, and color table will be read from
     * and written to this configuration.
     * @param sceneName - the name of the scene, used in looking up/storing information
     * in the configuration
     */
    BscanGraphicsScene(QtConfig &config, std::string sceneName);

    virtual ~BscanGraphicsScene();
    /**
     * @brief Return the number of radar gates currently being shown in the scene.
     */
    unsigned int nGates() const;
    /**
     * Return the time span, in seconds, currently covered by the scene.
     */
    unsigned int timeSpan() const;
    /**
     * @brief Return the index of the minimum gate currently being shown.
     */
    unsigned int minGate() const { return _minGate; }
    /**
     * @brief Set the minimum gate to display.
     * @param minGate the minimum gate to display.
     */
    void setMinGate(unsigned int minGate);
    /**
     * Return the index of the maximum gate currently being shown.
     */
    unsigned int maxGate() const { return _maxGate; }
    /**
     * @brief Set the maximum gate to display.
     * @param maxGate the maximum gate to display.
     */
    void setMaxGate(unsigned int maxGate);
    /**
     * @brief Pause the scene.  While paused, the scene's time bounds will be held 
     * fixed.
     * @see unpause()
     * @see setPaused()
     */
    void pause();
    /**
     * @brief Unpause the scene, allowing it to dynamically adjust time bounds
     * again.
     * @see pause()
     * @see setPaused()
     */
    void unpause();
    /**
     * @brief Returns true iff the scene is paused.
     */
    bool isPaused() const { return _isPaused; }
    /**
     * @brief Return the scene's start time.
     */
    double startTime() const { return _sceneStartTime; }
    /**
     * @brief Return the scene's end time.
     */
    double endTime() const { return(_sceneStartTime + _timeSpan); }
    /**
     * @brief Return the name of the currently displayed variable.
     * @return the name of the currently displayed variable
     */
    QString displayVar() const { return _displayVar; }
    /**
     * @brief Return the units string for the currently displayed variable.
     * @return the units string for the currently displayed variable.
     */
    QString displayVarUnits() const { return _displayVarUnits; }
    /**
     * @brief Return the list of available vars. The list is obtained from
     * the latest ray in the scene. If there are no rays in the scene,
     * an empty list is returned.
     * @return the list of var names known to the scene
     */
    QStringList varNames() const;
    /**
     * @brief Return the scene's color table.
     */
    const ColorTable * colorTable() const { return _colorTable; }
    /**
     * @brief Return the scene's name
     */
    std::string sceneName() const { return _sceneName; }
    /**
     * Set the scene's name, which is used when saving and retrieving values
     * for the scene in the configuration file.
     * @param newSceneName the new name for the scene
     */
    void setSceneName(std::string newSceneName);
public slots:
    /**
     * @brief Set the scene's gate limits.
     * @param minGate the minimum gate to display
     * @param maxGate the maximum gate to display
     */
    void setGateLimits(unsigned int minGate, unsigned int maxGate);
    /**
     * @brief Set the scene's time span.
     * @param timeSpan the desired time span, in seconds
     */
    void setTimeSpan(unsigned int timeSpan);
    /**
     * @brief Set the paused state of the scene.
     * @param state the new paused state
     * @see pause()
     * @see unpause()
     */
    void setPaused(bool state);
    /**
     * @brief Add the given BscanRay to the scene. If necessary, the end time 
     * displayed by the scene will be adjusted to accommodate the new ray and 
     * older rays outside the time span of the scene will be deleted.
     * @param ray the new BscanRay 
     */
    void addRay(const BscanRay & ray);
    /**
     * @brief Set time limits of the scene
     * @param startTime start time of the scene, in seconds since 1970-01-01
     *     00:00:00 UTC
     * @param timeSpan time span of the scene, in seconds
     */
    void setTimeLimits(double startTime, unsigned int timeSpan);
    /**
     * @brief Set the variable to display.
     * @param varName the name of the variable to display
     */
    void setDisplayVar(QString varName);
    /**
     * @brief Set the display limits for our color table.
     * @param minValue the minimum value to be represented by the color table
     * @param maxValue the maximum value to be represented by the color table
     */
    void setDisplayLimits(double minValue, double maxValue);
    /**
     * @brief Set the scene's color table based on the text of a QAction.
     */
    void setColorTable();
    /**
     * @brief Copy all the rays from another BscanGraphicsScene to this scene.
     */
    void copyRaysFrom(const BscanGraphicsScene & src);
protected:
    void initConnections_();
    /**
     * @brief Set the start time of the scene. 
     */
    void setStartTime_(double startTime);
    /**
     * @brief Set our scene rect to reflect current gate and time limits.
     */
    void updateSceneRect_();
    /**
     * @brief Create RayGraphicsItems-s from our BscanRay-s using the current
     * _displayVar, _colorTable, etc.
     */
    void updateRayGraphicsItems_();
        
    // virtual method from QGraphicsScene
    virtual void drawForeground(QPainter *painter, const QRectF & rect);
signals:
    /**
     * @brief This signal is emitted when the scene's gate limits change.
     * @param minGate the new minimum gate
     * @param maxGate the new maximum gate
     */
    void gateLimitsChanged(unsigned int minGate, unsigned int maxGate);
    /**
     * @brief This signal is emitted when the scene's time limits change
     * @param startTime the new start time of the scene, in seconds since
     *     1-Jan-1970 00:00 UTC
     * @param timeSpan the new time span in seconds
     */
    void timeLimitsChanged(double startTime, unsigned int timeSpan);
    /**
     * @brief This signal is emitted when the scene's pause state changes, with
     * state true if the scene is paused.
     * @param paused true if the scene is now paused
     */
    void pauseStateChanged(bool paused);
    /**
     * @brief This signal is emitted when the displayed variable changes.
     * @param varName the name of the new display variable.
     */
    void displayVarChanged(QString varName);
    /**
     * @brief This signal is emitted when the units of our displayed variable
     * change.
     */
    void unitsChanged();
    /**
     * @brief This signal is emitted when any trait of the color table changes
     */
    void colorTableChanged();
private:
    // Map from time to BscanRay-s
    typedef std::map<double, BscanRay*> BscanRayMap_t;
    BscanRayMap_t _bscanRayMap;
    // List of RayGraphicsItem-s held temporarily for later deletion
    std::vector<RayGraphicsItem*> _deleteLaterList;
    double _sceneStartTime;
    unsigned int _timeSpan;
    unsigned int _minGate;
    unsigned int _maxGate;
    bool _isPaused;
    QString _displayVar;
    QString _displayVarUnits;
    ColorTable * _colorTable;
    QtConfig &_config;
    /**
     * Name of the scene, used in getting/saving information about this
     * scene in the configuration.
     */
    std::string _sceneName;
};

/**
 * @brief Class which links a set of BscanGraphicsScene instances so that they 
 * share gate limits, time limits, and pause state. When one of these states 
 * changes in one of the scenes, it is changed in all of them.
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
        // If this is not the first scene in the group, set it up like the
        // rest of the group
        if (! _scenes.empty()) {
            scene->setGateLimits(_scenes[0]->minGate(), _scenes[0]->maxGate());
            scene->setTimeLimits(_scenes[0]->startTime(), _scenes[0]->timeSpan());
            scene->setPaused(_scenes[0]->isPaused());
        }
        // Add this scene to the group and set up connections so that state
        // will be shared.
        _scenes.push_back(scene);

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
     * @brief Remove a BscanGraphicsScene from the group.
     * @param scene the BscanGraphicsScene to be removed from the group. If
     * the scene is not in the group, this method has no effect.
     */
    void removeScene(BscanGraphicsScene* scene) {
        for (unsigned int i = 0; i < _scenes.size(); i++) {
            if (_scenes[i] == scene) {
                // Disconnect signals in both directions between this and the
                // scene being removed.
                disconnect(this, 0, scene, 0);
                disconnect(scene, 0, this, 0);
                // Remove the scene from our list
                _scenes.erase(_scenes.begin() + i);
                return;
            }
        }
    }
signals:
    void gateLimitsChanged(unsigned int minGate, unsigned int maxGate);
    void timeLimitsChanged(double startTime, unsigned int timeSpan);
    void pauseStateChanged(bool paused);
private:
    std::vector<BscanGraphicsScene *> _scenes;
};

#endif /* BSCANGRAPHICSSCENE_H_ */
