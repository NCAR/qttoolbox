#include "StatusGaugeTest.h"
#include "StatusGauge.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>

/////////////////////////////////////////////////////
StatusGaugeTest::StatusGaugeTest(QDialog* parent):
    QDialog(parent)
{
	setupUi(parent);
	
	unsigned int num = 7;
	std::vector<std::string> colors;
    colors.push_back("gold");
    colors.push_back("green");
    colors.push_back("purple");
    colors.push_back("red");
    colors.push_back("cyan");
	
    QHBoxLayout* layout1 = new QHBoxLayout;
    QVBoxLayout* layout2 = new QVBoxLayout;
    for (unsigned int i = 0; i < num; i++) {
        StatusGauge* g1 = new StatusGauge(colors, 70.0, StatusGauge::GaugeVertical);
        layout1->addWidget(g1);
        g1->on(i % colors.size(), true);
        connect(g1, SIGNAL(released()), this, SLOT(releasedSlot()));
    }
    groupBox1->setLayout(layout1);
    for (unsigned int i = 0; i < num; i++) {
        if (i == num-1) {
            // leave out the first indicator for the 
            // very last gauge
            colors[0] = "";
        }
        StatusGauge* g2 = new StatusGauge(colors, 95.0, StatusGauge::GaugeHorizontal);
        layout2->addWidget(g2);
        g2->on(i % colors.size(), true);
        connect(g2, SIGNAL(released()), this, SLOT(releasedSlot()));
    }
    groupBox2->setLayout(layout2);
}

/////////////////////////////////////////////////////
StatusGaugeTest::~StatusGaugeTest() {
}

/////////////////////////////////////////////////////
void
StatusGaugeTest::releasedSlot() {
    QMessageBox::information(this, "Note!", "A StatusGauge was punched!");
}

