#ifndef WIDGETPPI_H
#define WIDGETPPI_H

#include <qgl.h>
#include <vector>
#ifdef WIN32
//#include "glut.h"
#endif
#include "ColorBar/ColorMap.h"

class QT_WIDGET_PLUGIN_EXPORT PPI: public QGLWidget {

	Q_OBJECT        // must include this if you use Qt signals/slots

   /// Manage static data for one display beam that can
   /// render multiple variables.
   /// For the specified solid angle and number
   /// of gates:
   ///
   /// - a vector of vertices is created.
   /// - nVars vectors, each of length matching the vertices vector,
   ///   is allocated.
   /// - a vector of display list ids is allocated. These ids are
   ///   used later when display lists are drawn.
   /// 
   /// The colors for each variable will be set dynamically
   /// by the owner as data need to be rendered. At that time, 
   /// a display list is also created (by the beam owner)
   /// that uses the display list id. By having display lists
   /// create for all variables, the display of beams can be
   /// rapidly switched between variables just by executing
   /// the correct display list.
   class beam {
	public:
		beam(double startAngle, double stopAngle, int nGates, int nVars);
		virtual ~beam();
		double _startAngle;
		double _stopAngle;
		std::vector<GLuint> _glListId;
		double _c;
		double _d;
		bool _hidden;
		int _nVars;
		int _nGates;
		GLfloat* vertices();
		GLfloat* colors(int varN);
	protected:
		std::vector<std::vector<GLfloat> > _varColors;
		std::vector<GLfloat> _triStripVertices;
	};


public:
	PPI(
		QWidget * parent = 0, 
		const char * name = 0, 
		const QGLWidget * shareWidget = 0, 
		WFlags f = 0);

	virtual ~PPI();

	void configure(int nVars,
		int maxGates,
		int nBeams); 

	void configure(int nVars,
		int maxGates); 

	void selectVar(int index);

	void clearVar(int index);

	void fillColors(
		beam* beam, 
		std::vector<std::vector<double> >& _beamData, 
		int gates, 
		int stride,
		std::vector<ColorMap*>& maps);

	void addBeam(
		float startAngle, 
		float stopAngle, 
		int gates, 
		std::vector<std::vector<double> >& _beamData, 
		int stride, 
		std::vector<ColorMap*>& maps);

	inline int beamIndex(double startAngle, double stopAngle);

   void rings(bool enabled);

	double getZoom();

	int numBeams();

public slots:
	void setZoom(double factor);


	void pan(double horizFrac, double vertFrac);

protected:

	void initializeGL();

	void resizeGL( int w, int h );

	void paintGL();

	void makeDisplayList(PPI::beam* b, 
		int varN);

	void cullBeamList();

	std::vector<beam*> _beams;

	int _nVars;

	int _maxGates;

	bool _preAllocate;

	int _selectedVar;

	double _zoomFactor;

	double _panHoriz;

	double _panVert;

	float _clearRed;

	float _clearGreen;

	float _clearBlue;

   bool _ringsEnabled;

};

#endif
