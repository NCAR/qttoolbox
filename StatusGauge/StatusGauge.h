#ifndef STATUSGAUGEH_
#define STATUSGAUGEH_

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

/// A widget that displays a representation of .
class DLL_EXPORT StatusGauge: public QWidget
{
	Q_OBJECT

public:
	StatusGauge(QWidget* parent = 0);
	virtual ~StatusGauge(void);

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
	/// The paint event is where we will draw the color bar.
	virtual void paintEvent(QPaintEvent* e);
};

#endif
