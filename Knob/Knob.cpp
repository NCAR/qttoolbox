#include "Knob.h"
#include <qlabel.h>
#include <qwt_knob.h>

Knob::Knob( QWidget* parent, const char* name, WFlags fl )
    : KnobBase( parent, name, fl )
{
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
   _knob->setRange(min, max);
}

void
Knob::valueChangedSlot(double v)
{
   emit valueChanged(v);
}
