#pragma once
#include <JuceHeader.h>

// Represents one track in the playlist
struct Track
{
    juce::File file;
    juce::String title;
    juce::String artist;
};

// PlaylistComponent — shows a table of tracks and handles clicks
class PlaylistComponent : public juce::Component,
    public juce::TableListBoxModel
{
public:
    PlaylistComponent();
    ~PlaylistComponent() override = default;

    void resized() override;

    // Add track to the playlist
    void addTrack(const juce::File& file);

    // Get track by row index
    Track getTrack(int row) const;

    // Callbacks (you’ll use this to connect to PlayerAudio)
    std::function<void(const Track&)> onTrackDoubleClicked;

    // TableListBoxModel overrides
    int getNumRows() override;
    void paintRowBackground(juce::Graphics& g, int row, int width, int height, bool rowIsSelected) override;
    void paintCell(juce::Graphics& g, int row, int columnId, int width, int height, bool rowIsSelected) override;
    void cellDoubleClicked(int row, int columnId, const juce::MouseEvent&) override;

private:
    juce::TableListBox table;
    std::vector<Track> tracks;
};
