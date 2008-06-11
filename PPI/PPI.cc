#include "PPI.h"
#include <math.h>
#include <iostream>
#include <fstream>

#include <qtimer.h>
#include <QResizeEvent>

#include <GL/glut.h>

//
//
// Any drawing action must insure that the GL context is current. paintGL() and
// resize can be called by QGLWidget, and it will make sure that the context
// is current. However, external calls to zoom, pan, draw a new beam,
// and so forth will not have come through QGLWidget's GL code, and so we need
// to explicitly make sure that the context is current in these cases.
//
//
////////////////////////////////////////////////////////////////

PPI::beam::beam(double startAngle, double stopAngle, int nGates, int nVars):
_startAngle(startAngle),
_stopAngle(stopAngle),
_nVars(nVars),
_nGates(nGates)
{
	float cos1 = cos(M_PI*startAngle/180.0)/nGates;
	float sin1 = sin(M_PI*startAngle/180.0)/nGates;
	float cos2 = cos(M_PI*stopAngle/180.0)/nGates;
	float sin2 = sin(M_PI*stopAngle/180.0)/nGates;


	// now calculate the vertex values, to be used for all variables
	for (int j = 0; j < _nGates; j++) {
		_triStripVertices.push_back(j*cos1);
		_triStripVertices.push_back(j*sin1);
		_triStripVertices.push_back(j*cos2);
		_triStripVertices.push_back(j*sin2);
	}
	// Allocate space for the colors. Each vertex has an red, green and
	// blue component, and there are 2 vertices per gate.
	_varColors.resize(nVars);
	for (int v = 0; v < nVars; v++) {
		_varColors[v].resize(_nGates*6);
	}
	// there will be one display list id for each variable
	for (int i = 0; i < _nVars; i++) {
		///@todo add test to insure that the list has been created
		GLuint id = glGenLists(1);
		_glListId.push_back(id);
	}
}
////////////////////////////////////////////////////////////////

PPI::beam::~beam()
{
	for (unsigned int i = 0; i < _glListId.size(); i++)
		glDeleteLists(_glListId[i], 1);

	_glListId.clear();
	_varColors.clear();
	_triStripVertices.clear();
}


////////////////////////////////////////////////////////////////
GLfloat*
PPI::beam::vertices() 
{
	return &_triStripVertices[0];
}

////////////////////////////////////////////////////////////////
GLfloat*
PPI::beam::colors(int varN) 
{
	return &(_varColors[varN])[0];
}

////////////////////////////////////////////////////////////////
static bool glutInitialized = false;

PPI::PPI(QWidget* parent):
QGLWidget(parent),
_decimationFactor(1),
_selectedVar(0),
_zoomFactor(1.0),
_currentX(0.0),
_currentY(0.0),
_gridRingsColor("black"),
_backgroundColor("lightblue"),
_ringsEnabled(true),
_gridsEnabled(false),
_resizing(false),
_scaledLabel(ScaledLabel::DistanceEng),
_configured(false)
{
	initializeGL();

	if (!glutInitialized) {
	  int argc = 1;
	  char* argv[2];
	  argv[0] = "dummy";
	  argv[1] = 0;

	  glutInit(&argc, argv);
	  glutInitialized = true;
	}

	this->setAutoBufferSwap(false);

	// connect the resize timer
	_resizeTimer.setSingleShot(true);
	connect(&_resizeTimer, SIGNAL(timeout()), this, SLOT(resizeTimerTimeout()));
}
////////////////////////////////////////////////////////////////

void
PPI::configure(int nVars,
			   int maxGates, 
			   double distanceSpanKm,
			   int decimationFactor,
               double left,
               double right,
               double bottom,
               double top) 
{
	// Configure for dynamically allocated beams
	_nVars = nVars;
	_maxGates = maxGates/decimationFactor;
	_preAllocate = false;
	_distanceSpanKm = distanceSpanKm;
	_decimationFactor = decimationFactor;  
	_left = left;
    _right = right;
    _bottom = bottom;
    _top = top;

	_configured = true;
}
////////////////////////////////////////////////////////////////

void
PPI::configure(int nVars,
			   int maxGates,
			   int nBeams, 
			   double distanceSpanKm,
			   int decimationFactor,
			   double left,
			   double right,
			   double bottom,
			   double top) 
{
	// Configure for preallocated beamd
	_nVars = nVars;
	_maxGates = maxGates/decimationFactor;
	_preAllocate = true;
	_distanceSpanKm = distanceSpanKm;
	_decimationFactor = decimationFactor;
	_left = left;
	_right = right;
	_bottom = bottom;
    _top = top;

    _configured = true;

	for (unsigned int i = 0; i < _beams.size(); i++)
		delete _beams[i];
	_beams.clear();

	makeCurrent();
	// This constructor is called when we are preallocating beams.
	double angleInc = 360.0/nBeams;
	for (int i = 0; i < nBeams; i++) {
		_beams.push_back(new beam(i*angleInc, (i+1)*angleInc, _maxGates, _nVars));
	}
}
////////////////////////////////////////////////////////////////

PPI::~PPI() {

	// delete all of the dynamically created beams
	for (unsigned int i = 0; i < _beams.size(); i++) {
		delete _beams[i];
	}
}

////////////////////////////////////////////////////////////////

void 
PPI::initializeGL()
{
	glClearColor(_backgroundColor.red()/255.0,
		_backgroundColor.green()/255.0,
		_backgroundColor.blue()/255.0,
		0.0f);

	glDrawBuffer(GL_FRONT);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);

	glShadeModel(GL_FLAT);

	glLineWidth(1.0);


	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_POINT_SMOOTH);
	glDisable(GL_POLYGON_SMOOTH);
	glDisable(GL_DITHER);
	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_INDEX_ARRAY);
	glDisable(GL_EDGE_FLAG_ARRAY);
	glDisable(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_NORMAL_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);
	glDisableClientState(GL_EDGE_FLAG_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glEnable(GL_COLOR_ARRAY);
	glEnable(GL_VERTEX_ARRAY);
	glEnable(GL_STENCIL_TEST);

	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	// set the stencil buffer clear value.
	glClearStencil((GLint)0);

	// get a display list id for the rings
	_ringsListId = glGenLists(1);
	// get a display list id for the grid
	_gridListId = glGenLists(1);
}

////////////////////////////////////////////////////////////////

void 
PPI::resizeGL( int w, int h )
{
	// setup viewport, projection etc.:
	glViewport( 0, 0, (GLint)w, (GLint)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluOrtho2D(_left, _right, _bottom, _top);
	glMatrixMode(GL_MODELVIEW);
}

////////////////////////////////////////////////////////////////

void
PPI::paintGL()
{
	if(_resizing) {
		// clear the image
		glClear(GL_COLOR_BUFFER_BIT);
		return;
	}

	// draw into the back buffer
	glDrawBuffer(GL_BACK);

	// clear the display
	glClear(GL_COLOR_BUFFER_BIT);

	// redraw the beams
	for (unsigned int i = 0; i < _beams.size(); i++) {
		glCallList(_beams[i]->_glListId[_selectedVar]);
	}

	// draw rings/grid
	if (_ringsEnabled || _gridsEnabled) {
		//createStencil();
		makeRingsAndGrids();
		if (_ringsEnabled)
			glCallList(_ringsListId);
		if (_gridsEnabled)
			glCallList(_gridListId);
	} 

	// display the back buffer
	swapBuffers();

	// and resume drawing to the front buffer.
	glDrawBuffer(GL_FRONT);
}

////////////////////////////////////////////////////////////////
void
PPI::rings(bool enabled) {
	_ringsEnabled = enabled;

	//redraw
	makeCurrent();
	paintGL();
}

////////////////////////////////////////////////////////////////
void
PPI::grids(bool enabled) {
	_gridsEnabled = enabled;

	//redraw
	makeCurrent();
	paintGL();
}
////////////////////////////////////////////////////////////////

void 
PPI::setZoom(double factor)
{

	makeCurrent();
	_zoomFactor = factor;
	// if the zoom request is to go smaller than 1:1, 
	// restore to centered normal display
	if (_zoomFactor <= 1.0) {
		_zoomFactor = 1.0;
		_currentX = 0.0;
		_currentY = 0.0;
	}

	glLoadIdentity();
	glScalef(_zoomFactor, _zoomFactor, 1.0);
	glTranslatef(_currentX, _currentY, 0.0);
	// redraw
	paintGL();
}

////////////////////////////////////////////////////////////////

double
PPI::getZoom()
{
	return _zoomFactor;
}

////////////////////////////////////////////////////////////////

void
PPI::refresh()
{
	if(_resizing)
		return;
	//redraw
	makeCurrent();
	glClear(GL_COLOR_BUFFER_BIT);
	paintGL();
}
////////////////////////////////////////////////////////////////

void
PPI::pan(double x, double y) 
{
	makeCurrent();

	glTranslatef(x-_currentX, y-_currentY, 0.0);

	_currentX = x;
	_currentY = y;

	// redraw
	paintGL();
	return;
}

////////////////////////////////////////////////////////////////

void
PPI::resetView() 
{
	makeCurrent();

	_currentX = 0.0;
	_currentY = 0.0;
	_zoomFactor = 1.0;

	glLoadIdentity();
	glScalef(_zoomFactor, _zoomFactor, 1.0);
	glTranslatef(_currentX, _currentY, 0.0);
	// redraw
	paintGL();
	return;
}


////////////////////////////////////////////////////////////////
void 
PPI::resizeEvent( QResizeEvent * e )
{

	if(_resizing) {
		makeCurrent();
		glClear(GL_COLOR_BUFFER_BIT);
		return;
	}

	_resizing = true;

	_resizeTimer.start(500);
}

////////////////////////////////////////////////////////////////
void 
PPI::mousePressEvent( QMouseEvent * e )
{
	_oldMouseX = e->x();
	_oldMouseY = e->y();
}

////////////////////////////////////////////////////////////////
void 
PPI::mouseMoveEvent( QMouseEvent * e )
{
	makeCurrent();

	int x = e->x();
	int y = e->y();

	double deltaX = (x - _oldMouseX) / (double)width() / _zoomFactor / 2.0;
	double deltaY = (_oldMouseY - y) / (double)height()/ _zoomFactor / 2.0;

	glTranslatef(deltaX/2.0, deltaY/2.0, 0.0);

	_currentX += deltaX;
	_currentY += deltaY;

	// redraw
	updateGL();
}

////////////////////////////////////////////////////////////////
void 
PPI::resizeTimerTimeout()
{
	makeCurrent();
	resizeGL(this->width(), this->height());
	_resizing = false;
	refresh();
}

////////////////////////////////////////////////////////////////

int
PPI::numBeams() 
{
	return _beams.size();
}

////////////////////////////////////////////////////////////////////////

void 
PPI::selectVar(int index) 
{
	_selectedVar = index;
	updateGL();
	return;
}

////////////////////////////////////////////////////////////////////////

void
PPI::clearVar(int index) 
{
	if (index >= _nVars)
		return;

	// calling makeDisplayList with data == 0 causes the display list to 
	// be drawn completely with the background color.
	float r = _backgroundColor.red()/255.0;
	float g = _backgroundColor.green()/255.0;
	float b = _backgroundColor.blue()/255.0;

	for (unsigned int i = 0; i < _beams.size(); i++) {
		int cIndex = 0;
		GLfloat* colors = _beams[i]->colors(index);
		for (int gate = 0; gate < _maxGates; gate++) {
			colors[cIndex++] = r;
			colors[cIndex++] = g;
			colors[cIndex++] = b;
			colors[cIndex++] = r;
			colors[cIndex++] = g;
			colors[cIndex++] = b;
		}
	}

	if (index == _selectedVar) {
		selectVar(index);
	}

}

////////////////////////////////////////////////////////////////

void 
PPI::addBeam(float startAngle, 
			 float stopAngle, 
			 int gates, 
			 std::vector<std::vector<double> >& _beamData, 
			 int stride, 
			 std::vector<ColorMap*>& maps)
{

	makeCurrent();

	// add a new beam to the display. 
	// The steps are:
	// 1. preallocate mode: find the beam to be drawn, or dynamic mode:
	//    create the beam(s) to be drawn.
	// 2. fill the colors for all variables in the beams to be drawn
	// 3. make the display list for the selected variables in the beams
	//    to be drawn.
	// 4. call the new display list(s)

	beam* b;
	std::vector<beam*> newBeams;

	// the start and stop angle MUST specify a counterclockwise fill for the sector. Thus 
	// if startAngle > stopAngle, we know that we have crossed the 0 boundary, and must
	// break it up into 2 beams.

	// create the new beam(s), to keep track of the display information
	// Beam start and stop angles are adjusted here so that they always 
	// increase counterclockwise. Like wise, if a beam crosses the 0 degree
	// boundary, it is split into two beams, each of them again obeying the
	// counterclockwise rule. Prescribing these rules makes the beam culling
	// logic a lot simpler.

	startAngle = startAngle - ((int)(startAngle/360.0))*360.0;
	stopAngle  = stopAngle  - ((int)( stopAngle/360.0))*360.0;

	if (startAngle <= stopAngle) 
	{

		if (_preAllocate) {
			b = _beams[beamIndex(startAngle, stopAngle)];
			newBeams.push_back(b);
		} else {
			beam* b = new beam(startAngle, stopAngle, _maxGates, _nVars);
			_beams.push_back(b);
			newBeams.push_back(b);
		}
	} 
	else 
	{
		if (_preAllocate) {
			b = _beams[beamIndex(startAngle, 360.0)];
			newBeams.push_back(b);
		} else {
			b = new beam(startAngle, 360.0, _maxGates, _nVars);
			_beams.push_back(b);
			newBeams.push_back(b);
		}
		if (_preAllocate) {
			b = _beams[beamIndex(0.0, stopAngle)];
			newBeams.push_back(b);
		} else {
			b = new beam(0.0, stopAngle, _maxGates, _nVars);
			_beams.push_back(b);
			newBeams.push_back(b);
		}
		newBeams.push_back(b);
	}

	// newBeams has collected the beams to be rendered; now fill in 
	// their colors and draw them
	for (unsigned int i = 0; i < newBeams.size(); i++) {
		b = newBeams[i];
		fillColors(b, _beamData, gates, stride, maps);

		for (int v = 0; v < _nVars; v++) {
			makeDisplayList(b, v);
		}
	}
	
	// draw it
	for (unsigned int i = 0; i < newBeams.size(); i++) {
		b = newBeams[i];
		if(!_resizing)
			glCallList(b->_glListId[_selectedVar]);
	}

	// draw the rings and grid if they are enabled. Don't worry,
	// it is only two display list calls. They are relative short
	// lists compared to the beam drawing, and done on the graphics card anyway.
	if (_ringsEnabled)
		glCallList(_ringsListId);

	if (_gridsEnabled)
		glCallList(_gridListId);

	if (!_resizing)
		glFlush();

	if (!_preAllocate) {
		// in dynamic mode, cull hidden beams
		cullBeamList();
	}

}
////////////////////////////////////////////////////////////////

void
PPI::fillColors(beam* beam, 
				std::vector<std::vector<double> >& _beamData, 
				int gates, 
				int stride,
				std::vector<ColorMap*>& maps) 
{

	float red, green, blue;
	for (int v = 0; v < _nVars; v++) {

		ColorMap* map = maps[v];
		GLfloat* colors = beam->colors(v);
		int cIndex = 0;

		double* varData = &(_beamData[v][0]);
		for (int gate = 0; gate < gates; gate += _decimationFactor) {
			double data = varData[gate];
			map->dataColor(data, red, green, blue);
			colors[cIndex++] = red;
			colors[cIndex++] = green;
			colors[cIndex++] = blue;
			colors[cIndex++] = red;
			colors[cIndex++] = green;
			colors[cIndex++] = blue;
		}
		float r = _backgroundColor.red()/255.0;
		float g = _backgroundColor.green()/255.0;
		float b = _backgroundColor.blue()/255.0;

		for (int gate = gates; gate < _maxGates; gate++) {
			colors[cIndex++] = r;
			colors[cIndex++] = g;
			colors[cIndex++] = b;
			colors[cIndex++] = r;
			colors[cIndex++] = g;
			colors[cIndex++] = b;
		}
	} 
}

////////////////////////////////////////////////////////////////

void
PPI::makeDisplayList(beam* b, int v) 
{
//	glGenBuffers();
	
	// create a display list to hold the gl commands
	glNewList(b->_glListId[v], GL_COMPILE);

	// set the vertex pointer
	glVertexPointer(2, GL_FLOAT, 0, b->vertices());

	// set the colors pointer
	glColorPointer(3, GL_FLOAT, 0, b->colors(v));

	// draw a triangle strip. 
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2*_maxGates);

	// end the display list
	glEndList();

}

////////////////////////////////////////////////////////////////////////

void
PPI::cullBeamList()
{
	// This routine examines the collection of beams, and removes those that are 
	// completely occluded by other beams. The algorithm gives precedence to the 
	// most recent beams; i.e. beams at the end of the _beams vector.
	//
	// Remember that there won't be any beams that cross angles through zero; 
	// otherwise the beam culling logic would be a real pain, and PPI has
	// already split incoming beams into two, if it received a beam of this type.
	//
	// The logic is as follows. First of all, just consider the start and stop angle 
	// of a beam to be a linear region. We can diagram the angle interval of beam(AB) as:
	//         a---------b
	// 
	// The culling logic will compare all other beams (XY) to AB, looking for an overlap.
	// An example overlap might be:
	//         a---------b
	//    x---------y
	// 
	// If an overlap on beam XY is detected, the occluded region is recorded as the interval (CD):        
	//         a---------b
	//    x---------y
	//         c----d
	// 
	// The culling algorithm starts with the last beam in the list, and compares it with all
	// preceeding beams, setting their overlap regions appropriately. Then the next to the last
	// beam is compared with all preceeding beams. Previously found occluded regions will be 
	// expanded as they are detected.
	// 
	// Once the occluded region spans the entire beam, then the beam is known 
	// to be hidden, and it doesn't need to be tested any more, nor is it it used as a 
	// test on other beams.
	//
	// After the list has been completly processed in this manner, the completely occluded 
	// beams are removed.
	// .
	// Note now that if the list is rendered from beginning to end, the more recent beams will
	// overwrite the portions of previous beams that they share.
	//

	// do nothing if we don't have at least two beams 
	if (_beams.size() < 2)
		return;

	int i;
	int j;
	beam* beamAB;
	beam* beamXY;

	// initialize the house keeping
	for (unsigned int k = 0; k < _beams.size(); k++) {
		beamAB = _beams[k];
		beamAB->_hidden = false;
		beamAB->_c = -1.0;
		beamAB->_d = -1.0;
	}

	// Here is an outerloop and a nested innerloop. Work backwords
	// from the end of the display list, comparing one beam to all
	// of those which preceed it.
	for (i = _beams.size()-1; i >= 1; i--) {
		// select the next preceeding beam on the beam list
		beamAB = _beams[i];

		// if this beam is hidden, we don't need to compare it to preceeding ones, 
		// since they will be hidden by the ones that hid this beam.
		if (!beamAB->_hidden) {
			double a = beamAB->_startAngle;
			double b = beamAB->_stopAngle;

			for (j = i-1; j >= 0; j--) {
				// and compare it to all of its predecessors
				beamXY = _beams[j];

				// if this beam has alread been marked hidden, we don't need to 
				// look at it.
				if (!beamXY->_hidden) {
					double x = beamXY->_startAngle;
					double y = beamXY->_stopAngle;

					if (b <= x || a >= y) { 
						//  handles these cases:
						//  a-----b           a-----b
						//        x-----------y
						//  
						// they don't overlap at all
					} 
					else 
					{

						if (a <= x && b >= y) {
							//        a-----------b
							//        x-----------y
							// completely covered
							beamXY->_hidden = true;
						}  
						else 
						{

							if (a <= x && b <= y) {
								//   a-----------b
								//        x-----------y
								beamXY->_c = x;
								if (beamXY->_d < b)
									beamXY->_d = b;
								if ((beamXY->_c == x) && (beamXY->_d == y)) 
									beamXY->_hidden = true;
							} 
							else 
							{

								if (a >= x && b >= y) {
									//       a-----------b
									//   x-----------y
									beamXY->_d = y;
									if (a < beamXY->_c)
										beamXY->_c = a;
									if ((beamXY->_c == x) && (beamXY->_d == y)) 
										beamXY->_hidden = true;
								} 

								//   // all that is left is this pathological case:
								//      //     a-------b
								//      //   x-----------y
								//      // we need to extend c and d, if the are inside of a and b.
								if (beamXY->_c > a)
									beamXY->_c = a;
								if (beamXY->_d < b)
									beamXY->_d = b;

							}//               if (a <= x && b <= y) {
						} //            if (a <= x && b >= y) {
					} //           if (b <= x || a >= y) { 
				} //          if (!beamXY->_hidden) {
			}//        for (j = b; j >= 0; j--) {
		}//  if (beamAB->_hidden) {
	} //  for (i = _beams.size()-1; i >= 1; i--) {

	// now actually cull the list
	int nCulled = 0;
	for (i = _beams.size()-1; i >= 0; i--) {
		if (_beams[i]->_hidden) {
			delete _beams[i];
			_beams.erase(_beams.begin()+i);
			nCulled++;
		}
	}
}

////////////////////////////////////////////////////////////////////////

int
PPI::beamIndex(double startAngle, double stopAngle)
{
	int i = (int)(_beams.size()*(startAngle + (stopAngle-startAngle)/2)/360.0);
	if (i<0)
		i = 0;
	if (i>(int)_beams.size()-1)
		i = _beams.size()-1;

	return i;
}

////////////////////////////////////////////////////////////////////////
void
PPI::backgroundColor(QColor color)
{
	_backgroundColor = color;
	glClearColor(_backgroundColor.red()/255.0,
		_backgroundColor.green()/255.0,
		_backgroundColor.blue()/255.0,
		0.0f);

	makeCurrent();
	updateGL();
}
////////////////////////////////////////////////////////////////////////
void
PPI::gridRingsColor(QColor color)
{
	_gridRingsColor   = color;

	makeCurrent();
	updateGL();
}

////////////////////////////////////////////////////////////////////////
void
PPI::makeRingsAndGrids() {

	// don't try to draw rings if we haven't been configured yet
	if (!_configured)
		return;
	
	// or if the rings or grids aren't enabled
	if (!_ringsEnabled && !_gridsEnabled)
		return;

	double ringDelta = ringSpacing();
	double ringLabelIncrement = ringDelta;
	double ringLabelOffset = 0.02/_zoomFactor;  // used to move some of the labelling so that it does not overlap the rings.
	double lineWidth = 0.004/ _zoomFactor;

	// Do range rings?
	if (ringDelta > 0 && _ringsEnabled) {

		// create a display list to hold the gl commands
		glNewList(_ringsListId, GL_COMPILE);

		// set the color
		glColor3f(_gridRingsColor.red()/255.0, 
			_gridRingsColor.green()/255.0,
			_gridRingsColor.blue()/255.0);

		// Get a new quadric object.
		GLUquadricObj *quadObject = gluNewQuadric();

		GLdouble radius = ringDelta;

		// Draw our range rings.
		while (radius <= 1.0) {
			gluDisk(quadObject,radius-lineWidth/2,radius+lineWidth/2,100,1);
			radius += ringDelta;
		}

		// label the rings
		if (ringLabelIncrement > 0.0) {
			std::vector<std::string> ringLabels;
			// creat the labels. Note that we are not creating a lable at zero
			for (int i = 0; i < 1/ringLabelIncrement; i++) {
				double value = (i+1)*ringLabelIncrement*_distanceSpanKm / 2.0;
				ringLabels.push_back(_scaledLabel.scale(value));
			}

			for (unsigned int j = 0 ; j < ringLabels.size(); j++) {
				double d = 0.707*(j+1)*ringDelta;
				const char* cStart = ringLabels[j].c_str();
				const char* c;

				// upper right qudrant lables
				glRasterPos2d( d,  d); c = cStart;
				while (*c) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*c++);

				// lower left quadrant labels
				glRasterPos2d(-d, -d); c = cStart;
				while (*c) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*c++);

				// lower right quadrant labels
				glRasterPos2d( d+ringLabelOffset, -d-ringLabelOffset); c = cStart;
				while (*c) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*c++);

				// upper left qudrant labels
				glRasterPos2d(-d+ringLabelOffset,  d-ringLabelOffset); c = cStart;
				while (*c) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*c++);

			}
		}
		// get rid of quad object
		gluDeleteQuadric(quadObject);

		glEndList();

	}

	// do the grid
	if (ringDelta > 0 && _gridsEnabled) {

		// create a display list to hold the gl commands
		glNewList(_gridListId, GL_COMPILE);

		// set the color
		glColor3f(_gridRingsColor.red()/255.0, 
			_gridRingsColor.green()/255.0,
			_gridRingsColor.blue()/255.0);

		glLineWidth(2);

		glBegin(GL_LINES);
		// First the vertical lines.
		// set the first x value
		GLdouble x = (-(int)((1.0/ringDelta)/2)) * ringDelta;
		while (x <= 1.0) {
			glVertex2d(x, -1.0); 
			glVertex2d(x,  1.0); 
			x += ringDelta;
		}
		// Now horizontial lines
		// set the first y value to an even increment of the grid spacing.
		GLdouble y = (-(int)((1.0/ringDelta)/2)) * ringDelta;;
		while (y <= 1.0) {
			glVertex2d(-1.0, y); 
			glVertex2d( 1.0, y); 
			y += ringDelta;
		}
		glEnd();

		glEndList();
	}
}

////////////////////////////////////////////////////////////////////////
double
PPI::ringSpacing() {

	// R is the visible distance from center to edge
	double R = (_distanceSpanKm / _zoomFactor);
	double e = (int)floor(log10(R));
	double Rn = R / pow(10.0, e);

	double delta = 2.0;
	if (Rn <= 5.0) {
		delta = 1.0;
	}
	if (Rn <= 3.0) {
		delta = 0.5;
	} 
	if (Rn <= 1.0) {
		delta = 0.2;
	} 
	if (Rn <= 0.5) {
		delta = 0.1;
	} 

	delta = delta * pow(10.0, e);

	delta = delta/_distanceSpanKm;

	return delta;

}
////////////////////////////////////////////////////////////////////////
QImage*
PPI::getImage() {
	makeCurrent();
	updateGL();
	glReadBuffer(GL_FRONT);
	QImage* pImage = new QImage(grabFrameBuffer(true));
	return pImage;
}

////////////////////////////////////////////////////////////////////////
QPixmap*
PPI::getPixmap() {
	QPixmap* pImage = new QPixmap(renderPixmap());
	return pImage;
}
