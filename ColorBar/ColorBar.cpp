#include "ColorBar.h"

#include <qlabel.h>
#include <qframe.h>

ColorBar::ColorBar(QWidget* parent, const char* name, WFlags fl):
ColorBarBase( parent, name, fl ),
_defaultMap(0.0, 10.0)
{
	_vLayout = new QVBoxLayout(_frame);
   configure(_defaultMap);
}

/******************************************************************/

ColorBar::~ColorBar()
{
	for (size_t i = 0; i < _widgets.size(); i++)
		delete _widgets[i];
	_widgets.clear();
   delete _vLayout;
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

	QFont fn("Helvetica",8);
	QLabel* l;

	// create a blank swatch to indicate the maximum of the scale
	// and label it
	l = new QLabel(" ", _frame);
	_widgets.push_back(l);
	l->setFont(fn);
	l->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	l->setText(QString("%1").arg(max,0,'f',1));

	// now do the swatches
	for (int i = map.size()-1; i >= 0 ; i--) {

		QColor color(map.red()[i], map.green()[i], map.blue()[i]);

		// label with the midpoint of the swatch
		l = new QLabel(" ", _frame);
		_widgets.push_back(l);
		l->setFont(fn);
		l->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
		double val = min + (i+0.5)*((max-min)/map.size());
		l->setText(QString(" %1").arg(val,0,'f',1));
		l->setBackgroundColor(color);	
		l->setPaletteForegroundColor(QColor(0,0,0));
	}

	// create a blank swatch to indicate the minimum of the scale
	// and label it
	l = new QLabel(" ", _frame);
	_widgets.push_back(l);
	l->setFont(fn);
	l->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	l->setText(QString("%1").arg(min,0,'f',1));

   for (size_t i = 0; i < _widgets.size(); i++) {
      _vLayout->addWidget(_widgets[i]);
		_widgets[i]->show();
   }
}

/******************************************************************/
