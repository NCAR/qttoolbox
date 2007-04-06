#include "PPITest.h"
#include "PPI.h"
#include <qlcdnumber.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>
#include <qlayout.h>
#include <qcheckbox.h>
#include <qcolordialog.h>
#include <QVBoxLayout>

#include <iostream>

PPITest::PPITest( int nVars,
				 QWidget* parent):
QObject(parent),
_angle(0.0),
_nVars(nVars),
_angleInc(1.0),
_gates(1000)
{
	setupUi(parent);

	// create the color maps
	for (int i = 0; i < _nVars; i++) {
		_maps1.push_back(new ColorMap(0.0, 100.0/(i+1)));
		_maps2.push_back(new ColorMap(0.0, 100.0/((i+1)/0.8)));
	}

	_colorBar1->configure(*_maps1[0]);
	_colorBar2->configure(*_maps2[0]);
	////////////////////////////////////////////////////////////////////////
	//
	// Create radio buttons for selecting the variable to display
	//

	// create a layout manager for the button groups
	QVBoxLayout* buttonLayout1 = new QVBoxLayout;
	QVBoxLayout* buttonLayout2 = new QVBoxLayout;
	// create button groups to manage the buttons as collections
	// These are different from the button group boxes, which just
	// hold the buttons on the form
	QButtonGroup* buttonGroup1 = new QButtonGroup;
	QButtonGroup* buttonGroup2 = new QButtonGroup;

	// create the buttons
	for (int r = 0; r < _nVars; r++) {
		// make buttons
		QRadioButton* rb1 = new QRadioButton;
		QRadioButton* rb2 = new QRadioButton;
		// add them to the groups
		buttonGroup1->addButton(rb1, r);
		buttonGroup2->addButton(rb2, r);
		// add buttons to the group layout
		buttonLayout1->addWidget(rb1);
		buttonLayout2->addWidget(rb2);
		QString s = QString("%1").arg(r);
		rb1->setText(s);
		rb2->setText(s);
		if (r == 0){
			rb1->setChecked(true);
			rb2->setChecked(true);
		}
	}
	// connect the button pressed signal to the var changed slot
	connect( buttonGroup1, SIGNAL( buttonReleased(int) ), this, SLOT( varSelectSlot1(int) ));
	connect( buttonGroup2, SIGNAL( buttonReleased(int) ), this, SLOT( varSelectSlot2(int) ));
	// assign the layout to the group boxes on the form
	_buttonGroupBox1->setLayout(buttonLayout1);
	_buttonGroupBox2->setLayout(buttonLayout2);

	// Note that the following call determines whether PPI will 
	// use preallocated or dynamically allocated beams. If a third
	// parameter is specifiec, it will set the number of preallocated
	// beams.
	_ppi1->configure(_nVars, _gates, 360, 117.0);
	_ppi2->configure(_nVars, _gates, 360, 223.0);

	// set the rings to the current state of the check box
	_ppi1->rings(_ringsCheck->isChecked());
	_ppi2->rings(_ringsCheck->isChecked());


	connect(_startButton,   SIGNAL(released()),     this, SLOT(startSlot()));
	connect(_stopButton,    SIGNAL(released()),     this, SLOT(stopSlot()));
	connect(_gridCheck,     SIGNAL(clicked(bool)),  this, SLOT(gridSlot(bool)));
	connect(_ringsCheck,    SIGNAL(clicked(bool)),  this, SLOT(ringsSlot(bool)));
	connect(_reverseButton, SIGNAL(released()),     this, SLOT(changeDir()));
	connect(_zoomIn,        SIGNAL(released()),     this, SLOT(zoomInSlot()));
	connect(_zoomOut,       SIGNAL(released()),     this, SLOT(zoomOutSlot()));
	connect(&_timer,        SIGNAL(timeout()),      this, SLOT(addBeam()));
	connect(_backgroundButton, SIGNAL(released()),  this, SLOT(backgroundColorSlot()));

	_beamData.resize(_nVars);
}


///////////////////////////////////////////////////////////////////////

PPITest::~PPITest() 
{
	delete _colorBar1;
	delete _colorBar2;
	for (int i = 0; i < _maps1.size(); i++)
		delete _maps1[i];
	for (int i = 0; i < _maps2.size(); i++)
		delete _maps2[i];
}
///////////////////////////////////////////////////////////////////////

void
PPITest::startSlot() {

	_timer.start(50);

}

///////////////////////////////////////////////////////////////////////

void
PPITest::stopSlot() {
	_timer.stop();

}
///////////////////////////////////////////////////////////////////////

void
PPITest::addBeam() 
{

	int rep = 1;
	double angle;

	angle = _angle;
	for (int t = 0; t < rep; t++) { 
		double startAngle;
		double stopAngle;

		if (angle > 360.0)
			angle -= 360.0;

		if (_angleInc > 0.0) {
			startAngle = angle-0.05;
			stopAngle = angle + _angleInc+0.05;
		} else {
			startAngle = angle + _angleInc-0.05;
			stopAngle = angle+0.05;
		}

		if (startAngle < 0.0)
			startAngle += 360.0;
		if (startAngle > 360.0)
			startAngle -= 360.0;
		if (stopAngle < 0.0)
			stopAngle += 360.0;
		if (stopAngle > 360.0)
			stopAngle -= 360.0;

		for (int v = 0; v < _nVars; v++) {
			_beamData[v].resize(_gates);
			for (int g = 0; g < _gates; g++) {
				if (v == 0)
					_beamData[v][g] = 100.0*((double)rand())/RAND_MAX;
				else
					_beamData[v][g] = 50.0*(angle/360.0)+10.0*((double)rand())/RAND_MAX;
			}
		}

		_ppi1->addBeam(startAngle, stopAngle, _gates, _beamData, 1, _maps1);
		_ppi2->addBeam(startAngle, stopAngle, _gates, _beamData, 1, _maps2);

		angle += _angleInc;

		LCDNumberCurrentAngle->display(angle);
		LCDNumberBeamCount->display(_ppi2->numBeams());
		LCDNumberZoomFactor->display(_ppi2->getZoom());
	}

	_angle += rep*_angleInc;
	if (_angle > 360.0)
		_angle -= 360.0;

	if (_angle < 0.0)
		_angle += 360.0;
}


///////////////////////////////////////////////////////////////////////

void PPITest::zoomInSlot()
{
	_ppi1->setZoom(2.0);
	_ppi2->setZoom(2.0);
}

///////////////////////////////////////////////////////////////////////

void PPITest::zoomOutSlot()
{
	_ppi1->setZoom(0.5);
	_ppi2->setZoom(0.5);
}

///////////////////////////////////////////////////////////////////////

void PPITest::varSelectSlot1(int index)
{
	_ppi1->selectVar(index); 
	_colorBar1->configure(*_maps1[index]);
}

///////////////////////////////////////////////////////////////////////

void PPITest::varSelectSlot2(int index)
{
	_ppi2->selectVar(index); 
	_colorBar2->configure(*_maps2[index]);
}

///////////////////////////////////////////////////////////////////////

void PPITest::changeDir() 
{
	_angleInc = -1.0*_angleInc;
}

///////////////////////////////////////////////////////////////////////

void PPITest::clearVarSlot(int index)
{
	_ppi1->clearVar(index);
	_ppi2->clearVar(index);
}

///////////////////////////////////////////////////////////////////////

void PPITest::ringsSlot(bool enabled)
{
	_ppi1->rings(enabled);
	_ppi2->rings(enabled);
}
///////////////////////////////////////////////////////////////////////

void PPITest::gridSlot(bool enabled)
{
	_ppi1->grids(enabled);
	_ppi2->grids(enabled);
}
///////////////////////////////////////////////////////////////////////
void
PPITest::backgroundColorSlot() 
{
	QColor color = QColorDialog::getColor("white");

	_ppi1->backgroundColor(color);
	_ppi1->refresh();
	_ppi2->backgroundColor(color);
	_ppi2->refresh();
}

///////////////////////////////////////////////////////////////////////

void PPITest::panSlot(int panIndex)
{
	switch (panIndex) {
	case 0:
		_ppi1->setZoom(0.0);
		_ppi2->setZoom(0.0);
		break;
	case 1:
		_ppi1->pan(0.0, 0.10);
		_ppi2->pan(0.0, 0.10);
		break;
	case 2:
		_ppi1->pan(-0.10, 0.10);
		_ppi2->pan(-0.10, 0.10);
		break;
	case 3:
		_ppi1->pan(-0.10, 0.0);
		_ppi2->pan(-0.10, 0.0);
		break;
	case 4:
		_ppi1->pan(-0.10, -0.10);
		_ppi2->pan(-0.10, -0.10);
		break;
	case 5:
		_ppi1->pan(00, -0.10);
		_ppi2->pan(00, -0.10);
		break;
	case 6:
		_ppi1->pan(0.10, -0.10);
		_ppi2->pan(0.10, -0.10);
		break;
	case 7:
		_ppi1->pan(0.10, 0.0);
		_ppi2->pan(0.10, 0.0);
		break;
	case 8:
		_ppi1->pan(0.10, 0.10);
		_ppi2->pan(0.10, 0.10);
		break;
	default:
		break;
	}
}

