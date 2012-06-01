/*
 * BscanMainWindow.cpp
 *
 *  Created on: Sep 23, 2009
 *      Author: burghart
 */
#include <cmath>
#include <csignal>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <sstream>
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
    int nPlots = _config.getInt("numPlots", 1);
    for (int i = 0; i < nPlots; i++) {
        addNewPlot();
    }
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
    // Create a scene name for the new plot using the index of the plot.
    // I.e., the first plot is "bscan_0", etc.
    std::string bscanName = _makeSceneName(_nBscans());

    // Create the new bscan
    BscanWidget *newBscan;
    newBscan = new BscanWidget(_config, bscanName);

    // If this is not the first plot, copy rays from one of the existing
    // plots
    if (_nBscans() > 0) {
        newBscan->copyRaysFrom(*_bscans[0]);
    }
    // Add the new plot to our list
    _bscans.push_back(newBscan);
    
    // Save the new plot count to the config
    _config.setInt("numPlots", _nBscans());

    // Add the widget to our BscanWidgetGroup so they all share similar time
    // and gate limits.
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
BscanMainWindow::removeLastPlot() {
    removePlot(_nBscans() - 1);
}

void 
BscanMainWindow::removePlot(unsigned int plotIndex) {
    // Don't remove the last plot
    if (_nBscans() == 1)
        return;
    // Rename all the plots after the one being removed
    for (unsigned int i = plotIndex + 1; i < _nBscans(); i++) {
        _bscans[i]->setSceneName(_makeSceneName(i - 1));
    }
    BscanWidget *bscan = _bscans.at(plotIndex);
    _ui.frame->layout()->removeWidget(bscan);
    _bwGroup.removeBscanWidget(bscan);
    _bscans.erase(_bscans.begin() + plotIndex);
    delete(bscan);
    
    // Save the new plot count to the config
    _config.setInt("numPlots", _nBscans());

    // Disable "Remove Plot" if we have only one plot
    _ui.actionRemovePlot->setEnabled((_nBscans() > 1));
}

/*
 * Print a simple pixel dump of the UI's plot frame.
 */
void
BscanMainWindow::print() {
    // Ignore possible SIGPIPE when instantiating QPrintDialog
    struct sigaction oldAction;
    struct sigaction newAction;
    newAction.sa_handler = SIG_IGN;
    newAction.sa_flags = 0;
    sigemptyset(&(newAction.sa_mask));
    if (sigaction(SIGPIPE, &newAction, &oldAction) < 0) {
        std::cerr << __PRETTY_FUNCTION__ << 
                " failed to setup SIGPIPE ignore: " << strerror(errno) << 
                std::endl;
    }
    
    QPrintDialog printDialog(&_printer, this);
        
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
    
    // Restore the previous SIGPIPE handler
    sigaction(SIGPIPE, &oldAction, 0);
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

// static
std::string
BscanMainWindow::_makeSceneName(unsigned int index) {
    std::ostringstream sstream;
    sstream << "bscan_" << index;
    return(sstream.str());
}
