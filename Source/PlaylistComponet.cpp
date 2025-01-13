/*
  ==============================================================================

    PlaylistComponet.cpp
    Created: 14 Feb 2024 6:38:50pm
    Author:  Pichau

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponet.h"

//==============================================================================
PlaylistComponet::PlaylistComponet()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    tableComponent.getHeader().addColumn("Track title", 1, 400);
    tableComponent.getHeader().addColumn("Add", 2, 100);
    tableComponent.getHeader().addColumn("Remove", 3, 100);
    
    tableComponent.setModel(this);

    addButton.addListener(this);

    addAndMakeVisible(tableComponent);
    addAndMakeVisible(addButton);

    loadTracks();
}

PlaylistComponet::~PlaylistComponet()
{
}

void PlaylistComponet::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponet", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponet::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    int totalWidth = getWidth();
    int totalHeigh = getHeight();
    addButton.setBounds(0, 0, totalWidth/4, totalHeigh);
    tableComponent.setBounds(totalWidth / 4, 0, totalWidth/4*3, totalHeigh);
    tableComponent.getHeader().setColumnWidth(1, tableComponent.getWidth()/8*6);
    tableComponent.getHeader().setColumnWidth(2, tableComponent.getWidth() / 8);
    tableComponent.getHeader().setColumnWidth(3, tableComponent.getWidth() / 8);
}

int PlaylistComponet::getNumRows() {
    return trackTitles.size();
}

void PlaylistComponet::paintRowBackground(juce::Graphics& g, int rownNumber, int width, int height, bool rowIsSelected) {
    if (rowIsSelected) {
        g.fillAll(juce::Colours::orange);
    }
    else {
        g.fillAll(juce::Colours::darkgrey);
    }
}

void PlaylistComponet::paintCell(juce::Graphics& g, int rownNumber, int columnId, int width, int height, bool rowIsSelected) {
    g.drawText(trackTitles[rownNumber].name, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
}

juce::Component* PlaylistComponet::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, juce::Component* existingComponent) {
    if (columnId == 2) {
        if (existingComponent == nullptr) {
            juce::TextButton* addBtn = new juce::TextButton{ "PLAY" };
            juce::String id{ std::to_string(rowNumber) };
            addBtn->setComponentID(id);
            addBtn->addListener(this);
            existingComponent = addBtn;
        }
    }
    else if (columnId == 3) {
        if (existingComponent == nullptr) {
            juce::TextButton* removeBtn = new juce::TextButton{ "REMOVE" };
            juce::String id{ std::to_string(rowNumber) };
            removeBtn->setComponentID("remove" + id);
            removeBtn->addListener(this);
            existingComponent = removeBtn;
        }
    }
    return existingComponent;
}

void PlaylistComponet::buttonClicked(juce::Button* button) {
    if (button == &addButton) {
        auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
            {
                juce::File chosenFile = chooser.getResult();
                DBG("File chosen:" + chosenFile.getFullPathName());
                addTrack(chosenFile);
            });
    }
    else if (button->getComponentID().toStdString().find("remove") != std::string::npos) {
        removeTrack(Utils::clearInteger(button->getComponentID().toStdString()));
    }
    else {
        bool found = false;
        for (auto* player : players) {
            if (!player->isPlaying()) {
                player->loadURL(juce::URL{ trackTitles[std::stoi(button->getComponentID().toStdString())].trackFile });
                found = true;
                break;
            }
        }
        if (!found) {
            if (indexLastPlayerUsed == players.size())
                indexLastPlayerUsed = 0;
            players[indexLastPlayerUsed]->loadURL(juce::URL{ trackTitles[std::stoi(button->getComponentID().toStdString())].trackFile });
            indexLastPlayerUsed++;
        }
    }
}

void PlaylistComponet::addTrack(juce::File track) {
    std::string fileName = track.getFileNameWithoutExtension().toStdString();
    if (fileName != "") {
        dirToSave.createDirectory(); // if the directory exists nothing will be done here
        // will only copy the file if the it's not already inside the dirToSave
        if (track.getParentDirectory().getFullPathName() != dirToSave.getFullPathName()) {
            juce::File fileToSave = dirToSave.getNonexistentChildFile(track.getFileName(), track.getFileExtension());
            track.copyFileTo(fileToSave); // persists the track into a directory named "SavedTracks"
            fileName = fileToSave.getFileNameWithoutExtension().toStdString();
            track = fileToSave;
        }
        trackTitles.push_back(Track{ fileName, track});
        tableComponent.updateContent();
        repaint();
    }
}

void PlaylistComponet::removeTrack(int id) {
    std::vector<Track> aux{};
    for (int i = 0; i < trackTitles.size(); ++i) {
        if (i == id)
            trackTitles[i].trackFile.deleteFile();
        else
            aux.push_back(trackTitles[i]);
    }
    trackTitles = aux;
    tableComponent.updateContent();
    repaint();
}

void PlaylistComponet::setPlayers(std::vector<DeckGUI*> _players) {
    players = _players;
}

void PlaylistComponet::loadTracks() {
    auto childFiles = dirToSave.findChildFiles(juce::File::TypesOfFileToFind::findFiles, false, "*.*");
    for (auto file : childFiles) {
        addTrack(file);
    }
}


