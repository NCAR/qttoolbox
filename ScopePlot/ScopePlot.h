#ifndef SCOPEPLOT_H
#define SCOPEPLOT_H

#include "ScopePlotBase.h"
#include <qwt_array.h>
#include <qwt_plot_curve.h>

class CurveData
{
    // A container class for growing data
public:

    CurveData();

    void append(double *x, double *y, int count);

    int count() const;
    int size() const;
    const double *x() const;
    const double *y() const;

private:
    int d_count;
    QwtArray<double> d_x;
    QwtArray<double> d_y;
};

class ScopePlot: public ScopePlotBase {

   Q_OBJECT

public:
   ScopePlot(QWidget* parent, const char* name = 0, WFlags flags = 0);
   ~ScopePlot();
    void appendData(double x, double y);
    void appendData(double *x, double *y, int size);

    void removeData();

private:
    CurveData *d_data;
    QwtPlotCurve *d_curve;
};

#endif
