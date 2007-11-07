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

