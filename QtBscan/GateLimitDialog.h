/*
 * GateLimitDialog.h
 *
 *  Created on: Sep 3, 2009
 *      Author: burghart
 */

#ifndef GATELIMITDIALOG_H_
#define GATELIMITDIALOG_H_

#include <QtGui/QDialog>
#include "ui_GateLimitDialog.h"
class BscanGraphicsScene;

class GateLimitDialog : public QDialog {
    Q_OBJECT
public:
    GateLimitDialog(BscanGraphicsScene *scene);
public slots:
    int exec();
private slots:
    void accept();
    void initFromScene();
    void selectAllGates();
private:
    BscanGraphicsScene *_scene;
    Ui::GateLimitDialog _ui;
};
#endif /* GATELIMITDIALOG_H_ */
