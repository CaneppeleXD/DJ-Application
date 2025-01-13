/*
  ==============================================================================

    AudioRecorderComponent.cpp
    Created: 26 Feb 2024 9:38:11am
    Author:  Pichau

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AudioRecorderComponent.h"

//==============================================================================
AudioRecorderComponent::AudioRecorderComponent(AudioRecorder* recorder, PlaylistComponet* playlist) : playlistToStore(playlist), recorderPtr(recorder)
{
    addAndMakeVisible(recordButton);

    recordButton.addListener(this);
}

AudioRecorderComponent::~AudioRecorderComponent()
{
    
}

void AudioRecorderComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void AudioRecorderComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    recordButton.setBounds(0, 0, getWidth(), getHeight());
}

void AudioRecorderComponent::buttonClicked(juce::Button* button) {
    if (button == &recordButton) {
        if (recorderPtr->isRecording()) {
            stopRecording();
            recordButton.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::white);
        }
        else {
            startRecording();
            recordButton.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::orangered);
        }
    }
}

void AudioRecorderComponent::startRecording() {
    auto dirToSave = juce::File::getSpecialLocation(juce::File::currentExecutableFile).getParentDirectory();
    dirToSave.createDirectory();

    lastRecordedFile = dirToSave.getNonexistentChildFile("RecordFile", "wav");
   
    recorderPtr->startRecording(lastRecordedFile);

    recordButton.setButtonText("Stop Recording");
}

void AudioRecorderComponent::stopRecording() {
    recorderPtr->stop();

    chooser.launchAsync(juce::FileBrowserComponent::saveMode
        | juce::FileBrowserComponent::canSelectFiles
        | juce::FileBrowserComponent::warnAboutOverwriting,
        [this](const juce::FileChooser& c)
        {
            if (juce::FileInputStream inputStream(lastRecordedFile); inputStream.openedOk())
                if (const auto outputStream = c.getURLResult().createOutputStream())
                    outputStream->writeFromInputStream(inputStream, -1);

            playlistToStore->addTrack(lastRecordedFile);
            lastRecordedFile.deleteFile();
            recordButton.setButtonText("Start Recording");
        });
}