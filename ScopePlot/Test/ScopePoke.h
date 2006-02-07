#ifndef SCOPEPOKE_H
#define SCOPEPOKE_H

#include "ScopePlot.h"
#include <qobject.h>
#include <math.h>

class PokeScope: public QObject {
   Q_OBJECT
public:
   PokeScope(ScopePlot* scope){
      _scope = scope;
      _n = 2000;
      _x = new double[_n];
      _y = new double[_n];
      for (int i = 0; i < _n; i++)
         _x[i] = i;

      startTimer(20);

   };

protected:
   void timerEvent(QTimerEvent *e) {
      for (int i = 0; i < _n; i++) {
         _y[i] = 100.0*rand()/(RAND_MAX*1.0) + 500;
      }
      _scope->newTrace(_x,_y,_n);

   };

   double* _x;
   double* _y;
   int _n;
   ScopePlot* _scope;
};

#endif

