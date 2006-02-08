#include "ScopePoke.h"

ScopePoke::ScopePoke(ScopePlot* scope) {
   _scope = scope;
   _plotType = ScopePlot::TIMESERIES;
   _n = 1000;
   _x.resize(_n);
   _y1.resize(_n);
   _y2.resize(_n);
   for (int i = 0; i < _n; i++)
      _x[i] = i;

   startTimer(20);

}

void 
ScopePoke::setPlotType(ScopePlot::PLOTTYPE t) {
   _plotType = t;
};

void 
ScopePoke::timerEvent(QTimerEvent *e) {
   switch (_plotType) {
         case ScopePlot::TIMESERIES:

            for (int i = 0; i < _n; i++)
            {
               _y1[i] = 100.0*rand()/(RAND_MAX*1.0);
               _y2[i] = 100.0*rand()/(RAND_MAX*1.0);
            }
            _scope->TimeSeries(_y1,_y2,0.0, 100.0, 1);
            break;

         case ScopePlot::IVSQ:

            for (int i = 0; i < _n; i++) 
            {
               _y1[i] = 100.0*rand()/(RAND_MAX*1.0);
               _y2[i] = 100.0*rand()/(RAND_MAX*1.0);
            }
            _scope->IvsQ(_y1, _y2, 0, 100, 1);
            break;

         case ScopePlot::SPECTRUM: 

            for (int i = 0; i < _n; i++) 
            {
               _y1[i] = -30.0*rand()/(RAND_MAX*1.0);
            }
            _scope->Spectrum(_y1, -100, -30, 1);
            break;

   }

}

