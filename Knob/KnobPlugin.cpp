#include "KnobPlugin.h"
#include "Knob.h"

KnobPlugin::KnobPlugin(QObject *parent)
{
	initialized = false;
}

void
KnobPlugin::initialize(QDesignerFormEditorInterface *core)
{
	if (initialized)
		return;

	initialized = true;
}

bool 
KnobPlugin::isInitialized() const
{
	return initialized;
}

QWidget*
KnobPlugin::createWidget(QWidget *parent)
{
	return new Knob(parent);
}

QString 
KnobPlugin::name() const
{
	return "Knob";
}

QString 
KnobPlugin::group() const
{
	return "QtToolbox";
}

QIcon 
KnobPlugin::icon() const
{
	return QIcon();
}

QString 
KnobPlugin::toolTip() const
{
	return "A knob";
}

QString KnobPlugin::whatsThis() const
{
	return "A knob";
}

bool KnobPlugin::isContainer() const
{
	return false;
}


QString 
KnobPlugin::domXml() const
{
	return "<widget class=\"Knob\" name=\"knob\">\n"
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
KnobPlugin::includeFile() const
{
	return "Knob.h";
}

Q_EXPORT_PLUGIN2(QtToolbox, KnobPlugin)

