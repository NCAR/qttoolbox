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
#include "PPIPlugin.h"
#include "PPI.h"

PPIPlugin::PPIPlugin(QObject *parent)
{
	initialized = false;
}

void
PPIPlugin::initialize(QDesignerFormEditorInterface *core)
{
	if (initialized)
		return;

	initialized = true;
}

bool 
PPIPlugin::isInitialized() const
{
	return initialized;
}

QWidget*
PPIPlugin::createWidget(QWidget *parent)
{
	return new PPI(parent);
}

QString 
PPIPlugin::name() const
{
	return "PPI";
}

QString 
PPIPlugin::group() const
{
	return "QtToolbox";
}

QIcon 
PPIPlugin::icon() const
{
	return QIcon();
}

QString 
PPIPlugin::toolTip() const
{
	return "A PPI";
}

QString PPIPlugin::whatsThis() const
{
	return "A PPI";
}

bool PPIPlugin::isContainer() const
{
	return false;
}


QString 
PPIPlugin::domXml() const
{
	return "<widget class=\"PPI\" name=\"knob\">\n"
		" <property name=\"geometry\">\n"
		"  <rect>\n"
		"   <x>0</x>\n"
		"   <y>0</y>\n"
		"   <width>100</width>\n"
		"   <height>100</height>\n"
		"  </rect>\n"
		" </property>\n"
		" <property name=\"toolTip\" >\n"
		"  <string>knob</string>\n"
		" </property>\n"
		" <property name=\"whatsThis\" >\n"
		"  <string>A knob that you can turn.</string>\n"
		" </property>\n"
		"</widget>\n";
}

QString 
PPIPlugin::includeFile() const
{
	return "PPI.h";
}

Q_EXPORT_PLUGIN2(QtToolbox, PPIPlugin)

