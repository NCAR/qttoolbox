// ScaledLabel.cpp: implementation of the ScaledLabel class.
//
//////////////////////////////////////////////////////////////////////

#include "ScaledLabel.h"
#include <iomanip>
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ScaledLabel::ScaledLabel(ScalingType t)
{
    m_scalingType = t;
    m_stringStr << std::setiosflags(std::ios_base::fixed);
    
}

ScaledLabel::~ScaledLabel()
{
    
}

//////////////////////////////////////////////////////////////////////
std::string
ScaledLabel::scale(double value) {
    
    // value is in km. 
    
    double exp = floor(log10(value));
    std::string units;
    
    double scaleFactor;
    if (exp < -3) {
        units = "mm";
        scaleFactor = 0.000001;
    } else {
        if (exp < 0) {
            units = "m";
            scaleFactor = 0.001;
        } else {
            units = "km";
            scaleFactor = 1;
        }
    }
    
    double m = value/scaleFactor;
    if (m >=100) {
        m_stringStr << std::setprecision(0);
    } else { 
        if (m >= 10.0) {
            m_stringStr << std::setprecision(1);
        } else {
            m_stringStr << std::setprecision(2);
        }
    }

    m_stringStr.str("");
    m_stringStr << m << units;
    return m_stringStr.str();
    
}
