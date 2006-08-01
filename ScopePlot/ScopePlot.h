#ifndef SCOPEPLOT_H_
#define SCOPEPLOT_H_ 1

#include "ScopePlotBase.h"
#include <qwt_array.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_plot.h>
#include <qwt_scale_engine.h>

#include <vector>

/// Render time series and power spectrum data, using Qwt.
/// Three functions are provided, to display passed data 
/// as either time series, I versus Q, or power spectra.
/// The display will reconfigure itself when the two different
/// display functions are called in succesion. Thus no configuration
/// or setup calls are necessary.
/// 
/// This code was unabashedly lifted from the Qwt examples/realtime_plot
/// code.
class QT_WIDGET_PLUGIN_EXPORT ScopePlot: public ScopePlotBase
{
   Q_OBJECT

public:
   /// Plot type
   enum PLOTTYPE {TIMESERIES=0, IVSQ=1, SPECTRUM=2, PRODUCT=3};

   ScopePlot(QWidget *parent, const char* name = 0);

   virtual ~ScopePlot();

   /// The size hint.
   virtual QSize sizeHint() const;

   /// Draw a time series plot. I and Q traces will be overlaid.
   /// @param I The I data.
   /// @param Q The Q data
   /// @param scaleMin The minimum value to set the y scale to.
   /// @param scaleMax The maximum value to set the y scale to.
   /// @param sampleRateHz The rate of the data samples, in Hz
   /// @param xLabel The label for the x axis. Leave 
   /// empty if no label is required.
   /// @param yLabel The label for the y axis. Leave 
   /// empty if no label is required.
   void TimeSeries(std::vector<double>& I, 
      std::vector<double>& Q, 
      double scaleMin, 
      double scaleMax,
      double sampleRateHz,
      std::string xLabel="", 
      std::string yLabel="");

   /// Draw a plot of I versus Q.
   /// @param I The I data.
   /// @param Q The Q data
   /// @param scaleMin The minimum value to set the y scale to.
   /// @param scaleMax The maximum value to set the y scale to.
   /// @param sampleRateHz The rate of the data samples, in Hz
   /// @param xLabel The label for the x axis. Leave 
   /// empty if no label is required.
   /// @param yLabel The label for the y axis. Leave 
   /// empty if no label is required.
   void IvsQ(std::vector<double>& I, 
      std::vector<double>& Q, 
      double scaleMin, 
      double scaleMax,
      double sampleRateHz,
      std::string xLabel="", 
      std::string yLabel="");

   /// Draw a plot of the power spectrum. The data values
   /// are expected to be in dB, and so both x and y scales are
   /// linear.
   /// @param power The power values, ranging from negative through
   /// zero to positive frequencies.
   /// @param scaleMin The minimum value to set the y scale to.
   /// @param scaleMax The maximum value to set the y scale to.
   /// @param sampleRateHz The rate of the data samples, in Hz
   /// @param logYaxis The scale of the y-axis. TRUE=log, FALSE= linear 
   /// @param xLabel The label for the x axis. Leave 
   /// empty if no label is required.
   /// @param yLabel The label for the y axis. Leave 
   /// empty if no label is required.
   void Spectrum(std::vector<double>& power, 
      double scaleMin, 
      double scaleMax,
      double sampleRateHz,
	  bool logYaxis,	
      std::string xLabel="", 
      std::string yLabel="");

   /// Draw a plot of a product. The data values
   /// are expected to be linear, and so both x and y scales are
   /// linear.
   /// @param productData The product data values, ranging from negative through
   /// zero to positive depending on the particular product.
   /// @param productType used by application: here used solely to signal redraw of axis labels
   /// @param scaleMin The minimum value to set the y scale to.
   /// @param scaleMax The maximum value to set the y scale to.
   /// @param sampleRateHz The rate of the data samples, in Hz
   /// @param xLabel The label for the x axis. Leave 
   /// empty if no label is required.
   /// @param yLabel The label for the y axis. Leave 
   /// empty if no label is required.
   void Product(std::vector<double>& productData, 
      int productType,
      double scaleMin, 
      double scaleMax,
      double sampleRateHz,
      std::string xLabel="", 
      std::string yLabel="");

public slots:

   /// Enable the X grid
   /// @param tf  True to enable, false otherwise
   void enableXgrid(bool tf);

   /// Enable the Y grid
   /// @param tf  True to enable, false otherwise
   void enableYgrid(bool tf);

   /// Stop updating the display.
   /// @param tf True to enable, false otherwise
   void pause(bool tf);


protected:

   /// Remove existing curves, create (empty) new ones,
   /// add redraw the plot.
   void initCurve();

   /// Reconfigure plot to display time series.
   /// @param n The number of points in the time series.
   /// @param scaleMin The y scale minimum.
   /// @param scaleMax The y scale maximum.
   /// @param sampeRateHz The sample rate in Hz
   void configureForTimeSeries(int n, 
      double scaleMin, 
      double scaleMax,
      double sampleRateHz);

   /// Reconfigure plot to display I versus Q.
   /// @param scaleMin The x and y scale minimum.
   /// @param scaleMax The x and y scale maximum.
   void configureForIvsQ(double scaleMin, double scaleMax);

   /// Reconfigure plot to display I versus Q.
   /// @param n The number of points in the time series.
   /// @param scaleMin The y scale minimum.
   /// @param scaleMax The y scale maximum.
   /// @param sampleRateHz The sample rate, in Hz
   void configureForSpectrum(int n,
      double scaleMin, 
      double scaleMax,
      double sampleRateHz,
      bool logYaxis = true);

   /// Reconfigure plot to display a product.
   /// @param n The number of points in the power data.
   /// @param scaleMin The y scale minimum.
   /// @param scaleMax The y scale maximum.
   /// @param sampleRateHz The sample rate, in Hz
   void configureForProduct(int n,
      double scaleMin, 
      double scaleMax,
      double sampleRateHz);

   /// Label the axes
   /// @param xLabel Label for x
   /// @param yLabel Label for y
   void labelAxes(std::string xLabel, std::string yLabel);

   /// Curve id for the main plot.
   QwtPlotCurve* _curveId1;

   /// Curve id if we have a second curve, such as 
   /// the time series plot.
   QwtPlotCurve* _curveId2;

   /// The grid
   QwtPlotGrid* _grid;

   /// Type of current plot display.
   PLOTTYPE _plotType;

   /// Product type to display
   int _productType; 

   /// The current minimum scale. Use it to determine if
   /// we need reconfigure the display when the requested
   /// scale changes with the incoming plot request.
   double _scaleMin;

   /// The current maximum scale. Use it to determine if
   /// we need reconfigure the display when the requested
   /// scale changes with the incoming plot request.
   double _scaleMax;

   /// The current sample rate in Hz, Used to determine
   /// axis scaling
   double _sampleRateHz;

   /// Pre-calculated xaxis data. For repeating plots of
   /// time series and power spectra, when the number of
   /// data points doesn't change, we can use the x axis values
   /// over again.
   std::vector<double> _xdata;

   bool _paused;

};

#endif // SCOPEPLOT_H_

