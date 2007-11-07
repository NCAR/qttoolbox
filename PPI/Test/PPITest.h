#ifndef PPITESTH
#define PPITESTH
#include "ui_PPITest.h"

#include <vector>
#include <qtimer.h>
#include <QVBoxLayout>
#include "ColorBar.h"
#include "ColorMap.h"

class PPITest: public QObject, public Ui::PPITest
{
	Q_OBJECT
public:
	PPITest( int nVars, QWidget* parent = 0);
	virtual ~PPITest();

	public slots:

		void varSelectSlot1(int);
		void varSelectSlot2(int);
		void changeDir();
		void clearVarSlot(int);
		void panUpSlot();
		void panDownSlot();
		void panLeftSlot();
		void panRightSlot();
		void resetViewSlot();
		void ringsSlot(bool);
		void gridSlot(bool);
		void backgroundColorSlot();
		void ringColorSlot();
		void startSlot();
		void stopSlot();
		void addBeam();
		void zoomInSlot();
		void zoomOutSlot();
		void saveImageSlot();
		void decimationSlot(int);

protected:
	void pan(double x, double y);
	double _angle;
	double _angleInc;
	int _nVars;
	int _gates;
	double _beamWidth;
	std::vector<int> _varIndices;
	std::vector<std::vector<double> > _beamData;
	std::vector<ColorMap*> _maps1;
	std::vector<ColorMap*> _maps2;
	QTimer _timer;
	double _currentX;
	double _currentY;
	int _decimation;
};

#endif

