#ifndef COLORMAP_H
#define COLORMAP_H

#include <vector>
#include <qwidgetplugin.h>

	/// A color map manager. It holds the RGB values for each entry in the
	/// color map, and will map a data value into the color map. It will also
	/// provide the complete color tables, which is useful to clients such
	/// as plotlib::ColorBar.
	class QT_WIDGET_PLUGIN_EXPORT ColorMap {
	public:
		ColorMap(double minRange, double maxRange);
		virtual ~ColorMap();

		/**
		 * @returns The red color table
		 */
		std::vector<int>& red();
		/**
 		 * @returns The green color table
       		 */
		std::vector<int>& green();
		/**
		 * @returns The blue color table
		 */
		std::vector<int>& blue();
		/**
		 * @returns The size of the color tables.
		 */
		int size();
		/// Map the data value to a color. Return values will be luminances
		/// in the range 0-255.
		/// @param data The data value
		/// Data valsue less than this will be mapped to the minimum color table
		/// color.
		/// Data values greater than this will be mapped to the maximum color table
		/// color.
		/// @param red value returned here.
		/// @param green value returned here.
		/// @param blue value returned here.
		void dataColor(
			double data, 
			int& red,
			int& green,
			int& blue
			) const;

		/// Map the data value to a color. Return values will be luminances
		/// in the range 0-255.
		/// @param data The data value
		/// Data values less than _rangeMin will be mapped to the minimum color table
		/// color.
		/// Data values greater than _rangeMax will be mapped to the maximum color table
		/// color.
		/// @param red value returned here.
		/// @param green value returned here.
		/// @param blue value returned here.
		void dataColor(
			double data, 
			double& red,
			double& green,
			double& blue
			) const;

		/// @return The minimum range value
		double rangeMin();

		/// @return The maximum range value
		double rangeMax();


	protected:
		double _minRange;
		double _maxRange;
		double _range;
		int _tableSize;

		/// The red color table.
		std::vector<int> _red;
		/// The green color table.
		std::vector<int> _green;
		/// The blue color table.
		std::vector<int> _blue;

	};

#endif
