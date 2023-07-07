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
        // Radius of knob
        auto radius = jmin(width / 2, height / 2) - 5.0f;
        // Centre point (centreX, centreY) of knob
        auto centreX = x + width * 0.5f;
        auto centreY = y + height * 0.5f;

        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        // current angle of the slider
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        // Draw path of the slider background (in darker background colour)
        Path backgroundArc;
        backgroundArc.addCentredArc(centreX, centreY, radius, radius, 0.0f, rotaryStartAngle, rotaryEndAngle, true);
        Colour myColour = findColour(0x1005700);
        myColour = myColour.darker(.8f);
        g.setColour(myColour);
        g.strokePath(backgroundArc, PathStrokeType(radius * 0.1f, PathStrokeType::curved, PathStrokeType::rounded));

        // Draw path of slider foreground (in white)
        Path foregroundArc;
        foregroundArc.addCentredArc(centreX, centreY, radius, radius, 0.0f, rotaryStartAngle, angle, true);
        g.setColour(Colours::antiquewhite);
        g.strokePath(foregroundArc, PathStrokeType(radius * 0.1f, PathStrokeType::curved, PathStrokeType::rounded));

        // Calculate inner circle dimensions
        auto innerRadius = radius * 0.9f;
        auto innerX = centreX - innerRadius;
        auto innerY = centreY - innerRadius;

        // Create the gradient for the inner circle
        ColourGradient gradient(Colour(0xFF333333), innerX, innerY, Colour(0xFFCCCCCC), innerX + innerRadius * 2.0f, innerY + innerRadius * 2.0f, false);

        // Draw the gradient-filled inner circle
        g.setGradientFill(gradient);
        g.fillEllipse(innerX, innerY, innerRadius * 2.0f, innerRadius * 2.0f);

        // Restore color and stroke settings for the pointer
        g.setColour(Colours::antiquewhite); // Set the color of the pointer


        // Pointer
//        Path p;
//        auto pointerLength = radius * .8f;
//        auto pointerThickness = radius * .08f;
//        p.addRectangle(pointerThickness * 0.2f, -radius - 5, pointerThickness, pointerLength);
//        p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));
//        g.fillPath(p);
        
        // Pointer
        
        /** gets janky at smaller sizes */
        
        Path p;
        auto pointerLength = radius * .8f;
        auto pointerThickness = radius * .05f;
        auto cornerSize = pointerThickness * 0.2f;
        p.addRoundedRectangle(-pointerThickness, -radius - 3, 2 * pointerThickness, pointerLength, 2);
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

