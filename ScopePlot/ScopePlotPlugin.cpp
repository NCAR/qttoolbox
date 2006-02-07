#include "ScopePlot/ScopePlotPlugin.h"
#include "ScopePlot.h"

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

ScopePlotPlugin::ScopePlotPlugin()
{
}

QStringList ScopePlotPlugin::keys() const
{
  QStringList list;
  list << "ScopePlot";
  return list;
}

QWidget* ScopePlotPlugin::create( const QString &key, QWidget* parent,
			      const char* name )
{
  if ( key == "ScopePlot" )
    return new ScopePlot( parent );
  return 0;
}

QString ScopePlotPlugin::group( const QString& feature ) const
{
  if ( feature == "ScopePlot" )
    return "Display";
  return QString::null;
}

QIconSet ScopePlotPlugin::iconSet( const QString& ) const
{
  return QIconSet( QPixmap( filechooser_pixmap ) );
}

QString ScopePlotPlugin::includeFile( const QString& feature ) const
{
  if ( feature == "ScopePlot" )
    return "ScopePlot/ScopePlot.h";
  return QString::null;
}

QString ScopePlotPlugin::toolTip( const QString& feature ) const
{
  if ( feature == "ScopePlot" )
    return "ScopePlot Widget";
  return QString::null;
}

QString ScopePlotPlugin::whatsThis( const QString& feature ) const
{
  if ( feature == "ScopePlot" )
    return "A widget with two knobs with lcd readouts";
  return QString::null;
}

bool ScopePlotPlugin::isContainer( const QString& ) const
{
  return FALSE;
}


Q_EXPORT_PLUGIN( ScopePlotPlugin )
