#include "ScopePlot.h"
#include <QPainter>
#include <QPixmap>

#include <iostream>
#include <stdlib.h>
#include "ScopePlot.h"
#include <qpen.h>
#include <qwt_math.h>
#include <qwt_symbol.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_text.h>
#include <qwt_scale_widget.h>
#include <qwt_double_rect.h>

//////////////////////////////////////////////////////////////////////////////////

ScopePlot::ScopePlot(QWidget *parent):
QWidget(parent),
_curveId1(0),
_curveId2(0),
_plotType(TIMESERIES),
_scaleMin(0.0),
_scaleMax(0.0),
_paused(false)
{
	setupUi(this);

	_qwtPlot->setFrameStyle(QFrame::NoFrame);
	_qwtPlot->setLineWidth(0);
	_qwtPlot->setCanvasLineWidth(2);
	_qwtPlot->setAxisScaleEngine(QwtPlot::xBottom, new QwtLinearScaleEngine());

	_grid = new QwtPlotGrid();
	_grid->attach(_qwtPlot);

	_qwtPlot->setCanvasBackground(QColor(29, 100, 141)); // nice blue

    // enable zooming

    _zoomer = new ScrollZoomer(_qwtPlot->canvas());
    _zoomer->setRubberBandPen(QPen(Qt::red, 2, Qt::DotLine));
    _zoomer->setTrackerPen(QPen(Qt::white));

}

//////////////////////////////////////////////////////////////////////////////////

ScopePlot::~ScopePlot()
{
	delete _zoomer;
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
	_curveId1->attach(_qwtPlot);
	_curveId1->setStyle(QwtPlotCurve::Lines);
	_curveId1->setPen(QPen(Qt::cyan));

	// if we are doing timeseries, then make two curves
	if (_plotType == TIMESERIES) {
		_curveId2 = new QwtPlotCurve("Data2");
		_curveId2->attach(_qwtPlot);
		_curveId2->setStyle(QwtPlotCurve::Lines);
		_curveId2->setPen(QPen(Qt::red));
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
		sampleRateHz  != _sampleRateHz ||
		xLabel        != _timeSeriesXlabel ||
		yLabel        != _timeSeriesYlabel ) 
	{
		configureForTimeSeries(I.size(), scaleMin, scaleMax, sampleRateHz);
		_timeSeriesXlabel = xLabel;
		_timeSeriesYlabel = yLabel;
		labelAxes(_timeSeriesXlabel, _timeSeriesYlabel);
	}

	initCurve();

	_curveId1->setData(&_xdata[0], &I[0], I.size());
	_curveId2->setData(&_xdata[0], &Q[0], Q.size());

	_qwtPlot->replot();
}

//////////////////////////////////////////////////////////////////////////////////

void 
ScopePlot::IvsQ(std::vector<double>& I, 
				std::vector<double>& Q,
				double scaleMin,
				double scaleMax,
				double /*sampleRateHz*/,
				std::string xLabel, 
				std::string yLabel)
{

	if (_paused)
		return;

	if (_plotType != IVSQ      ||
		scaleMin  != _scaleMin || 
		scaleMax  != _scaleMax ||
		xLabel        != _iqXlabel ||
		yLabel        != _iqYlabel) {
			configureForIvsQ(scaleMin, scaleMax);
			_iqXlabel = xLabel;
			_iqYlabel = yLabel;
			labelAxes(_iqXlabel, _iqYlabel);
	}

	initCurve();

	_curveId1->setData(&I[0], &Q[0], I.size());

	_qwtPlot->replot();
}

//////////////////////////////////////////////////////////////////////////////////

void 
ScopePlot::Spectrum(std::vector<double>& power,
					double scaleMin,
					double scaleMax,
					double sampleRateHz,
					bool logYaxis,
					std::string xLabel, 
					std::string yLabel)
{

	if (_paused)
		return;
	if (_plotType != SPECTRUM ||
		_xdata.size() != power.size() || 
		scaleMin      != _scaleMin    || 
		scaleMax      != _scaleMax    ||
		sampleRateHz  != _sampleRateHz ||
		xLabel        != _spectrumXlabel ||
		yLabel        != _spectrumYlabel) {
			configureForSpectrum(power.size(), scaleMin, scaleMax, sampleRateHz, logYaxis);
			_spectrumXlabel = xLabel;
			_spectrumYlabel = yLabel;
			labelAxes(_spectrumXlabel, _spectrumYlabel);
	}

	initCurve();

	_curveId1->setData(&_xdata[0], &power[0], power.size());

	_qwtPlot->replot();
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
		sampleRateHz  != _sampleRateHz ||
		xLabel        != _productXlabel ||
		yLabel        != _productYlabel) {
			configureForProduct(productData.size(), scaleMin, scaleMax, sampleRateHz);
			_productXlabel = xLabel;
			_productYlabel = yLabel;
			labelAxes(_productXlabel, _productYlabel);
	}

	initCurve();

	_curveId1->setData(&_xdata[0], &productData[0], productData.size());

	_qwtPlot->replot();
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

	_qwtPlot->setAxisScaleEngine(QwtPlot::yLeft, new QwtLinearScaleEngine());

	_qwtPlot->setAxisScale(QwtPlot::xBottom, 0, n/_sampleRateHz);
	_qwtPlot->setAxisScale(QwtPlot::yLeft, _scaleMin, _scaleMax);

	_xdata.resize(n);

	for (int i = 0; i < n; i++)
		_xdata[i] = i/_sampleRateHz;

	initCurve();

	_qwtPlot->replot();

	if(_zoomer->zoomStack().size() == 1)
		_zoomer->setZoomBase();
}

//////////////////////////////////////////////////////////////////////////////////

void 
ScopePlot::configureForIvsQ(double scaleMin, 
							double scaleMax)
{
	_plotType = IVSQ;
	_scaleMin = scaleMin;
	_scaleMax = scaleMax;

	_qwtPlot->setAxisScaleEngine(QwtPlot::yLeft, new QwtLinearScaleEngine());

	_qwtPlot->setAxisScale(QwtPlot::xBottom, _scaleMin, _scaleMax);
	_qwtPlot->setAxisScale(QwtPlot::yLeft,   _scaleMin, _scaleMax);

	initCurve();

	_qwtPlot->replot();
	_zoomer->setZoomBase();

}

//////////////////////////////////////////////////////////////////////////////////

void 
ScopePlot::configureForSpectrum(int n, 
								double scaleMin, 
								double scaleMax, 
								double sampleRateHz,
								bool logYaxis)
{
	_plotType = SPECTRUM;
	_scaleMin = scaleMin;
	_scaleMax = scaleMax;
	_sampleRateHz = sampleRateHz;

	if (logYaxis)
		_qwtPlot->setAxisScaleEngine(QwtPlot::yLeft, new QwtLog10ScaleEngine());
	else
		_qwtPlot->setAxisScaleEngine(QwtPlot::yLeft, new QwtLinearScaleEngine());
	_qwtPlot->setAxisScale(QwtPlot::xBottom, -_sampleRateHz/2.0, _sampleRateHz/2.0);
	_qwtPlot->setAxisScale(QwtPlot::yLeft, scaleMin, scaleMax);

	_xdata.resize(n);

	for (int i = 0; i < n; i++)
		_xdata[i] = -(_sampleRateHz/n)*(i - n/2);

	initCurve();

	_qwtPlot->replot();
	_zoomer->setZoomBase();

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

	_qwtPlot->setAxisScaleEngine(QwtPlot::yLeft, new QwtLinearScaleEngine());
	_qwtPlot->setAxisScale(QwtPlot::xBottom, 0, _sampleRateHz);
	_qwtPlot->setAxisScale(QwtPlot::yLeft, scaleMin, scaleMax);

	_xdata.resize(n);

	for (int i = 0; i < n; i++)
		_xdata[i] = i;

	initCurve();

	_qwtPlot->replot();
	_zoomer->setZoomBase();

}
//////////////////////////////////////////////////////////////////////////////////

void 
ScopePlot::labelAxes(std::string xLabel, std::string yLabel) {
	_qwtPlot->setAxisTitle(QwtPlot::xBottom, xLabel.c_str());	
	_qwtPlot->setAxisTitle(QwtPlot::yLeft,   yLabel.c_str());
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
////////////////////////////////////////////////////////////////////////
void
ScopePlot::saveImageToFile(std::string filePath) {
	QPixmap pixmap = QPixmap::grabWidget(this);

	pixmap.save(filePath.c_str(), "PNG", 100);
}
