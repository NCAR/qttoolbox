#ifndef KNOB_H
#define KNOB_H

#include "ui_Knob.h"
#include <qdialog.h>
#include <string>

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

///
class DLL_EXPORT Knob: public QWidget, private Ui::Knob
{
    Q_OBJECT

public:
    Knob( QWidget* parent = 0);
    ~Knob();

    void setTitle(std::string title);
    void setRange(double min, double max);
    void getRange(double& min, double& max);
	void setValue(double val);
	void setScaleMaxMajor(int ticks);
	void setScaleMaxMinor(int ticks);
	double value();

public slots:
   void valueChangedSlot(double);

signals:
   void valueChanged(double);

};

#endif // KNOB_H
