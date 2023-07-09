/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ComboverAudioProcessorEditor::ComboverAudioProcessorEditor (ComboverAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
    tabs(audioProcessor),
    topBar("COMBOVER", tabs, audioProcessor.apvts)
{
    addAndMakeVisible(topBar);
    addAndMakeVisible(tabs);
    
    topBar.toFront(false);

    setResizable(true, true);
//    getConstrainer()->setFixedAspectRatio(2.0f);
    setResizeLimits(400, 550, 5000, 5000);
    setSize (700, 600);

}

ComboverAudioProcessorEditor::~ComboverAudioProcessorEditor()
{
}

//==============================================================================
void ComboverAudioProcessorEditor::paint (juce::Graphics& g)
{
    
    g.fillAll(juce::Colour::fromRGB(35, 35, 45).darker());
    g.setFont (15.0f);

    // Define the parameters for the aluminum texture pattern
    int tileSize = 20; // Size of each tile in pixels
    int numTilesX = getWidth() / tileSize;
    int numTilesY = getHeight() / tileSize;
    
//     Create the aluminum texture pattern
    /**
    for (int y = 0; y < numTilesY; ++y)
    {
        for (int x = 0; x < numTilesX; ++x)
        {
            // Calculate the position of the current tile
            int xPos = x * tileSize;
            int yPos = y * tileSize;

            // Calculate the color of the current tile
            Colour tileColor = Colour::fromRGB(220, 220, 220); // Adjust the RGB values as desired

            // Draw the current tile
            g.setColour(tileColor);
            g.fillRect(xPos, yPos, tileSize, tileSize);

            // Draw a diagonal line in the tile
            g.setColour(Colours::black); // Adjust the line color as desired
            g.drawLine(xPos, yPos, xPos + tileSize, yPos + tileSize, 1.0f);
        }
    }
    */
}

void ComboverAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    topBar.setBounds(getLocalBounds().removeFromTop(topBarHeight));
    
    tabs.setBounds(getLocalBounds().reduced(45));
}


bool ComboverAudioProcessorEditor::keyPressed(const KeyPress& press)
{
    int numTabs = tabs.getNumTabs();
    int current = tabs.getCurrentTabIndex();

    if (press == juce::KeyPress::pageUpKey && current + 1 < numTabs)
    {
        tabs.setCurrentTabIndex(current + 1);
        return true;
            
    }
    else if (press == juce::KeyPress::pageDownKey && current > 0)
    {
        tabs.setCurrentTabIndex(current - 1);
        return true;
    }
    

    switch (press.getTextCharacter())
    {
        case '0':
            tabs.setCurrentTabIndex(0);
            return true;
        case '1':
            tabs.setCurrentTabIndex(1);
            return true;
        case '2':
            tabs.setCurrentTabIndex(2);
            return true;
        case '3':
            if (numTabs > 3){
                tabs.setCurrentTabIndex(3);
                return true;
            }
        case '4':
            if (numTabs > 4){
                tabs.setCurrentTabIndex(4);
                return true;
            }
            
        case '5':
            if (numTabs > 5){
                tabs.setCurrentTabIndex(5);
                return true;
            }
            
        case '6':
            if (numTabs > 6) {
                tabs.setCurrentTabIndex(6);
                return true;
            }
            
        case '7':
            if (numTabs > 7)
            {
                tabs.setCurrentTabIndex(7);
                return true;
            }
            break;
        case '8':
            if (numTabs > 8)
            {
                tabs.setCurrentTabIndex(8);
                return true;
            }
            break;
        case '9':
            tabs.setCurrentTabIndex(numTabs - 1);
            return true;

    }
    
    return false;
}
