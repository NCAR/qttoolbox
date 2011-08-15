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
#include "BscanGraphicsView.h"
#include "QtProductReader.h"

/**
 * The BscanWidget is a frame containing a BscanGraphicsView (which displays 
 * the bscan data) and adds some annotation: color bar, var name, pause 
 * indicator, etc.
 */
class BscanWidget : public QWidget {
    Q_OBJECT
public:
    /**
     * Construct, building a new BscanGraphicsScene which gets its data from
     * the given QtProductReader.
     * @param productReader the data source QtProductReader
     */
    BscanWidget(QtProductReader *productReader);
    /*
     * Construct, building a new BscanGraphicsScene as a copy of the given
     * BscanGraphicsScene.
     * @param scene the template BscanGraphicsScene to be copied for use
     * in this widget.
     */
    BscanWidget(const BscanGraphicsScene & scene);
    virtual ~BscanWidget();
    
    /**
     * Return a pointer to our BscanGraphicsView.
     * @return a pointer to our BscanGraphicsView
     */
    BscanGraphicsView *view() { return _view; }
protected:
    // react to some key presses
    virtual void keyPressEvent(QKeyEvent *event);
private slots:
    void _buildAnnotation();
private:
    void _init();
    
    BscanGraphicsView *_view;
    BscanGraphicsScene *_scene;
    QLabel _annot;
};

/**
 * Class to tie together settings among BscanWidget-s: gate limits, time
 * limits, pause state, zoom, etc.
 */
class BscanWidgetGroup : public QObject {
    Q_OBJECT
public:
    BscanWidgetGroup() {}
    void addBscanWidget(BscanWidget *widget) {
        _sceneGroup.addScene(widget->view()->scene());
        _viewGroup.addView(widget->view());
    }
private:
    BscanViewGroup _viewGroup;
    BscanSceneGroup _sceneGroup;
};
#endif /* BSCANWIDGET_H_ */
