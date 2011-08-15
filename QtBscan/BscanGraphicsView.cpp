/*
 * BscanGraphicsView.cpp
 *
 *  Created on: Aug 21, 2009
 *      Author: burghart
 */

#include <iostream>
#include <QGraphicsScene>
#include <QMenu>
#include <QMouseEvent>
#include <QInputDialog>
#include <QPrintDialog>
#include <QPrinter>

#include "BscanGraphicsView.h"
#include "RayGraphicsItem.h"

#include "DisplayLimitDialog.h"
#include "GateLimitDialog.h"
#include "TimeSpanDialog.h"


BscanGraphicsView::BscanGraphicsView(QWidget *parent) : 
    QGraphicsView(parent),
    _zoom(1.0) {
    // Set mouse tracking to true so that mouse move events are delivered
    // to our mouseMoveEvent() method.
    setMouseTracking(true);
    // Set a reasonable minimum size
    setMinimumSize(400, 200);
    // Allow for getting bigger, but not for shrinking below our minimum size
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
}

BscanGraphicsView::~BscanGraphicsView() {
    // currently empty
}

/*
 * Set our size hint to our minimum size
 */
QSize
BscanGraphicsView::sizeHint() const {
    return minimumSize();
}

void
BscanGraphicsView::setScene(BscanGraphicsScene *scene) {
    // Scale for this scene's rectangle before doing QGraphicsView::setScene()
    // (Otherwise, we'll get automatically resized based on the scene's size)
    updateSceneRect(scene->sceneRect());
    
    // Now do the default QGraphicsView::setScene() stuff
    QGraphicsView::setScene(scene);
    
    // Connect our BscanGraphicsScene's timeLimitsChanged() signal to our 
    // updateTimeLimits() slot
    connect(scene, SIGNAL(timeLimitsChanged(double, unsigned int)),
            this, SLOT(updateTimeLimits(double, unsigned int)));
}

void
BscanGraphicsView::print() {
    if (QPrintDialog(&_printer).exec() == QDialog::Accepted) {
        QPainter painter(&_printer);
        painter.setRenderHint(QPainter::TextAntialiasing);
#if PRINT_FULL_PAGE
        // This one will fill the entire page
        render(&painter, QRectF(), QRect(), Qt::IgnoreAspectRatio);
#else
        // This one will scale as large as possible, but will keep
        // the aspect ratio that's on the screen
        render(&painter);
#endif // PRINT_FULL_PAGE
    }
}

/**
 * Set our transform matrix so that the given scene rectangle will 
 * fill the view.  Note that our scene's sceneRectChanged() signal is 
 * automatically connected to this slot.
 * @param rect the rectangle (in scene coordinates) which should fill the view
 */
void
BscanGraphicsView::updateSceneRect(const QRectF & rect) {
    //
    // Set the transform translation and scaling so that at a zoom of 1.0 
    // the entire scene will fill our space.  We allow for the width of our 
    // frame here, and add a few pixels of buffer around the edge of the 
    // scene as well.
    //
    int pixBorder = 4 + frameWidth();
    float xscale = (_zoom * width() - 2 * pixBorder) / rect.width();
    float yscale = (_zoom * height() - 2 * pixBorder) / rect.height();
    
    QMatrix newMatrix;
    newMatrix.translate(-rect.left(), 0);
    newMatrix.scale(xscale, yscale);
    setMatrix(newMatrix);
}

void
BscanGraphicsView::leaveEvent(QEvent *event) {
    emit newDataUnderMouse(-1, 0, "", 0);
}

// Emit a signal with the (time,gate) coordinates for the ray/gate under
// the mouse
void
BscanGraphicsView::mouseMoveEvent(QMouseEvent *event) {
    // Ship out a newDataUnderMouse() signal
    emitDataUnderMouse(event->pos());
}

void
BscanGraphicsView::resizeEvent(QResizeEvent *event) {
    // Set the transformation matrix based on our new size so that the scene 
    // continues to fill our display
    if (scene())
        updateSceneRect(scene()->sceneRect());
}

// Pop up the context menu
void
BscanGraphicsView::contextMenuEvent(QContextMenuEvent *event) {
    QMenu menu(this);
    QAction *action;
    // Actions to change gate limits and time span
    GateLimitDialog *glDialog = new GateLimitDialog(scene());
    action = new QAction("Set Gate Limits...", this);
    connect(action, SIGNAL(triggered()), glDialog, SLOT(exec()));
    menu.addAction(action);

    TimeSpanDialog *tsDialog = new TimeSpanDialog(scene());
    action = new QAction("Set Time Span...", this);
    connect(action, SIGNAL(triggered()), tsDialog, SLOT(exec()));
    menu.addAction(action);
    // Action to change display limits for current variable
    DisplayLimitDialog *dlDialog = new DisplayLimitDialog(scene());
    action = new QAction("Set Display Limits...", this);
    connect(action, SIGNAL(triggered()), dlDialog, SLOT(exec()));
    menu.addAction(action);
    
    menu.addSeparator();
    
    // Submenu to change color table
    QMenu ctMenu("Color Tables", this);
    action = new QAction("chris.ct", this);
    action->setData(action->text());
    connect(action, SIGNAL(triggered()), scene(), SLOT(setColorTable()));
    ctMenu.addAction(action);
    action = new QAction("eldoraDbz.ct", this);
    action->setData(action->text());
    connect(action, SIGNAL(triggered()), scene(), SLOT(setColorTable()));
    ctMenu.addAction(action);
    action = new QAction("eldoraVel.ct", this);
    action->setData(action->text());
    connect(action, SIGNAL(triggered()), scene(), SLOT(setColorTable()));
    ctMenu.addAction(action);
    menu.addMenu(&ctMenu);
    menu.addSeparator();
    
    // Add an action for each available variable
    QStringList varNames = scene()->varNames();
    for (int i = 0; i < varNames.size(); i++) {
        QString name = varNames[i];
        QAction *action = new QAction(name, this);
        action->setData(name);
        // Put on a check mark if this is the currently displayed variable
        // (this is the 
        if (name == scene()->displayVar()) {
            action->setCheckable(true);
            action->setChecked(true);
        }
        connect(action, SIGNAL(triggered()), this, SLOT(setDisplayVar()));
        menu.addAction(action);
    }
    // Pop up the context menu at the current cursor location
    menu.exec(event->globalPos());
    
    // Delete all the actions we created
    menu.clear();
}

void
BscanGraphicsView::setDisplayVar() {
    QAction *action = qobject_cast<QAction*>(sender());
    if (action)
        scene()->setDisplayVar(action->data().toString());
}

void
BscanGraphicsView::setZoom(float zoom) {
    if (_zoom != zoom) {    
        _zoom = zoom;
        if (scene())
            updateSceneRect(scene()->sceneRect());
        emit zoomChanged(_zoom);
    }
}

void
BscanGraphicsView::updateTimeLimits(double startTime, unsigned int timeSpan) {
    // If the mouse is in our view, emit a newDataUnderMouse() signal, since
    // the data under the mouse will have changed.
    if (viewport()->underMouse()) {
        QPoint mouseLoc = mapFromGlobal(QCursor::pos());
        emitDataUnderMouse(mouseLoc);
    }
}

void
BscanGraphicsView::emitDataUnderMouse(const QPoint &mouseLoc) {
    QPointF sceneLoc = mapToScene(mouseLoc);
    // Get the ray (if any) actually displayed under the mouse, and use
    // its time
    RayGraphicsItem *ray = 
        dynamic_cast<RayGraphicsItem*>(scene()->itemAt(sceneLoc));
    if (ray) {
        unsigned int gate = int(sceneLoc.y());
        emit newDataUnderMouse(ray->time(), gate, scene()->displayVar(), 
                ray->gateValue(gate));
    } else {
        emit newDataUnderMouse(-1, 0, "", 0);
    }
}
