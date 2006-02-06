#ifndef KNOB_H
#define KNOB_H

#include "KnobBase.h"
#include <string>

///
/// A plugin widget which combines a QwtKnob, a QLabel, and a QLCDNumber
/// A signal is generated when the knob value changes.
class QT_WIDGET_PLUGIN_EXPORT Knob : public KnobBase
{
    Q_OBJECT

public:
    Knob( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~Knob();

    void setTitle(std::string title);
    void setRange(double min, double max);

public slots:
   void valueChangedSlot(double);

signals:
   void valueChanged(double);

};

#endif // KNOB_H
