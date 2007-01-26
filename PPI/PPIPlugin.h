#ifndef PPIPLUGIN_H_
#define PPIPLUGIN_H_


#include <QtPlugin>
#include <QDesignerCustomWidgetInterface>

///
/// Qt plugin class for PPI.
///
class PPIPlugin: public QObject, public QDesignerCustomWidgetInterface 
{
	Q_OBJECT
	Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
	PPIPlugin(QObject *parent = 0);
	bool isContainer() const;
	bool isInitialized() const;
	QIcon icon() const;
	QString domXml() const;
	QString group() const;
	QString includeFile() const;
	QString name() const;
	QString toolTip() const;
	QString whatsThis() const;
	QWidget *createWidget(QWidget *parent);
	void initialize(QDesignerFormEditorInterface *core);

private:
	bool initialized;
};


#endif
