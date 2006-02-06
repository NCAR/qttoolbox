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
      _x = 0.0;
   };
public slots:
   void timeout() {

      _scope->removeData();

      double y[100];
      double x[100];
      for (int i = 0; i < 100; i++) {
         x[i] = _x;
         y[i] = 100.0*rand()/(RAND_MAX*1.0);
         _x += 1.0;
      }
      _scope->appendData(x,y,100);
   };
protected:
   double _x;
   ScopePlot* _scope;
};

#endif

