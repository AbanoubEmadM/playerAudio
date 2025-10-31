#include"PlayerAudio.h"

PlayerAudio::PlayerAudio() {
	formatManager.registerBasicFormats();
}
PlayerAudio::~PlayerAudio() {
    transportSource.setSource(nullptr);
}
void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
	transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
	transportSource.getNextAudioBlock(bufferToFill);

	//check for looping after track ends if looping is enabled start from beginning
    if (loop && transportSource.hasStreamFinished())
    {
        transportSource.setPosition(0.0);
        transportSource.start();
    }
}
void PlayerAudio::releaseResources() {
	transportSource.releaseResources();
}
bool PlayerAudio::loadFile(const juce::File& file) {
        if (auto* reader = formatManager.createReaderFor(file))
        {
            // Disconnect old source first
            transportSource.stop();
            transportSource.setSource(nullptr);
            readerSource.reset();

            const auto& meta = reader->metadataValues;
            artist = meta["artist"].isNotEmpty() ? meta["artist"] : meta["IART"];
            title = meta["title"].isNotEmpty() ? meta["title"] : meta["INAM"];
            album = meta["album"].isNotEmpty() ? meta["album"] : meta["IPRD"];


            // Create new reader source
            readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);

            // Attach safely
            transportSource.setSource(readerSource.get(),
                0,
                nullptr,
                reader->sampleRate);
            transportSource.start();
            return true;
        }
    return false;
}

void PlayerAudio::play()
{
    transportSource.start();
}
void PlayerAudio::stop()
{
    transportSource.stop();
    transportSource.setPosition(0.0);
}
void PlayerAudio::pause()
{
    transportSource.stop();
}
void PlayerAudio::setGain(float newGain)
{
    transportSource.setGain(newGain);
	if (newGain==0.0f)
		muted = true;
    else if (newGain!=0.0f)
		muted = false;
}
void PlayerAudio::setPosition(double newPosition)
{
    transportSource.setPosition(newPosition);
}
void PlayerAudio::setVolume(float newVolume)
{
    lastVolume = newVolume;
    if (!muted)
        transportSource.setGain(newVolume);
}
float PlayerAudio::getVolume() const
{
    return lastVolume;
}
void PlayerAudio::setMuted(bool shouldMute)
{
    if (shouldMute && !muted)
	{
		lastVolume = transportSource.getGain();
		muted = true;
        transportSource.setGain(0.0f);
	}
    else if (!shouldMute && muted)
	{
		muted = false;
        transportSource.setGain(lastVolume);
	}
}
bool PlayerAudio::isMuted() const
{
    return muted;
}
void PlayerAudio::looptrack(bool looping_state)
{
    loop = looping_state;
	// If looping state is on and transport is not playing anything restart from beginning
     if (!transportSource.isPlaying())
     {
            transportSource.setPosition(0.0);
            transportSource.start();
     }
}
double PlayerAudio::getPosition() const
{
    return transportSource.getCurrentPosition();
}
double PlayerAudio::getLengthInSeconds() const
{
    return transportSource.getLengthInSeconds();
}