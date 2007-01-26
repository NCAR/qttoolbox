#include "TwoKnobs.h"
#include <qlabel.h>
#include <qwt_knob.h>

TwoKnobs::TwoKnobs( QWidget* parent):
QWidget(parent)
{
	setupUi(this);
}

/*
*  Destroys the object and frees any allocated resources
*/
TwoKnobs::~TwoKnobs()
{
	// no need to delete child widgets, Qt does it all for us
}

void
TwoKnobs::setTitles(std::string titleKnobOne, std::string titleKnobTwo)
{
	_label1->setText(titleKnobOne.c_str());
	_label2->setText(titleKnobTwo.c_str());
}

void
TwoKnobs::setRanges(double knobOneMin, double knobOneMax,
					double knobTwoMin, double knobTwoMax)
{
	_knob1->setRange(knobOneMin, knobOneMax);
	_knob2->setRange(knobTwoMin, knobTwoMax);
}

void
TwoKnobs::setValues(double knobOne, double knobTwo)
{
	_knob1->setValue(knobOne);
	_knob2->setValue(knobTwo);
}

void 
TwoKnobs::knobOneChangedSlot(double v)
{
	emit knobOneChanged(v);
}

void 
TwoKnobs::knobTwoChangedSlot(double v)
{
	emit knobTwoChanged(v);
}

void 
TwoKnobs::setScaleMaxMajor(int knobNum, int ticks)
{
	switch (knobNum) {
		case 1:
			_knob1->setScaleMaxMajor(ticks);
			break;
		case 2:
			_knob2->setScaleMaxMajor(ticks);
			break;
		default:
			break;
	}
}

void 
TwoKnobs::setScaleMaxMinor(int knobNum, int ticks)
{
	switch (knobNum) {
		case 1:
			_knob1->setScaleMaxMinor(ticks);
			break;
		case 2:
			_knob2->setScaleMaxMinor(ticks);
			break;
		default:
			break;
	}
}

void 
TwoKnobs::getRanges(double& knobOneMin, double& knobOneMax,
       double& knobTwoMin, double& knobTwoMax)
{
	knobOneMin = _knob1->minValue();
	knobOneMax = _knob1->maxValue();
	knobTwoMin = _knob2->minValue();
	knobTwoMax = _knob2->maxValue();
}

void 
TwoKnobs::getValues(double& val1, double& val2)
{
	val1 = _knob1->value();
	val2 = _knob2->value();
}
