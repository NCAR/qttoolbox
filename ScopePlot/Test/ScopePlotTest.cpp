
#include "ScopePlotTest.h"
#include <QButtonGroup>
#include <QCheckBox>

ScopePlotTest::ScopePlotTest(QWidget* parent):
QObject(parent),
_scopePoke(0)
{
	setupUi(parent);

	// put the plot type buttons in a button group
	QButtonGroup* plotGroup = new QButtonGroup;
	plotGroup->addButton(tsButton, 0);
	plotGroup->addButton(xyButton, 1);
	plotGroup->addButton(specButton, 2);
	connect(xGrid, SIGNAL(toggled(bool)), _scopePlot, SLOT(enableXgrid(bool)));
	connect(yGrid, SIGNAL(toggled(bool)), _scopePlot, SLOT(enableYgrid(bool)));
	connect(pause, SIGNAL(toggled(bool)), _scopePlot, SLOT(pause(bool)));

	connect(plotGroup, SIGNAL(buttonReleased(int)), this, SLOT(plotType(int)));

}

ScopePlotTest::~ScopePlotTest() {
}

void
ScopePlotTest::setScopePoke(ScopePoke* scopePoke) {
	_scopePoke = scopePoke;
}

void 
ScopePlotTest::plotType(int j)
{


	switch (j) {
	  case 0:
		  if (_scopePoke)
			  _scopePoke->setPlotType(ScopePlot::TIMESERIES);
		  break;
	  case 1:
		  if (_scopePoke)
			  _scopePoke->setPlotType(ScopePlot::IVSQ);
		  break;
	  case 2:
		  if (_scopePoke)
			  _scopePoke->setPlotType(ScopePlot::SPECTRUM);
		  break;
	  default: {}
	}

}

