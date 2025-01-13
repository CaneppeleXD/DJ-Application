/*
  ==============================================================================

    SliderWithLabel.h
    Created: 21 Feb 2024 9:23:25pm
    Author:  Pichau

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class SliderWithLabel  : public juce::Component
{
public:
    SliderWithLabel(std::string _text);
    ~SliderWithLabel() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    juce::Slider getSlider();

    juce::Label label;
    juce::Slider slider;

private:
    std::string text;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderWithLabel)
};
