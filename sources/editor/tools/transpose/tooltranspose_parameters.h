#ifndef TOOLTRANSPOSE_PARAMETERS_H
#define TOOLTRANSPOSE_PARAMETERS_H

#include "abstracttoolparameters.h"

class ToolTranspose_parameters: public AbstractToolParameters
{
public:
    /// Load the configuration from the ini file
    void loadConfiguration() override;

    /// Save the configuration in the ini file
    void saveConfiguration() override;
    
    double getSemiTones() { return _semiTones; }
    void setSemiTones(double semiTones) { _semiTones = semiTones; }
    
    bool getAdaptKeyRanges() { return _adaptKeyRanges; }
    void setAdaptKeyRanges(bool adaptKeyRanges) { _adaptKeyRanges = adaptKeyRanges; }
    
private:
    double _semiTones;
    bool _adaptKeyRanges;
};

#endif // TOOLTRANSPOSE_PARAMETERS_H