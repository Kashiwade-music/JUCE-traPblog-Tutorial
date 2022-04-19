/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#define _USE_MATH_DEFINES
#include "math.h"
void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
    float sliderPos, float rotaryStartAngle,
    float rotaryEndAngle, juce::Slider& slider)  {

    auto outline = slider.findColour(juce::Slider::rotarySliderOutlineColourId);
    auto fill = slider.findColour(juce::Slider::rotarySliderFillColourId);
    auto background = slider.findColour(juce::Slider::backgroundColourId);
    auto thumb = slider.findColour(juce::Slider::thumbColourId);

    auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat().reduced(10);
    auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;

    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto lineW = juce::jmin(4.0f, radius * 0.5f);
    auto arcRadius = radius - lineW * 0.5f;

    juce::Path backgroundArc;
    backgroundArc.addCentredArc(bounds.getCentreX(), bounds.getCentreY(), arcRadius, arcRadius, 0.0f, rotaryStartAngle, rotaryEndAngle, true);
    g.setColour(outline);
    g.strokePath(backgroundArc, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    juce::Path valueArc;
    valueArc.addCentredArc(bounds.getCentreX(), bounds.getCentreY(), arcRadius, arcRadius, 0.0f, 2 * M_PI, toAngle, true);
    g.setColour(fill);
    g.strokePath(valueArc, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    radius = radius - lineW * 2;
    g.setColour(background);
    g.fillEllipse(bounds.getCentreX() - radius, bounds.getCentreY() - radius, radius * 2, radius * 2);

    juce::Path p;
    auto pointerLength = radius * 0.5f;
    auto pointerThickness = 2.4f;
    p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    p.applyTransform(juce::AffineTransform::rotation(toAngle).translated(bounds.getCentreX(), bounds.getCentreY()));
    g.setColour(thumb);
    g.fillPath(p);
}

void CustomLookAndFeel::drawComboBox(juce::Graphics& g, int width, int height, bool, int, int,
    int, int, juce::ComboBox& box) {
    juce::Rectangle<int> boxBounds(0, 0, width, height);

    g.setColour(box.findColour(juce::ComboBox::backgroundColourId));
    g.fillRect(boxBounds.toFloat());
    
    g.setColour(box.findColour(juce::ComboBox::outlineColourId));
    g.drawRect(boxBounds.toFloat(), 1.0f);

    juce::Rectangle<int> arrowZone(width - 15, 0, 10, height);
    juce::Path path;
    path.startNewSubPath((float)arrowZone.getX() + 2.0f, (float)arrowZone.getCentreY() - 1.0f);
    path.lineTo((float)arrowZone.getCentreX(), (float)arrowZone.getCentreY() + 2.0f);
    path.lineTo((float)arrowZone.getRight() - 2.0f, (float)arrowZone.getCentreY() - 1.0f);

    g.setColour(box.findColour(juce::ComboBox::arrowColourId).withAlpha((box.isEnabled() ? 0.9f : 0.2f)));
    g.strokePath(path, juce::PathStrokeType(2.0f));
}

void CustomLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
    float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle style,
    juce::Slider& slider) {

    auto track = slider.findColour(juce::Slider::trackColourId);
    auto background = slider.findColour(juce::Slider::backgroundColourId);
    
    auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat();

    float lineW = 10.0f;
    
    juce::Path line;
    auto lineMain = slider.findColour(juce::Slider::textBoxTextColourId);
    line.startNewSubPath(bounds.getCentreX() - 30, 136.86);
    line.lineTo(bounds.getCentreX() + 30, 136.86);
    g.setColour(lineMain);
    g.strokePath(line, juce::PathStrokeType(1.0f));


    g.setColour(background);
    g.fillRect(juce::Rectangle<float>(bounds.getCentreX() - lineW * 0.5, bounds.getY(), lineW, bounds.getHeight()));

    g.setColour(track);
    g.fillRect(juce::Rectangle<float>(bounds.getCentreX() - lineW * 0.5, sliderPos, lineW, bounds.getBottom() - sliderPos));

    juce::Image knob = juce::ImageCache::getFromMemory(BinaryData::fader_png, BinaryData::fader_pngSize);
    float knobWidth = knob.getWidth() * 0.15;
    float knobHeight = knob.getHeight() * 0.15;
    g.drawImage(knob, juce::Rectangle<float>(bounds.getCentreX() - knobWidth * 0.5, sliderPos - knobHeight * 0.5, knobWidth, knobHeight));
}

//==============================================================================
GainPantutorialAudioProcessorEditor::GainPantutorialAudioProcessorEditor(
    GainPantutorialAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor(&p), audioProcessor(p), valueTreeState(vts)
{
    gainSliderAttachment.reset(new SliderAttachment(valueTreeState, "gain", gainSlider));
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    gainSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, width * 2 / 3, PannerAria.getHeight() / 6);
    gainSlider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colour::Colour(46, 52, 64));
    gainSlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colour::Colour(136, 192, 208));
    gainSlider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colour::Colour(114, 122, 140));
    gainSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colour::Colour(236, 239, 244));
    gainSlider.setColour(juce::Slider::ColourIds::textBoxHighlightColourId, juce::Colour::Colour(129, 161, 193));
    gainSlider.setLookAndFeel(&customLookAndFeel);
    gainSlider.setTextValueSuffix(" dB");
    addAndMakeVisible(gainSlider);

    panAngleSliderAttachment.reset(new SliderAttachment(valueTreeState, "panangle", panAngleSlider));
    panAngleSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    panAngleSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, width * 2 / 3, PannerAria.getHeight() / 6);
    panAngleSlider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colour::Colour(236, 239, 244));
    panAngleSlider.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colour::Colour(46, 52, 64));
    panAngleSlider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colour::Colour(136, 192, 208));
    panAngleSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colour::Colour(46, 52, 64));
    panAngleSlider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colour::Colour(114, 122, 140));
    panAngleSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colour::Colour(236, 239, 244));
    panAngleSlider.setColour(juce::Slider::ColourIds::textBoxHighlightColourId, juce::Colour::Colour(129, 161, 193));
    panAngleSlider.setLookAndFeel(&customLookAndFeel);
    addAndMakeVisible(panAngleSlider); 

    panRuleBox.addItemList(
        juce::StringArray("linear", "balanced", "sin3dB", "sin4.5dB", "sin6dB",
            "sqrt3dB", "sqrt4.5dB"),
        1);
    panRuleBoxAttachment.reset(new ComboBoxAttachment(valueTreeState, "panrule", panRuleBox));
    panRuleBox.setLookAndFeel(&customLookAndFeel);
    panRuleBox.setColour(juce::ComboBox::ColourIds::backgroundColourId, juce::Colour::Colour(59, 66, 82));
    panRuleBox.setColour(juce::ComboBox::ColourIds::textColourId, juce::Colour::Colour(236, 239, 244));
    panRuleBox.setColour(juce::ComboBox::ColourIds::arrowColourId, juce::Colour::Colour(236, 239, 244));
    panRuleBox.setColour(juce::ComboBox::ColourIds::outlineColourId, juce::Colour::Colour(114, 122, 140));
    panRuleBox.setScrollWheelEnabled(true);
    addAndMakeVisible(panRuleBox); 
        // Make sure that before the constructor has finished, you've set the
        // editor's size to whatever you need it to be.
    setSize(width, height);
}

GainPantutorialAudioProcessorEditor::~GainPantutorialAudioProcessorEditor()
{
}

//==============================================================================
void GainPantutorialAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colour::Colour(59, 66, 82));

    
}

void GainPantutorialAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    gainSlider.setBounds(GainAria.reduced(width / 6, 5)); 
    panAngleSlider.setBounds(PannerAria.reduced(width / 6, 5));
    panRuleBox.setBounds(PannerRuleAria.reduced(width / 6, 4)); 

}