#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponet.h"
#include "MyLookAndFeel.h"
#include "AudioRecorderComponent.h"
#include "AudioRecorder.h"
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint(juce::Graphics& g) override;
    void resized() override;
private:
    //==============================================================================
    // Your private member variables go here...

    DJAudioPlayer player1{ formatManager };
    DJAudioPlayer player2{ formatManager };

    DeckGUI deckGUI1{ &player1, formatManager, thumbCache };
    DeckGUI deckGUI2{ &player2, formatManager, thumbCache };

    juce::MixerAudioSource mixerSource;

    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbCache{ 100 };

    PlaylistComponet playlistComponent;

    MyLookAndFeel customLookAndFeel{};

    AudioRecorder recorder{};

    AudioRecorderComponent audioRecordComp{&recorder, &playlistComponent};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};