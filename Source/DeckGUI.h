/*
  ==============================================================================

    DeckGUI.h
    Created: 22 Jan 2024 1:20:22pm
    Author:  Pichau

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveFormDisplay.h";
#include "SliderWithLabel.h"

//==============================================================================
/*
*/
class DeckGUI  : public juce::Component, public juce::Button::Listener, public juce::Slider::Listener, public juce::FileDragAndDropTarget, public juce::Timer
{
public:
    DeckGUI(DJAudioPlayer* player, juce::AudioFormatManager& formatManager, juce::AudioThumbnailCache& thumbCache);
    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void buttonClicked(juce::Button*) override; 

    void sliderValueChanged(juce::Slider*) override;

    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y);

    void timerCallback() override;

    /** Checks if the player is playing */
    bool isPlaying();

    /** Loads the specified URL to the player */
    void loadURL(juce::URL url);

private:

    juce::FileChooser fChooser{ "Select a file..." };

    juce::TextButton playButton{ "PLAY" };
    juce::TextButton stopButton{ "STOP" };
    juce::TextButton loadButton{ "LOAD" };

    SliderWithLabel volSlider{"VOLUME"};
    SliderWithLabel speedSlider{"SPEED"};
    SliderWithLabel posSlider{"POSITION"};

    juce::Label currentTrackLabel;

    juce::URL currentTrack;

    WaveFormDisplay waveFormDisplay;

    DJAudioPlayer* player;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
