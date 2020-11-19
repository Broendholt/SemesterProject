/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <JuceHeader.h>
using namespace juce;

//==============================================================================
/**
*/



class NewProjectAudioProcessorEditor : public juce::AudioProcessorEditor,
    public juce::MidiInputCallback,
    public juce::MidiKeyboardStateListener
{
public:

    class HighlightableKeyboard : public juce::MidiKeyboardComponent {
    public:

        HighlightableKeyboard(MidiKeyboardState& state, Orientation orientation) : juce::MidiKeyboardComponent(state, orientation) {};

        enum ColourIds
        {
            whiteNoteColourId = 0x1005000,
            blackNoteColourId = 0x1005001,
            keySeparatorLineColourId = 0x1005002,
            mouseOverKeyOverlayColourId = 0x1005003,  /**< This colour will be overlaid on the normal note colour. */
            keyDownOverlayColourId = 0x1005004,  /**< This colour will be overlaid on the normal note colour. */
            textLabelColourId = 0x1005005,
            upDownButtonBackgroundColourId = 0x1005006,
            upDownButtonArrowColourId = 0x1005007,
            shadowColourId = 0x1005008,

            //NOT ORIGINAL EDIT# set colour id for single notes
            highlightColourId = 0x1005009,
            chordsColourId = 0x1005011,
            scalesColourId = 0x1005012,
        };


        void drawWhiteNote(int midiNoteNumber,
            Graphics& g, Rectangle<float> area,
            bool isDown, bool isOver,
            Colour lineColour, Colour textColour) override {

            auto c = Colours::transparentWhite;

            

            if (showChordsVisually) {
                for (int i = 0; i < sizeof(highlightedChords) / sizeof(highlightedChords[0]); i++) {

                    if (highlightedChords[i] == midiNoteNumber) {

                        c = findColour(chordsColourId);

                        repaintNote(midiNoteNumber);
                        break;
                    }
                }
            }
            else if (showScalesVisually) {
                
                for (int k = 0; k < sizeof(scales) / sizeof(scales[0]); k++) {

                    if (midiNoteNumber == (scaleRootNote + scales[k] + 60)){
                                
                        c = juce::Colours::green;

                        repaintNote(midiNoteNumber);
                        break;
                    }
                }
            }

            if (isDown) {
                c = c.overlaidWith(findColour(keyDownOverlayColourId));
            }
            else if (isDown || isOver)
            {
                c = c.overlaidWith(findColour(mouseOverKeyOverlayColourId));
            }


                g.setColour(c);
                g.fillRect(area);

                auto text = getWhiteNoteText(midiNoteNumber);

                if (text.isNotEmpty())
                {
                    auto fontHeight = jmin(12.0f, getKeyWidth() * 0.9f);

                    g.setColour(textColour);
                    g.setFont(Font(fontHeight).withHorizontalScale(0.8f));

                    switch (getOrientation())
                    {
                    case horizontalKeyboard:            g.drawText(text, area.withTrimmedLeft(1.0f).withTrimmedBottom(2.0f), Justification::centredBottom, false); break;
                    case verticalKeyboardFacingLeft:    g.drawText(text, area.reduced(2.0f), Justification::centredLeft, false); break;
                    case verticalKeyboardFacingRight:   g.drawText(text, area.reduced(2.0f), Justification::centredRight, false); break;
                    default: break;
                    }
                }

                if (!lineColour.isTransparent())
                {
                    g.setColour(lineColour);

                    switch (getOrientation())
                    {
                    case horizontalKeyboard:            g.fillRect(area.withWidth(1.0f)); break;
                    case verticalKeyboardFacingLeft:    g.fillRect(area.withHeight(1.0f)); break;
                    case verticalKeyboardFacingRight:   g.fillRect(area.removeFromBottom(1.0f)); break;
                    default: break;
                    }

                    if (midiNoteNumber == getRangeEnd())
                    {
                        switch (getOrientation())
                        {
                        case horizontalKeyboard:            g.fillRect(area.expanded(1.0f, 0).removeFromRight(1.0f)); break;
                        case verticalKeyboardFacingLeft:    g.fillRect(area.expanded(0, 1.0f).removeFromBottom(1.0f)); break;
                        case verticalKeyboardFacingRight:   g.fillRect(area.expanded(0, 1.0f).removeFromTop(1.0f)); break;
                        default: break;
                        }
                    }
                }
            };

            void drawBlackNote(int midiNoteNumber,
                Graphics& g, Rectangle<float> area,
                bool isDown, bool isOver,
                Colour noteFillColour) override{
                
                auto c = noteFillColour;

                
                
                if (showChordsVisually) {
                    for (int i = 0; i < sizeof(highlightedChords) / sizeof(highlightedChords[0]); i++) {

                        if (highlightedChords[i] == midiNoteNumber) {

                            c = findColour(chordsColourId);

                            repaintNote(midiNoteNumber);
                            break;
                        }
                    }
                }
                else if (showScalesVisually) {

                    for (int k = 0; k < sizeof(scales) / sizeof(scales[0]); k++) {
                        int test = (scaleRootNote + scales[k] + 60);
                        if (midiNoteNumber == test) {

                            c = juce::Colours::green;

                            repaintNote(midiNoteNumber);
                            break;
                        }
                    }
                }

                if (isDown) {
                    c = c.overlaidWith(findColour(keyDownOverlayColourId));
                }
                else if (isOver) {
                    c = c.overlaidWith(findColour(mouseOverKeyOverlayColourId));
                }
                
                g.setColour(c);
                g.fillRect(area);

                if (isDown)
                {
                    g.setColour(noteFillColour);
                    g.drawRect(area);
                }
                else
                {
                    g.setColour(c.brighter());
                    auto sideIndent = 1.0f / 8.0f;
                    auto topIndent = 7.0f / 8.0f;
                    auto w = area.getWidth();
                    auto h = area.getHeight();

                    switch (getOrientation())
                    {
                    case horizontalKeyboard:            g.fillRect(area.reduced(w * sideIndent, 0).removeFromTop(h * topIndent)); break;
                    case verticalKeyboardFacingLeft:    g.fillRect(area.reduced(0, h * sideIndent).removeFromRight(w * topIndent)); break;
                    case verticalKeyboardFacingRight:   g.fillRect(area.reduced(0, h * sideIndent).removeFromLeft(w * topIndent)); break;
                    default: break;
                    }
                }
            };


            void setHighlightedChords(int chords[6]) {
                for (int i = 0; i < 6; i++) {
                    highlightedChords[i] = chords[i]; 
                }
            }

            void showChords(bool show) {
                showChordsVisually = show;
            }

            void setScales(int notes[9]) {
                for (int i = 0; i < 9; i++) {
                    scales[i] = notes[i];
                }
            }

            void showScales(bool show) {
                showScalesVisually = show;
            }

            void setScaleRootNote(int note) {
                scaleRootNote = note;
            }

        private:

            void repaintNote(int noteNum)
            {
                if (noteNum >= getRangeStart() && noteNum <= getRangeEnd())
                    repaint(getRectangleForKey(noteNum).getSmallestIntegerContainer());
            }

            int highlightedChords[6] = { -1, -1, -1, -1, -1, -1 };
            int scales[9] = { -1, -1, -1, -1, -1, -1, -1, -1, -1 };
            bool showChordsVisually = false;
            bool showScalesVisually = false;

            int scaleRootNote = 0;
    };

    NewProjectAudioProcessorEditor (NewProjectAudioProcessor&);
    ~NewProjectAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void handleNoteOn(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float /*velocity*/) override;
    void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;
    void chordButtonsPressed(juce::TextButton* button, int chordIndex);
    void showChord(juce::TextButton* button);
    
    void noteController(int chordIndex);
    void NewProjectAudioProcessorEditor::scaleRootNoteSelected(juce::ComboBox* comboBox);
    void NewProjectAudioProcessorEditor::scaleButtonPressed(juce::TextButton* button, int scaleIndex);

    void graphicsController();

    juce::MidiKeyboardState keyboardState;
    HighlightableKeyboard highlightableKeyboard;

    bool showChords = false;
    bool showScales = false;

    int hKeys[25];
    int hScale[9] = { -1, -1, -1, -1, -1, -1, -1, -1, -1 };
    int hChord[6] = { 0, -1, -1, -1, -1, -1 };
    int pressedChord[6] = { 0, -1, -1, -1, -1, -1 };
    
    const int major[6] = { 0, 4, 7, -1, -1, -1 };
    const int minor[6] = { 0, 3, 7, -1, -1, -1 };
    const int diminished[6] = { 0, 3, 6, -1, -1, -1 };
    const int augmented[6] = { 0, 4, 8, -1, -1, -1 };
    const int sus2[6] = { 0, 2, 7, -1, -1, -1 };
    const int sus4[6] = { 0, 5, 7, -1, -1, -1 };
    const int perfect4h[6] = { 0, 5, -1, -1, -1, -1 };
    const int perfect5h[6] = { 0, 4, -1, -1, -1, -1 };
    const int major7th[6] = { 0, 4, 7, 11, -1, -1 };
    const int minor7th[6] = { 0, 3, 7, 10, -1, -1 };
    const int major9th[6] = { 0, 4, 7, 11, 14, -1 };
    const int minor9th[6] = { 0, 3, 7, 10, 2, -1 };
    const int major11th[6] = { 0, 4, 7, 11, 14, 17 };
    const int minor11th[6] = { 0, 3, 7, 10, 14, 17 };
    
    const int noChord[6] = { 0, -1, -1, -1, -1, -1 };
    
    const int majorScale[9] = {0, 2, 4, 5, 7, 9, 11, 12, 0};
    const int minorScale[9] = { 0, 3, 4, 5, 7, 9, 10, 12, 0 };
    const int diminishedScale[9] = { 0, 2, 3, 5, 6, 8, 9, 11, 12 };
    const int augmentedScale[9] = { 0, 3, 4, 7, 8, 11, 12, 0, 0 };

    // ===== MIDI STUFF =====
    juce::AudioDeviceManager deviceManager;           // [1]
    juce::ComboBox midiInputList;                     // [2]
    
    int lastInputIndex = 0;                           // [3]
    bool isAddingFromMidiInput = false;               // [4]
    
    //Sets the midi input to be read from the connected midi keyboard.
    //If two keyboard are connected, it takes the first one it finds. 
    void setMidiInput(int index)
    {
        auto list = juce::MidiInput::getAvailableDevices();

        deviceManager.removeMidiInputDeviceCallback(list[lastInputIndex].identifier, this);

        auto newInput = list[index];

        if (!deviceManager.isMidiInputDeviceEnabled(newInput.identifier))
            deviceManager.setMidiInputDeviceEnabled(newInput.identifier, true);

        deviceManager.addMidiInputDeviceCallback(newInput.identifier, this);
        midiInputList.setSelectedId(index + 1, juce::dontSendNotification);

        lastInputIndex = index;
    }

private:
    juce::Label ChordsTitleLabel;
    juce::Label ScalesTitleLabel;
    juce::Label ArpeggioTitleLabel;

    juce::TextButton MajorChordBtn;
    juce::TextButton MinorChordBtn;
    juce::TextButton DiminishedChordBtn;
    juce::TextButton AugmentedChordBtn;
    juce::TextButton Sus2ChordBtn;
    juce::TextButton Sus4ChordBtn;
    juce::TextButton Perfect4ThChordBtn;
    juce::TextButton Perfect5ThChordBtn;
    juce::TextButton Major7ThChordBtn;
    juce::TextButton Minor7ThChordBtn;
    juce::TextButton Major9ThChordBtn;
    juce::TextButton Minor9ThChordBtn;
    juce::TextButton Major11ThChordBtn;
    juce::TextButton Minor11ThChordBtn;

    juce::TextButton showChordsOnKeys;

    juce::TextButton MajorScale;
    juce::TextButton MinorScale;
    juce::TextButton DiminishedScale;
    juce::TextButton AugmentedScale;
    
    juce::ComboBox rootNote;


    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NewProjectAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessorEditor)
};
