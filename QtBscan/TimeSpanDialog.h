/*
 * TimeSpanDialog.h
 *
 *  Created on: Sep 3, 2009
 *      Author: burghart
 */

#ifndef TIMESPANDIALOG_H_
#define TIMESPANDIALOG_H_

#include <QtGui/QDialog>
#include "ui_TimeSpanDialog.h"
class BscanGraphicsScene;

class TimeSpanDialog : public QDialog {
    Q_OBJECT
public:
    TimeSpanDialog(BscanGraphicsScene *scene);
public slots:
    int exec();
private slots:
    void accept();
    void initFromScene();
private:
    BscanGraphicsScene *_scene;
    Ui::TimeSpanDialog _ui;
};
#endif /* TIMESPANDIALOG_H_ */
