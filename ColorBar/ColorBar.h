#ifndef COLORBARH_
#define COLORBARH_

#include "ColorBarBase.h"
#include "ColorMap.h"

#include <qwidget.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qlayout.h>
#include <vector>


	/// A widget that displays a representation of a plotlib::ColorMap.
	/// Swatches are displayed in a vertical column, with text labels
	/// indicating the value of the swatch.
	class QT_WIDGET_PLUGIN_EXPORT ColorBar :
		public ColorBarBase
	{
		Q_OBJECT

	public:
		ColorBar(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
		virtual ~ColorBar(void);

		/// Configure the ColorBar.
		/// @param map The corresponding color map.
		void configure(ColorMap& map);

	protected:

		/// (Re)make the color swatches and the labels.
		void makeSwatches(ColorMap& map, double min, double max);
		/// Container for the swatched and labels, so that 
		/// they can be deleted when the configuration is changed.
		std::vector<QWidget*> _widgets;
      /// A layout for the swatches
      QVBoxLayout* _vLayout;

		/// The vertical box for the swatches. 
		QVBox* _vbox1;
		/// The vertical box for the labels.
		QVBox* _vbox2;
      /// A default color map, so that the plugin can
      /// display something.
      ColorMap _defaultMap;
	};

#endif
