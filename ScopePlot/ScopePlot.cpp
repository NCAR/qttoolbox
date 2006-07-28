#include "ScopePlot.h"

#include <iostream>
#include <stdlib.h>
#include "ScrollZoomer.h"
#include "ScopePlot.h"
#include <qpen.h>
#include <qwt_math.h>
#include <qwt_symbol.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_text.h>

//////////////////////////////////////////////////////////////////////////////////

ScopePlot::ScopePlot(QWidget *parent, const char* name):
ScopePlotBase(parent),
_curveId1(0),
_curveId2(0),
_plotType(TIMESERIES),
_scaleMin(0.0),
_scaleMax(0.0),
_paused(false)
{
   qwtPlot->setFrameStyle(QFrame::NoFrame);
   qwtPlot->setLineWidth(0);
   qwtPlot->setCanvasLineWidth(2);
   qwtPlot->setAxisScaleEngine(QwtPlot::xBottom, new QwtLinearScaleEngine());

   _grid = new QwtPlotGrid();
   _grid->attach(qwtPlot);

   qwtPlot->setCanvasBackground(QColor(29, 100, 141)); // nice blue

   // enable zooming

   ScrollZoomer *zoomer = new ScrollZoomer(qwtPlot->canvas());
   zoomer->setEnabled(true);

   zoomer->setRubberBandPen(QPen(Qt::yellow, 0, Qt::DotLine));
   //  zoomer->setCursorLabelPen(QPen(Qt::yellow));

}

//////////////////////////////////////////////////////////////////////////////////

ScopePlot::~ScopePlot()
{
}

//////////////////////////////////////////////////////////////////////////////////

QSize 
ScopePlot::sizeHint() const
{
   return QSize(540,400);
}

//////////////////////////////////////////////////////////////////////////////////

void 
ScopePlot::initCurve()
{

   if ( _curveId1 > 0 )
   {
      delete _curveId1;
      _curveId1 = 0;
   }

   if ( _curveId2 > 0 )
   {
      delete _curveId2;
      _curveId2 = 0;
   }

   _curveId1 = new QwtPlotCurve("Data1");
   _curveId1->attach(qwtPlot);
   _curveId1->setStyle(QwtPlotCurve::Lines);
   _curveId1->setPen(QPen(cyan));

   // if we are doing timeseries, then make two curves
   if (_plotType == TIMESERIES) {
      _curveId2 = new QwtPlotCurve("Data2");
      _curveId2->attach(qwtPlot);
      _curveId2->setStyle(QwtPlotCurve::Lines);
      _curveId2->setPen(QPen(red));
   }
}

//////////////////////////////////////////////////////////////////////////////////

void 
ScopePlot::TimeSeries(std::vector<double>& I, 
                      std::vector<double>& Q,
                      double scaleMin,
                      double scaleMax,
                      double sampleRateHz,
                      std::string xLabel, 
                      std::string yLabel)
{
   if (_paused)
      return;

   if (_plotType     != TIMESERIES || 
      _xdata.size() != I.size()   || 
      scaleMin      != _scaleMin  || 
      scaleMax      != _scaleMax  ||
      sampleRateHz  != _sampleRateHz) {
         configureForTimeSeries(I.size(), scaleMin, scaleMax, sampleRateHz);
         labelAxes(xLabel, yLabel);
      }

      initCurve();

      _curveId1->setData(&_xdata[0], &I[0], I.size());
      _curveId2->setData(&_xdata[0], &Q[0], Q.size());

      qwtPlot->replot();
}

//////////////////////////////////////////////////////////////////////////////////

void 
ScopePlot::IvsQ(std::vector<double>& I, 
                std::vector<double>& Q,
                double scaleMin,
                double scaleMax,
                double sampleRateHz,
                std::string xLabel, 
                std::string yLabel)
{

   if (_paused)
      return;

   if (_plotType != IVSQ      ||
      scaleMin  != _scaleMin || 
      scaleMax  != _scaleMax) {
         configureForIvsQ(scaleMin, scaleMax);
         labelAxes(xLabel, yLabel);
      }

      initCurve();

      _curveId1->setData(&I[0], &Q[0], I.size());

      qwtPlot->replot();

}

//////////////////////////////////////////////////////////////////////////////////

void 
ScopePlot::Spectrum(std::vector<double>& power,
                    double scaleMin,
                    double scaleMax,
                    double sampleRateHz,
                    std::string xLabel, 
                    std::string yLabel)
{

   if (_paused)
      return;
   if (_plotType != SPECTRUM ||
      _xdata.size() != power.size() || 
      scaleMin      != _scaleMin    || 
      scaleMax      != _scaleMax    ||
      sampleRateHz  != _sampleRateHz) {
         configureForSpectrum(power.size(), scaleMin, scaleMax, sampleRateHz);
         labelAxes(xLabel, yLabel);
      }

      initCurve();

      _curveId1->setData(&_xdata[0], &power[0], power.size());

      qwtPlot->replot();
}


//////////////////////////////////////////////////////////////////////////////////

void 
ScopePlot::Product(std::vector<double>& productData,
				    int productType,
                    double scaleMin,
                    double scaleMax,
                    double sampleRateHz,
                    std::string xLabel, 
                    std::string yLabel)
{

   if (_paused)
      return;
   if (_plotType != PRODUCT ||
	  productType	!= _productType || 
      _xdata.size() != productData.size() || 
      scaleMin      != _scaleMin    || 
      scaleMax      != _scaleMax    ||
      sampleRateHz  != _sampleRateHz) {
         configureForProduct(productData.size(), scaleMin, scaleMax, sampleRateHz);
         labelAxes(xLabel, yLabel);
      }

      initCurve();

      _curveId1->setData(&_xdata[0], &productData[0], productData.size());

      qwtPlot->replot();
}


//////////////////////////////////////////////////////////////////////////////////

void 
ScopePlot::configureForTimeSeries(
                                  int n, 
                                  double scaleMin, 
                                  double scaleMax, 
                                  double sampleRateHz)
{
   _plotType = TIMESERIES;
   _scaleMin = scaleMin;
   _scaleMax = scaleMax;
   _sampleRateHz = sampleRateHz;

   qwtPlot->setAxisScaleEngine(QwtPlot::yLeft, new QwtLinearScaleEngine());

   qwtPlot->setAxisScale(QwtPlot::xBottom, 0, n/_sampleRateHz);
   qwtPlot->setAxisScale(QwtPlot::yLeft, _scaleMin, _scaleMax);

   _xdata.resize(n);

   for (int i = 0; i < n; i++)
      _xdata[i] = i/_sampleRateHz;

   initCurve();

   qwtPlot->replot();
}

//////////////////////////////////////////////////////////////////////////////////

void 
ScopePlot::configureForIvsQ(double scaleMin, 
                            double scaleMax)
{
   _plotType = IVSQ;
   _scaleMin = scaleMin;
   _scaleMax = scaleMax;

   qwtPlot->setAxisScaleEngine(QwtPlot::yLeft, new QwtLinearScaleEngine());

   qwtPlot->setAxisScale(QwtPlot::xBottom, _scaleMin, _scaleMax);
   qwtPlot->setAxisScale(QwtPlot::yLeft,   _scaleMin, _scaleMax);

   initCurve();

}

//////////////////////////////////////////////////////////////////////////////////

void 
ScopePlot::configureForSpectrum(int n, 
                                double scaleMin, 
                                double scaleMax, 
                                double sampleRateHz)
{
   _plotType = SPECTRUM;
   _scaleMin = scaleMin;
   _scaleMax = scaleMax;
   _sampleRateHz = sampleRateHz;

// original:   qwtPlot->setAxisScaleEngine(QwtPlot::yLeft, new QwtLog10ScaleEngine());
   qwtPlot->setAxisScaleEngine(QwtPlot::yLeft, new QwtLinearScaleEngine());
   qwtPlot->setAxisScale(QwtPlot::xBottom, -_sampleRateHz/2.0, _sampleRateHz/2.0);
   qwtPlot->setAxisScale(QwtPlot::yLeft, scaleMin, scaleMax);

   _xdata.resize(n);

   for (int i = 0; i < n; i++)
      _xdata[i] = -(_sampleRateHz/n)*(i - n/2);

   initCurve();

}
//////////////////////////////////////////////////////////////////////////////////

void 
ScopePlot::configureForProduct(int n, 
                                double scaleMin, 
                                double scaleMax, 
                                double sampleRateHz)
{
   _plotType = PRODUCT;
   _scaleMin = scaleMin;
   _scaleMax = scaleMax;
   _sampleRateHz = sampleRateHz;

   qwtPlot->setAxisScaleEngine(QwtPlot::yLeft, new QwtLinearScaleEngine());
   qwtPlot->setAxisScale(QwtPlot::xBottom, 0, _sampleRateHz);
   qwtPlot->setAxisScale(QwtPlot::yLeft, scaleMin, scaleMax);

   _xdata.resize(n);

   for (int i = 0; i < n; i++)
      _xdata[i] = i;

   initCurve();

}
//////////////////////////////////////////////////////////////////////////////////

void 
ScopePlot::labelAxes(std::string xLabel, std::string yLabel) {
   qwtPlot->setAxisTitle(QwtPlot::xBottom, xLabel.c_str());	
   qwtPlot->setAxisTitle(QwtPlot::yLeft,   yLabel.c_str());
}

//////////////////////////////////////////////////////////////////////////////////

void 
ScopePlot::enableXgrid(bool tf) {
   _grid->enableX(tf);
}

//////////////////////////////////////////////////////////////////////////////////

void 
ScopePlot::enableYgrid(bool tf) {
   _grid->enableY(tf);
}

//////////////////////////////////////////////////////////////////////////////////

void 
ScopePlot::pause(bool tf) {
   _paused = tf;
}
