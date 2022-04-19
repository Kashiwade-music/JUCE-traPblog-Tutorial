/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class CustomLookAndFeel : public juce::LookAndFeel_V4 {
    void drawRotarySlider(juce::Graphics&, int x, int y, int width, int height,
        float sliderPosProportional, float rotaryStartAngle,
        float rotaryEndAngle, juce::Slider&) override;
    void drawComboBox(juce::Graphics& g, int width, int height, bool, int, int,
        int, int, juce::ComboBox& box) override;
    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        const juce::Slider::SliderStyle style,
        juce::Slider& slider) override;
};

//==============================================================================
/**
*/
class GainPantutorialAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    GainPantutorialAudioProcessorEditor(GainPantutorialAudioProcessor&,
        juce::AudioProcessorValueTreeState& vts); 
    ~GainPantutorialAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    CustomLookAndFeel customLookAndFeel;

    int width = 150;
    int height = 550;

    juce::Rectangle<int> GainAria{ width, height * 4 / 5 - 30 };
    juce::Rectangle<int> PannerAria{ 0, GainAria.getBottom(), width, height / 5};
    juce::Rectangle<int> PannerRuleAria{ 0, PannerAria.getBottom(), width, height - GainAria.getHeight() - PannerAria.getHeight()};

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GainPantutorialAudioProcessor& audioProcessor;

    typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment; 
    typedef juce::AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;

    juce::AudioProcessorValueTreeState& valueTreeState;
    juce::Slider gainSlider;
    std::unique_ptr<SliderAttachment> gainSliderAttachment;
    juce::Slider panAngleSlider;
    std::unique_ptr<SliderAttachment> panAngleSliderAttachment; 
    juce::ComboBox panRuleBox; 
    std::unique_ptr<ComboBoxAttachment> panRuleBoxAttachment;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainPantutorialAudioProcessorEditor)
};
