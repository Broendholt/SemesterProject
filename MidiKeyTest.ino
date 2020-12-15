#include <MIDI.h>
#define HWSERIAL Serial5
#include <FastLED.h>

MIDI_CREATE_INSTANCE(HardwareSerial, Serial3, MIDI);

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#define NUM_LEDS 25
#define DATA_PIN 39
CRGB leds[NUM_LEDS];


// GUItool: begin automatically generated code
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveform       waveform4; //xy=77,422.00000190734863
AudioSynthWaveform       waveform7; //xy=77,570.0000019073486
AudioSynthWaveform       waveform3; //xy=78,383.00000190734863
AudioSynthWaveform       waveform5; //xy=78,491.00000190734863
AudioSynthWaveform       waveform6; //xy=78,530.0000019073486
AudioSynthWaveform       waveform1;      //xy=79,304.00000190734863
AudioSynthWaveform       waveform2; //xy=79,343.00000190734863
AudioSynthWaveform       waveform8; //xy=78.00000762939453,608.0000019073486
AudioEffectEnvelope      envelope6; //xy=226.0056915283203,530.914794921875
AudioEffectEnvelope      envelope7; //xy=226.0056915283203,571.914794921875
AudioEffectEnvelope      envelope8; //xy=226.0056915283203,609.914794921875
AudioEffectEnvelope      envelope3; //xy=227.0056915283203,385.9147644042969
AudioEffectEnvelope      envelope5;  //xy=227.0056915283203,490.9147644042969
AudioEffectEnvelope      envelope2; //xy=228.00569915771484,345.9147644042969
AudioEffectEnvelope      envelope4; //xy=228.0056915283203,424.9147644042969
AudioEffectEnvelope      envelope1;      //xy=229.00569915771484,302.9147644042969
AudioMixer4              mixer1;         //xy=431,367
AudioMixer4              mixer2; //xy=432.99997329711914,549.0000152587891
AudioMixer4              mixer3; //xy=621,460
AudioOutputI2S           i2s1;           //xy=858.9999542236328,458.9999485015869
AudioConnection          patchCord1(waveform4, envelope4);
AudioConnection          patchCord2(waveform7, envelope7);
AudioConnection          patchCord3(waveform3, envelope3);
AudioConnection          patchCord4(waveform5, envelope5);
AudioConnection          patchCord5(waveform6, envelope6);
AudioConnection          patchCord6(waveform1, envelope1);
AudioConnection          patchCord7(waveform2, envelope2);
AudioConnection          patchCord8(waveform8, envelope8);
AudioConnection          patchCord9(envelope6, 0, mixer2, 1);
AudioConnection          patchCord10(envelope7, 0, mixer2, 2);
AudioConnection          patchCord11(envelope8, 0, mixer2, 3);
AudioConnection          patchCord12(envelope3, 0, mixer1, 2);
AudioConnection          patchCord13(envelope5, 0, mixer2, 0);
AudioConnection          patchCord14(envelope2, 0, mixer1, 1);
AudioConnection          patchCord15(envelope4, 0, mixer1, 3);
AudioConnection          patchCord16(envelope1, 0, mixer1, 0);
AudioConnection          patchCord17(mixer1, 0, mixer3, 0);
AudioConnection          patchCord18(mixer2, 0, mixer3, 1);
AudioConnection          patchCord19(mixer3, 0, i2s1, 0);
AudioConnection          patchCord20(mixer3, 0, i2s1, 1);
// GUItool: end automatically generated code

// GUItool: end automatically generated code

// GUItool: end automatically generated code

AudioControlSGTL5000     audioShield; 

#define volumePin 38
#define encoder1Pin 35
#define encoder2Pin 36
#define encoderSPin 37

#define NUM_LED_COLUMNS (4)
#define NUM_LED_ROWS (4)
#define NUM_BTN_COLUMNS (4)
#define NUM_BTN_ROWS (4)
#define NUM_COLORS (1)

#define MAX_DEBOUNCE (3)

static bool LED_buffer[NUM_LED_COLUMNS][NUM_LED_ROWS];

//{40, 42, 44, 46}
//{45, 43, 41, 55}

static const uint8_t btncolumnpins[NUM_BTN_COLUMNS] = {43, 44, 45, 46};
static const uint8_t btnrowpins[NUM_BTN_ROWS]       = {55, 40, 41, 42};
static const uint8_t ledcolumnpins[NUM_LED_COLUMNS] = {48, 50, 52, 54};
static const uint8_t colorpins[NUM_LED_ROWS]        = {47, 49, 51, 53};

static int8_t debounce_count[NUM_BTN_COLUMNS][NUM_BTN_ROWS];

float volumeLevel = 0;

int counter;
int aState;
int bState;
int aLastState;

bool didSelectScale = true;
bool didSelectScaleLED = false;
int scaleIndex = -1;
bool showScale;;

int majorKey [] = {0, 2, 4, 5, 7, 9, 11, 12, -1};       
int minorKey [] = {0, 2, 3, 5, 7, 8, 10, 12, -1};       
int augKey   [] = {0, 3, 4, 7, 8, 11, 12, -1, -1};      
int dimKey   [] = {0, 2, 3, 5, 6, 8, 9, 11, 12};        

int *scalesArray [] = {majorKey, minorKey, augKey, dimKey};   //Pointer array of all the scales

int rootNote;

int majChord [] = {0, 4, 7, -1, -1, -1};
int minChord [] = {0, 3, 7, -1, -1, -1};
int augChord [] = {0, 3, 6, -1, -1, -1};
int dimChord [] = {0, 4, 8, -1, -1, -1};
int sus2Chord [] = {0, 2, 7, -1, -1, -1};
int sus4Chord [] = {0, 5, 7, -1, -1, -1};
int perf5Chord [] = {0, 7, -1, -1, -1, -1};
int perf4Chord [] = {0, 5, -1, -1, -1, -1};
int maj7Chord [] = {0, 4, 7, 11, -1, -1};
int min7Chord [] = {0, 3, 7, 11, -1, -1};
int maj9Chord [] = {0, 4, 7, 11, 14, -1};
int min9Chord [] = {0, 3, 7, 10, 14, -1};
int maj11Chord [] = {0, 4, 7, 11, 14, 17};
int min11Chord [] = {0, 3, 7, 10, 14, 17};

//Pointer array for all the chords
int *chordsArray [] = {majChord, minChord, augChord, dimChord, sus2Chord, sus4Chord, perf5Chord, perf4Chord, maj7Chord, min7Chord, maj9Chord, min9Chord, maj11Chord, min11Chord};

int chordIndex = -1;

char outgoingData[8];
int notesPressed[8] = { -1, -1, -1, -1, -1, -1, -1, -1};

void startUp(){
  for(int i = 0; i < 25; i++){
    if(i != 0){
      leds[i - 1] = CRGB::Black;
      leds[i] = CRGB::Red;
    }
    else{
      leds[i] = CRGB::Red;
    }
    FastLED.show();
    delay(15);
  }

  for(int i = 24; i >= 0; i--){
    if(i != 24){
      leds[i + 1] = CRGB::Black;
      leds[i] = CRGB::Red;
    }
    else{
      leds[i] = CRGB::Red;
    }
    FastLED.show();
    delay(15);
  }
  leds[0] = CRGB::Black;
  FastLED.show();
}

void setup() {

  pinMode(32, OUTPUT);
  digitalWrite(32, HIGH);
  
  sendMsg(0, 0, 0, 'R');
  
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  FastLED.setBrightness(30);
  FastLED.show();

  AudioMemory(50);
  audioShield.enable();

  AudioNoInterrupts();

  mixer1.gain(0, 0.20);
  mixer1.gain(1, 0.20);
  mixer1.gain(2, 0.20);
  mixer1.gain(3, 0.20);

  mixer2.gain(0, 0.20);
  mixer2.gain(1, 0.20);
  mixer2.gain(2, 0.20);
  mixer2.gain(3, 0.20);

  mixer3.gain(0, 0.2);
  mixer3.gain(1, 0.2);

  envelope1.attack(10);
  envelope2.attack(10);
  envelope3.attack(10);
  envelope4.attack(10);
  envelope5.attack(10);
  envelope6.attack(10);
  envelope7.attack(10);
  envelope8.attack(10);

  envelope1.release(10);
  envelope2.release(10);
  envelope3.release(10);
  envelope4.release(10);
  envelope5.release(10);
  envelope6.release(10);
  envelope7.release(10);
  envelope8.release(10);

  waveform1.begin(0.1, 440, WAVEFORM_SQUARE);
  waveform2.begin(0.1, 440, WAVEFORM_SQUARE);
  waveform3.begin(0.1, 440, WAVEFORM_SQUARE);
  waveform4.begin(0.1, 440, WAVEFORM_SQUARE);
  waveform5.begin(0.1, 440, WAVEFORM_SQUARE);
  waveform6.begin(0.1, 440, WAVEFORM_SQUARE);
  waveform7.begin(0.1, 440, WAVEFORM_SQUARE);
  waveform8.begin(0.1, 440, WAVEFORM_SQUARE);
  
  audioShield.volume(0.005);

  AudioInterrupts();
  
  MIDI.begin(MIDI_CHANNEL_OMNI);
  HWSERIAL.begin(57600);
  
  Serial.begin(57600);
  Serial.println("MIDI Input Test");

  setuppins();

  // init global variables
  for (uint8_t i = 0; i < NUM_LED_COLUMNS; i++)
  {
    for (uint8_t j = 0; j < NUM_LED_ROWS; j++)
    {
      LED_buffer[i][j] = 0;
    }
  }
  
  pinMode(volumePin, INPUT);
  pinMode(encoder1Pin, INPUT);
  pinMode(encoder2Pin, INPUT);
  pinMode(encoderSPin, INPUT);
  
  aLastState = digitalRead(encoder1Pin);

  startUp(); 
  digitalWrite(32, LOW);
}

unsigned long counter1 = 0;
unsigned long prevCounter1 = 0;

unsigned long counter2 = 0;
unsigned long prevCounter2 = 0;
bool rootNoteLedState = false;

void loop() {
  int type, note, velocity, channel, d1, d2;
  
  if (MIDI.read()) {                    // Is there a MIDI message incoming ?
    byte type = MIDI.getType();
    switch (type) {
      case midi::NoteOn:
        note = MIDI.getData1();
        velocity = MIDI.getData2();
        channel = MIDI.getChannel();
        if (velocity > 0) {
          Serial.println(String("Note On:  ch=") + channel + ", note=" + note + ", velocity=" + velocity);
          sendNote(note, 1);
          addPressedNote(note);

          if(newNoteIndex <= 24 && newNoteIndex >= 0){
            if(chordIndex != -1){
              leds[newNoteIndex] = CRGB::Green;    
              for(int i = 1; i < 6; i++){
                if(chordsArray[chordIndex][i] != -1){
                  if(newNoteIndex + chordsArray[chordIndex][i] <= 24){
                    leds[newNoteIndex + chordsArray[chordIndex][i]] = CRGB::Blue;        
                  }
                }
              }
            }
            else{
              leds[newNoteIndex] = CRGB::Green;
              FastLED.show();  
            }
            
          }
          
          
        } else {
          Serial.println(String("Note Off: ch=") + channel + ", note=" + note);
        }
        delay(30);
        break;
      case midi::NoteOff:
        note = MIDI.getData1();
        velocity = MIDI.getData2();
        channel = MIDI.getChannel();
        Serial.println(String("Note Off: ch=") + channel + ", note=" + note + ", velocity=" + velocity);
        sendNote(note, 0);
        removePressedNote(note);

        byte newNoteIndex = note - 48;
        if(newNoteIndex <= 24 && newNoteIndex >= 0){
          if(chordIndex != -1){
              for(int i = 0; i < 6; i++){
                if(chordsArray[chordIndex][i] != -1){
                  if(newNoteIndex + chordsArray[chordIndex][i] <= 24){
                    leds[newNoteIndex + chordsArray[chordIndex][i]] = CRGB::Black;  
                  }    
                }
              }
            }
            else{
              leds[newNoteIndex] = CRGB::Black;
              FastLED.show();  
            }
        }
          
        
        delay(30);
        break;
      default:
        d1 = MIDI.getData1();
        d2 = MIDI.getData2();
        break;
    }
    playNotes();
  }
  
  readEncoder();
  readVolume();
  scan();
 
  audioShield.volume(0.005);
  delay(1);

  
  
  if(!didSelectScale){
    if(counter1 > 500){
      didSelectScaleLED = !didSelectScaleLED;

      prevCounter1 = millis();
    }

    LED_buffer[0][3] = !didSelectScaleLED;
    LED_buffer[1][3] = !didSelectScaleLED;
    LED_buffer[2][3] = !didSelectScaleLED;
    LED_buffer[3][3] = !didSelectScaleLED;
  }

  counter1 = millis() - prevCounter1;
  counter2 = millis() - prevCounter2;

  if(scaleIndex != -1){

    for(int i = 1; i < 8; i++){
      if(scalesArray[scaleIndex][i] != -1){
        leds[rootNote + scalesArray[scaleIndex][i]] = CRGB::Blue;  
      }
    }
    
    if(counter2 > 500){

      if(rootNoteLedState){
        leds[rootNote] = CRGB::Black;
        rootNoteLedState = !rootNoteLedState;
      }
      else{
        leds[rootNote] = CRGB::Blue;  
        rootNoteLedState = !rootNoteLedState;
      }

      
      
      FastLED.show();
      prevCounter2 = millis();

    }
  }
}

float midiToHz(float note){
  return (float)(440.0 * powf(2.0, (float)(note - 69) * 0.08333333));
}

void sendNote(int noteNumber, int state){
  outgoingData[0] = 'N';
  outgoingData[1] = '0';
  outgoingData[2] = '0';

  int i1 = noteNumber / 100;
  int i2 = ((noteNumber % 100) / 10);
  int i3 = noteNumber % 10;

  char n1 = i1 + 48;
  char n2 = i2 + 48;
  char n3 = i3 + 48;
  
  outgoingData[3] = n1;
  outgoingData[4] = n2;
  outgoingData[5] = n3;
  
  char _state = '0';
  if(state == 1){ _state = '1'; }
  
  outgoingData[6] = _state; 
  outgoingData[7] = '\0'; 
  
  Serial.println(outgoingData);
  HWSERIAL.print(outgoingData);
}

void chooseOption(bool state){
  int type = -1;

  FastLED.clear();
  FastLED.show();

  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      if(LED_buffer[i][j] == 1){
        type = (i * NUM_BTN_ROWS) + j;
      }
    } 
  }

  Serial.print("STATE: ");
  Serial.print(state);
  Serial.print(" ROOTNOTE: ");
  Serial.println(rootNote);

  switch(type){
    case 0:       //major11th
      Serial.println("MAJOR 11TH");
      sendMsg(rootNote, 12, state, 'C');
      chordIndex = 12;
      scaleIndex = -1;
      didSelectScale = true;
      break;
    case 1:       //major7th
      Serial.println("MAJOR 7TH");
      sendMsg(rootNote, 8, state, 'C');
      chordIndex = 8;
      scaleIndex = -1;
      didSelectScale = true;
      break;
    case 2:       //suspended 2
      Serial.println("SUSPENDED 2");
      sendMsg(rootNote, 4, state, 'C');
      chordIndex = 4;
      scaleIndex = -1;
      didSelectScale = true;
      break;
    case 3:       //major
      Serial.println("MAJOR");
      if(!didSelectScale){
        scaleIndex = 0;
        didSelectScale = true;
        sendMsg(rootNote, 0, state, 'S');
        for(int i = 1; i < 8; i++){
          leds[rootNote + scalesArray[scaleIndex][i]] = CRGB::Blue;
        }
        FastLED.show();
      }else{
        chordIndex = 0;  
        scaleIndex = -1;
        sendMsg(rootNote, 0, state, 'C');
      }
      
      break;
    case 4:       //minor11th
      Serial.println("MINOR 11TH");
      sendMsg(rootNote, 13, state, 'C');
      chordIndex = 13;
      scaleIndex = -1;
      didSelectScale = true;
      break;
    case 5:       //minpr7th
      Serial.println("MINOR 7TH");
      sendMsg(rootNote, 9, state, 'C');
      chordIndex = 9;
      scaleIndex = -1;
      didSelectScale = true;
      break;
    case 6:       //suspended 4
      Serial.println("SUSPENDED 4");
      sendMsg(rootNote, 5, state, 'C');
      chordIndex = 5;
      scaleIndex = -1;
      didSelectScale = true;
      break;
    case 7:       //minor
      Serial.println("MINOR");
      if(!didSelectScale){
        scaleIndex = 1;
        didSelectScale = true;
        sendMsg(1, 1, state, 'S');
        for(int i = 1; i < 8; i++){
          leds[rootNote + scalesArray[scaleIndex][i]] = CRGB::Blue;
        }
        FastLED.show();
      }else{
        chordIndex = 1;  
        scaleIndex = -1;
        sendMsg(rootNote, 1, state, 'C');
      }
      break;
    case 8:       //scale
      Serial.println("SCALE");
      chordIndex = -1;
      if(state == 1){
        didSelectScale = false;  
      }
      else{
        didSelectScale = true;
      }
      
      break;
    case 9:       //major9th
      Serial.println("MAJOR 9TH");
      sendMsg(rootNote, 10, state, 'C');
      chordIndex = 10;
      scaleIndex = -1;
      didSelectScale = true;
      break;
    case 10:       //sperfect 4th
      Serial.println("PERFECT 4TH");
      sendMsg(rootNote, 7, state, 'C');
      chordIndex = 7;
      scaleIndex = -1;
      didSelectScale = true;
      break;
    case 11:       //diminished
      Serial.println("DIMINISHED");
      if(!didSelectScale){
        scaleIndex = 3;
        didSelectScale = true;
        sendMsg(rootNote, 3, state, 'S');
        for(int i = 1; i < 8; i++){
          leds[rootNote + scalesArray[scaleIndex][i]] = CRGB::Blue;
        }
        FastLED.show();
      }else{
        chordIndex = 3;
        scaleIndex = -1;  
        sendMsg(rootNote, 3, state, 'C');
      }
      break;
    case 12:       //appeggio
      Serial.println("APPEGIO");
      sendMsg(0, 0, 0, 'R');
      didSelectScale = true;
      break;
    case 13:       //minor 9th
      Serial.println("MINOR 9TH");
      sendMsg(rootNote, 11, state, 'C');
      chordIndex = 11;
      scaleIndex = -1;
      didSelectScale = true;
      break;
    case 14:       //perfect 5th
      Serial.println("PERFECT 5th");
      sendMsg(rootNote, 6, state, 'C');
      chordIndex = 6;
      scaleIndex = -1;
      didSelectScale = true;
      break;
    case 15:       //augmented
      Serial.println("AUGMENTED");
      if(!didSelectScale){
        scaleIndex = 2;
        didSelectScale = true;
        sendMsg(rootNote, 2, state, 'S');
        for(int i = 1; i < 8; i++){
          leds[rootNote + scalesArray[scaleIndex][i]] = CRGB::Blue;
        }
        FastLED.show();
      }else{
        chordIndex = 2; 
        scaleIndex = -1; 
        sendMsg(rootNote, 2, state, 'C');
      }
      break;
    
    default:
      Serial.println("UNKNOWN");
      HWSERIAL.print("R000000\0");
      chordIndex = -1;
      scaleIndex = -1;
      break;
  }

  
}

void sendMsg(int noteNumber, int index, int state, char event){
  outgoingData[0] = event;

  int o1 = index / 10;
  int o2 = index % 10;

  char p1 = o1 + 48;
  char p2 = o2 + 48;
  
  outgoingData[1] = p1;
  outgoingData[2] = p2;

  int i1 = noteNumber / 100;
  int i2 = ((noteNumber % 100) / 10);
  int i3 = noteNumber % 10;

  char n1 = i1 + 48;
  char n2 = i2 + 48;
  char n3 = i3 + 48;
  
  outgoingData[3] = n1;
  outgoingData[4] = n2;
  outgoingData[5] = n3;
  
  char _state = '0';
  if(state == 1){ _state = '1'; }
  
  outgoingData[6] = _state; 
  Serial.println(outgoingData);
  HWSERIAL.print(outgoingData);
}

void readEncoder(){
  if(analogRead(encoder1Pin) > 200){aState = 0;} else{aState = 1;}
  
  if(aState != aLastState){
    if(analogRead(encoder2Pin) > 200){bState = 0;} else{bState = 1;}
    if(bState != aLastState){
      counter++;  
      sendMsg(counter/2, 0, 0, 'M');
    }else{
      counter--;
      sendMsg(counter/2, 0, 0, 'M');
    }

    if(counter >= 24){counter = 0;}else if(counter < 0){counter = 24;}
  }
  
  if(analogRead(encoderSPin) == 1){
    rootNote = (int)(counter/2);
    if(rootNote == 12){
      rootNote = 11;
    }  

    for(int i = 0; i < 25; i++){
      leds[i] = CRGB::Black;
    }

    if(scaleIndex != -1){
      for(int i = 1; i < 8; i++){
        if(scalesArray[scaleIndex][i] != -1){
          leds[rootNote + scalesArray[scaleIndex][i]] = CRGB::Blue;  
        } 
      }  
    }
    
    FastLED.show();
  
    sendMsg(rootNote, 0, 1, 'M');
    Serial.print("ROOTNOTE: ");
    Serial.println(rootNote);
  }
  
  aLastState = aState;
}

void readVolume(){
  volumeLevel = (float)analogRead(volumePin);
}

/* CONTROLS FOR PRESSED NOTES AND POLYPHONICS */
void addPressedNote(int note){
  if(getIndexOfPressedNote(note) == -1){
    
    for(int i = 0; i < 8; i++){
      if(notesPressed[i] == -1){
        Serial.println("TEST1");
        notesPressed[i] = note;
        break;
      }
    }
  }
}

void removePressedNote(int note){
  for(int i = 0; i < 8; i++){
    if(notesPressed[i] == note){
      notesPressed[i] = -1;
      break;
    }
  }
}

int getIndexOfPressedNote(int note){
  for(int i = 0; i < 8; i++){
    if(notesPressed[i] == note){
      return i;
    }
  }

  return -1;
}

void playNotes(){
  AudioNoInterrupts();
  for(int i = 0; i < 8; i++){
    if(notesPressed[i] != -1){
      if(chordIndex != -1){
        for(int j = 0; j < 6; j++){
          if(chordsArray[chordIndex][j] != -1){
            noteEvent(1, notesPressed[i] + chordsArray[chordIndex][j], i + j);  
          }
        }
      }
      else{
        noteEvent(1, notesPressed[i], i);  
      }
    }  
    else{
      if(chordIndex == -1){
        noteEvent(0, notesPressed[i], i);  
      }else{
        bool endChord = true;
        for(int j = 0; j < 8; j++){
          if(notesPressed[j] != -1){
            endChord = false;
            break;
          }
        }
        if(endChord){
          noteEvent(0, notesPressed[i], i);   
        }
      }
    }
  }
  AudioInterrupts();
  
}

void noteEvent(int state, int note, int noteIndex){
  
  
  switch(noteIndex){
    case 0:
      Serial.print("NOTE 1 | state: ");
      Serial.println(state);
      if(state == 1){
        waveform1.frequency(midiToHz((float)note));
        envelope1.noteOn();
      }
      else{
        envelope1.noteOff();
      }
      break;
    case 1:
      Serial.print("NOTE 2 | state: ");
      Serial.println(state);
      if(state == 1){
        waveform2.frequency(midiToHz((float)note));
        envelope2.noteOn();
      }
      else{
        envelope2.noteOff();
      }
      break;
    case 2:
      Serial.print("NOTE 3 | state: ");
      Serial.println(state);
      if(state == 1){
        waveform3.frequency(midiToHz((float)note));
        envelope3.noteOn();
      }
      else{
        envelope3.noteOff();
      }
      break;
    case 3:
      Serial.print("NOTE 4 | state: ");
      Serial.println(state);
      if(state == 1){
        waveform4.frequency(midiToHz((float)note));
        envelope4.noteOn();
      }
      else{
        envelope4.noteOff();
      }
      break;
    case 4:
      Serial.print("NOTE 5 | state: ");
      Serial.println(state);
      if(state == 1){
        waveform5.frequency(midiToHz((float)note));
        envelope5.noteOn();
      }
      else{
        envelope5.noteOff();
      }
      break;
    case 5:
      Serial.print("NOTE 6 | state: ");
      Serial.println(state);
      if(state == 1){
        waveform6.frequency(midiToHz((float)note));
        envelope6.noteOn();
      }
      else{
        envelope6.noteOff();
      }
      break;
    case 6:
      Serial.print("NOTE 7 | state: ");
      Serial.println(state);
      if(state == 1){
        waveform7.frequency(midiToHz((float)note));
        envelope7.noteOn();
      }
      else{
        envelope7.noteOff();
      }
      break;
    case 7:
      Serial.print("NOTE 8 | state: ");
      Serial.println(state);
      if(state == 1){
        waveform8.frequency(midiToHz((float)note));
        envelope8.noteOn();
      }
      else{
        envelope8.noteOff();
      }
      break;
    default:
      break;
    
  }
  FastLED.show();
  AudioInterrupts();
}

static void setuppins()
{
  uint8_t i;

  // initialize
  // select lines
  // LED columns
  for (i = 0; i < NUM_LED_COLUMNS; i++)
  {
    pinMode(ledcolumnpins[i], OUTPUT);

    // with nothing selected by default
    digitalWrite(ledcolumnpins[i], HIGH);
  }

  // button columns
  for (i = 0; i < NUM_BTN_COLUMNS; i++)
  {
    pinMode(btncolumnpins[i], OUTPUT);

    // with nothing selected by default
    digitalWrite(btncolumnpins[i], HIGH);
  }

  // button row input lines
  for (i = 0; i < NUM_BTN_ROWS; i++)
  {
    pinMode(btnrowpins[i], INPUT_PULLUP);
  }

  // LED drive lines
  for (i = 0; i < NUM_LED_ROWS; i++)
  {
    pinMode(colorpins[i], OUTPUT);
    digitalWrite(colorpins[i], LOW);
  }

  // Initialize the debounce counter array
  for (uint8_t i = 0; i < NUM_BTN_COLUMNS; i++)
  {
    for (uint8_t j = 0; j < NUM_BTN_ROWS; j++)
    {
      debounce_count[i][j] = 0;
    }
  }
}

static void scan()
{
  static uint8_t current = 0;
  uint8_t val;
  uint8_t i, j;

  // Select current columns
  digitalWrite(btncolumnpins[current], LOW);
  digitalWrite(ledcolumnpins[current], LOW);

  // output LED row values
  for (i = 0; i < NUM_LED_ROWS; i++)
  {
    if (LED_buffer[current][i])
    {
      digitalWrite(colorpins[i], HIGH);
    }
  }

  // pause a moment
  delay(1);

  // Read the button inputs
  for ( j = 0; j < NUM_BTN_ROWS; j++)
  {
    val = digitalRead(btnrowpins[j]);
    if (val == LOW)
    {
      // active low: val is low when btn is pressed
      if ( debounce_count[current][j] < MAX_DEBOUNCE)
      {
        debounce_count[current][j]++;
        if ( debounce_count[current][j] == MAX_DEBOUNCE )
        {
          //Serial.print("Key Down ");
          //Serial.println((current * NUM_BTN_ROWS) + j);

          for(int k = 0; k < 4; k++){  
            for(int h = 0; h < 4; h++){
              if(h == current && k == j){
              }
              else{
                LED_buffer[h][k] = 0;                 
              }
            }
            
          }

          LED_buffer[current][j] = !LED_buffer[current][j];
          chooseOption(LED_buffer[current][j]);    
          
        }
      }
    }
    else
    {
      // otherwise, button is released
      if ( debounce_count[current][j] > 0)
      {
        debounce_count[current][j]--;
        if ( debounce_count[current][j] == 0 )
        {
          //Serial.print("Key Up ");
          //Serial.println((current * NUM_BTN_ROWS) + j);

          // If you want to do something when a key is released, do it here:

        }
      }
    }
  }// for j = 0 to 3;

  delay(1);

  digitalWrite(btncolumnpins[current], HIGH);
  digitalWrite(ledcolumnpins[current], HIGH);

  for (i = 0; i < NUM_LED_ROWS; i++)
  {
    digitalWrite(colorpins[i], LOW);
  }

  current++;
  if (current >= NUM_LED_COLUMNS)
  {
    current = 0;
  }

}
