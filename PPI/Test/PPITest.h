#ifndef PPITESTH
#define PPITESTH
#include "PPITestBase.h"

#include <vector>
#include <qtimer.h>
#include "ColorBar.h"
#include "ColorMap.h"

class PPITest: public PPITestBase
  {
  Q_OBJECT
  public:
    PPITest( int nVars, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    virtual ~PPITest();
    
    void startSlot();
    void stopSlot();
    void varSelectSlot1(int);
    void varSelectSlot2(int);
    void changeDir();
    void clearVarSlot(int);
    void panSlot(int);
    void ringsSlot(bool);
    void gridSlot(bool);
   
  public slots:
    void addBeam();
    void zoomInSlot();
    void zoomOutSlot();
    
    protected:
      double _angle;
      double _angleInc;
      int _nVars;
      int _gates;
	  double _beamWidth;
      std::vector<int> _varIndices;
	  std::vector<std::vector<double> > _beamData;
	  std::vector<ColorMap*> _maps1;
	  std::vector<ColorMap*> _maps2;
	  ColorBar* _colorBar1;
	  ColorBar* _colorBar2;
    
    QTimer _timer;
  };

#endif

