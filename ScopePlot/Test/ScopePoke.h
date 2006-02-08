#ifndef SCOPEPOKE_H
#define SCOPEPOKE_H

#include "ScopePlot.h"
#include <qobject.h>
#include <stdlib.h>

class PokeScope: public QObject {
   Q_OBJECT
public:
   PokeScope(ScopePlot* scope){
      _scope = scope;
      _n = 1000;
      _x.resize(_n);
      _y1.resize(_n);
      _y2.resize(_n);
      for (int i = 0; i < _n; i++)
         _x[i] = i;

      startTimer(20);

   };

protected:
   void timerEvent(QTimerEvent *e) {
      for (int i = 0; i < _n; i++) {
         _y1[i] = 100.0*rand()/(RAND_MAX*1.0);
         _y2[i] = 100.0*rand()/(RAND_MAX*1.0);
      }
      _scope->newTimeSeries(_y1,_y2,0.0, 100.0, 1);

   };

   std::vector<double> _x;
   std::vector<double> _y1;
   std::vector<double> _y2;
   int _n;
   ScopePlot* _scope;
};

#endif

