#ifndef SCOPEPLOTTEST_H
#define SCOPEPLOTTEST_H

#include "ScopePoke.h"
#include "ui_ScopePlotTest.h"

class ScopePlotTest: public QObject, public Ui::ScopePlotTest
{
	Q_OBJECT
public:
   ScopePlotTest(QWidget* parent);
   ~ScopePlotTest();
   void setScopePoke(ScopePoke* scopePoke);

public slots:
    void plotType(int);

protected:
   ScopePoke* _scopePoke;
};

#endif
