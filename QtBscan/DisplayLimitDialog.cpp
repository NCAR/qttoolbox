/*
 * DisplayLimitDialog.cpp
 *
 *  Created on: Sep 3, 2009
 *      Author: burghart
 */
#include <QtGui/QMessageBox>

#include "DisplayLimitDialog.h"
#include "BscanGraphicsScene.h"
#include "ui_DisplayLimitDialog.h"

DisplayLimitDialog::DisplayLimitDialog(BscanGraphicsScene* scene) : _scene(scene) {
    _ui.setupUi(this);
}

int
DisplayLimitDialog::exec() {
    initFromScene();
    return QDialog::exec();
}

void
DisplayLimitDialog::accept() {
    double min = _ui.minSpinBox->value();
    double max = _ui.maxSpinBox->value();
    if (min < max) {
        _scene->setDisplayLimits(min, max);
        done(0);
    } else {
        QMessageBox msgBox;
        msgBox.setText("Min must be less than max!");
        msgBox.exec();
    }
}

void
DisplayLimitDialog::initFromScene() {
    _ui.minSpinBox->setMinimum(-10000.0);
    _ui.minSpinBox->setMaximum(10000.0);
    _ui.maxSpinBox->setMinimum(-10000.0);
    _ui.maxSpinBox->setMaximum(10000.0);
    _ui.minSpinBox->setValue(_scene->colorTable()->minimumValue());
    _ui.maxSpinBox->setValue(_scene->colorTable()->maximumValue());
}
