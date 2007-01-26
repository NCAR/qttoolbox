#include "TwoKnobsPlugin.h"
#include "TwoKnobs.h"


TwoKnobsPlugin::TwoKnobsPlugin(QObject *parent)
{
	initialized = false;
}

void
TwoKnobsPlugin::initialize(QDesignerFormEditorInterface *core)
{
	if (initialized)
		return;

	initialized = true;
}

bool 
TwoKnobsPlugin::isInitialized() const
{
	return initialized;
}

QWidget*
TwoKnobsPlugin::createWidget(QWidget *parent)
{
	return new TwoKnobs(parent);
}

QString 
TwoKnobsPlugin::name() const
{
	return "TwoKnobs";
}

QString 
TwoKnobsPlugin::group() const
{
	return "QtToolbox";
}

QIcon 
TwoKnobsPlugin::icon() const
{
	return QIcon();
}

QString 
TwoKnobsPlugin::toolTip() const
{
	return "A TwoKnobs";
}

QString TwoKnobsPlugin::whatsThis() const
{
	return "A TwoKnobs";
}

bool TwoKnobsPlugin::isContainer() const
{
	return false;
}


QString 
TwoKnobsPlugin::domXml() const
{
	return "<widget class=\"TwoKnobs\" name=\"TwoKnobs\">\n"
		" <property name=\"geometry\">\n"
		"  <rect>\n"
		"   <x>0</x>\n"
		"   <y>0</y>\n"
		"   <width>100</width>\n"
		"   <height>100</height>\n"
		"  </rect>\n"
		" </property>\n"
		" <property name=\"toolTip\" >\n"
		"  <string>TwoKnobs</string>\n"
		" </property>\n"
		" <property name=\"whatsThis\" >\n"
		"  <string>A TwoKnobs that you can turn.</string>\n"
		" </property>\n"
		"</widget>\n";
}

QString 
TwoKnobsPlugin::includeFile() const
{
	return "TwoKnobs.h";
}

Q_EXPORT_PLUGIN2(QtToolbox, TwoKnobsPlugin)

