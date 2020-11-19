/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

int selectedScale = -1;
int selectedChord = -1;

//==============================================================================
NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor (NewProjectAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), highlightableKeyboard (keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
    
{
    setSize (1000, 600);

    addAndMakeVisible(highlightableKeyboard);
    keyboardState.addListener(this);
    highlightableKeyboard.setAvailableRange(60, 84);
    

    highlightableKeyboard.setColour(highlightableKeyboard.highlightColourId, juce::Colours::red);
    highlightableKeyboard.setColour(highlightableKeyboard.chordsColourId, juce::Colours::darkgrey);
    highlightableKeyboard.setColour(highlightableKeyboard.scalesColourId, juce::Colours::green);
    highlightableKeyboard.setColour(highlightableKeyboard.whiteNoteColourId, juce::Colours::white);
    highlightableKeyboard.setColour(highlightableKeyboard.keyDownOverlayColourId, juce::Colours::grey);
    highlightableKeyboard.setColour(highlightableKeyboard.mouseOverKeyOverlayColourId, juce::Colours::darkgrey);

    // ===== MIDI STUFF =====
    auto midiInputs = juce::MidiInput::getAvailableDevices();
    juce::StringArray midiInputNames;

    
    // find the first enabled device and use that by default
    for (auto input : midiInputs)
    {
        if (deviceManager.isMidiInputDeviceEnabled(input.identifier))
        {
            setMidiInput(midiInputs.indexOf(input));
            break;
        }
    }

    if (midiInputList.getSelectedId() == 0) {
        setMidiInput(0);
    } 
    
    
    
#pragma region AddAndMakeVisible Buttons


    // ------------------------------- LAYOUTS -------------------------------
    addAndMakeVisible(ChordsTitleLabel);
    ChordsTitleLabel.setText("Chords", juce::NotificationType::dontSendNotification);
    ChordsTitleLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(ScalesTitleLabel);
    ScalesTitleLabel.setText("Scales", juce::NotificationType::dontSendNotification);
    ScalesTitleLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(ArpeggioTitleLabel);
    ArpeggioTitleLabel.setText("Arpeggio", juce::NotificationType::dontSendNotification);
    ArpeggioTitleLabel.setJustificationType(juce::Justification::centred);

    // ------------------------------- BUTTONS -------------------------------
    addAndMakeVisible(MajorChordBtn);
    MajorChordBtn.setButtonText("Major");
    MajorChordBtn.onClick = [this] {chordButtonsPressed(&MajorChordBtn, 1);  };
    MajorChordBtn.setColour(MajorChordBtn.buttonColourId, juce::Colours::transparentWhite);

    addAndMakeVisible(MinorChordBtn);
    MinorChordBtn.setButtonText("Minor");
    MinorChordBtn.onClick = [this] {chordButtonsPressed(&MinorChordBtn, 2);  };
    MinorChordBtn.setColour(MinorChordBtn.buttonColourId, juce::Colours::transparentWhite);

    addAndMakeVisible(DiminishedChordBtn);
    DiminishedChordBtn.setButtonText("Diminished");
    DiminishedChordBtn.onClick = [this] {chordButtonsPressed(&DiminishedChordBtn, 3);  };
    DiminishedChordBtn.setColour(DiminishedChordBtn.buttonColourId, juce::Colours::transparentWhite);

    addAndMakeVisible(AugmentedChordBtn);
    AugmentedChordBtn.setButtonText("Augmented");
    AugmentedChordBtn.onClick = [this] {chordButtonsPressed(&AugmentedChordBtn, 4);  };
    AugmentedChordBtn.setColour(AugmentedChordBtn.buttonColourId, juce::Colours::transparentWhite);

    addAndMakeVisible(Sus2ChordBtn);
    Sus2ChordBtn.setButtonText("Sus2");
    Sus2ChordBtn.onClick = [this] {chordButtonsPressed(&Sus2ChordBtn, 5);  };
    Sus2ChordBtn.setColour(Sus2ChordBtn.buttonColourId, juce::Colours::transparentWhite);

    addAndMakeVisible(Sus4ChordBtn);
    Sus4ChordBtn.setButtonText("Sus4");
    Sus4ChordBtn.onClick = [this] {chordButtonsPressed(&Sus4ChordBtn, 6);  };
    Sus4ChordBtn.setColour(Sus4ChordBtn.buttonColourId, juce::Colours::transparentWhite);

    addAndMakeVisible(Perfect4ThChordBtn);
    Perfect4ThChordBtn.setButtonText("Perfect 4th");
    Perfect4ThChordBtn.onClick = [this] {chordButtonsPressed(&Perfect4ThChordBtn, 7);  };
    Perfect4ThChordBtn.setColour(Perfect4ThChordBtn.buttonColourId, juce::Colours::transparentWhite);

    addAndMakeVisible(Perfect5ThChordBtn);
    Perfect5ThChordBtn.setButtonText("Perfect 5th");
    Perfect5ThChordBtn.onClick = [this] {chordButtonsPressed(&Perfect5ThChordBtn, 8);  };
    Perfect5ThChordBtn.setColour(Perfect5ThChordBtn.buttonColourId, juce::Colours::transparentWhite);

    addAndMakeVisible(Major7ThChordBtn);
    Major7ThChordBtn.setButtonText("Major 7th");
    Major7ThChordBtn.onClick = [this] {chordButtonsPressed(&Major7ThChordBtn, 9);  };
    Major7ThChordBtn.setColour(Major7ThChordBtn.buttonColourId, juce::Colours::transparentWhite);

    addAndMakeVisible(Minor7ThChordBtn);
    Minor7ThChordBtn.setButtonText("Minor 7th");
    Minor7ThChordBtn.onClick = [this] {chordButtonsPressed(&Minor7ThChordBtn, 10);  };
    Minor7ThChordBtn.setColour(Minor7ThChordBtn.buttonColourId, juce::Colours::transparentWhite);

    addAndMakeVisible(Major9ThChordBtn);
    Major9ThChordBtn.setButtonText("Major 9th");
    Major9ThChordBtn.onClick = [this] {chordButtonsPressed(&Major9ThChordBtn, 11);  };
    Major9ThChordBtn.setColour(Major9ThChordBtn.buttonColourId, juce::Colours::transparentWhite);

    addAndMakeVisible(Minor9ThChordBtn);
    Minor9ThChordBtn.setButtonText("Minor 9th");
    Minor9ThChordBtn.onClick = [this] {chordButtonsPressed(&Minor9ThChordBtn, 12);  };
    Minor9ThChordBtn.setColour(Minor9ThChordBtn.buttonColourId, juce::Colours::transparentWhite);

    addAndMakeVisible(Major11ThChordBtn);
    Major11ThChordBtn.setButtonText("Major 11th");
    Major11ThChordBtn.onClick = [this] {chordButtonsPressed(&Major11ThChordBtn, 13);  };
    Major11ThChordBtn.setColour(Major11ThChordBtn.buttonColourId, juce::Colours::transparentWhite);

    addAndMakeVisible(Minor11ThChordBtn);
    Minor11ThChordBtn.setButtonText("Minor 11th");
    Minor11ThChordBtn.onClick = [this] {chordButtonsPressed(&Minor11ThChordBtn, 14);  };
    Minor11ThChordBtn.setColour(Minor11ThChordBtn.buttonColourId, juce::Colours::transparentWhite);

    addAndMakeVisible(showChordsOnKeys);
    showChordsOnKeys.setButtonText("Show Choords");
    showChordsOnKeys.onClick = [this] {showChord(&showChordsOnKeys);  };
    showChordsOnKeys.setColour(showChordsOnKeys.buttonColourId, juce::Colours::transparentWhite);
    

    /*-------------------------------------------- SCALES --------------------------------------------*/

    addAndMakeVisible(MajorScale);
    MajorScale.setButtonText("Major");
    MajorScale.onClick = [this] {scaleButtonPressed(&MajorScale, 1);  };
    MajorScale.setColour(showChordsOnKeys.buttonColourId, juce::Colours::transparentWhite);

    addAndMakeVisible(MinorScale);
    MinorScale.setButtonText("Minor");
    MinorScale.onClick = [this] {scaleButtonPressed(&MinorScale, 2);  };
    MinorScale.setColour(MinorScale.buttonColourId, juce::Colours::transparentWhite);

    addAndMakeVisible(DiminishedScale);
    DiminishedScale.setButtonText("Diminished");
    DiminishedScale.onClick = [this] {scaleButtonPressed(&DiminishedScale, 3);  };
    DiminishedScale.setColour(DiminishedScale.buttonColourId, juce::Colours::transparentWhite);
    
    addAndMakeVisible(AugmentedScale);
    AugmentedScale.setButtonText("Augmented");
    AugmentedScale.onClick = [this] {scaleButtonPressed(&AugmentedScale, 4);  };
    AugmentedScale.setColour(AugmentedScale.buttonColourId, juce::Colours::transparentWhite);


    addAndMakeVisible(rootNote);
    rootNote.addItem("C", 1);
    rootNote.addItem("C#", 2);
    rootNote.addItem("D", 3);
    rootNote.addItem("D#", 4);
    rootNote.addItem("E", 5);
    rootNote.addItem("F", 6);
    rootNote.addItem("F#", 7);
    rootNote.addItem("G", 8);
    rootNote.addItem("G#", 9);
    rootNote.addItem("A", 10);
    rootNote.addItem("A#", 11);
    rootNote.addItem("B", 12);

    rootNote.onChange = [this] {scaleRootNoteSelected(&rootNote);  };

#pragma endregion    
    
}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
    keyboardState.removeListener(this);
}

//==============================================================================
void NewProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void NewProjectAudioProcessorEditor::resized()
{
    int componentWidth = getWidth();
    int componentHeight = getHeight();

    int margin = 10;

    int sectionWidth = componentWidth / 3;
    int headerHeight = 40;
    
    int chordButtonX = margin;
    int chordButtonY = headerHeight + margin;
    int chordButtonWidth = (sectionWidth / 2) - (margin * 1.5);
    int chordButtonHeight = getHeight() / 20;
    
#pragma region SetBounds

    // ------------------------------- LAYOUT -------------------------------
    ChordsTitleLabel.setBounds((sectionWidth) * 0, 0, sectionWidth, 30);
    ScalesTitleLabel.setBounds((sectionWidth) * 1, 0, sectionWidth, 30);
    ArpeggioTitleLabel.setBounds((sectionWidth) * 2, 0, sectionWidth, 30);

    // ------------------------------- BUTTONS -------------------------------
    MajorChordBtn.setBounds(chordButtonX, chordButtonY, chordButtonWidth, chordButtonHeight);
    MinorChordBtn.setBounds(chordButtonX + sectionWidth - margin - chordButtonWidth, chordButtonY, chordButtonWidth, chordButtonHeight);
    
    DiminishedChordBtn.setBounds(chordButtonX, chordButtonY + margin + chordButtonHeight, chordButtonWidth, chordButtonHeight);
    AugmentedChordBtn.setBounds(chordButtonX + sectionWidth - margin - chordButtonWidth, chordButtonY + margin + chordButtonHeight, chordButtonWidth, chordButtonHeight);
    
    Sus2ChordBtn.setBounds(chordButtonX, chordButtonY + (margin + chordButtonHeight) * 2, chordButtonWidth, chordButtonHeight);
    Sus4ChordBtn.setBounds(chordButtonX + sectionWidth - margin - chordButtonWidth, chordButtonY + (margin + chordButtonHeight) * 2, chordButtonWidth, chordButtonHeight);

    Perfect4ThChordBtn.setBounds(chordButtonX, chordButtonY + (margin + chordButtonHeight) * 3, chordButtonWidth, chordButtonHeight);
    Perfect5ThChordBtn.setBounds(chordButtonX + sectionWidth - margin - chordButtonWidth, chordButtonY + (margin + chordButtonHeight) * 3, chordButtonWidth, chordButtonHeight);

    Major7ThChordBtn.setBounds(chordButtonX, chordButtonY + (margin + chordButtonHeight) * 4, chordButtonWidth, chordButtonHeight);
    Minor7ThChordBtn.setBounds(chordButtonX + sectionWidth - margin - chordButtonWidth, chordButtonY + (margin + chordButtonHeight) * 4, chordButtonWidth, chordButtonHeight);

    Major9ThChordBtn.setBounds(chordButtonX, chordButtonY + (margin + chordButtonHeight) * 5, chordButtonWidth, chordButtonHeight);
    Minor9ThChordBtn.setBounds(chordButtonX + sectionWidth - margin - chordButtonWidth, chordButtonY + (margin + chordButtonHeight) * 5, chordButtonWidth, chordButtonHeight);

    Major11ThChordBtn.setBounds(chordButtonX, chordButtonY + (margin + chordButtonHeight) * 6, chordButtonWidth, chordButtonHeight);
    Minor11ThChordBtn.setBounds(chordButtonX + sectionWidth - margin - chordButtonWidth, chordButtonY + (margin + chordButtonHeight) * 6, chordButtonWidth, chordButtonHeight);

    showChordsOnKeys.setBounds(chordButtonX + (chordButtonWidth / 2) + margin, chordButtonY + (margin + chordButtonHeight) * 8, chordButtonWidth, chordButtonHeight);


    MajorScale.setBounds(chordButtonX + sectionWidth, chordButtonY, chordButtonWidth, chordButtonHeight);
    MinorScale.setBounds(chordButtonX + sectionWidth - margin - chordButtonWidth + sectionWidth, chordButtonY, chordButtonWidth, chordButtonHeight);

    DiminishedScale.setBounds(chordButtonX + sectionWidth, chordButtonY + margin + chordButtonHeight, chordButtonWidth, chordButtonHeight);
    AugmentedScale.setBounds(chordButtonX + sectionWidth + sectionWidth - margin - chordButtonWidth, chordButtonY + margin + chordButtonHeight, chordButtonWidth, chordButtonHeight);

    rootNote.setBounds(chordButtonX + (chordButtonWidth / 2) + margin + sectionWidth, chordButtonY + (margin + chordButtonHeight) * 3, chordButtonWidth, chordButtonHeight);
    
    // ------------------------------- KEYBOARD -------------------------------
    highlightableKeyboard.setKeyWidth(50);
    int keyboardHeight = getHeight() / 5;
    highlightableKeyboard.setBounds((getWidth() / 2) - ((highlightableKeyboard.getKeyWidth() * 15) / 2), getHeight() - keyboardHeight, highlightableKeyboard.getKeyWidth() * 15, keyboardHeight);

#pragma endregion
}

void NewProjectAudioProcessorEditor::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message)
{
    const juce::ScopedValueSetter<bool> scopedInputFlag(isAddingFromMidiInput, true);
    keyboardState.processNextMidiEvent(message);
}

void NewProjectAudioProcessorEditor::handleNoteOn(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
    int tmp[6] = { -1, -1, -1, -1, -1, -1 };
    if (selectedChord != -1) {
        for (int i = 0; i < sizeof(hChord) / sizeof(hChord[0]); i++) {

            if (hChord[i] != -1) {
                tmp[i] = midiNoteNumber + hChord[i];
                audioProcessor.keyOn(midiNoteNumber + hChord[i], int(127 * velocity));
            }
            else {
                break;
            }
        }
    }
    else {
        audioProcessor.keyOn(midiNoteNumber, int(127 * velocity));
    }

    

    highlightableKeyboard.setHighlightedChords(tmp);
}

void NewProjectAudioProcessorEditor::handleNoteOff(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float /*velocity*/)
{
    if (selectedChord != -1) {
        std::copy(noChord, noChord + 6, pressedChord);
        for (int i = 0; i < sizeof(hChord) / sizeof(hChord[0]); i++) {
            if (hChord[i] == -1) {
                break;
            }
            else {
                audioProcessor.keyOff(midiNoteNumber + hChord[i]);
            }
        }
    }
    else {
        audioProcessor.keyOff(midiNoteNumber);
    }

    int tmp[6] = { -1, -1, -1, -1, -1, -1 };
    highlightableKeyboard.setHighlightedChords(tmp);
}

void NewProjectAudioProcessorEditor::chordButtonsPressed(juce::TextButton* button, int chordIndex)
{
    MajorChordBtn.setColour(MajorChordBtn.buttonColourId, juce::Colours::transparentBlack);
    MinorChordBtn.setColour(MinorChordBtn.buttonColourId, juce::Colours::transparentWhite);
    DiminishedChordBtn.setColour(DiminishedChordBtn.buttonColourId, juce::Colours::transparentWhite);
    AugmentedChordBtn.setColour(AugmentedChordBtn.buttonColourId, juce::Colours::transparentWhite);
    Sus2ChordBtn.setColour(Sus2ChordBtn.buttonColourId, juce::Colours::transparentWhite);
    Sus4ChordBtn.setColour(Sus4ChordBtn.buttonColourId, juce::Colours::transparentWhite);
    Perfect4ThChordBtn.setColour(Perfect4ThChordBtn.buttonColourId, juce::Colours::transparentWhite);
    Perfect5ThChordBtn.setColour(Perfect5ThChordBtn.buttonColourId, juce::Colours::transparentWhite);
    Major7ThChordBtn.setColour(Major7ThChordBtn.buttonColourId, juce::Colours::transparentWhite);
    Minor7ThChordBtn.setColour(Minor7ThChordBtn.buttonColourId, juce::Colours::transparentWhite);
    Major9ThChordBtn.setColour(Major9ThChordBtn.buttonColourId, juce::Colours::transparentWhite);
    Minor9ThChordBtn.setColour(Minor9ThChordBtn.buttonColourId, juce::Colours::transparentWhite);
    Major11ThChordBtn.setColour(Major11ThChordBtn.buttonColourId, juce::Colours::transparentWhite);
    Minor11ThChordBtn.setColour(Minor11ThChordBtn.buttonColourId, juce::Colours::transparentWhite);
    
    
    if (selectedChord == chordIndex) {
        button->setColour(button->buttonColourId, juce::Colours::transparentWhite);
        selectedChord = -1;
        noteController(-1);
    }
    else {
        button->setColour(button->buttonColourId, juce::Colours::orange);
        selectedChord = chordIndex;
        noteController(chordIndex);
    }
    
    
}

void NewProjectAudioProcessorEditor::showChord(juce::TextButton* button) {
    
    if (showChords) {
        showChords = false;
        showChordsOnKeys.setColour(showChordsOnKeys.buttonColourId, juce::Colours::transparentWhite);
    }
    else if(!showChords){
        showChords = true;
        showChordsOnKeys.setColour(showChordsOnKeys.buttonColourId, juce::Colours::orange);
    }

    highlightableKeyboard.showChords(showChords);
    
}

void NewProjectAudioProcessorEditor::noteController(int chordIndex) {
    //Controls the keyOn and keyOff. 

    switch (chordIndex) {
    case 1: // -- Major Chords
        std::copy(major, major + 6, hChord);
        break;
    case 2: // -- Minor Chords
        std::copy(minor, minor + 6, hChord);
        break;
    case 3: // Diminished Chord
        std::copy(diminished, diminished + 6, hChord);
        break;
    case 4: // Augmented Chord
        std::copy(augmented, augmented + 6, hChord);
        break;
    case 5: // Sus2 Chord
        std::copy(sus2, sus2 + 6, hChord);
        break;
    case 6: // Sus4 Chord
        std::copy(sus4, sus4 + 6, hChord);
        break;
    case 7: // Perfect 4th Chord
        std::copy(perfect4h, perfect4h + 6, hChord);
        break;
    case 8: // Perfect 5th Chord
        std::copy(perfect5h, perfect5h + 6, hChord);
        break;
    case 9: // Major 7th
        std::copy(major7th, major7th + 6, hChord);
        break;
    case 10: // Minor 7th
        std::copy(minor7th, minor7th + 6, hChord);
        break;
    case 11: // Major 9th
        std::copy(major9th, major9th + 6, hChord);
        break;
    case 12: // Minor 9th
        std::copy(minor9th, minor9th + 6, hChord);
        break;
    case 13: // Major 11th
        std::copy(major11th, major11th + 6, hChord);
        break;
    case 14: // Minor 11th
        std::copy(minor11th, minor11th + 6, hChord);
        break;
    default:
        std::copy(noChord, noChord + 6, hChord);
        break;
    }
}

void NewProjectAudioProcessorEditor::scaleButtonPressed(juce::TextButton* button, int scaleIndex) {

    if (button != nullptr) {
        MajorScale.setColour(MajorScale.buttonColourId, juce::Colours::transparentWhite);
        MinorScale.setColour(MinorScale.buttonColourId, juce::Colours::transparentWhite);
        DiminishedScale.setColour(DiminishedScale.buttonColourId, juce::Colours::transparentWhite);
        AugmentedScale.setColour(AugmentedScale.buttonColourId, juce::Colours::transparentWhite);
    }
    

    //rootNote.setSelectedId(-1, juce::NotificationType::dontSendNotification);
    
    
    if (selectedScale == scaleIndex && button != nullptr) {
        scaleIndex = -1;
        selectedScale = -1;
        if (button != nullptr) {
            button->setColour(button->buttonColourId, juce::Colours::transparentWhite);
        }
        
    }
    else {
        
        rootNote.setItemEnabled(1, true);   //C
        rootNote.setItemEnabled(2, true);  //C#
        rootNote.setItemEnabled(3, true);   //D
        rootNote.setItemEnabled(4, true);  //D#
        rootNote.setItemEnabled(5, true);   //E
        rootNote.setItemEnabled(6, true);   //F
        rootNote.setItemEnabled(7, true);  //F#
        rootNote.setItemEnabled(8, true);   //G
        rootNote.setItemEnabled(9, true);  //G#
        rootNote.setItemEnabled(10, true);  //A
        rootNote.setItemEnabled(11, true); //A#
        rootNote.setItemEnabled(12, true);  //B
        
        
        if (button != nullptr) {
            selectedScale = scaleIndex;
            button->setColour(button->buttonColourId, juce::Colours::orange);
        }
    }

    if (rootNote.getSelectedId() != 0) {
        switch (scaleIndex) {
        case 1:
            std::copy(majorScale, majorScale + 9, hScale);
            highlightableKeyboard.showScales(true);
            break;
        case 2:
            std::copy(minorScale, minorScale + 9, hScale);
            highlightableKeyboard.showScales(true);
            break;
        case 3:
            std::copy(diminishedScale, diminishedScale + 9, hScale);
            highlightableKeyboard.showScales(true);
            break;
        case 4:
            std::copy(augmentedScale, augmentedScale + 9, hScale);
            highlightableKeyboard.showScales(true);
            break;
        default:

            rootNote.setItemEnabled(1, false);   //C
            rootNote.setItemEnabled(2, false);  //C#
            rootNote.setItemEnabled(3, false);   //D
            rootNote.setItemEnabled(4, false);  //D#
            rootNote.setItemEnabled(5, false);   //E
            rootNote.setItemEnabled(6, false);   //F
            rootNote.setItemEnabled(7, false);  //F#
            rootNote.setItemEnabled(8, false);   //G
            rootNote.setItemEnabled(9, false);  //G#
            rootNote.setItemEnabled(10, false);  //A
            rootNote.setItemEnabled(11, false); //A#
            rootNote.setItemEnabled(12, false);  //B

            highlightableKeyboard.showScales(false);
            break;
        }
    }
    
    highlightableKeyboard.setScales(hScale);
    highlightableKeyboard.setVisible(false);
    highlightableKeyboard.setVisible(true);
    
}

void NewProjectAudioProcessorEditor::scaleRootNoteSelected(juce::ComboBox* comboBox) {
    highlightableKeyboard.setScaleRootNote(comboBox->getSelectedId() - 1);
    highlightableKeyboard.setScales(hScale);

    scaleButtonPressed(nullptr, selectedScale);

    highlightableKeyboard.setVisible(false);
    highlightableKeyboard.setVisible(true);
}

void NewProjectAudioProcessorEditor::graphicsController() {
    


}





