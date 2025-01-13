/*
  ==============================================================================

    SliderWithLabel.cpp
    Created: 21 Feb 2024 9:23:25pm
    Author:  Pichau

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SliderWithLabel.h"

//==============================================================================
SliderWithLabel::SliderWithLabel(std::string _text) : text(_text)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    addAndMakeVisible(slider);
    addAndMakeVisible(label);

    label.attachToComponent(&slider, true);
}

SliderWithLabel::~SliderWithLabel()
{
}

void SliderWithLabel::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    label.setText(text, juce::NotificationType::dontSendNotification);
    label.setJustificationType(juce::Justification::right);
}

void SliderWithLabel::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    double labelWidth = getWidth() / 5;

    slider.setBounds(labelWidth, 0, getWidth() - labelWidth, getHeight());
    label.setBounds(0, 0, labelWidth, getHeight());
    label.setFont(label.getFont().withHeight(getHeight()/3));

}
