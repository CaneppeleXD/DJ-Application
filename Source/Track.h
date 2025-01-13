/*
  ==============================================================================

    Track.h
    Created: 19 Feb 2024 6:44:23pm
    Author:  Pichau

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Track {
public:
    Track(std::string _name, juce::File _trackFile)
        :
        name(_name),
        trackFile(_trackFile)
    {
    }

    std::string name;
    
    juce::File trackFile;
};
