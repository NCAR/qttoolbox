#include "ScopePlotPlugin.h"
#include "ScopePlot.h"

ScopePlotPlugin::ScopePlotPlugin(QObject *parent)
{
	initialized = false;
}

void
ScopePlotPlugin::initialize(QDesignerFormEditorInterface *core)
{
	if (initialized)
		return;

	initialized = true;
}

bool 
ScopePlotPlugin::isInitialized() const
{
	return initialized;
}

QWidget*
ScopePlotPlugin::createWidget(QWidget *parent)
{
	return new ScopePlot(parent);
}

QString 
ScopePlotPlugin::name() const
{
	return "ScopePlot";
}

QString 
ScopePlotPlugin::group() const
{
	return "QtToolbox";
}

QIcon 
ScopePlotPlugin::icon() const
{
	return QIcon();
}

QString 
ScopePlotPlugin::toolTip() const
{
	return "A ScopePlot";
}

QString ScopePlotPlugin::whatsThis() const
{
	return "A ScopePlot";
}

bool ScopePlotPlugin::isContainer() const
{
	return false;
}


QString 
ScopePlotPlugin::domXml() const
{
	return "<widget class=\"ScopePlot\" name=\"scopePlot\">\n"
		" <property name=\"geometry\">\n"
		"  <rect>\n"
		"   <x>0</x>\n"
		"   <y>0</y>\n"
		"   <width>200</width>\n"
		"   <height>200</height>\n"
		"  </rect>\n"
		" </property>\n"
		"</widget>\n";
}

QString 
ScopePlotPlugin::includeFile() const
{
	return "ScopePlot.h";
}

Q_EXPORT_PLUGIN2(QtToolbox, ScopePlotPlugin)

