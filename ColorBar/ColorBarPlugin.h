#ifndef ColorBarPLUGIN_H_
#define ColorBarPLUGIN_H_

#include <qwidgetplugin.h>

///
/// Qt plugin class for ColorBar.
///
class ColorBarPlugin: public QWidgetPlugin {
public:
  ColorBarPlugin();
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
