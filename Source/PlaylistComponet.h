/*
  ==============================================================================

    PlaylistComponet.h
    Created: 14 Feb 2024 6:38:50pm
    Author:  Pichau

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include "Track.h"
#include "DeckGUI.h"
#include "Utils.h"

//==============================================================================
/*
*/
class PlaylistComponet  : public juce::Component, public juce::TableListBoxModel, public juce::Button::Listener
{
public:
    PlaylistComponet();
    ~PlaylistComponet() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getNumRows() override;

    void paintRowBackground(juce::Graphics& g, int rownNumber, int width, int height, bool rowIsSelected) override;

    void paintCell(juce::Graphics& g, int rownNumber, int columnId, int width, int height, bool rowIsSelected) override;

    juce::Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, juce::Component* existingComponent) override;

    void buttonClicked(juce::Button* button) override;

    void setPlayers(std::vector<DeckGUI*> _players);

    /** 
        Adds a new track to the trackTitles vector and also makes a copy of the file to the directory specified at the dirToSave variable
    */
    void addTrack(juce::File trackFile);
private:

    juce::TableListBox tableComponent;

    std::vector<Track> trackTitles;

    std::vector<DeckGUI*> players;

    int indexLastPlayerUsed = 0;

    juce::TextButton addButton{"Add new Track"};

    void removeTrack(int id);

    /** Load and add all the tracks saved in the dirToSave variable */
    void loadTracks();

    /** Location where the tracks will be persisted */
    juce::File dirToSave = juce::File::getSpecialLocation(juce::File::currentExecutableFile).getParentDirectory().getChildFile("SavedTracks");

    juce::FileChooser fChooser{"Select a file..."};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponet)
};
