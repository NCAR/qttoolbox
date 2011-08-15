/*
 * DisplayLimitDialog.h
 *
 *  Created on: Sep 3, 2009
 *      Author: burghart
 */

#ifndef DISPLAYLIMITDIALOG_H_
#define DISPLAYLIMITDIALOG_H_

#include <QtGui/QDialog>
#include "ui_DisplayLimitDialog.h"
class BscanGraphicsScene;

class DisplayLimitDialog : public QDialog {
    Q_OBJECT
public:
    DisplayLimitDialog(BscanGraphicsScene *scene);
public slots:
    int exec();
private slots:
    void accept();
    void initFromScene();
private:
    BscanGraphicsScene *_scene;
    Ui::DisplayLimitDialog _ui;
};
#endif /* DISPLAYLIMITDIALOG_H_ */
