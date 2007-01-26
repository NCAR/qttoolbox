#ifndef ColorBarPLUGIN_H_
#define ColorBarPLUGIN_H_
#include <QtPlugin>
#include <QDesignerCustomWidgetInterface>

///
/// Qt plugin class for ColorBar.
///
class ColorBarPlugin: public QObject, public QDesignerCustomWidgetInterface 
{
	Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
  ColorBarPlugin(QObject *parent = 0);
	 bool isContainer() const;
     bool isInitialized() const;
     QIcon icon() const;
	 QString codeTemplate() const;
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
