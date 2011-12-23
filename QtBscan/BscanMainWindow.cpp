/*
 * BscanMainWindow.cpp
 *
 *  Created on: Sep 23, 2009
 *      Author: burghart
 */
#include <cmath>
#include <iostream>
#include <QDateTime>
#include <QLabel>
#include <QPainter>
#include <QPrintDialog>
#include <QPrinter>
#include <QScrollBar>

#include <QtConfig.h>

#include "BscanMainWindow.h"

#include "ui_Bscan.h"

BscanMainWindow::BscanMainWindow(QtConfig &config) : 
  _mousePosLabel(),
  _config(config) {
    _ui.setupUi(this);
    // Remove the UI main frame's layout and set it to a QVBoxLayout
    delete _ui.frame->layout();
    QVBoxLayout *layout = new QVBoxLayout();
    // Use deprecated setMargin() instead of setContentsMargins()
    // until CentOS has Qt >= 4.3.
    layout->setMargin(1);
//    layout->setContentsMargins(1, 1, 1, 1);
    layout->setSpacing(1);
    _ui.frame->setLayout(layout);
    // Add our mouse position label to the status bar of the UI
    _ui.statusbar->insertWidget(0, &_mousePosLabel);
    // Start with one plot
    addNewPlot();
    setPaused(false);
    // Set up our gate limit and time span dialogs
    _glDialog = new GateLimitDialog(_scene(0));
    _tsDialog = new TimeSpanDialog(_scene(0));
}

BscanMainWindow::~BscanMainWindow() {
    // Delete our instances of BscanWidget
    for (unsigned int i = 0; i < _nBscans(); i++) {
        delete(_bscans[i]);
    }
    // Delete the layout we created
    delete _ui.frame->layout();
}

void 
BscanMainWindow::addNewPlot() {
    // Create the new bscan and add it to our group to share settings
    BscanWidget *newBscan;
    if (_nBscans() == 0) {
        newBscan = new BscanWidget(_config);
    } else {
      newBscan = new BscanWidget(*_bscans[0]);
    }
    
    _bscans.push_back(newBscan);
    
    // Add the widget to our BscanWidgetGroup so they all share settings
    _bwGroup.addBscanWidget(newBscan);
    
    // Display data under mouse when reported by the BscanWidget's view
    connect(newBscan->view(), 
            SIGNAL(newDataUnderMouse(double, unsigned int, const QString&, float)),
            this, 
            SLOT(_showLocAndData(double, unsigned int, const QString&, float)));
    
    // Add this widget to the UI's layout
    _ui.frame->layout()->addWidget(newBscan);
    
    // Enable "Remove Plot" if we have more than one plot
    _ui.actionRemovePlot->setEnabled((_nBscans() > 1));
}

void 
BscanMainWindow::removePlot() {
    // Don't remove the last plot
    if (_nBscans() == 1)
        return;
    
    BscanWidget *lastBscan = _bscans.at(_nBscans() - 1);
    _ui.frame->layout()->removeWidget(lastBscan);
    _bwGroup.removeBscanWidget(lastBscan);
    _bscans.pop_back();
    delete(lastBscan);
    
    // Disable "Remove Plot" if we have only one plot
    _ui.actionRemovePlot->setEnabled((_nBscans() > 1));
}

/*
 * Print a simple pixel dump of the UI's plot frame.
 */
void
BscanMainWindow::print() {
    QPrintDialog printDialog(&_printer);
    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&_printer);
        QPixmap pm = QPixmap::grabWidget(_ui.frame);
        // Get the aspect ratio of the printer device and of the pixmap
        int devWidth = painter.device()->width();
        int devHeight = painter.device()->height();
        float devAspect = float(devWidth) / devHeight;
        float pmAspect = float(pm.width()) / pm.height();
        // Select printer area to print the pixmap as large as possible
        // while maintaining its aspect ratio.
        int printWidth = devWidth;
        int printHeight = devHeight;
        if (pmAspect > devAspect) {
            printHeight = printWidth / pmAspect;
        } else {
            printWidth = printHeight * pmAspect;
        }
        // Now print the pixmap. Set the print origin to center the image on
        // the page.
        painter.drawPixmap(QRect((devWidth - printWidth) / 2, 
                (devHeight - printHeight) / 2, printWidth, printHeight), pm);
    }
}

void
BscanMainWindow::setPaused(bool state) {
    if (state != _scene(0)->isPaused()) {
        // We only have to set the paused state for our first scene, since 
        // the rest of our scenes are tied to it through our BscanWidgetGroup
        _scene(0)->setPaused(state);
    }
    // Enable/disable the pause and unpause actions
    _ui.actionPause->setDisabled(state);
    _ui.actionUnpause->setEnabled(state);
}

void
BscanMainWindow::pause() {
    setPaused(true);
}

void
BscanMainWindow::unpause() {
    setPaused(false);
}

bool
BscanMainWindow::isPaused() const {
    return _scene(0)->isPaused();
}

void
BscanMainWindow::setZoom(float zoom) {
    if (zoom != _view(0)->zoom()) {
        // We only have to set the zoom state for the first view, since
        // the rest of our views are tied to it through our BscanWidgetGroup
        _view(0)->setZoom(zoom);
    }
}

void
BscanMainWindow::_showLocAndData(double time, unsigned int gate, 
        const QString &varName, float value) {
    if (time == -1) {
        _mousePosLabel.setText("no data under cursor");
    } else {
        QDateTime qdt = QDateTime::fromTime_t((time_t)time);
        qdt = qdt.addMSecs(qint64(fmod(time, 1.0) * 1000));   // add on the fractional seconds
        QString timeLabel = qdt.toString("hh:mm:ss.zzz");
        QString label(varName + ": " + QString::number(value) + " @ gate " + 
                QString::number(gate) + ", " + timeLabel);
        _mousePosLabel.setText(label);
    }
}

void
BscanMainWindow::addRay(const BscanRay & ray) {
    // Pass this ray to each of our BscanWidget instances
    for (unsigned int i = 0; i < _bscans.size(); i++) {
        _bscans[i]->addRay(ray);
    }
}
