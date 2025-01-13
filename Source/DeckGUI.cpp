/*
  ==============================================================================

    DeckGUI.cpp
    Created: 22 Jan 2024 1:20:22pm
    Author:  Pichau

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* player, juce::AudioFormatManager& formatManager, juce::AudioThumbnailCache& thumbCache
) : player(player),
    waveFormDisplay(formatManager, thumbCache)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(waveFormDisplay);
    addAndMakeVisible(currentTrackLabel);

    playButton.addListener(this);

    stopButton.addListener(this);

    loadButton.addListener(this);

    volSlider.slider.addListener(this);
    volSlider.slider.setRange(0.0, 1.0);
    volSlider.slider.setValue(1.0);
    volSlider.slider.setColour(juce::Slider::thumbColourId, juce::Colours::blue);

    speedSlider.slider.addListener(this);
    speedSlider.slider.setRange(0.0, 10.0);
    speedSlider.slider.setValue(1.0);
    speedSlider.slider.setColour(juce::Slider::thumbColourId, juce::Colours::red);

    posSlider.slider.addListener(this);
    posSlider.slider.setRange(0.0, 1.0);
    posSlider.slider.setColour(juce::Slider::thumbColourId, juce::Colours::green);
   
    currentTrackLabel.setJustificationType(juce::Justification::centred);

    startTimer(1);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);

    if (currentTrack.getFileName() == "") {
        currentTrackLabel.setText("No Track Added", juce::NotificationType::dontSendNotification);
    }
    else {
        currentTrackLabel.setText(currentTrack.getFileName(), juce::NotificationType::dontSendNotification);
    }
}

void DeckGUI::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    double margin = getHeight() / 10;

    double rowHeight = (getHeight()-margin) / 8;

    currentTrackLabel.setBounds(0, 0, getWidth(), getHeight()/10);
    playButton.setBounds(0, margin, getWidth(), rowHeight);
    stopButton.setBounds(0, rowHeight+margin, getWidth(), rowHeight);
    volSlider.setBounds(0, rowHeight * 2 + margin, getWidth(), rowHeight);
    speedSlider.setBounds(0, rowHeight * 3 + margin, getWidth(), rowHeight);
    posSlider.setBounds(0, rowHeight * 4 + margin, getWidth(), rowHeight);
    waveFormDisplay.setBounds(0, rowHeight * 5 + margin, getWidth(), rowHeight*2);
    loadButton.setBounds(0, rowHeight * 7 + margin, getWidth(), rowHeight);

}

void DeckGUI::buttonClicked(juce::Button* button) {
    if (button == &playButton)
        player->start();
    else if (button == &stopButton)
        player->stop();
    else if (button == &loadButton) {
        // this does work in 6.1 but the syntax is a little funky
        // https://docs.juce.com/master/classFileChooser.html#ac888983e4abdd8401ba7d6124ae64ff3
        // - configure the dialogue
        auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles;
        // - launch out of the main thread
        // - note how we use a lambda function which you've probably
        // not seen before. Please do not worry too much about that. 
        fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
            {
                juce::File chosenFile = chooser.getResult();
                loadURL(juce::URL{ chosenFile });
            });
    }
}

void DeckGUI::sliderValueChanged(juce::Slider* slider) {
    if (slider == &volSlider.slider) {
        player->setGain(slider->getValue());
    }
    else if (slider == &speedSlider.slider) {
        player->setSpeed(slider->getValue());
    } /** will only set the positions of the player if slider is focused 
          because the value of the slider changes dinamicaly when it's not focused as well
        */
    else if (slider == &posSlider.slider && slider->hasKeyboardFocus(true)) {
        player->setPositionRelative(slider->getValue());
    }
}

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files) {
    DBG("DeckGUI::isInterestedInFileDrag");
    return true;
}
void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y) {
    DBG("DeckGUI::filesDropped");
    if (files.size() == 1) {
        loadURL(juce::URL{ juce::File{files[0]} });
    }
}

void DeckGUI::timerCallback() {
    waveFormDisplay.setPositionRelative(player->getPositionRelative());
    
    /** it will move the slider together with the track if it's not focused */
    if (!posSlider.hasKeyboardFocus(true))
        posSlider.slider.setValue(player->getPositionRelative()*posSlider.slider.getRange().getEnd());
}

bool DeckGUI::isPlaying() {
    return player->isPlaying();
}

void DeckGUI::loadURL(juce::URL url) {
    player->loadURL(url);
    waveFormDisplay.loadURL(url);
    currentTrack = url;
    repaint();
}
