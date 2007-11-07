#ifndef TWOKNOBS_H
#define TWOKNOBS_H

#include "ui_TwoKnobs.h"
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

/// A plugin widget which combines two Knob plugins. Signals
/// are generated when the knob values change.
class DLL_EXPORT TwoKnobs : public QWidget, public Ui::TwoKnobsBase
{
    Q_OBJECT

public:
    TwoKnobs( QWidget* parent = 0);
    ~TwoKnobs();

    void setTitles(std::string titleKnobOne, std::string titleKnobTwo);
    void setRanges(double knobOneMin, double knobOneMax,
       double knobTwoMin, double knobTwoMax);
	void setValues(double  val1, double val2);
	void setScaleMaxMajor(int knobNum, int ticks);
	void setScaleMaxMinor(int knobNum, int ticks);
    void getRanges(double& knobOneMin, double& knobOneMax,
       double& knobTwoMin, double& knobTwoMax);
	void getValues(double& val1, double& val2);

public slots:
    virtual void knobOneChangedSlot(double);
    virtual void knobTwoChangedSlot(double);

signals:
    void knobOneChanged(double);
    void knobTwoChanged(double);

};

#endif // TWOKNOBS_H
