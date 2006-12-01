#include "PPI.h"
#include <math.h>
#include <iostream>
#include <fstream>

//
//
// Any drawing action must insure that the GL context is current. paintGL() and
//resize can be called by QGLWidget, and it will make sure that the context
// is current. However, external calls to zoom, pan, draw a new beam,
// and so forth will not have come through QGLWidget's GL code, and so we need
// to explicitly make sure that the context is current in these cases.
//
//
////////////////////////////////////////////////////////////////

PPI::beam::beam(double startAngle, double stopAngle, int nGates, int nVars):
_startAngle(startAngle),
_stopAngle(stopAngle),
_nGates(nGates),
_nVars(nVars)
{

	float cos1 = cos(3.14159*startAngle/180.0)/nGates;
	float sin1 = sin(3.14159*startAngle/180.0)/nGates;
	float cos2 = cos(3.14159*stopAngle/180.0)/nGates;
	float sin2 = sin(3.14159*stopAngle/180.0)/nGates;

	_varColors.resize(nVars);

	// now calculate the vertex values, to be used for all variables
	for (int j = 0; j < _nGates; j++) {
		_triStripVertices.push_back(j*cos1);
		_triStripVertices.push_back(j*sin1);
		_triStripVertices.push_back(j*cos2);
		_triStripVertices.push_back(j*sin2);
	}
   // Allocate space for the colors. Each vertex has an red, green and
   // blue component, and there are 2 vertices per gate.
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

PPI::PPI(QWidget* parent, 
						   const char * name, 
						   const QGLWidget * shareWidget, 
						   WFlags f):
QGLWidget(parent, name, shareWidget, f),
_selectedVar(0),
_zoomFactor(1.0),
_panHoriz(0.0),
_panVert(0.0),
_clearRed(0.6f),
_clearGreen(0.3f),
_clearBlue(0.3f),
_ringsEnabled(true)
{
	initializeGL();
}
////////////////////////////////////////////////////////////////

void
PPI::configure(int nVars,
						int maxGates) 
{
	// Configure for dynamically allocated beams
	_nVars = nVars;
	_maxGates = maxGates;
	_preAllocate = false;
}
////////////////////////////////////////////////////////////////

void
PPI::configure(int nVars,
						int maxGates,
						int nBeams) 
{
	// Configure for preallocated beamd
	_nVars = nVars;
	_maxGates = maxGates;
	_preAllocate = true;

   for (int i = 0; i < _beams.size(); i++)
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

	//	makeCurrent();

	glClearColor(_clearRed, _clearGreen, _clearBlue, 0.0f);

	glPolygonMode(GL_FRONT, GL_FILL);

	glShadeModel(GL_FLAT);

	glLineWidth(1.0);


	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_POINT_SMOOTH);
	glDisable(GL_POLYGON_SMOOTH);
	glDisable(GL_DITHER);
	glDisable(GL_BLEND);

	glEnable(GL_COLOR_ARRAY);
	glEnable(GL_VERTEX_ARRAY);
	glDisable(GL_INDEX_ARRAY);
	glDisable(GL_EDGE_FLAG_ARRAY);
	glDisable(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_NORMAL_ARRAY);

	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);
	glDisableClientState(GL_EDGE_FLAG_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glClearStencil(0.0f);
	glEnable(GL_STENCIL_TEST);
}

////////////////////////////////////////////////////////////////

void 
PPI::resizeGL( int w, int h )
{
	// setup viewport, projection etc.:
	glViewport( 0, 0, (GLint)w, (GLint)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
//	gluOrtho2D(-1.0,1.0, -0.5, 0.8);
	gluOrtho2D(-1.0,1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

////////////////////////////////////////////////////////////////
void
PPI::rings(bool enabled) {
   _ringsEnabled = enabled;

	//redraw
	makeCurrent();
	paintGL();
	swapBuffers();
}
////////////////////////////////////////////////////////////////

void 
PPI::paintGL()
{

  if (_ringsEnabled) {
	glStencilFunc(GL_NEVER, 0x0, 0x0);
	glStencilOp(GL_INCR, GL_INCR, GL_INCR);

 	   // draw range rings
	   glClear(GL_STENCIL_BUFFER_BIT);  
	   glColor3f(1.0, 1.0, 0.0);

      GLUquadricObj* o = gluNewQuadric();

   	for (double x = 0.1; x <=1.0; x += 0.1)
	   	gluDisk(o, x, x+0.003, 100, 1);

   	glStencilFunc(GL_NOTEQUAL, 0x1, 0x1);
	   glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
  } else {
     glClear(GL_STENCIL_BUFFER_BIT);  
	  glColor3f(1.0, 1.0, 0.0);
  }

	glClear(GL_COLOR_BUFFER_BIT);
	for (unsigned int i = 0; i < _beams.size(); i++) {
		glCallList(_beams[i]->_glListId[_selectedVar]);
	}
}

////////////////////////////////////////////////////////////////

void 
PPI::setZoom(double factor)
{

	makeCurrent();
	_zoomFactor *= factor;
	// if the zoom request is to go smaller than 1:1, 
	// restore to centered normal display
	if (_zoomFactor < 1.0) {
		_zoomFactor = 1.0;
		_panHoriz = 0.0;
		_panVert = 0.0;
		factor = 1.0;
		glLoadIdentity();
	}

	// first translate back to zero, so that our zoom doesn't
	// effect an offset
	glTranslatef(-_panHoriz, -_panVert, 0.0);

	// now use the projection matrix to zoom
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0/_zoomFactor, 1.0/_zoomFactor, -1.0/_zoomFactor, 1.0/_zoomFactor);

	// now translate back to where we were panning to
	glMatrixMode(GL_MODELVIEW);
	_panHoriz /= factor;
	_panVert /= factor;
	glTranslatef(_panHoriz, _panVert, 0.0);

	// redraw
	paintGL();
	swapBuffers();
}

////////////////////////////////////////////////////////////////

double
PPI::getZoom()
{
	return _zoomFactor;
}

////////////////////////////////////////////////////////////////

void
PPI::pan(double horizFrac, double vertFrac) 
{
	// pan, by setting up a translation
	glTranslatef(horizFrac/_zoomFactor, vertFrac/_zoomFactor, 0.0);
	_panHoriz += horizFrac/_zoomFactor;
	_panVert += vertFrac/_zoomFactor;

	//redraw
	makeCurrent();
	paintGL();
	swapBuffers();
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
	makeCurrent();
	glClear(GL_COLOR_BUFFER_BIT);
	for (unsigned int i = 0; i < _beams.size(); i++) {
		makeDisplayList(_beams[i],_selectedVar);
		// draw it
		glCallList(_beams[i]->_glListId[_selectedVar]);
	}
	swapBuffers();
}

////////////////////////////////////////////////////////////////////////

void
PPI::clearVar(int index) 
{
	if (index >= _nVars)
		return;

	// calling makeDisplayList with data == 0 casues the display list to be drawn completely with the background color.
	for (unsigned int i = 0; i < _beams.size(); i++) {
		int cIndex = 0;
		GLfloat* colors = _beams[i]->colors(index);
		for (int g = 0; g < _maxGates; g++) {
			colors[cIndex++] = _clearRed;
			colors[cIndex++] = _clearGreen;
			colors[cIndex++] = _clearBlue;
			colors[cIndex++] = _clearRed;
			colors[cIndex++] = _clearGreen;
			colors[cIndex++] = _clearBlue;
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

		makeDisplayList(b,_selectedVar);

		// draw it
		glCallList(b->_glListId[_selectedVar]);
	}

	swapBuffers();

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

	double red, green, blue;
	for (int v = 0; v < _nVars; v++) {

		ColorMap* map = maps[v];
		GLfloat* colors = beam->colors(v);
		int cIndex = 0;
		std::vector<double>& varData = _beamData[v];
		for (int g = 0; g < gates; g++) {
			double data = varData[g];
			map->dataColor(data, red, green, blue);
			colors[cIndex++] = red/255.0;
			colors[cIndex++] = green/255.0;
			colors[cIndex++] = blue/255.0;
			colors[cIndex++] = red/255.0;
			colors[cIndex++] = green/255.0;
			colors[cIndex++] = blue/255.0;
		}
		for (int g = gates; g < _maxGates; g++) {
			colors[cIndex++] = _clearRed;
			colors[cIndex++] = _clearGreen;
			colors[cIndex++] = _clearBlue;
			colors[cIndex++] = _clearRed;
			colors[cIndex++] = _clearGreen;
			colors[cIndex++] = _clearBlue;
		}
	} 
}

////////////////////////////////////////////////////////////////

void
PPI::makeDisplayList(beam* b, int v) 
{

	// create a display list to hold the gl commands
	glNewList(b->_glListId[v], GL_COMPILE);

	// set the vertex pointer
	glVertexPointer(2, GL_FLOAT, 0, b->vertices());

   // set the colors pointer
	glColorPointer(3, GL_FLOAT, 0, b->colors(v));

   // draw a triangle strip
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
	int i = _beams.size()*(startAngle + (stopAngle-startAngle)/2)/360.0;
	if (i<0)
		i = 0;
	if (i>(int)_beams.size()-1)
		i = _beams.size()-1;

	return i;
}





