
#include "ColorBarTest.h"
#include "ColorBar.h"
#include "ColorMap.h"

ColorBarTest::ColorBarTest()
{
   ColorMap cmap(0.0, 100.0);
   _colorBar->configure(cmap);
}

ColorBarTest::~ColorBarTest() {
}

