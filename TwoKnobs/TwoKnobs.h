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
#ifndef TWOKNOBS_H
#define TWOKNOBS_H

#include "ui_TwoKnobs.h"
#include <string>

#ifndef DLL_EXPORT
#ifdef WIN32
#ifdef QT_PLUGIN
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif
#else
#define DLL_EXPORT
#endif
#endif

/// A plugin widget which combines two Knob plugins. Signals
/// are generated when the knob values change.
class DLL_EXPORT TwoKnobs : public QWidget, public Ui::TwoKnobsBase
{
    Q_OBJECT

public:
    TwoKnobs( QWidget* parent = 0);
    ~TwoKnobs();

    void setTitles(std::string titleKnobOne, std::string titleKnobTwo);
    void setRanges(double knobOneMin, double knobOneMax,
       double knobTwoMin, double knobTwoMax);
	void setValues(double  val1, double val2);
	void setScaleMaxMajor(int knobNum, int ticks);
	void setScaleMaxMinor(int knobNum, int ticks);
    void getRanges(double& knobOneMin, double& knobOneMax,
       double& knobTwoMin, double& knobTwoMax);
	void getValues(double& val1, double& val2);

public slots:
    virtual void knobOneChangedSlot(double);
    virtual void knobTwoChangedSlot(double);

signals:
    void knobOneChanged(double);
    void knobTwoChanged(double);

};

#endif // TWOKNOBS_H
