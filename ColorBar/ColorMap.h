#ifndef COLORMAP_H
#define COLORMAP_H

#include <vector>

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

/// A color map manager. It holds the RGB values for each entry in the
/// color map, and will map a data value into the color map. It will also
/// provide the complete color tables, which is useful to clients such
/// as plotlib::ColorBar.
class DLL_EXPORT ColorMap {
public:
	/// Create a color map using the built-in
	/// color table.
	/// @param minRange The minimum value for the color map.
	/// @param maxRange The maximum value for the color map.
	ColorMap(double minRange, double maxRange);

	/// Create a color map using the provided colors.
	/// The color table will be constructed from the color
	/// vectors, using the number of entries found in
	/// the shortest of the three color vectors. (They 
	/// really should all be the same length).
	ColorMap(
		double minRange,         ///< The minimum map range
		double maxRange,         ///< The maximum map range
		std::vector<float> red,  ///< A vector of red hues, ranging between 0 and 255
		std::vector<float> green,///< A vector of green hues, ranging between 0 and 255
		std::vector<float> blue  ///< A vector of blue hues, ranging between 0 and 255
		);

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
