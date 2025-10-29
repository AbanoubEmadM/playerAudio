/*
  ==============================================================================

    PlayerGUI.h
    Created: 14 Oct 2025 2:07:00am
    Author:  emadb

  ==============================================================================
*/

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
    juce::TextButton loadButton{ "Load" };
    juce::TextButton playButton{ "Play" };
    juce::TextButton stopButton{ "Stop" };
	juce::TextButton returnButton{ "Return" };
    juce::TextButton loopButton{ "Loop" };
    juce::TextButton endButton{ "End" };
	juce::Label metadataLabel{ "metadata", "No File Loaded " };
    juce::Slider volumeSlider;
	juce::Slider positionSlider;
    // File chooser
    std::unique_ptr<juce::FileChooser> fileChooser;

    // Event handlers
    void buttonClicked(juce::Button* button);
    void sliderValueChanged(juce::Slider* slider);
    void addSongToPlaylist(const juce::File& file);


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
