#ifndef WIDGETPPI_H
#define WIDGETPPI_H

#include <qdialog.h>
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

#include <qgl.h>
#include <QResizeEvent>
#include <vector>
#include <qtimer.h>
#include "ColorBar/ColorMap.h"
#include "ScaledLabel.h"
//#include "GLColor.hh"

#ifdef WIN32
//#include "glut.h"
#endif

/// A Qt plugin that will display a Plan Position Index (PPI)
/// display. Of course the same display can provide 
/// relative Height Index (RHI) display just by manipulating
/// the beam angles correctly.
///
/// A beam is the basic building block for the PPI. It has
/// a starting angle, ending angle, a fixed number of range 
/// gates, and a fixed number of variables that can be rendered 
/// on the beam.
///
/// The PPI is given color maps for each of the variables. 
/// It is then simply called with the data for all variables
/// for a given beam. GL display lists are created for each
/// of the variables, and the display list for the curently
/// selected variable is drawn. When the selected variable
/// is changed, the image is cleared, and all of the display lists
/// for all of the beams, for the newly selected variable,
/// are drawn. This allows or fairly quick (but not 
/// blazing) switches between variables.
///
///
/// The PPI can operate in a mode where the number of beams
/// are specified, and are preallocated. Incoming data are sent to
/// the nearest preallocated beam.
/// It has another mode, where the beams are created on the
/// fly, so that varying beam widths will be displayed 
/// more accurately. This requires a culling of covered beams,
/// and the constant generation of beam vertices, and so the
/// performance is significantly slower than in the preallocated
/// mode.
class DLL_EXPORT PPI: public QGLWidget//, public Ui::PPI 
{

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
		beam(
			double startAngle, ///< Start angle of the beam
			double stopAngle,  ///< Stop angle of the beam
			int nGates,        ///< Number of gates in the beam
			int nVars          ///< Number of variables for the beam
			);
		/// destructor
		virtual ~beam();
		/// The start angel of the beam.
		double _startAngle;
		/// The stop angle of the beam
		double _stopAngle;
		/// Vector of display list ids; one per variable
		std::vector<GLuint> _glListId;
		/// Left end point store of segment(dynamic beams only).
		double _c;
		/// Right end point store of segment(dynamic beams only).
		double _d;
		/// If beam is hidden (dynamic beams only).
		bool _hidden;
		/// Number of variables in this beam.
		int _nVars;
		/// Number of gates in this beam.
		int _nGates;
		/// @return pointer to the array of triangle strip vertices. This
		/// will be used to generate the display list for the beam.
		GLfloat* vertices();
		/// @return pointer to the array of triangle strip colors for
		/// variable varN. This will be used to generate the display 
		/// lists for the beam.
		GLfloat* colors(int varN);
	protected:
		/// internal storage for the olors for each variable.
		std::vector<std::vector<GLfloat> > _varColors;
		/// internal storage for the beam triangle strip vertices.
		std::vector<GLfloat> _triStripVertices;
	};


public:
	PPI(
		QWidget* parent = 0               ///< parent widget
		);
	/// Destructor
	virtual ~PPI();
	/// Configure the PPI for preallocated beams. 
	void configure(int nVars,  ///< Number of variables
		int maxGates,           ///< Max number of gates per beam
		int nBeams,             ///< Number of beams
		double distanceSpanKm=100.0,  ///< The distance spanned by the complete PPI.
		int ringsPerPPI=4			///< The number of rings to show in the exposed PPI display.
		); 
	/// Configure the PPI for dynamically allocated beams. 
	void configure(int nVars,  ///< Number of variables.
		int maxGates,            ///< Maximum number of gates in a beam.
		double distanceSpanKm=100.0,  ///< The distance spanned by the complete PPI.
		int ringsPerPPI=4			///< The number of rings to show in the exposed PPI display.
		); 
	/// Select the variable to display.
	void selectVar(int index   ///< Index of the variable to display, zero based.
		);
	/// Clear the specified variable.
	void clearVar(int index    ///< Index of the variable to be cleared, zero based
		);
	///
	void fillColors(
		beam* beam, 
		std::vector<std::vector<double> >& _beamData, 
		int gates, 
		int stride,
		std::vector<ColorMap*>& maps);
	///
	void addBeam(
		float startAngle, 
		float stopAngle, 
		int gates, 
		std::vector<std::vector<double> >& _beamData, 
		int stride, 
		std::vector<ColorMap*>& maps);
	/// Specify the background color
	/// Call refresh()
	void backgroundColor(QColor color  ///< The background color.
		);
	/// Set the number of rings to show for the exposed display.
	/// This will also cause the grid spacing to match the rings.
	void ringsPerPPI(int rings);
	/// Find the index in the _beams array of the 
	/// beam that corresponds to this angle. The
	/// beam angles must sweep in a counter clockwise,
	/// i.e. cartessian, direction.
	inline int beamIndex(
		double startAngle,   ///< Beginning angle of the beam. 
		double stopAngle);   ///< Ending angle of the beam.
	/// Set ring visibility.
	void rings(bool enabled    ///< True to show them, false otherwise
		);
	/// Set grids visibility.
	void grids(bool enabled    ///< True to show them, false otherwise
		);
	/// @return The current zoom factor
	double getZoom();
	/// @return The current number of beams. This is interesting
	/// to monitor when PPI is operating in the dynamically
	/// allocated beam mode.
	int numBeams();
	/// refresh the display. Useful after resizes, etc.
	void refresh();
	public slots:
		///
		void setZoom(double factor);
		///
		void pan(double x, double y);
		///
		void resizeTimerTimeout();
		///
		void resetView();

protected:
	///
	void initializeGL();
	///
	void resizeGL( int w, int h );
	///
	void paintGL();
	///
	void makeDisplayList(PPI::beam* b, 
		int varN);
	///
	void cullBeamList();
	///
	virtual void resizeEvent( QResizeEvent * e );
	/// capture mouse move events
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	// create the stencil that will draw the grid and range rings.
	void createStencil();
	/// clear the range ring and grids stencil
	void clearStencil();
	/// Do the actual range ring and grid drawing. It will 
	/// draw into what ever is currently active, so this
	/// could be used if we wanted to draw acutal rings and grids
	/// in a different color, rather than using the stencil.
	void makeRingsAndGrids();
	/// Dtermine a ring spacing which will give even distances,
	/// and fit a reasonable number of rings in the display
	double ringSpacing();
	///
	std::vector<beam*> _beams;
	/// The number of variables we ar representing. A display list
	/// will be created for each variable.
	int _nVars;
	/// Maximum number of gates along a beam
	int _maxGates;
	/// true if the beams have been preallocated.
	bool _preAllocate;
	/// The index of the variable selected for display
	int _selectedVar;
	// The current zoom factor. as the zoom in increases, it will
	// increase. At full zoom out, it is equal to 1.
	double _zoomFactor;
	///
	double _currentX;
	///
	double _currentY;
	/// red value for the background (and thus stencil) color.
	float _clearRed;
	/// green value for the background (and thus stencil) color.
	float _clearGreen;
	/// blue value for the background (and thus stencil) color.
	float _clearBlue;
	// True if the ring display is enabled.
	bool _ringsEnabled;
	/// True if the grids display is enabled.
	bool _gridsEnabled;
	/// A timer used to block redraws while the component is being resized.
	QTimer _resizeTimer;
	/// set when resizing is active; used in conjunction
	/// with a timer in order to prevent many refresh cycles
	/// during the resize.
	bool _resizing;         
	/// This will create labels wiith nocely scaled values and
	/// approriate units.
	ScaledLabel _scaledLabel;
	/// The number of rings to display in the whole exposed PPI area.
	int _ringsPerPPI;
	/// The distance across the whole display, in km.
	/// It affects the labelling of the range rings
	double _distanceSpanKm;
	/// Last X location of the mouse during mouse move events; used for panning
	int _oldMouseX;
	/// Last Y location of the mouse during mouse move events; used for panning
	int _oldMouseY;
};

#endif
