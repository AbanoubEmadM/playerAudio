#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"
#include "PlaylistComponent.h"

class PlayerGUI : public juce::Component,
                  public juce::Button::Listener,
                  public juce::Slider::Listener
{
public:
	PlayerGUI();
	~PlayerGUI() override;
    // Audio
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

    // GUI
    void paint(juce::Graphics& g);
    void resized() override;

private:
	PlayerAudio playerAudio;
    PlaylistComponent playlist;

    // Audio player component
    juce::AudioTransportSource transportSource;
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    // GUI components
    juce::ImageButton loadButton;
    juce::ImageButton playButton;
    bool isPlaying = false;
    int currentTrackIndex = -1;   // -1 means no track loaded yet

    juce::Image playImage;
    juce::Image pauseImage;

	juce::ImageButton returnButton;
    juce::ImageButton loopButton;
    juce::ImageButton endButton;
	juce::ImageButton muteButton;
	juce::Label metadataLabel{ "metadata", "No File Loaded " };
    juce::Slider volumeSlider;
	juce::Slider positionSlider;
    juce::Slider speedSlider;
    // File chooser
    std::unique_ptr<juce::FileChooser> fileChooser;

    // Event handlers
    void buttonClicked(juce::Button* button);
    void sliderValueChanged(juce::Slider* slider);
    void addSongToPlaylist(const juce::File& file);
    void playPreviousTrack();
    void playNextTrack();
    void setImageButton(juce::ImageButton& button,
        const void* imageData, int imageSize,
        const juce::String& tooltip);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
