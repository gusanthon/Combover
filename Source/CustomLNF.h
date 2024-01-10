/*
  ==============================================================================

    KnobLookAndFeel.h
    Created: 21 Mar 2019 5:47:22pm
    Author:  Joonas

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

using namespace juce;


class CustomLNF : public LookAndFeel_V4
{
public:
    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
                          const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
    {
        
        auto radius = jmin(width / 2, height / 2) - 5.0f;

        auto centreX = x + width * 0.5f;
        auto centreY = y + height * 0.5f;

        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        Path foregroundArc;
        foregroundArc.addCentredArc(centreX, centreY, radius, radius, 0.0f, rotaryStartAngle, angle, true);
        g.setColour(Colours::antiquewhite);
        g.strokePath(foregroundArc, PathStrokeType(radius * 0.1f, PathStrokeType::curved, PathStrokeType::rounded));

        auto innerRadius = radius * 0.9f;
        auto innerX = centreX - innerRadius;
        auto innerY = centreY - innerRadius;

        ColourGradient gradient(Colour(0xFF333333), innerX, innerY, Colour(0xFFCCCCCC), innerX + innerRadius * 2.0f, innerY + innerRadius * 2.0f, false);

        juce::Colour ptr_colour = juce::Colours::antiquewhite;

        if (slider.isMouseOverOrDragging())
        {
            gradient.multiplyOpacity(.75);

            ptr_colour = ptr_colour.withAlpha(0.25f);
        }
        
        g.setGradientFill(gradient);
        g.fillEllipse(innerX, innerY, innerRadius * 2.0f, innerRadius * 2.0f);

        g.setColour(ptr_colour);
                
        Path p;
        auto pointerLength = radius * .8f;
        auto pointerThickness = radius * .05f;

        p.addRoundedRectangle(-pointerThickness, -radius - 1, 2 * pointerThickness, pointerLength, 2);
        p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));
        g.fillPath(p);

    }



    // Slider textbox
    void drawLabel(Graphics& g, Label& label) override
    {
        g.setColour(Colours::white);

        String text = label.getText();
        int width = label.getWidth();
        int height = label.getHeight();
        g.setFont(Font("Futura", "Bold", 15.f));
        g.drawFittedText(text, 0, 0, width, height, Justification::centred, 1);
    }
    
    
    juce::Font getComboBoxFont(juce::ComboBox& comboBox) override
    {
        return juce::Font("Futura", 1, juce::Font::plain);
    }
    
    juce::Font getPopupMenuFont() override
    {
        // Return the desired font for the ComboBox options
        return juce::Font("Futura", 14.0f, juce::Font::plain);
    }
    
};

