#include "StatusGaugePlugin.h"
#include "StatusGauge.h"

StatusGaugePlugin::StatusGaugePlugin(QObject *parent):
QObject(parent)
{
	initialized = false;
}

void
StatusGaugePlugin::initialize(QDesignerFormEditorInterface *core)
{
	if (initialized)
		return;

	initialized = true;
}

bool 
StatusGaugePlugin::isInitialized() const
{
	return initialized;
}

QWidget*
StatusGaugePlugin::createWidget(QWidget *parent)
{
	StatusGauge* p = new StatusGauge(parent);
	return p;
}

QString 
StatusGaugePlugin::name() const
{
	return "StatusGauge";
}

QString 
StatusGaugePlugin::codeTemplate() const
{
	return "";
}

QString 
StatusGaugePlugin::group() const
{
	return "QtToolbox";
}

QIcon 
StatusGaugePlugin::icon() const
{
	return QIcon();
}

QString 
StatusGaugePlugin::toolTip() const
{
	return "";
}

QString StatusGaugePlugin::whatsThis() const
{
	return "";
}

bool StatusGaugePlugin::isContainer() const
{
	return false;
}


QString 
StatusGaugePlugin::domXml() const
{
	return "<widget class=\"StatusGauge\" name=\"statusGauge\">\n"
		" <property name=\"geometry\">\n"
		"  <rect>\n"
		"   <x>0</x>\n"
		"   <y>0</y>\n"
		"   <width>20</width>\n"
		"   <height>60</height>\n"
		"  </rect>\n"
		" </property>\n"
		"</widget>\n";
}

QString 
StatusGaugePlugin::includeFile() const
{
	return "StatusGauge.h";
}

Q_EXPORT_PLUGIN2(StatusGaugeplugin, StatusGaugePlugin)

