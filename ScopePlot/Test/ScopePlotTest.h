#ifndef SCOPEPLOTTEST_H
#define SCOPEPLOTTEST_H

#include "ScopePlotTestBase.h"
#include "ScopePoke.h"

class ScopePlotTest: public ScopePlotTestBase {

public:
   ScopePlotTest();
   ~ScopePlotTest();
   void setScopePoke(ScopePoke* scopePoke);

public slots:
    virtual void plotTypeSlot(bool);

protected:
   ScopePoke* _scopePoke;
};

#endif
