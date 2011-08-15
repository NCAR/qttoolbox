/*
 * BscanGraphicsView.h
 *
 *  Created on: Aug 21, 2009
 *      Author: burghart
 */

#ifndef BSCANGRAPHICSVIEW_H_
#define BSCANGRAPHICSVIEW_H_

#include <QGraphicsView>
#include <QPrinter>
#include <QScrollBar>
#include "BscanGraphicsScene.h"

class QMouseEvent;
class QResizeEvent;
class QPoint;

/**
 * Specialization of QGraphicsView especially for handling time-height/B-scan 
 * displays of ray data.  The view scales itself to always display the entire 
 * contents of its associated scene, rather than using scroll bars.  It also
 * adds grids and text tags in the foreground to label the scene and provides
 * printing capability.
 */
class BscanGraphicsView : public QGraphicsView {
    Q_OBJECT
    
public:
    BscanGraphicsView(QWidget *parent = 0);
    virtual ~BscanGraphicsView();
    void setScene(BscanGraphicsScene *scene);
    virtual QSize sizeHint() const;
    BscanGraphicsScene* scene() const {
        return dynamic_cast<BscanGraphicsScene*>(QGraphicsView::scene());
    }
    /**
     * Return the current zoom factor.
     * @see setZoom()
     */
    float zoom() const { return _zoom; }
public slots:
    void print();
    /**
     * Set the zoom level.  The default zoom is 1, which will scale to
     * fill the view with the entire scene.  A zoom greater than 1 will 
     * display the scene larger and bring up scroll bars for panning around
     * the scene.
     * @param zoom the new zoom factor
     */
    void setZoom(float zoom);
    /**
     * Set the displayed variable, based on the text obtained from the
     * sender() QAction.
     */
    void setDisplayVar();
signals:
    /**
     * This signal is emitted whenever the zoom factor is changed.
     * @param zoom the new zoom factor
     */
    void zoomChanged(float zoom);
    /**
     * This signal is emitted when the data value under the mouse location
     * changes, either because the mouse moved or the scene shifted in the
     * view. When the mouse leaves the window, the signal will be emitted
     * with time set to -1.
     * @param time the time of the mouse position, in seconds since 
     *     1970-01-01 00:00:00 UTC, or -1 if the mouse leaves the widget or
     *     there is no data under the mouse
     * @param gate the index of the gate under the mouse
     * @param varName the name of the variable displayed at the mouse location
     * @param value the data value at the mouse location
     */
    void newDataUnderMouse(double time, unsigned int gate, 
            const QString &varName, float value);
protected slots:
    void updateSceneRect(const QRectF & rect);
    void updateTimeLimits(double startTime, unsigned int timeSpan);
protected:
    virtual void leaveEvent(QEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void contextMenuEvent(QContextMenuEvent *event);
    void emitDataUnderMouse(const QPoint &mouseLoc);
private:
    QPrinter _printer;
    float _zoom;
};


/**
 * Class which links BscanGraphicsView-s so that they share zoom state and
 * horizontal and vertical scroll bar settings.
 */
class BscanViewGroup : QObject {
    Q_OBJECT
public:
    BscanViewGroup() {};
    void addView(BscanGraphicsView* view) {
        connect(view->horizontalScrollBar(), SIGNAL(valueChanged(int)), 
                this, SIGNAL(horizontalValueChanged(int)));
        connect(this, SIGNAL(horizontalValueChanged(int)),
                view->horizontalScrollBar(), SLOT(setValue(int)));
        connect(view->verticalScrollBar(), SIGNAL(valueChanged(int)), 
                this, SIGNAL(verticalValueChanged(int)));
        connect(this, SIGNAL(verticalValueChanged(int)),
                view->verticalScrollBar(), SLOT(setValue(int)));
        connect(view, SIGNAL(zoomChanged(float)), this, SIGNAL(zoomChanged(float)));
        connect(this, SIGNAL(zoomChanged(float)), view, SLOT(setZoom(float)));
    }
signals:
    void horizontalValueChanged(int);
    void verticalValueChanged(int);
    void zoomChanged(float);
};


#endif /* BSCANGRAPHICSVIEW_H_ */
