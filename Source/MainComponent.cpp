#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    std::vector<DeckGUI*> decks;
    decks.push_back(&deckGUI1);
    decks.push_back(&deckGUI2);
    playlistComponent.setPlayers(decks);

    setLookAndFeel(&customLookAndFeel);

    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }

    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);

    addAndMakeVisible(playlistComponent);

    addAndMakeVisible(audioRecordComp);

    formatManager.registerBasicFormats();

}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);

    recorder.audioDeviceAboutToStart(sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
    recorder.audioRecorderCallback(bufferToFill.buffer->getArrayOfReadPointers(), bufferToFill.buffer->getNumChannels(), bufferToFill.buffer->getNumSamples());
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.   

    double deckHeigh = getHeight() / 4 * 3;

    deckGUI1.setBounds(0, 0, getWidth()/2, deckHeigh);
    deckGUI2.setBounds(getWidth() / 2, 0, getWidth() / 2, deckHeigh);

    playlistComponent.setBounds(0, getHeight()/4*3, getWidth()/2, getHeight()/4*1);

    audioRecordComp.setBounds(getWidth() / 2, getHeight() / 4 * 3, getWidth() / 2, getHeight() / 4 * 1);
}
