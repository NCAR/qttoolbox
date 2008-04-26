#ifndef STATUSGAUGEH_
#define STATUSGAUGEH_

#include <qwidget.h>
#include <qlayout.h>
#include <QVBoxLayout>
#include <QImage>
#include <QPixmap>
#include <vector>
#include <string>

#ifndef DLL_EXPORT
#ifdef WIN32
#ifdef QT_PLUGIN
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif
#else
#define DLL_EXPORT
#endif
#endif

/// A widget that displays a representation of .
class DLL_EXPORT StatusGauge : public QWidget {
    Q_OBJECT

    public:
        /// Gauge orientation
        enum StatusGaugeOrientation {GaugeVertical, GaugeHorizontal};

        /// Defaut constructor will create a vertical gauge with red, yello and green indicators
        StatusGauge(QWidget* parent = 0);
        /// Create a gauge with specified color indicators. 
        /// @param orient The orientation of the gauge.
        /// @param cellPercent The percentage of the cell to occupied by the indicator (%)
        /// @param colors Contains color names for all indicators to
        /// be created. Specifiy a color of "" to create placeholder with no indicator.
        StatusGauge(std::vector<std::string> colors,
                    double cellPercent = 90.0,
                    StatusGaugeOrientation orient = GaugeVertical,
                    QWidget* parent = 0);
        /// Destructor
        virtual ~StatusGauge(void);
        /// Reconfigure the gauge.
        /// @param orient The orientation of the gauge.
        /// @param cellPercent The percentage of the cell to occupied by the indicator (%)
        /// @param colors Contains color names for all indicators to
        /// be created. Specifiy a color of "" to create placeholder with no indicator.        
        void reconfigure(std::vector<std::string> colors,
                         double cellPercent = 90.0,
                         StatusGaugeOrientation orient = GaugeVertical);
        /// @return The number of indicators
        int size();
        /// Turn an indicator on or off
        /// @param index the index number of the gauge. If not within 
        /// the range of indicators, do nothing.
        /// @param ison If true, turn on indicator, otherwise turn off.
        void on(int index,
                bool ison);
        /// @returns An image of the color bar. The caller must delete
        /// it when finished.
        QImage* getImage();

        /// @returns A pixmap of the color bar. The caller must delete it 
        /// when finished.
        QPixmap* getPixmap();

    signals:
        void released();

    protected:
        /// Capture a mouse release and emit a released() signal.
        virtual void mouseReleaseEvent(QMouseEvent* e);
        /// The paint event is where we will draw the color bar.
        virtual void paintEvent(QPaintEvent* e);
        /// The percentage of the cell to be occupied by the indicator (1.0-100.0)
        double _cellPercent;
        /// The gauge orientation
        StatusGaugeOrientation _orient;
        /// The colors of the indicators. Use "" to indicate
        /// a place holder with no indicator
        std::vector<std::string> _colors;
        /// If the indicator is on or off. Paired one-for-one
        /// with colors.
        std::vector<bool> _ison;
};

#endif
