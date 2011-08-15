/*
 * GateLimitDialog.cpp
 *
 *  Created on: Sep 3, 2009
 *      Author: burghart
 */
#include <QtGui/QMessageBox>

#include "GateLimitDialog.h"
#include "BscanGraphicsScene.h"
#include "ui_GateLimitDialog.h"

GateLimitDialog::GateLimitDialog(BscanGraphicsScene* scene) : _scene(scene) {
    _ui.setupUi(this);
    connect(_ui.resetButton, SIGNAL(clicked()), this, SLOT(initFromScene()));
    connect(_ui.showAllButton, SIGNAL(clicked()), this, SLOT(selectAllGates()));
}

int
GateLimitDialog::exec() {
    initFromScene();
    return QDialog::exec();
}

void
GateLimitDialog::accept() {
    int min = _ui.minGateSpinBox->value();
    int max = _ui.maxGateSpinBox->value();
    if (min <= max) {
        _scene->setGateLimits(min, max);
        done(0);
    } else {
        QMessageBox msgBox;
        msgBox.setText("Min gate must be <= max gate!");
        msgBox.exec();
    }
}

void
GateLimitDialog::initFromScene() {
    _ui.minGateSpinBox->setMaximum(_scene->nGates());
    _ui.maxGateSpinBox->setMaximum(_scene->nGates());
    _ui.minGateSpinBox->setValue(_scene->minGate());
    _ui.maxGateSpinBox->setValue(_scene->maxGate());
}

void
GateLimitDialog::selectAllGates() {
    _ui.minGateSpinBox->setValue(0);
    _ui.maxGateSpinBox->setValue(_scene->nGates());
}
