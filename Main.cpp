/*
FCAI – OOP Programming – 2025/2026 - Assignment 2
Program Name: Babyeditor
Last Modification Date: 24/10/2025
worked on by :

Abanoub Emad Makram           ID: 20240002 worked on sepration , play/stop/return/end buttons,display metadata
Amal Ahmad Mohamed            ID: 20240090 worked on sepration , mute button
Rayan Abdelrahman Ibrahim     ID: 20210727 worked on sepration , looping button

coures instructor :
Dr Mohamed El-Ramely
thank you for using our program :)
--------------------------------------------------------------
this is a simple first version of audio player
made using JUCE library
it can load and play audio files (wav,mp3)
it can also display metadata of the audio file
and supports mute and looping functionality
it's on progress for more features
and a better GUI
stay tuned !
--------------------------------------------------------------
*/

#include <JuceHeader.h>
#include "MainComponent.h"

// Our application class
class SimpleAudioPlayer : public juce::JUCEApplication
{
public:
    const juce::String getApplicationName() override { return "Simple Audio Player"; }
    const juce::String getApplicationVersion() override { return "1.0"; }

    void initialise(const juce::String&) override
    {
        // Create and show the main window
        mainWindow = std::make_unique<MainWindow>(getApplicationName());
    }

    void shutdown() override
    {
        mainWindow = nullptr; // Clean up
    }

private:
    // The main window of the app
    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow(juce::String name)
            : DocumentWindow(name,
                juce::Colours::darkviolet,
                DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(new MainComponent(), true); // MainComponent = our UI + logic
            centreWithSize(600, 300);
            setVisible(true);
        }

        void closeButtonPressed() override
        {
            juce::JUCEApplication::getInstance()->systemRequestedQuit();
        }
    };

    std::unique_ptr<MainWindow> mainWindow;
};

// This macro starts the app
START_JUCE_APPLICATION(SimpleAudioPlayer)
