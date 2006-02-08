#ifndef SCOPEPOKE_H
#define SCOPEPOKE_H

#include "ScopePlot.h"
#include <qobject.h>
#include <stdlib.h>
#include <vector>

class ScopePoke: public QObject {
   Q_OBJECT
public:
   ScopePoke(ScopePlot* scope);
   void setPlotType(ScopePlot::PLOTTYPE t);

protected:
   void timerEvent(QTimerEvent *e);

   std::vector<double> _x;
   std::vector<double> _y1;
   std::vector<double> _y2;
   int _n;
   ScopePlot* _scope;
   ScopePlot::PLOTTYPE _plotType;
};

#endif

