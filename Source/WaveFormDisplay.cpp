/*
  ==============================================================================

    WaveFormDisplay.cpp
    Created: 29 Jan 2024 8:42:51pm
    Author:  Pichau

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveFormDisplay.h"

//==============================================================================
WaveFormDisplay::WaveFormDisplay(juce::AudioFormatManager& formatManager, juce::AudioThumbnailCache& thumbCache
) : audioThumb(1000, formatManager, thumbCache),
    fileLoaded(false),
    positionRelative(0)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    audioThumb.addChangeListener(this);
}

WaveFormDisplay::~WaveFormDisplay()
{
}

void WaveFormDisplay::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::orange);
    if (fileLoaded) {
        audioThumb.drawChannel(g,
            getLocalBounds(),
            0,
            audioThumb.getTotalLength(),
            0,
            1.0f);
        g.setColour(juce::Colours::lightgreen);
        double rectWidth = getWidth() / 60;
        g.drawRect(positionRelative * getWidth() - rectWidth * 2, 0, rectWidth, getHeight());
    }
    else {
        g.setFont(22.0f);
        g.drawText("File not loaded yet", getLocalBounds(),
            juce::Justification::centred, true);   // draw some placeholder text
    }
}

void WaveFormDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void WaveFormDisplay::loadURL(juce::URL audioURL) {

    audioThumb.clear();

    fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));

}

void WaveFormDisplay::changeListenerCallback(juce::ChangeBroadcaster* source) {
    repaint();
}

void WaveFormDisplay::setPositionRelative(double positionRelative) {
    if (this->positionRelative != positionRelative) {
        this->positionRelative = positionRelative;
        repaint();
    }
}
