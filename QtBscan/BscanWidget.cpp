// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=* 
// ** Copyright UCAR (c) 1990 - 2016                                         
// ** University Corporation for Atmospheric Research (UCAR)                 
// ** National Center for Atmospheric Research (NCAR)                        
// ** Boulder, Colorado, USA                                                 
// ** BSD licence applies - redistribution and use in source and binary      
// ** forms, with or without modification, are permitted provided that       
// ** the following conditions are met:                                      
// ** 1) If the software is modified to produce derivative works,            
// ** such modified software should be clearly marked, so as not             
// ** to confuse it with the version available from UCAR.                    
// ** 2) Redistributions of source code must retain the above copyright      
// ** notice, this list of conditions and the following disclaimer.          
// ** 3) Redistributions in binary form must reproduce the above copyright   
// ** notice, this list of conditions and the following disclaimer in the    
// ** documentation and/or other materials provided with the distribution.   
// ** 4) Neither the name of UCAR nor the names of its contributors,         
// ** if any, may be used to endorse or promote products derived from        
// ** this software without specific prior written permission.               
// ** DISCLAIMER: THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS  
// ** OR IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED      
// ** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.    
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=* 
/*
 * BscanWidget.cpp
 *
 *  Created on: Dec 7, 2009
 *      Author: burghart
 */

#include "BscanWidget.h"
#include <cmath>
#include <QtWidgets>

// semi-transparent PNG image of a "pause" marker
static const uchar PauseImageData[] = {
        137, 80, 78, 71, 13, 10, 26, 10, 0, 0, 0, 13, 73, 72, 68, 82, 0, 0, 0,
        32, 0, 0, 0, 32, 8, 6, 0, 0, 0, 115, 122, 122, 244, 0, 0, 0, 1, 115, 82,
        71, 66, 0, 174, 206, 28, 233, 0, 0, 0, 6, 98, 75, 71, 68, 0, 255, 0,
        255, 0, 255, 160, 189, 167, 147, 0, 0, 0, 9, 112, 72, 89, 115, 0, 0, 11,
        19, 0, 0, 11, 19, 1, 0, 154, 156, 24, 0, 0, 0, 7, 116, 73, 77, 69, 7,
        217, 12, 30, 15, 37, 2, 129, 114, 220, 28, 0, 0, 0, 25, 116, 69, 88,
        116, 67, 111, 109, 109, 101, 110, 116, 0, 67, 114, 101, 97, 116, 101,
        100, 32, 119, 105, 116, 104, 32, 71, 73, 77, 80, 87, 129, 14, 23, 0, 0,
        2, 180, 73, 68, 65, 84, 88, 195, 237, 151, 203, 78, 235, 58, 20, 134,
        191, 56, 151, 198, 18, 138, 2, 180, 92, 234, 1, 99, 30, 40, 234, 75, 68,
        234, 19, 32, 109, 137, 65, 199, 145, 120, 9, 212, 199, 65, 85, 6, 208,
        65, 6, 73, 58, 66, 136, 1, 84, 45, 141, 207, 96, 203, 217, 13, 39, 129,
        170, 218, 112, 206, 128, 37, 89, 142, 252, 199, 107, 253, 94, 190, 253,
        182, 180, 214, 246, 235, 235, 43, 73, 146, 92, 221, 220, 220, 144, 231,
        57, 95, 105, 74, 41, 226, 56, 102, 60, 30, 95, 75, 41, 177, 94, 94, 94,
        236, 203, 203, 203, 171, 44, 203, 248, 78, 187, 184, 184, 32, 77, 211,
        107, 145, 36, 73, 29, 60, 138, 34, 138, 162, 160, 170, 42, 180, 214,
        127, 181, 84, 85, 69, 81, 20, 68, 81, 4, 64, 150, 101, 36, 73, 114, 101,
        41, 165, 126, 229, 121, 78, 20, 69, 220, 222, 222, 98, 89, 214, 151,
        142, 92, 107, 205, 104, 52, 98, 58, 157, 162, 148, 194, 2, 126, 1, 20,
        69, 193, 249, 249, 249, 183, 164, 191, 44, 75, 134, 195, 33, 0, 194, 52,
        158, 157, 157, 125, 219, 252, 111, 199, 114, 204, 199, 46, 169, 215, 90,
        55, 106, 211, 199, 212, 159, 225, 109, 177, 156, 93, 89, 155, 133, 185,
        217, 108, 26, 1, 108, 219, 110, 16, 232, 194, 133, 16, 173, 126, 157,
        93, 131, 219, 182, 221, 104, 43, 138, 2, 207, 243, 232, 245, 122, 117,
        221, 134, 251, 190, 143, 231, 121, 191, 231, 187, 133, 132, 216, 37,
        237, 102, 68, 219, 54, 159, 207, 201, 243, 156, 167, 167, 39, 150, 203,
        229, 135, 248, 106, 181, 234, 244, 179, 83, 6, 54, 155, 205, 191, 218,
        102, 179, 25, 39, 39, 39, 84, 85, 213, 58, 50, 131, 107, 173, 233, 245,
        122, 184, 174, 219, 250, 159, 179, 111, 6, 30, 30, 30, 88, 175, 215,
        132, 97, 72, 16, 4, 157, 248, 209, 209, 17, 131, 193, 160, 246, 243,
        126, 65, 138, 93, 15, 143, 247, 246, 248, 248, 200, 243, 243, 51, 203,
        229, 146, 183, 183, 183, 79, 241, 170, 170, 90, 125, 239, 77, 96, 189,
        94, 215, 142, 219, 156, 27, 124, 123, 87, 236, 77, 160, 107, 103, 152,
        173, 249, 17, 254, 153, 9, 254, 99, 251, 33, 240, 67, 224, 135, 192,
        222, 4, 132, 16, 8, 33, 58, 117, 132, 193, 255, 10, 129, 182, 32, 174,
        235, 226, 56, 78, 103, 32, 131, 111, 235, 133, 189, 245, 128, 101, 89,
        228, 121, 206, 253, 253, 61, 119, 119, 119, 24, 21, 29, 4, 1, 190, 239,
        227, 56, 14, 243, 249, 156, 44, 203, 152, 205, 102, 173, 248, 222, 130,
        196, 178, 44, 44, 203, 194, 243, 60, 194, 48, 68, 41, 133, 227, 252,
        238, 118, 122, 122, 202, 225, 225, 33, 190, 239, 3, 16, 134, 33, 195,
        225, 176, 129, 135, 97, 136, 231, 121, 181, 159, 78, 2, 109, 87, 165,
        49, 219, 182, 145, 82, 210, 239, 247, 1, 234, 58, 8, 2, 142, 143, 143,
        57, 56, 56, 168, 207, 127, 33, 4, 131, 193, 160, 198, 251, 253, 62, 82,
        202, 134, 162, 218, 190, 63, 106, 2, 139, 197, 162, 85, 150, 191, 207,
        128, 239, 251, 172, 86, 43, 128, 134, 36, 51, 11, 79, 74, 217, 192, 141,
        36, 219, 206, 192, 98, 177, 248, 67, 64, 41, 69, 158, 231, 140, 199,
        227, 206, 135, 137, 153, 63, 223, 247, 113, 93, 183, 83, 148, 218, 182,
        141, 231, 121, 31, 138, 82, 173, 53, 227, 241, 184, 126, 39, 138, 56,
        142, 1, 152, 78, 167, 140, 70, 35, 202, 178, 108, 189, 98, 205, 106,
        119, 93, 183, 81, 76, 251, 103, 184, 214, 154, 178, 44, 235, 87, 17, 64,
        28, 199, 255, 131, 199, 169, 148, 146, 52, 77, 175, 39, 147, 9, 74, 169,
        47, 15, 172, 148, 98, 50, 153, 144, 166, 233, 181, 148, 146, 127, 0,
        235, 46, 163, 63, 58, 250, 228, 86, 0, 0, 0, 0, 73, 69, 78, 68, 174, 66,
        96, 130
};

static const QImage *PauseImage = 0;


BscanWidget::BscanWidget(QtConfig &config, std::string sceneName) :
    _view(),
    _scene(config, sceneName) {
    _init();
}

BscanWidget::~BscanWidget() {
}

void
BscanWidget::_init() {
    // Connect signals so we update annotation whenever something interesting
    // changes in the scene.
    connect(&_scene, SIGNAL(colorTableChanged()), this, SLOT(_buildAnnotation()));
    connect(&_scene, SIGNAL(displayVarChanged(QString)), this, SLOT(_buildAnnotation()));
    connect(&_scene, SIGNAL(pauseStateChanged(bool)), this, SLOT(_buildAnnotation()));
    connect(&_scene, SIGNAL(unitsChanged()), this, SLOT(_buildAnnotation()));
    
    _view.setScene(&_scene);
    
    _buildAnnotation();
    
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(&_annot);
    layout->addWidget(&_view);
    setLayout(layout);
}

//
// Fill our _annot annotation widget with var name, units, colorbar, etc.
//
void
BscanWidget::_buildAnnotation() {
    QImage annotImg(50, 200, QImage::Format_ARGB32);
    annotImg.fill(Qt::transparent);
    
    QPainter painter(&annotImg);
    
    // Go with 8-pixel text
    int textHeight = 8; // pixels
    QFont font = painter.font();
    font.setPixelSize(textHeight);
    painter.setFont(font);
    
    //
    // Draw each color from the color table as a single pixel in a 
    // (1 x nColors) QImage
    //
    const ColorTable &colorTable = *_scene.colorTable();
    const QList<QColor> colors = colorTable.colors();
    int nColors = colors.size();
    QImage colorbarImg(1, nColors, QImage::Format_ARGB32);
    for (int c = 0; c < nColors; c++) {
        // Index so that the first color in the table is at the bottom
        colorbarImg.setPixel(0, nColors - (c + 1), colors[c].rgb());
    }

    // Paint our colorbar into _annot, filling the rectangle defined by 
    // colorbarRect
    QRect colorbarRect(5, 12, 10, 176);
    painter.drawImage(colorbarRect, colorbarImg);
    
    // Put some value labels next to the colorbar
    double min = colorTable.minimumValue();
    double max = colorTable.maximumValue();
    double range = max - min;
    // Display enough precision in the values that the last decimal place shown
    // is <= (0.05 * range)
    int precision = 0;  // default to showing integer values
    double minShow = 0.05;  // last decimal place should be <= (minShow * range)
    if ((range * minShow) < 1.0)
        precision = int(ceilf(-log10(minShow * range)));
    QString valueString;
    valueString.setNum(colorTable.minimumValue(), 'f', precision);
    painter.drawText(16, 188 + textHeight / 2, valueString);
    valueString.setNum(colorTable.maximumValue(), 'f', precision);
    painter.drawText(16, 12 + textHeight / 2, valueString);
    
    //
    // 1x1 images with the too-high and too-low colors
    //
    QImage tooHighImg(1, 1, QImage::Format_ARGB32);
    tooHighImg.setPixel(0, 0, colorTable.tooHighColor().rgb());
    painter.drawImage(QRect(1, 1, 10, 4), tooHighImg);
    valueString = "HI";
    painter.drawText(11, 3 + textHeight / 2, valueString);
    
    QImage tooLowImg(1, 1, QImage::Format_ARGB32);
    tooLowImg.setPixel(0, 0, colorTable.tooLowColor().rgb());
    painter.drawImage(QRect(1, 195, 10, 4), tooLowImg);
    valueString = "LO";
    painter.drawText(11, 196 + textHeight / 2, valueString);

    // Rotated string with var name and units
    QString varString = _scene.displayVar() + " (" + 
        _scene.displayVarUnits() + ")";
    painter.setFont(QFont("Helvetica", 8));
    QRect textRect = painter.boundingRect(QRect(), 0, varString);
    
    painter.save();
    painter.rotate(-90);
    painter.drawText(-100 - textRect.width() / 2, 2 + textHeight, varString);
    painter.restore();
    
    // If we're paused, draw a semi-transparent "pause" symbol in the 
    // middle of our annotation.
    if (_scene.isPaused()) {
        if (! PauseImage)
            PauseImage = new QImage(QImage::fromData(PauseImageData, sizeof(PauseImageData)));
        painter.drawImage(annotImg.width() / 2 - PauseImage->width() / 2,
                annotImg.height() / 2 - PauseImage->height() / 2,
                *PauseImage);
    }
    
    _annot.setPixmap(QPixmap::fromImage(annotImg));
}

// Handle some key presses
void
BscanWidget::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    // On 'p', toggle our paused state
    case Qt::Key_P:
        _scene.setPaused(!_scene.isPaused());
        event->accept();
        break;
    // ignore everything else
    default:
        event->ignore();
    }
}

void
BscanWidget::addRay(const BscanRay & ray) {
    // Just pass this ray to our scene
    _scene.addRay(ray);
}

void
BscanWidget::copyRaysFrom(const BscanWidget & src) {
    _scene.copyRaysFrom(src._scene);
}
