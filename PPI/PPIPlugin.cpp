#include "PPIPlugin.h"
#include "PPI.h"

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

PPIPlugin::PPIPlugin()
{
}

QStringList PPIPlugin::keys() const
{
  QStringList list;
  list << "PPI";
  return list;
}

QWidget* PPIPlugin::create( const QString &key, QWidget* parent,
			      const char* name )
{
  if ( key == "PPI" )
    return new PPI( parent );
  return 0;
}

QString PPIPlugin::group( const QString& feature ) const
{
  if ( feature == "PPI" )
    return "Display";
  return QString::null;
}

QIconSet PPIPlugin::iconSet( const QString& ) const
{
  return QIconSet( QPixmap( filechooser_pixmap ) );
}

QString PPIPlugin::includeFile( const QString& feature ) const
{
  if ( feature == "PPI" )
    return "PPI/PPI.h";
  return QString::null;
}

QString PPIPlugin::toolTip( const QString& feature ) const
{
  if ( feature == "PPI" )
    return "PPI Widget";
  return QString::null;
}

QString PPIPlugin::whatsThis( const QString& feature ) const
{
  if ( feature == "PPI" )
    return "A widget with two knobs with lcd readouts";
  return QString::null;
}

bool PPIPlugin::isContainer( const QString& ) const
{
  return FALSE;
}


Q_EXPORT_PLUGIN( PPIPlugin )
