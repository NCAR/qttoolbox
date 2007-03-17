#include "ColorBar.h"

#include <qwidget.h>
#include <qlabel.h>
#include <QVBoxLayout>

ColorBar::ColorBar(QWidget* parent):
QWidget(parent),
_defaultMap(0.0, 10.0)
{
	setMinimumSize(30, 100);
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	_swatchLayout = new QVBoxLayout(this);
	configure(_defaultMap);
}

/******************************************************************/
ColorBar::~ColorBar()
{
	for (size_t i = 0; i < _widgets.size(); i++)
		delete _widgets[i];
	_widgets.clear();
	delete _swatchLayout;
}

/******************************************************************/
void
ColorBar::configure(ColorMap& map) {
	makeSwatches(map, map.rangeMin(), map.rangeMax());
}

/******************************************************************/

void
ColorBar::makeSwatches (ColorMap& map, double min, double max) 
{
// get rid of the existing swatches and labels
	for (size_t i = 0; i < _widgets.size(); i++)
		delete _widgets[i];
	_widgets.clear();

	QFont fn("Helvetica",7);
	QLabel* l;

	// create a blank swatch to indicate the maximum of the scale
	// and label it
	l = new QLabel(" ", this);
	_widgets.push_back(l);
	l->setFont(fn);
	l->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	l->setText(QString("%1").arg(max,0,'f',1));

	// now do the swatches
	for (int i = map.size()-1; i >= 0 ; i--) {

		QColor color(map.red()[i], map.green()[i], map.blue()[i]);

		// label with the midpoint of the swatch
		l = new QLabel(" ", this);
		_widgets.push_back(l);
		QPalette palette = l->palette();
		palette.setColor(QPalette::Active, QPalette::Background, color);
		l->setPalette(palette);
		l->setAutoFillBackground(true);
		l->setFont(fn);
		l->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
		double val = min + (i+0.5)*((max-min)/map.size());
		l->setText(QString(" %1").arg(val,0,'f',1));
	}

	// create a blank swatch to indicate the minimum of the scale
	// and label it
	l = new QLabel(" ", this);
	_widgets.push_back(l);
	l->setFont(fn);
	l->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	l->setText(QString("%1").arg(min,0,'f',1));

	for (size_t i = 0; i < _widgets.size(); i++) {
		_swatchLayout->addWidget(_widgets[i]);
		_widgets[i]->show();
	}
}

/******************************************************************/
void
ColorBar::mouseReleaseEvent(QMouseEvent *e) {
	emit released();
}

/******************************************************************/
