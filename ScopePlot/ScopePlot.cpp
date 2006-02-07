#include "ScopePlot/ScopePlot.h"

#include <qwt_plot.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>

#include "ScrollZoomer.h"

ScopePlot::ScopePlot(QWidget* parent, const char* name, WFlags flags):
ScopePlotBase(parent, name, flags),
_curve(0),
_x(0),
_y(0),
_n(0),
_paused(false)
{
   qwtPlot->setAutoReplot(false);
   qwtPlot->setFrameStyle(QFrame::NoFrame);
   qwtPlot->setLineWidth(0);
   qwtPlot->setCanvasLineWidth(2);

   QwtPlotGrid* grid = new QwtPlotGrid;
   grid->attach(qwtPlot);
   qwtPlot->setCanvasBackground(QColor(29, 100, 141)); // nice blue

   // enable zooming

   ScrollZoomer *zoomer = new ScrollZoomer(qwtPlot->canvas());
   zoomer->setRubberBandPen(QPen(Qt::yellow, 0, Qt::DotLine));

}

ScopePlot::~ScopePlot() { 
}

void 
ScopePlot::newTrace(double *x, double *y, int size)
{
   if (_paused)
      return;

   if (size != _n) {
      if (_x)
         delete [] _x;
      if (_y)
         delete [] _y;
      if (_curve)
         delete _curve;
      _n = 0;
      _x = 0;
      _y = 0;
   }

   if (_n == 0 ) {
      _n = size;
      _x = new double[_n];
      _y = new double[_n];
      _curve = new QwtPlotCurve("trace");
      _curve->attach(qwtPlot);
      _curve->setPen(QPen(Qt::blue));
   }

   for (int i = 0; i < _n; i++) {
      _x[i] = x[i];
      _y[i] = y[i];
   }
   _curve->setData(_x, _y, _n);
   qwtPlot->replot();

}

void ScopePlot::replot()
{
   qwtPlot->replot();
}

void
ScopePlot::pause(bool doPause)
{
   _paused = doPause;
}