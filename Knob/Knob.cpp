#include "Knob.h"
#include <qlabel.h>
#include <qwt_knob.h>

Knob::Knob( QWidget* parent):
QWidget(parent)
{
	setupUi(this);
	connect(_knob, SIGNAL(valueChanged(double)), this, SLOT(valueChangedSlot(double)));
}

/*
 *  Destroys the object and frees any allocated resources
 */
Knob::~Knob()
{
    // no need to delete child widgets, Qt does it all for us
}

void
Knob::setTitle(std::string title)
{
   _label->setText(title.c_str());
}

void
Knob::setRange(double min, double max)
{
#if (QWT_VERSION < 0x060100)
   _knob->setRange(min, max);
#else
   _knob->setScale(min, max);
#endif
}

void
Knob::setValue(double val)
{
	_knob->setValue(val);
}

void
Knob::valueChangedSlot(double v)
{
   emit valueChanged(v);
}

void 
Knob::setScaleMaxMajor(int ticks)
{
	_knob->setScaleMaxMajor(ticks);
}

void 
Knob::setScaleMaxMinor(int ticks)
{
	_knob->setScaleMaxMinor(ticks);
}

void 
Knob::getRange(double& min, double& max)
{
#if (QWT_VERSION < 0x060100)
	min = _knob->minValue();
	max = _knob->maxValue();
#else
	min = _knob->lowerBound();
	max = _knob->upperBound();
#endif
}

double
Knob::value()
{
	return _knob->value();
}
