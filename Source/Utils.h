/*
  ==============================================================================

    Utils.h
    Created: 19 Feb 2024 7:07:23pm
    Author:  Pichau

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Utils {
public:
    /** Receives an string and returns just the integer part of it. So "d85dsw45d" will return 8545 */
    static int clearInteger(std::string);
};
