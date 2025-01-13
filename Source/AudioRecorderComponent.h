/*
  ==============================================================================

    AudioRecorderComponent.h
    Created: 26 Feb 2024 9:38:11am
    Author:  Pichau

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "AudioRecorder.h";
#include "PlaylistComponet.h"

//==============================================================================
/*
*/
class AudioRecorderComponent : public juce::Component, public juce::Button::Listener
{
public:
    AudioRecorderComponent(AudioRecorder* recorder, PlaylistComponet* playlist);
    ~AudioRecorderComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void buttonClicked(juce::Button* button);

private:

    juce::TextButton recordButton{"Start Recording"};

    AudioRecorder* recorderPtr;

    PlaylistComponet* playlistToStore;

    void startRecording();

    void stopRecording();

    juce::AudioDeviceManager device;

    juce::File lastRecordedFile;

    juce::FileChooser chooser{ "Output file...", juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("recording.wav"), "*.wav" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioRecorderComponent)
};
