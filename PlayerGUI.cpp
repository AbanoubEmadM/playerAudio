#include <string>
#include "PlayerGUI.h"


void PlayerGUI::setImageButton(juce::ImageButton& button,
    const void* imageData, int imageSize,
    const juce::String& tooltip)
{
    auto img = juce::ImageFileFormat::loadFrom(imageData, imageSize);
    button.setImages(true, true, true,
        img, 1.0f, juce::Colours::transparentBlack,
        img, 0.8f, juce::Colours::transparentBlack,
        img, 0.5f, juce::Colours::transparentBlack);
    button.setTooltip(tooltip);
    addAndMakeVisible(button);
    button.addListener(this);
}

PlayerGUI::PlayerGUI()
{
    addAndMakeVisible(playlist);

    playlist.onTrackDoubleClicked = [this](const Track& track)
        {
            if (playerAudio.loadFile(track.file))
            {
                metadataLabel.setText(
                    "Title: " + track.title + " | Artist: " + track.artist,
                    juce::dontSendNotification
                );

                // 🔹 Reflect that playback started automatically
                playButton.setImages(true, true, true,
                    pauseImage, 1.0f, juce::Colours::transparentBlack,
                    pauseImage, 0.8f, juce::Colours::transparentBlack,
                    pauseImage, 0.5f, juce::Colours::transparentBlack);
                isPlaying = true;
            }
        };


    playImage = juce::ImageFileFormat::loadFrom(BinaryData::play_png, BinaryData::play_pngSize)
        .rescaled(36, 36, juce::Graphics::highResamplingQuality);
    pauseImage = juce::ImageFileFormat::loadFrom(BinaryData::stop_png, BinaryData::stop_pngSize)
        .rescaled(36, 36, juce::Graphics::highResamplingQuality);
    setImageButton(loadButton, BinaryData::load_png, BinaryData::load_pngSize, "Load File");
    setImageButton(playButton, BinaryData::play_png, BinaryData::play_pngSize, "Play / Pause");
    //setImageButton(stopButton, BinaryData::stop_png, BinaryData::stop_pngSize, "Stop");
    setImageButton(returnButton, BinaryData::return_png, BinaryData::return_pngSize, "Return to Start");
    setImageButton(loopButton, BinaryData::loop_png, BinaryData::loop_pngSize, "Loop");
    setImageButton(endButton, BinaryData::end_png, BinaryData::end_pngSize, "Go to End");
    //setImageButton(muteButton, BinaryData::mute_png, BinaryData::mute_pngSize, "Mute / Unmute");

    // ?? Volume slider
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);

    // ?? Metadata label
    metadataLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    metadataLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    metadataLabel.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(metadataLabel);

    // ?? Playlist
    addAndMakeVisible(playlist);

    // (optional: preload test song)
    // playlist.addTrack(juce::File("C:/Music/test.mp3"));
}

PlayerGUI::~PlayerGUI()
{
    playerAudio.releaseResources();
}

void PlayerGUI::releaseResources(){
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

void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::white);
}

void PlayerGUI::resized()
{
    int x = 10;
    int y = 10;
    int buttonWidth = 36;
    int buttonHeight = 36;
    int spacing = 16;

    auto layout = [&](juce::ImageButton& btn)
        {
            btn.setBounds(x, y, buttonWidth, buttonHeight);
            x += buttonWidth + spacing;
        };

    layout(loadButton);
    layout(playButton);
    layout(returnButton);
    layout(loopButton);
    layout(endButton);
    layout(muteButton);

    metadataLabel.setBounds (20, 60, getWidth() - 20, 20);
    volumeSlider.setBounds  (10, 80, getWidth() - 20, 30);
    playlist.setBounds(10, 150, 400, 200);

}

void PlayerGUI::addSongToPlaylist(const juce::File& file)
{
    if (file.existsAsFile())
    {
        // Add to the playlist table
        playlist.addTrack(file);

        // Optionally auto-update metadata label
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

    }
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton)
    {
        if (!isPlaying)
        {
            playerAudio.play();
            playButton.setImages(true, true, true,
                pauseImage, 1.0f, juce::Colours::transparentBlack,
                pauseImage, 0.8f, juce::Colours::transparentBlack,
                pauseImage, 0.5f, juce::Colours::transparentBlack);
            isPlaying = true;
        }
        else
        {
            playerAudio.pause();
            playButton.setImages(true, true, true,
                playImage, 1.0f, juce::Colours::transparentBlack,
                playImage, 0.8f, juce::Colours::transparentBlack,
                playImage, 0.5f, juce::Colours::transparentBlack);
            isPlaying = false;
        }
    }

    else if (button == &loadButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>(
            "Select an audio file...", juce::File{}, "*.wav;*.mp3");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile())
                {
                    playerAudio.loadFile(file);
                    addSongToPlaylist(file);

                    playButton.setImages(true, true, true,
                        pauseImage, 1.0f, juce::Colours::transparentBlack,
                        pauseImage, 0.8f, juce::Colours::transparentBlack,
                        pauseImage, 0.5f, juce::Colours::transparentBlack);
                    isPlaying = true;

                }
            });
    }

    else if (button == &loopButton)
        playerAudio.looptrack(loopButton.getToggleState());

    else if (button == &muteButton)
        playerAudio.setMuted(!playerAudio.isMuted());
    else if (button == &endButton) {
        //playlist.getTrack()
    }
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
        playerAudio.setGain((float)slider->getValue());
	if ((float)slider->getValue() != 0.0f) 
		muteButton.setButtonText("Mute");
	else if ((float)slider->getValue() == 0.0f)
		muteButton.setButtonText("Unmute");
}
