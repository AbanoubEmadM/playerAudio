/*
  ==============================================================================

    PlayerGUI.cpp
    Created: 14 Oct 2025 2:06:50am
    Author:  emadb

  ==============================================================================
*/
#include <string>
#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
    // Add buttons
    for (auto* btn : { &loadButton, &playButton , &stopButton, &returnButton, &endButton ,&loopButton})
    {
        addAndMakeVisible(btn);
        btn->addListener(this);
    }
    addAndMakeVisible(playlist);
    playlist.onTrackDoubleClicked = [this](const Track& track)
        {
            playerAudio.loadFile(track.file);
            metadataLabel.setText(
                "Title: " + track.title + " | Artist: " + track.artist,
                juce::dontSendNotification
            );
        };
    playlist.addTrack(juce::File::getSpecialLocation(juce::File::userMusicDirectory).getChildFile("test.mp3"));

	//loop button settings trigger
	loopButton.setClickingTogglesState(true);
    
    // Volume slider
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);

	metadataLabel.setColour(juce::Label::textColourId, juce::Colours::white);
	metadataLabel.setJustificationType(juce::Justification::centredLeft);
	metadataLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    addAndMakeVisible(metadataLabel);

    //setSize(500, 250);
    //setAudioChannels(0, 2);
}
PlayerGUI::~PlayerGUI()
{
	playerAudio.releaseResources();
    //shutdownAudio();
}
void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    playerAudio.getNextAudioBlock(bufferToFill);
}

void PlayerGUI::releaseResources()
{
    playerAudio.releaseResources();
}

void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}
void PlayerGUI::resized()
{
    int y = 10;
    loadButton.setBounds(10, y, 80, 30);
    playButton.setBounds(100, y, 80, 30);
    stopButton.setBounds(190, y, 80, 30);
    returnButton.setBounds(280, y, 80, 30);
    endButton.setBounds(370, y, 80, 30);
    loopButton.setBounds(100, 85, 80, 30);

    metadataLabel.setBounds(10, y + 40, getWidth() - 20, 20);
    volumeSlider.setBounds(10, 60, getWidth() - 20, 30);

    playlist.setBounds(10, 110, getWidth() - 20, getHeight() - 120);
}

void PlayerGUI::addSongToPlaylist(const juce::File& file)
{
    if (file.existsAsFile())
    {
        // Add to the playlist table
        playlist.addTrack(file);

        // Optionally auto-update metadata label
        metadataLabel.setText("Added: " + file.getFileName(), juce::dontSendNotification);
    }
}


void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>(
            "Select an audio file...",
            juce::File::getSpecialLocation(juce::File::userMusicDirectory),
            "*.wav;*.mp3"
        );
        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile() && playerAudio.loadFile(file))
                {

                    if (playerAudio.getArtist().isEmpty() && playerAudio.getTitle().isEmpty() && playerAudio.getAlbum().isEmpty())
                    {
                        metadataLabel.setText(
                            file.getFileName(),
                            juce::dontSendNotification
                        );
                    }
                    else {
                        metadataLabel.setText(
                            "Artist: " + playerAudio.getArtist() +
                            " | Title: " + playerAudio.getTitle() +
                            " | Album: " + playerAudio.getAlbum(),
                            juce::dontSendNotification
                        );
                    }
                    addSongToPlaylist(file);

                }
            });
    }

    if (button == &playButton)
    {
        playerAudio.play();
    }

    if (button == &stopButton)
    {
        playerAudio.pause();
        //playerAudio.setPosition(0.0);
    }
    if (button == &returnButton)
    {
        playerAudio.setPosition(0.0);
	}
    if (button == &endButton)
    {
        playerAudio.setPosition(playerAudio.getLengthInSeconds());
	}
    if (button == &loopButton)
    {
		playerAudio.looptrack(loopButton.getToggleState());
    }

}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
        playerAudio.setGain((float)slider->getValue());
}
//void PlayerGUI::printMetaDate()
//{
//    // Print metadata
//    if (readerSource && readerSource->getAudioFormatReader())
//    {
//        auto* reader = readerSource->getAudioFormatReader();
//        for (auto& key : reader->metadataValues.getAllKeys())
//        {
//            juce::String value = reader->metadataValues[key];
//            DBG(key << ": " << value);
//        }
//    }
//
//}
