#include "ScopePoke.h"
//Added by qt3to4:
#include <QTimerEvent>

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
               _y1[i] = 30.0*(0.5-rand()/(RAND_MAX*1.0))+100.0*sin(2*3.14159*i/200);
               _y2[i] = 30.0*(0.5-rand()/(RAND_MAX*1.0))+90.0*cos(2*3.14159*i/200);
            }
            _scope->TimeSeries(_y1, _y2, 
               -120.0, 120.0, 
               1000.0, 
               "Time (s)", "Amplitude (counts)");
            break;

         case ScopePlot::IVSQ:

            for (int i = 0; i < _n; i++) 
            {
               _y1[i] = 30.0*(0.5-rand()/(RAND_MAX*1.0))+100.0*sin(2*3.14159*i/100);
               _y2[i] = 30.0*(0.5-rand()/(RAND_MAX*1.0))+90.0*cos(2*3.14159*i/100);
            }
            _scope->IvsQ(_y1, _y2, 
               -120.0, 120.0, 
               1000.0,
               "I Amplitude (counts)", "Q Amplitude (counts)");
            break;

         case ScopePlot::SPECTRUM: 

            for (int i = 0; i < _n; i++) 
            {
               _y1[i] = pow(10.0, -70+40.0*rand()/(RAND_MAX*1.0));
            }
            _scope->Spectrum(_y1, 
               1.0e-70, 1.0e-30, 
               1000.0,
               "Frequency (Hz)", "Power (dB)");
            break;

   }

}

