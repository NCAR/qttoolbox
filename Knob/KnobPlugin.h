#ifndef KnobPLUGIN_H_
#define KnobPLUGIN_H_

#include <qwidgetplugin.h>

///
/// Qt plugin class for Knob.
///
class KnobPlugin: public QWidgetPlugin {
public:
  KnobPlugin();
  QStringList keys() const;
  QWidget* create( const QString &classname, 
         QWidget* parent = 0,
		   const char* name = 0 );
  QString group( const QString& ) const;
  QIconSet iconSet( const QString& ) const;
  QString includeFile( const QString& ) const;
  QString toolTip( const QString& ) const;
  QString whatsThis( const QString& ) const;
  bool isContainer( const QString& ) const;
protected:
};

#endif