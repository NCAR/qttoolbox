#ifndef SCOPEPLOT_H_
#define SCOPEPLOT_H_ 1

#include "ScopePlotBase.h"
#include <qwt_array.h>
#include <qwt_plot_curve.h>
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

    /// Plot type
    enum PLOTTYPE {TIMESERIES=0, IVSQ=1, SPECTRUM=2};

 public:
  ScopePlot(QWidget *parent);

  virtual ~ScopePlot();

  /// The size hint.
  virtual QSize sizeHint() const;

  /// Draw a time series plot. I and Q traces will be overlaid.
  /// @param I The I data.
  /// @param Q The Q data
  /// @param scaleMin The minimum value to set the y scale to.
  /// @param scaleMax The maximum value to set the y scale to.
  /// @param sampleRateHz The rate of the data samples, in Hz
  void newTimeSeries(std::vector<double>& I, 
             std::vector<double>& Q, 
             double scaleMin, 
             double scaleMax,
             double sampleRateHz);

  /// Draw a plot of I versus Q.
  /// @param I The I data.
  /// @param Q The Q data
  /// @param scaleMin The minimum value to set the y scale to.
  /// @param scaleMax The maximum value to set the y scale to.
  /// @param sampleRateHz The rate of the data samples, in Hz
  void newIvsQ(std::vector<double>& I, 
           std::vector<double>& Q, 
           double scaleMin, 
           double scaleMax,
           double sampleRateHz);

  /// Draw a plot of the power spectrum. The data values
  /// are expected to be in dB, and so both x and y scales are
  /// linear.
  /// @param power The power values, ranging from negative through
  /// zero to positive frequencies.
  /// @param scaleMin The minimum value to set the y scale to.
  /// @param scaleMax The maximum value to set the y scale to.
  /// @param sampleRateHz The rate of the data samples, in Hz
  void newSpectrum(std::vector<double>& power, 
           double scaleMin, 
           double scaleMax,
           double sampleRateHz);

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
  /// @param scaleMin The y scale minimum.
  /// @param scaleMax The y scale maximum.
  void configureForIvsQ(double scaleMin, 
            double scaleMax);

  /// Reconfigure plot to display I versus Q.
  /// @param n The number of points in the time series.
  /// @param scaleMin The y scale minimum.
  /// @param scaleMax The y scale maximum.
  /// @param sampleRateHz The sample rate, in Hz
  void configureForSpectrum(int n,
                double scaleMin, 
                double scaleMax,
                double sampleRateHz);
 
  /// Curve id for the main plot.
  QwtPlotCurve* _curveId1;

  /// Curve id if we have a second curve, such as 
  /// the time series plot.
  QwtPlotCurve* _curveId2;

  /// Type of current plot display.
  PLOTTYPE _plotType;

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

  /// The scale engine used for a logrithmic left axis.
  QwtLog10ScaleEngine _leftAxisLog;

  /// The scale engine used for a linear left axis.
  QwtLinearScaleEngine _leftAxisLinear;

  /// The scale engine used for a linear bottom axis.
  QwtLinearScaleEngine _bottomAxisLinear;

};

#endif // SCOPEPLOT_H_

