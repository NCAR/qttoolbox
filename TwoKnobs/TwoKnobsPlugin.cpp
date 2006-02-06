#include "TwoKnobs/TwoKnobsPlugin.h"
#include "TwoKnobs.h"

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

TwoKnobsPlugin::TwoKnobsPlugin()
{
}

QStringList TwoKnobsPlugin::keys() const
{
  QStringList list;
  list << "TwoKnobs";
  return list;
}

QWidget* TwoKnobsPlugin::create( const QString &key, QWidget* parent,
			      const char* name )
{
  if ( key == "TwoKnobs" )
    return new TwoKnobs( parent );
  return 0;
}

QString TwoKnobsPlugin::group( const QString& feature ) const
{
  if ( feature == "TwoKnobs" )
    return "Display";
  return QString::null;
}

QIconSet TwoKnobsPlugin::iconSet( const QString& ) const
{
  return QIconSet( QPixmap( filechooser_pixmap ) );
}

QString TwoKnobsPlugin::includeFile( const QString& feature ) const
{
  if ( feature == "TwoKnobs" )
    return "TwoKnobs/TwoKnobs.h";
  return QString::null;
}

QString TwoKnobsPlugin::toolTip( const QString& feature ) const
{
  if ( feature == "TwoKnobs" )
    return "TwoKnobs Widget";
  return QString::null;
}

QString TwoKnobsPlugin::whatsThis( const QString& feature ) const
{
  if ( feature == "TwoKnobs" )
    return "A widget with two knobs with lcd readouts";
  return QString::null;
}

bool TwoKnobsPlugin::isContainer( const QString& ) const
{
  return FALSE;
}


Q_EXPORT_PLUGIN( TwoKnobsPlugin )
