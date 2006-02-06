#include "Knob/KnobPlugin.h"
#include "Knob.h"

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

KnobPlugin::KnobPlugin()
{
}

QStringList KnobPlugin::keys() const
{
  QStringList list;
  list << "Knob";
  return list;
}

QWidget* KnobPlugin::create( const QString &key, QWidget* parent,
			      const char* name )
{
  if ( key == "Knob" )
    return new Knob( parent );
  return 0;
}

QString KnobPlugin::group( const QString& feature ) const
{
  if ( feature == "Knob" )
    return "Display";
  return QString::null;
}

QIconSet KnobPlugin::iconSet( const QString& ) const
{
  return QIconSet( QPixmap( filechooser_pixmap ) );
}

QString KnobPlugin::includeFile( const QString& feature ) const
{
  if ( feature == "Knob" )
    return "Knob/Knob.h";
  return QString::null;
}

QString KnobPlugin::toolTip( const QString& feature ) const
{
  if ( feature == "Knob" )
    return "Knob Widget";
  return QString::null;
}

QString KnobPlugin::whatsThis( const QString& feature ) const
{
  if ( feature == "Knob" )
    return "A widget with two knobs with lcd readouts";
  return QString::null;
}

bool KnobPlugin::isContainer( const QString& ) const
{
  return FALSE;
}


Q_EXPORT_PLUGIN( KnobPlugin )
