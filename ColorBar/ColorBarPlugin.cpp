#include "ColorBar/ColorBarPlugin.h"
#include "ColorBar.h"

#include <qstring.h>

static const char *filechooser_pixmap[] = {
  "22 22 3 1",
  "       c #FFFF00000000",
  ".      c #0000FFFF0000",
  "X      c #00000000FFFF",
  "                      ",
  "                      ",
  "                      ",
  "                      ",
  "                      ",
  "                      ",
  "                      ",
  "                      ",
  "                      ",
  "......................",
  "......................",
  "......................",
  "......................",
  "......................",
  "......................",
  "......................",
  "XXXXXXXXXXXXXXXXXXXXXX",
  "XXXXXXXXXXXXXXXXXXXXXX",
  "XXXXXXXXXXXXXXXXXXXXXX",
  "XXXXXXXXXXXXXXXXXXXXXX",
  "XXXXXXXXXXXXXXXXXXXXXX",
  "XXXXXXXXXXXXXXXXXXXXXX"
};

ColorBarPlugin::ColorBarPlugin()
{
}

QStringList ColorBarPlugin::keys() const
{
  QStringList list;
  list << "ColorBar";
  return list;
}

QWidget* ColorBarPlugin::create( const QString &key, QWidget* parent,
			      const char* name )
{
  if ( key == "ColorBar" )
    return new ColorBar( parent );
  return 0;
}

QString ColorBarPlugin::group( const QString& feature ) const
{
  if ( feature == "ColorBar" )
    return "Display";
  return QString::null;
}

QIconSet ColorBarPlugin::iconSet( const QString& ) const
{
  return QIconSet( QPixmap( filechooser_pixmap ) );
}

QString ColorBarPlugin::includeFile( const QString& feature ) const
{
  if ( feature == "ColorBar" )
    return "ColorBar/ColorBar.h";
  return QString::null;
}

QString ColorBarPlugin::toolTip( const QString& feature ) const
{
  if ( feature == "ColorBar" )
    return "ColorBar Widget";
  return QString::null;
}

QString ColorBarPlugin::whatsThis( const QString& feature ) const
{
  if ( feature == "ColorBar" )
    return "A widget with two knobs with lcd readouts";
  return QString::null;
}

bool ColorBarPlugin::isContainer( const QString& ) const
{
  return FALSE;
}


Q_EXPORT_PLUGIN( ColorBarPlugin )
