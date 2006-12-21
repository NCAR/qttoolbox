#include "TwoKnobs.h"
#include "Knob/Knob.h"
#include <qlabel.h>
#include <qwt_knob.h>

TwoKnobs::TwoKnobs( QWidget* parent, const char* name, WFlags fl ):
TwoKnobsBase( parent, name, fl )
{
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
	_knobOne->setTitle(titleKnobOne.c_str());
	_knobTwo->setTitle(titleKnobTwo.c_str());
}

void
TwoKnobs::setRanges(double knobOneMin, double knobOneMax,
					double knobTwoMin, double knobTwoMax)
{
	_knobOne->setRange(knobOneMin, knobOneMax);
	_knobTwo->setRange(knobTwoMin, knobTwoMax);
}

void
TwoKnobs::setValues(double knobOne, double knobTwo)
{
	_knobOne->setValue(knobOne);
	_knobTwo->setValue(knobTwo);
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
			_knobOne->setScaleMaxMajor(ticks);
			break;
		case 2:
			_knobTwo->setScaleMaxMajor(ticks);
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
			_knobOne->setScaleMaxMinor(ticks);
			break;
		case 2:
			_knobTwo->setScaleMaxMinor(ticks);
			break;
		default:
			break;
	}
}
