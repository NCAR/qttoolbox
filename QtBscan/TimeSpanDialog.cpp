/*
 * TimeSpanDialog.cpp
 *
 *  Created on: Sep 3, 2009
 *      Author: burghart
 */
#include <QtGui/QMessageBox>

#include "TimeSpanDialog.h"
#include "BscanGraphicsScene.h"
#include "ui_TimeSpanDialog.h"

TimeSpanDialog::TimeSpanDialog(BscanGraphicsScene* scene) : _scene(scene) {
    _ui.setupUi(this);
}

int
TimeSpanDialog::exec() {
    initFromScene();
    return QDialog::exec();
}

void
TimeSpanDialog::accept() {
    _scene->setTimeSpan(_ui.timeSpanSpinBox->value());
    done(0);
}

void
TimeSpanDialog::initFromScene() {
    _ui.timeSpanSpinBox->setValue(_scene->timeSpan());
}
