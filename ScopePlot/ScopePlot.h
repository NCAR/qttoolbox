#ifndef SCOPEPLOT_H
#define SCOPEPLOT_H

#include "ScopePlotBase.h"
#include <qwt_array.h>
#include <qwt_plot_curve.h>

class QT_WIDGET_PLUGIN_EXPORT ScopePlot: public ScopePlotBase {

   Q_OBJECT

public:
   ScopePlot(QWidget* parent, const char* name = 0, WFlags flags = 0);
   ~ScopePlot();
    void newTrace(double *x, double *y, int size);
    void replot();

public slots:
    void pause(bool doPause);

private:
    QwtPlotCurve* _curve;
    double* _x;
    double* _y;
    int     _n;
    bool    _paused;
};

#endif
