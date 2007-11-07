// ScaledLabel.h: interface for the ScaledLabel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(SCALEDLABEL_H)
#define SCALEDLABEL_H

#include <string>
#include <sstream>
#include <map>

/**
Create a label that is scaled in engineering units. At
construction, ScaledLabel is instructed on the type of
conversion it will perform. Subsequent calls to scale(double)
will return a string, scaled accordingly.
*/
class ScaledLabel  
{
public:
    enum ScalingType {
        /// Scale for distance, in engineering units, with the appropriate units designation appended.
        DistanceEng
    };
    
    // Constructor. 
    ScaledLabel(
        /// The type of scaling to apply.
        ScalingType t);
    
    //Destructor
    virtual ~ScaledLabel();
    
    /// Return a string containing the scaled representation.
    std::string scale(
        /// The value to be scaled.
        double scale
        );
    
protected:
    /// The type of scaling we are performing.
    ScalingType m_scalingType;
    
    /// A stringstream used to format numbers
    std::ostringstream m_stringStr;
};

#endif // !defined(SCALEDLABEL_H)
