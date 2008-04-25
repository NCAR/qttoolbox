#include "StatusGauge.h"

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPainter>
#include <QRectF>
#include <QPaintEvent>

StatusGauge::StatusGauge(QWidget* parent):
QWidget(parent)
{
	setMinimumSize(30, 100);
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

/******************************************************************/
StatusGauge::~StatusGauge()
{
}

/******************************************************************/
void
StatusGauge::paintEvent(QPaintEvent* e) {
	
	int h = e->rect().height();
	int w = e->rect().width();

	h = height();
	w = width();
	int deltaY = h/3;

	QRectF r(0, 0, w, deltaY);
	QPainter p;
	p.setPen(Qt::SolidLine);
	QStringList colors;
	colors.push_back("red");
	colors.push_back("yellow");
	colors.push_back("green");
	p.begin(this);
	for (unsigned int i = 0; i < 3; i++) {
		QColor color(colors[i]);
        p.setBrush(color);
		double topY = h-(i+1)*deltaY;
		QRectF r(0, topY, w, deltaY);
		// fill the swatch with the color
		p.fillRect(r, color);
	}
	p.end();

}

/******************************************************************/
void
StatusGauge::mouseReleaseEvent(QMouseEvent *e) {
	emit released();
}

/******************************************************************/
QImage*
StatusGauge::getImage()
{	
	QPixmap pixmap = QPixmap::grabWidget(this);
	QImage* image = new QImage(pixmap.toImage());
	return image;
}

/******************************************************************/
QPixmap*
StatusGauge::getPixmap()
{	
	QPixmap* pixmap = new QPixmap(QPixmap::grabWidget(this));
	return pixmap;
}
