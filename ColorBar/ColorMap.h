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
/// If a color is referenced as an int, it will hae the range 0-255. If it is 
/// referenced as a float or a double, it will be in the range 0-1. 
/// The internal tables are stored in the range of 0-1, so when integer values
/// are specified or returned, they must be translated by a factor of 255.
class DLL_EXPORT ColorMap {
    public:
        /// Default constructor for a ColorMap
        /// Will create a map with the standard colors
        ColorMap();
        /// Create a color map using the built-in
        /// color table.
        /// @param minRange The minimum value for the color map.
        /// @param maxRange The maximum value for the color map.
        ColorMap(
                double minRange, double maxRange);

        /// Create a color map using the provided colors.
        /// The color table will be constructed from the color
        /// vectors, using the number of entries found in
        /// the shortest of the three color vectors. (They 
        /// really should all be the same length).
        ColorMap(
                double minRange, ///< The minimum map range
                double maxRange, ///< The maximum map range
                std::vector<int> red, ///< A vector of red hues, ranging between 0 and 255
                std::vector<int> green,///< A vector of green hues, ranging between 0 and 255
                std::vector<int> blue ///< A vector of blue hues, ranging between 0 and 255
                );

        /// Create a color map using the builtin color map 
        /// with that name. If that name is not located,
        /// use the dfault color map.
        ColorMap(
                double minRange, ///< The minimum map range
                double maxRange, ///< The maximum map range
                std::string builtinName ///< The builtin map name..
                );

        /// Create a color map using the provided colors.
        /// The color table will be constructed from the color
        /// vectors, using the number of entries found in
        /// the outer vector. The inner vectors must be of
        /// length three. If they are not, a color map using
        /// the default color table will be constructed.
        ColorMap(
                double minRange, ///< The minimum map range
                double maxRange, ///< The maximum map range
                std::vector<std::vector<int> >colors ///< A vector of vectors of hues, ranging between 0 and 255. The inner vector values correcpond to red, green and blue.
                );

        /// Create a color map using the provided colors.
        /// The color table will be constructed from the color
        /// vectors, using the number of entries found in
        /// the outer vector. The inner vectors must be of
        /// length three. If they are not, a color map using
        /// the default color table will be constructed.
        ColorMap(
                double minRange, ///< The minimum map range
                double maxRange, ///< The maximum map range
                std::vector<std::vector<float> >colors ///< A vector of vectors of hues, ranging between 0 and 1. The inner vector values correcpond to red, green and blue.
                );

        /// Destructor
        virtual ~ColorMap();

        /// Change the color map using the provided colors.
        /// The color table will be constructed from the color
        /// vectors, using the number of entries found in
        /// the shortest of the three color vectors. (They 
        /// really should all be the same length).
        void setMap(
                double minRange, ///< The minimum map range
                double maxRange, ///< The maximum map range
                std::vector<int> red, ///< A vector of red hues, ranging between 0 and 255
                std::vector<int> green,///< A vector of green hues, ranging between 0 and 255
                std::vector<int> blue ///< A vector of blue hues, ranging between 0 and 255
                );

        /// Change the color map using the provided colors.
        /// The color table will be constructed from the color
        /// vectors, using the number of entries found in
        /// the shortest of the three color vectors. (They 
        /// really should all be the same length).
        void setMap(
                double minRange, ///< The minimum map range
                double maxRange, ///< The maximum map range
                std::vector<float> red, ///< A vector of red hues, ranging between 0 and 1
                std::vector<float> green,///< A vector of green hues, ranging between 0 and 1
                std::vector<float> blue ///< A vector of blue hues, ranging between 0 and 1
                );

        /// Change the range of an existing map.
        void setRange(
                double minRange, ///< The minimum map range
                double maxRange ///< The maximum map range
                );

        /**
         * @returns The red color table
         */
        std::vector<int> red();
        /**
         * @returns The green color table
         */
        std::vector<int> green();
        /**
         * @returns The blue color table
         */
        std::vector<int> blue();
        /**
         * @returns The size of the color tables.
         */
        unsigned int size() {
            return _red.size();
        }
        ;
        /// Map the data value to a color. Return values will be luminances
        /// from the supplied color tables.
        /// @param data The data value
        /// Data values less than _rangeMin will be mapped to the minimum color table
        /// color.
        /// Data values greater than _rangeMax will be mapped to the maximum color table
        /// color.
        /// @param red value returned here.
        /// @param green value returned here.
        /// @param blue value returned here.
        void dataColor(
                double data, int& red, int& green, int& blue) const ;

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
                double data, double& red, double& green, double& blue) const;

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
                double data, float& red, float& green, float& blue) const ;

        /// @return The minimum range value
        double rangeMin();

        /// @return The maximum range value
        double rangeMax();

        /// @return The name of the builtin color maps.
        static std::vector<std::string> builtinMaps();

    protected:
        double _minRange;
        double _maxRange;
        double _range;
        unsigned int _tableSize;
        /// The red color table.
        std::vector<float> _red;
        /// The green color table.
        std::vector<float> _green;
        /// The blue color table.
        std::vector<float> _blue;
};

#endif
