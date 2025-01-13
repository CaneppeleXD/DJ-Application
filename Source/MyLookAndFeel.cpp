/*
  ==============================================================================

    MyLookAndFeel.cpp
    Created: 21 Feb 2024 7:24:23pm
    Author:  Pichau

  ==============================================================================
*/

#include "MyLookAndFeel.h"

MyLookAndFeel::MyLookAndFeel() {
    setColour(juce::ResizableWindow::backgroundColourId, juce::Colour{79, 47, 47});
    setColour(juce::TextButton::buttonColourId, juce::Colours::darkslategrey);
}
