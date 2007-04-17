#ifndef COLORBARH_
#define COLORBARH_

#include "ColorMap.h"

#include <qwidget.h>
#include <qlayout.h>
#include <QVBoxLayout>
#include <vector>
#include <QImage>
#include <QPixmap>

#ifndef DLL_EXPORT
#ifdef WIN32
#ifdef QT_PLUGIN
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif
#else
#define DLL_EXPORT
#endif
#endif

/// A widget that displays a representation of ColorMap.
/// Swatches are displayed in a vertical column, with text labels
/// indicating the value of the swatch.
class DLL_EXPORT ColorBar: public QWidget
{
	Q_OBJECT

public:
	ColorBar(QWidget* parent = 0);
	virtual ~ColorBar(void);

	/// Configure the ColorBar.
	/// @param map The corresponding color map.
	void configure(ColorMap& map);

	/// @returns An image of the color bar. The caller must delte
	/// it when finished.
	QImage* getImage();

	/// @returns A pixmap of the color bar. The caller must delete it 
	/// when finished.
	QPixmap* getPixmap();

signals:
	void released();

protected:
	/// Capture a mouse release and emit a released() signal.
	virtual void mouseReleaseEvent(QMouseEvent* e);
	/// (Re)make the color swatches and the labels.
	void makeSwatches(ColorMap& map, double min, double max);
	/// Container for the swatched and labels, so that 
	/// they can be deleted when the configuration is changed.
	std::vector<QWidget*> _widgets;
	/// A layout for the swatches
	QVBoxLayout* _swatchLayout;
	/// A default color map, so that the plugin can
	/// display something.
	ColorMap _defaultMap;
};

#endif
