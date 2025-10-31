#include "PlaylistComponent.h"

PlaylistComponent::PlaylistComponent()
{
    addAndMakeVisible(table);

    table.getHeader().addColumn("Title", 1, 200);
    table.getHeader().addColumn("Artist", 2, 200);
    table.setModel(this);
}

void PlaylistComponent::resized()
{
    table.setBounds(getLocalBounds());
}

void PlaylistComponent::addTrack(const juce::File& file)
{
    Track track;
    track.file = file;
    track.title = file.getFileNameWithoutExtension();
    // You could later parse metadata for the artist field
    track.artist = "Unknown Artist";

    tracks.push_back(track);
    table.updateContent();
}

Track PlaylistComponent::getTrack(int row) const
{
    if (row >= 0 && row < (int)tracks.size())
        return tracks[row];
    return {};
}

int PlaylistComponent::getNumRows()
{
    return (int)tracks.size();
}
void PlaylistComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::red); // TEMP: bright red to verify visibility

}


void PlaylistComponent::paintRowBackground(juce::Graphics& g, int row, int width, int height, bool selected)
{
    if (selected)
        g.fillAll(juce::Colours::lightblue);
}

void PlaylistComponent::paintCell(juce::Graphics& g, int row, int columnId, int width, int height, bool)
{
    if (row < (int)tracks.size())
    {
        const auto& track = tracks[row];
        juce::String text;

        if (columnId == 1) text = track.title;
        else if (columnId == 2) text = track.artist;

        g.drawText(text, 2, 0, width - 4, height, juce::Justification::centredLeft);
    }
}

void PlaylistComponent::cellDoubleClicked(int row, int, const juce::MouseEvent&)
{
    if (onTrackDoubleClicked && row >= 0 && row < (int)tracks.size())
    {
        onTrackDoubleClicked(tracks[row]);
    
    }
}