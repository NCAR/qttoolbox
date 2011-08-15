/*
 * BscanMainWindow.h
 *
 *  Created on: Sep 23, 2009
 *      Author: burghart
 */

#ifndef BSCANMAINWINDOW_H_
#define BSCANMAINWINDOW_H_

#include <vector>
#include "BscanWidget.h"
#include "GateLimitDialog.h"
#include "TimeSpanDialog.h"
#include "ui_Bscan.h"

#include <QLabel>
#include <QMainWindow>
#include <QObject>
#include <QPrinter>

class QtProductReader;
class BscanGraphicsScene;

/**
 * Main window class for a bscan display.
 * Appropriate connections to the data thread are made automatically.
 */
class BscanMainWindow : public QMainWindow {
    Q_OBJECT
public:
    BscanMainWindow(QtProductReader *productReader);
    bool isPaused() const;
public slots:
    /**
     * Add a new plot to the model.  This creates a 
     * BscanGraphicsView/BscanGraphicsScene pair and adds the view to 
     * the UI's plot frame.
     */
    void addNewPlot();
    /**
     * Remove the last (bottom) plot from the model.
     */
    void removePlot();
    /**
     * Print the UI's plot frame, popping up a dialog for selecting the
     * print device.
     */
    void print();
    /**
     * Set the paused state for the model.
     * @param state the new paused state
     */
    void setPaused(bool state);
    /**
     * Pause the display(s).
     */
    void pause();
    /**
     * Unpause the display(s).
     */
    void unpause();
    /**
     * Set zoom level, where a zoom of 1 fits the entire scene on the display.
     * @param zoom the zoom factor
     */
    void setZoom(float zoom);
protected slots:
    void _showLocAndData(double time, unsigned int gate, const QString &varName, 
            float value);
    /**
     * Slots which are automatically connected to actions in the UI
     */
    void on_actionPrint_triggered() { print(); }
    void on_actionAddPlot_triggered() { addNewPlot(); }
    void on_actionRemovePlot_triggered() { removePlot(); }
    void on_actionPause_triggered() { pause(); }
    void on_actionUnpause_triggered() { unpause(); }
    void on_actionZoom_100_triggered() { setZoom(1.00); }
    void on_actionZoom_150_triggered() { setZoom(1.50); }
    void on_actionZoom_200_triggered() { setZoom(2.00); }
    void on_actionZoom_300_triggered() { setZoom(3.00); }
    void on_actionQuit_triggered() { qApp->closeAllWindows(); }
    void on_actionSetGateRange_triggered() { _glDialog->exec(); }
    void on_actionSetTimeSpan_triggered() { _tsDialog->exec(); }
private:
    BscanGraphicsView* _view(int index) const { return _bscans[index]->view(); }
    BscanGraphicsScene* _scene(int index) const { return _view(index)->scene(); }
    int _nBscans() const { return _bscans.size(); }
private:
    Ui::Bscan _ui;
    GateLimitDialog *_glDialog;
    TimeSpanDialog *_tsDialog;
    QtProductReader *_productReader;
    std::vector<BscanWidget*> _bscans;
    BscanWidgetGroup _bscanGroup;
    // If a printer is chosen via the dialog popped up during print(), keep
    // it around so that the user will get the same choice by default next
    // time.
    QPrinter _printer;
    QLabel _mousePosLabel;
};

#endif /* BSCANMAINWINDOW_H_ */
