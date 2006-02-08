
#include "ScopePlotTest.h"
#include "ScopePlot.h"
#include <qbuttongroup.h>

ScopePlotTest::ScopePlotTest():
_scopePoke(0)
{
}

ScopePlotTest::~ScopePlotTest() {
}

void
ScopePlotTest::setScopePoke(ScopePoke* scopePoke) {
   _scopePoke = scopePoke;
}

void 
ScopePlotTest::plotTypeSlot(bool b)
{

   switch (buttonGroup->selectedId()) {
      case 0:
         if (_scopePoke)
            _scopePoke->setPlotType(ScopePlot::TIMESERIES);
         break;
      case 1:
         if (_scopePoke)
            _scopePoke->setPlotType(ScopePlot::IVSQ);
         break;
      case 2:
         if (_scopePoke)
            _scopePoke->setPlotType(ScopePlot::SPECTRUM);
         break;
      default: {}
   }

}

