#include "ColorBar.h"

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPainter>
#include <QRectF>
#include <QPaintEvent>

ColorBar::ColorBar(QWidget* parent):
QWidget(parent),
_colorMap(0.0, 10.0, "default")
{
	setMinimumSize(30, 100);
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	configure(_colorMap);
}

/******************************************************************/
ColorBar::~ColorBar()
{
}

/******************************************************************/
void
ColorBar::configure(ColorMap& map) {
	_colorMap = map;
	this->update();
}

/******************************************************************/
void
ColorBar::paintEvent(QPaintEvent* e) {
	
	if (_colorMap.size() == 0)
		return;
	
	int h = e->rect().height();
	int w = e->rect().width();

	h = height();
	w = width();

	double deltaVal = (_colorMap.rangeMax() - _colorMap.rangeMin())/_colorMap.size();
	double deltaY = (double)(height())/_colorMap.size();
	QRectF r(0, 0, w, deltaY);
	QPainter p;
	p.setPen(Qt::SolidLine);
	p.begin(this);
	for (unsigned int i = 0; i < _colorMap.size(); i++) {
		QColor color(_colorMap.red()[i], _colorMap.green()[i], _colorMap.blue()[i]);
        p.setBrush(color);
		double topY = h-(i+1)*deltaY;
		QRectF r(0, topY, w, deltaY);
		// fill the swatch with the color
		p.fillRect(r, color);
		// add the label
		double midValue = _colorMap.rangeMin()
			+ deltaVal*(i+0.5);
		QString label = QString("%1").arg(midValue,0,'f',1);
		p.drawText(0, (int)topY, w, (int)deltaY, 
			Qt::AlignVCenter | Qt::AlignHCenter, 
			label);
	}
	p.end();

}

/******************************************************************/
void
ColorBar::mouseReleaseEvent(QMouseEvent *e) {
	emit released();
}

/******************************************************************/
QImage*
ColorBar::getImage()
{	
	QPixmap pixmap = QPixmap::grabWidget(this);
	QImage* image = new QImage(pixmap.toImage());
	return image;
}

/******************************************************************/
QPixmap*
ColorBar::getPixmap()
{	
	QPixmap* pixmap = new QPixmap(QPixmap::grabWidget(this));
	return pixmap;
}
