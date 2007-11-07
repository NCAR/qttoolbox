
#include "ColorBarTest.h"
#include "ColorBar.h"
#include "ColorMap.h"
#include <QRadioButton>
#include <QButtonGroup>

/////////////////////////////////////////////////////
ColorBarTest::ColorBarTest(QDialog* dialog)
{
	setupUi(dialog);

	_maps = ColorMap::builtinMaps();

	QVBoxLayout* vbox = new QVBoxLayout;
	for (int i = 0; i < _maps.size(); i++) {
		QRadioButton* b = new QRadioButton(_maps[i].c_str());
		_bgroup.addButton(b, i);
		vbox->addWidget(b);
		if (i == 0)
			b->setChecked(true);
	}
	connect(&_bgroup, SIGNAL(buttonClicked(int)), this, SLOT(buttonSlot(int)));
	_groupBox->setLayout(vbox);
	ColorMap m(0.0, 160.0, _maps[0]);
	_colorBar->configure(m);

}

/////////////////////////////////////////////////////
ColorBarTest::~ColorBarTest() {
}

/////////////////////////////////////////////////////
void
ColorBarTest::buttonSlot(int) {
	int id = _bgroup.checkedId();
	std::string map = _maps[id];
	ColorMap m(0.0, 160.0, map);
	_colorBar->configure(m);
}
