/*
 * BscanWidget.h
 *
 *  Created on: Dec 7, 2009
 *      Author: burghart
 */

#ifndef BSCANWIDGET_H_
#define BSCANWIDGET_H_

#include <QLabel>
#include <QWidget>
#include <QtConfig.h>
#include "BscanGraphicsView.h"
#include "BscanRay.h"
#include "QtConfig.h"

/**
 * @brief The BscanWidget is a frame containing a BscanGraphicsView (which displays 
 * the bscan data) and adds some annotation: color bar, var name, pause 
 * indicator, etc. Connect a signal to the addRay() slot to get data into
 * the widget.
 */
class BscanWidget : public QWidget {
    Q_OBJECT
public:
    /**
     * @brief Instantiate a BscanWidget.
     * @param config The QtConfig to be used for obtaining and storing
     * information for this widget.
     * @param sceneName The scene name for this bscan, used in building names for
     * some parameters stored in the configuration.
     */
    BscanWidget(QtConfig &config, std::string sceneName = "SCENE");

    virtual ~BscanWidget();
    
    /**
     * @brief Return a pointer to our BscanGraphicsView.
     * @return a pointer to our BscanGraphicsView
     */
    BscanGraphicsView * view() { return(& _view); }
    /**
     * @brief Set the scene name, which is used when saving and retrieving
     * values for the scene in the configuration file.
     * @param newSceneName the new name for the scene
     */
    void setSceneName(std::string newSceneName) { _scene.setSceneName(newSceneName); }
    /**
     * @brief Copy all the rays from an another BscanWidget
     */
    void copyRaysFrom(const BscanWidget& src);
public slots:
    /**
     * @brief This slot accepts incoming BscanRay data to be displayed in 
     * the widget.
     */
    void addRay(const BscanRay & ray);
protected:
    // react to some key presses
    virtual void keyPressEvent(QKeyEvent *event);
private slots:
    void _buildAnnotation();
private:
    void _init();
    
    BscanGraphicsView _view;
    BscanGraphicsScene _scene;
    QLabel _annot;
};

/**
 * @brief Class to tie together settings among BscanWidget instances: gate 
 * limits, time limits, pause state, zoom state, and horizontal/vertical scroll
 * settings. A change to any of these parameters in one group member is 
 * propagated to all others.
 */
class BscanWidgetGroup : public QObject {
    Q_OBJECT
public:
    BscanWidgetGroup() {}
    /**
     * @brief Add a BscanWidget to the group
     * @param widget the BscanWidget to be added
     */
    void addBscanWidget(BscanWidget * widget) {
        _bscans.push_back(widget);
        _sceneGroup.addScene(widget->view()->scene());
        _viewGroup.addView(widget->view());
    }
    /**
     * @brief Remove a BscanWidget from the group. If the chosen widget is not
     * in the group, nothing is changed.
     * @param widget the BscanWidget to be removed
     */
    void removeBscanWidget(BscanWidget * widget) {
        std::vector<BscanWidget *>::iterator it;
        for (it = _bscans.begin(); it != _bscans.end(); it++) {
            if (*it == widget) {
                _viewGroup.removeView(widget->view());
                _sceneGroup.removeScene(widget->view()->scene());
                _bscans.erase(it);
                return;
            }
        }
    }
    /**
     * @brief Return a pointer to the BscanWidget at position ndx in the 
     * group.
     * @param ndx the index of the desired BscanWidget
     * @return a pointer to the BscanWidget at position ndx in the group.
     * @throw std::out_of_range exception if the index is out of range
     */
    BscanWidget * operator[](unsigned int ndx) const {
        return(_bscans[ndx]);
    }
    /**
     * @brief Return the number of BscanWidget instances in the group.
     * @return the number of BscanWidget instances in the group.
     */
    unsigned int nBscans() const { return _bscans.size(); }
private:
    std::vector<BscanWidget *> _bscans;
    /*
     * BscanViewGroup to keep views for all widgets in the group synchronized.
     */
    BscanViewGroup _viewGroup;
    /*
     * BscanSceneGroup to keep scenes for all widgets in the group synchronized.
     */
    BscanSceneGroup _sceneGroup;
};
#endif /* BSCANWIDGET_H_ */
