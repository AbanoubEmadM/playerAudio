#pragma once

#include <JuceHeader.h>
class PlayerAudio {
public:
	PlayerAudio();
	~PlayerAudio();

    // Audio
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

	bool loadFile(const juce::File& file);

	void play();
	void stop();
	void pause();
	void setGain(float newGain);
	void setPosition(double newPosition);
	void looptrack(bool looping_state);
	double getPosition() const;
	double getLengthInSeconds() const;
	
	juce::String getArtist() const { return artist; }
	juce::String getTitle() const { return title; }
	juce::String getAlbum() const { return album; }

private:
    // Audio
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;

	juce::String artist{ "Unknown Artist" };
	juce::String title{ "Unknown Title" };
	juce::String album{ "Unknown Album" };

	bool loop{ false };
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};
