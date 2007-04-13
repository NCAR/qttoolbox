#include "ColorMap.h"

typedef  struct {unsigned char r,g,b;} RGB;
#define RGBSIZE(table) (sizeof(table)/sizeof(table[0]))

char* builtinNames[] = {
	"default",
	"rainbow",
	"dbz1",
	"dbz2",
	"velocity1",
	"velocity2",
	"div1"
};

static RGB
rainbowRGB[] = {
	{ 0, 0, 0}, { 84, 155, 0}, { 45, 232, 0}, { 0, 255, 30},
	{ 0, 255, 114}, { 0, 255, 199}, { 0, 220, 255}, { 0, 135, 255},
	{ 0, 55, 255}, { 73, 0, 255}, { 157, 0, 255}, { 246, 0, 255}, 
	{ 255, 0, 224}, { 255, 0, 136}, { 255, 0, 53}, { 255, 255, 255}
};


// eldora dbZ
static RGB
dbz1RGB[33] = {
	{60, 60, 60}, {0, 69, 0}, {0, 101, 10},
	{0, 158, 30}, {0, 177, 59}, {0, 205, 116}, {0, 191, 150},
	{0, 159, 206}, {8, 127, 219}, {28, 71, 232}, {56, 48, 222},
	{110, 13, 198}, {144, 12, 174}, {200, 15, 134}, {196, 67, 134},
	{192, 100, 135}, {191, 104, 101}, {190, 108, 68}, {210, 136, 59},
	{250, 196, 49}, {254, 217, 33}, {254, 250, 3}, {254, 221, 28},
	{254, 154, 88}, {254, 130, 64}, {254, 95, 5}, {249, 79, 8},
	{253, 52, 28}, {200, 117, 104}, {215, 183, 181}, {210, 210, 210},
	{151, 151, 0}, {219, 219, 0}
};

// eldora velocity
static RGB
velocity1RGB[33] = {
	{255, 0,   255}, {242, 0,   254}, {222, 0,   254},
	{200, 0,   254}, {186, 0,   254}, {175, 0,   253}, {165, 0,   252},
	{139, 0,   248}, {113, 1,   242}, {71,  19,  236}, {50,  75,  229},
	{0,   110, 229}, {0,   182, 228}, {4,   232, 152}, {151, 151, 0}, 
	{2,   116,  76}, {125, 125, 125}, {217, 149, 49}, {238, 184, 31},
	{252, 218, 18},   {219, 219, 0},  {230, 218, 33},  {230, 177, 100},
	{230, 145, 150}, {230, 131, 131}, {230, 108, 108},  {230, 75,  75}, 
	{254, 120,  0},   {254, 90,  0},   {225, 0, 0},   {200, 0, 0},
	{175, 0,   0}, {150, 0,   0},
};

/*      spol NCAR official reflectivity color map */
static RGB
dbz2RGB[] = {
	{0,0,0},{60,60,60},{0,69,0},{0,101,10},
	{0,158,30},{0,177,59},{0,205,116},{0,191,150},
	{0,159,206},{8,127,219},{28,71,232},{56,48,222},
	{110,13,198},{144,12,174},{200,15,134},{196,67,134},
	{192,100,135},{191,104,101},{190,108,68},{210,136,59},
	{250,196,49},{254,217,33},{254,250,3},{254,221,28},
	{254,154,88},{254,130,64},{254,95,5},{249,79,8},
	{253,52,28},{200,117,104},{215,183,181},{210,210,210},{210,210,210}
};

/*      spol NCAR official divergence color map */
static RGB
div1RGB[] = {
	{0,0,0},{254,0,254},{253,0,254},{248,0,254},
	{222,0,254},{186,0,254},{175,0,253},{165,0,252},
	{139,0,248},{113,1,242},{71,19,236},{19,55,229},
	{0,110,229},{0,182,228},{4,232,152},
	/*  {2,116,76},  */
	{125,125,125},{125,125,125},{125,125,125},
	/*  {226,193,133},  */
	{217,149,49},{238,184,31},
	{252,218,18},{254,218,33},{254,177,100},{254,145,150},
	{254,131,131},{254,108,58},{254,93,7},{254,86,0},
	{254,55,0},{254,13,0},{254,0,0},{255,0,0},{0,0,0}
};

/*      spol NCAR official velocity color map */
static RGB
velocity2RGB[] = {
	{0,0,0},{254,0,254},{253,0,254},{248,0,254},
	{222,0,254},{186,0,254},{175,0,253},{165,0,252},
	{139,0,248},{113,1,242},{71,19,236},{19,55,229},
	{0,110,229},{0,182,228},{4,232,152},{2,116,76},
	{125,125,125},{226,193,133},{217,149,49},{238,184,31},
	{252,218,18},{254,218,33},{254,177,100},{254,145,150},
	{254,131,131},{254,108,58},{254,93,7},{254,86,0},
	{254,55,0},{254,13,0},{254,0,0},{255,0,0},{0,0,0}
};


/**********************************************************/
ColorMap::ColorMap()
{
	*this = ColorMap(0.0, 1.0);
}
/**********************************************************/
ColorMap::ColorMap(double minRange, 
				   double maxRange):
_minRange(minRange),
_maxRange(maxRange),
_range(maxRange-minRange)
{ 
	int s = sizeof(rainbowRGB)/(sizeof(rainbowRGB[0])/sizeof(rainbowRGB[0].r));
	for (int i = 0; i < s; i++) {
		_red.push_back(rainbowRGB[i].r);
		_green.push_back(rainbowRGB[i].g);
		_blue.push_back(rainbowRGB[i].b);
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
			red.resize(_red.size());
			green.resize(_red.size());
			blue.resize(_red.size());
			for (int j = 0; j < _red.size(); j++) {
				red[j]   = rainbowRGB[j].r;
				green[j] = rainbowRGB[j].g;
				blue[j]  = rainbowRGB[j].b;
				break;
			}
		}
	}

	// if the incoming colors passed the test, then create the rgb vectors
	if (red.size() == 0) {
		red.resize(colors.size());
		green.resize(colors.size());
		blue.resize(colors.size());
		for (int i = 0; i < colors.size(); i++) {
			red[i]  = colors[i][0];
			green[i] = colors[i][1];
			blue[i]  = colors[i][2];
		}
	}

	setMap(minRange, maxRange, red, green, blue);
}
/**********************************************************/
ColorMap::ColorMap(
				   double minRange,         ///< The minimum map range
				   double maxRange,         ///< The maximum map range
				   std::string builtinName  /// the builtin map name
				   )
{
	RGB* colors;
	int tableSize = 0;
	if (!builtinName.compare("velocity1")) {
		colors = &velocity1RGB[0];
		tableSize = RGBSIZE(velocity1RGB);
	} else {
		if (!builtinName.compare("velocity2")) { 
			colors = &velocity2RGB[0];
			tableSize = RGBSIZE(velocity2RGB);
		} else {
			if (!builtinName.compare("dbz1")) {
				colors = &dbz1RGB[0];
				tableSize = RGBSIZE(dbz1RGB);
			} else {
				if (!builtinName.compare("dbz2")) {
					colors = &dbz2RGB[0];
					tableSize = RGBSIZE(dbz2RGB);
				} else {
					if (!builtinName.compare("div1")) {
						colors = &div1RGB[0];
						tableSize = RGBSIZE(div1RGB);
					} else {
						if (!builtinName.compare("rainbow")) {
							colors = &rainbowRGB[0];
							tableSize = RGBSIZE(rainbowRGB);
						} else {
							// rainbow will be our default map.
							colors = &rainbowRGB[0];
							tableSize = RGBSIZE(rainbowRGB);
						}
					}
				}
			}
		}
	}

	std::vector<int> red;
	std::vector<int> green;
	std::vector<int> blue;
	red.resize(tableSize);
	green.resize(tableSize);
	blue.resize(tableSize);

	for (int i = 0; i < tableSize; i++) {
		red[i]   = colors[i].r;
		green[i] = colors[i].g;
		blue[i]  = colors[i].b;
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

/**********************************************************/
std::vector<std::string>
ColorMap::builtinMaps() {
	std::vector<std::string> result;
	for (int i = 0; i < sizeof(builtinNames)/sizeof(char*); i++) {
		result.push_back(std::string(builtinNames[i]));
	}
	return result;
}
