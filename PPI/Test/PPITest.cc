#include "PPITest.h"
#include "PPI.h"
#include <qlcdnumber.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>
#include <qlayout.h>
#include <qcheckbox.h>
#include <qcolordialog.h>

#include <iostream>

PPITest::PPITest( int nVars,
				 QWidget* parent, 
				 const char* name, 
				 bool modal, 
				 WFlags fl):
PPITestBase(parent,
			name,
			modal,
			fl),
			_angle(0.0),
			_nVars(nVars),
			_angleInc(1.0),
			_gates(1000)
{

	// create the color maps
	for (int i = 0; i < _nVars; i++) {
		_maps1.push_back(new ColorMap(0.0, 100.0/(i+1)));
		_maps2.push_back(new ColorMap(0.0, 100.0/((i+1)/0.8)));
	}

	QVBoxLayout* colorBarLayout1 = new QVBoxLayout(frameColorBar1);
	QVBoxLayout* colorBarLayout2 = new QVBoxLayout(frameColorBar2);
	_colorBar1 = new ColorBar(frameColorBar1);
	_colorBar2 = new ColorBar(frameColorBar2);
	_colorBar1->configure(*_maps1[0]);
	_colorBar2->configure(*_maps2[0]);
	colorBarLayout1->addWidget(_colorBar1);
	colorBarLayout2->addWidget(_colorBar2);
	////////////////////////////////////////////////////////////////////////
	//
	// Create radio buttons for selecting the variable to display
	//

	// configure policy for the button group
	btnGroupVarSelect1->setColumnLayout(0, Qt::Vertical );
	btnGroupVarSelect2->setColumnLayout(0, Qt::Vertical );
	btnGroupVarSelect1->layout()->setSpacing( 6 );
	btnGroupVarSelect2->layout()->setSpacing( 6 );

	// create a layout manager for the button group
	QVBoxLayout* btnGroupVarSelectLayout1 = new QVBoxLayout( btnGroupVarSelect1->layout());
	btnGroupVarSelectLayout1->setAlignment( Qt::AlignTop );
	QVBoxLayout* btnGroupVarSelectLayout2 = new QVBoxLayout( btnGroupVarSelect2->layout());
	btnGroupVarSelectLayout2->setAlignment( Qt::AlignTop );

	// create the buttons
	for (int r = 0; r < _nVars; r++) {
		QRadioButton* rb1 = new QRadioButton(btnGroupVarSelect1);
		QRadioButton* rb2 = new QRadioButton(btnGroupVarSelect2);
		// add button to the group layou
		btnGroupVarSelectLayout1->addWidget(rb1);
		btnGroupVarSelectLayout2->addWidget(rb2);
		QString s = QString("%1").arg(r);
		rb1->setText(s);
		rb2->setText(s);
		if (r == 0){
			rb1->setChecked(true);
			rb2->setChecked(true);
		}
	}

	// Note that the following call determines whether PPI will 
	// use preallocated or dynamically allocated beams. If a third
	// parameter is specifiec, it will set the number of preallocated
	// beams.
	_ppi1->configure(_nVars, _gates, 360);
	_ppi2->configure(_nVars, _gates, 360);

   // set the rings to the current state of the check box
   _ppi1->rings(ringsCheckBox->state());
   _ppi1->rings(ringsCheckBox->state());

	// connect the button pressed signal to the var changed slot
	connect( btnGroupVarSelect1, SIGNAL( clicked(int) ), this, SLOT( varSelectSlot1(int) ));
	connect( btnGroupVarSelect2, SIGNAL( clicked(int) ), this, SLOT( varSelectSlot2(int) ));

	////////////////////////////////////////////////////////////////////////
	//
	//  
	//
	connect(&_timer, SIGNAL(timeout()), this, SLOT(addBeam()));

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
				   _beamData[v][g] = 50.0*(angle/360.0);
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
   QColor color = QColorDialog::getColor("white",this,"Colourtable");

   _ppi1->backgroundColor(color);
   _ppi1->refresh();
   _ppi2->backgroundColor(color);
   _ppi2->refresh();
}

///////////////////////////////////////////////////////////////////////
void
PPITest::gridRingColorSlot() 
{
   QColor color = QColorDialog::getColor("white",this,"Colourtable");

   _ppi1->gridRingColor(color);
   _ppi1->refresh();
   _ppi2->gridRingColor(color);
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

