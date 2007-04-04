#include "ColorMap.h"

/// red entries for the color table. They range from 0-255
int redTable[] = 
{
	0, 84, 45,  0,  0,  0,  0,  0,
	0, 73,157,246,255,255,255,255
};

/// blue entries for the color table. They range from 0-255
int blueTable[] = 
{
	0,155,232,255,255,255,220,135,
	55,  0,  0,  0,  0,  0,  0,255
};

/// green entries for the color table. They range from 0-255
int greenTable[] = 
{
	0,  0,  0, 30,114,199,255,255,
	255,255,255,255,224,136, 53,255
};

/**********************************************************/
ColorMap::ColorMap(double minRange, 
				   double maxRange):
_minRange(minRange),
_maxRange(maxRange),
_range(maxRange-minRange)
{
	int s = sizeof(redTable)/sizeof(int);
	for (int i = 0; i < s; i++) {
		_red.push_back(redTable[i]);
		_green.push_back(greenTable[i]);
		_blue.push_back(blueTable[i]);
	}

	_tableSize = _red.size();
}

/**********************************************************/
ColorMap::ColorMap(
				   double minRange,         ///< The minimum map range
				   double maxRange,         ///< The maximum map range
				   std::vector<int> red,  ///< A vector of red hues, ranging between 0 and 255
				   std::vector<int> green,///< A vector of green hues, ranging between 0 and 255
				   std::vector<int> blue  ///< A vector of blue hues, ranging between 0 and 255
				   )
{
	setMap(minRange, maxRange, red, blue, green);
}

/**********************************************************/
ColorMap::ColorMap(
				   double minRange,         ///< The minimum map range
				   double maxRange,         ///< The maximum map range
				   std::vector<std::vector<int> >colors
				   )
{
	std::vector<int> red;
	std::vector<int> green;
	std::vector<int> blue;

	// verify that the inner vectors are of length three. If
	// not, then ignore the colors vector and use our default one.
	for (int i = 0; i < colors.size(); i++) {
		if (colors[i].size() != 3) {
			int tableSize = sizeof(redTable)/sizeof(redTable[0]);
			red.resize(tableSize);
			green.resize(tableSize);
			blue.resize(tableSize);
			for (int j = 0; j < sizeof(redTable)/sizeof(redTable[0]); j++) {
				red[j]   = redTable[j];
				green[j] = greenTable[j];
				blue[j]  = blueTable[j];
				break;
			}
		}
	}

	// if the incoming colors passed the test, then create the rgb vectors
	if (red.size() == 0) {
		red.resize(colors.size());
		for (int i = 0; i < colors.size(); i++) {
			red[i]  = colors[i][0];
			green[i] = colors[i][1];
			blue[i]  = colors[i][2];
		}
	}

	setMap(minRange, maxRange, red, blue, green);
}
/**********************************************************/
void
ColorMap::setMap(
				 double minRange,         ///< The minimum map range
				 double maxRange,         ///< The maximum map range
				 std::vector<int> red,  ///< A vector of red hues, ranging between 0 and 255
				 std::vector<int> green,///< A vector of green hues, ranging between 0 and 255
				 std::vector<int> blue  ///< A vector of blue hues, ranging between 0 and 255
				 )
{
	setRange(minRange, maxRange);

	_tableSize = red.size();
	if (_tableSize > green.size())
		_tableSize = green.size();
	if (_tableSize >  blue.size())
		_tableSize = blue.size();

	_red.clear();
	_blue.clear();
	_green.clear();

	for (int i = 0; i < _tableSize; i++) {
		_red.push_back(red[i]);
		_green.push_back(green[i]);
		_blue.push_back(blue[i]);
	}
}

/**********************************************************/
void
ColorMap::setRange(double minRange, double maxRange) {
	_minRange = minRange;
	_maxRange = maxRange;
	_range = maxRange - minRange;
}

/**********************************************************/
ColorMap::~ColorMap()
{
}

/**********************************************************/
int 
ColorMap::size()
{
	return _red.size();
}

/**********************************************************/
std::vector<int>&
ColorMap::red()
{
	return _red;
}

/**********************************************************/
std::vector<int>&
ColorMap::green()
{
	return _green;
}

/**********************************************************/
std::vector<int>&
ColorMap::blue()
{
	return _blue;
}

/**********************************************************/
void
ColorMap::dataColor(double data, 
					int& red,
					int& green,
					int& blue) const
{


	int index = (int)(_tableSize* (data - _minRange)/(_range));

	if (index < 0) 
		index = 0;
	else
		if (index > _tableSize-1)
			index = _tableSize - 1;

	red = _red[index];
	blue = _blue[index];
	green = _green[index];
}

/**********************************************************/
void
ColorMap::dataColor(
					double data, 
					double& red,
					double& green,
					double& blue) const
{

	int index = (int)(_tableSize* (data - _minRange)/_range);

	if (index < 0) 
		index = 0;
	else
		if (index > _tableSize-1)
			index = _tableSize - 1;

	red = _red[index];
	blue = _blue[index];
	green = _green[index];
}

/**********************************************************/
double
ColorMap::rangeMin() 
{
	return _minRange;
}

/**********************************************************/
double
ColorMap::rangeMax() 
{
	return _maxRange;
}
