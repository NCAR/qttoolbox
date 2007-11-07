#include "ColorBarPlugin.h"
#include "ColorBar.h"

ColorBarPlugin::ColorBarPlugin(QObject *parent):
QObject(parent)
{
	initialized = false;
}

void
ColorBarPlugin::initialize(QDesignerFormEditorInterface *core)
{
	if (initialized)
		return;

	initialized = true;
}

bool 
ColorBarPlugin::isInitialized() const
{
	return initialized;
}

QWidget*
ColorBarPlugin::createWidget(QWidget *parent)
{
	ColorBar* p = new ColorBar(parent);
	return p;
}

QString 
ColorBarPlugin::name() const
{
	return "ColorBar";
}

QString 
ColorBarPlugin::codeTemplate() const
{
	return "";
}

QString 
ColorBarPlugin::group() const
{
	return "QtToolbox";
}

QIcon 
ColorBarPlugin::icon() const
{
	return QIcon();
}

QString 
ColorBarPlugin::toolTip() const
{
	return "";
}

QString ColorBarPlugin::whatsThis() const
{
	return "";
}

bool ColorBarPlugin::isContainer() const
{
	return false;
}


QString 
ColorBarPlugin::domXml() const
{
	return "<widget class=\"ColorBar\" name=\"colorbar\">\n"
		" <property name=\"geometry\">\n"
		"  <rect>\n"
		"   <x>0</x>\n"
		"   <y>0</y>\n"
		"   <width>50</width>\n"
		"   <height>300</height>\n"
		"  </rect>\n"
		" </property>\n"
		"</widget>\n";
}

QString 
ColorBarPlugin::includeFile() const
{
	return "ColorBar.h";
}

Q_EXPORT_PLUGIN2(colorbarplugin, ColorBarPlugin)

