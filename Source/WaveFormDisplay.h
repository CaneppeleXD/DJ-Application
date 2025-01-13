/*
  ==============================================================================

    WaveFormDisplay.h
    Created: 29 Jan 2024 8:42:51pm
    Author:  Pichau

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveFormDisplay  : public juce::Component, public juce::ChangeListener 
{
public:
    WaveFormDisplay(juce::AudioFormatManager& formatManager, juce::AudioThumbnailCache& thumbCache);
    ~WaveFormDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void loadURL(juce::URL audioURL);

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    /** set the relative positions of the playhead*/
    void setPositionRelative(double positionRelative);

private:
    
    juce::AudioThumbnail audioThumb;

    bool fileLoaded;

    double positionRelative;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveFormDisplay)
};
