#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#include <SPI.h>          // f.k. for Arduino-1.5.2
#include "Adafruit_GFX.h"// Hardware-specific library
#include <MCUFRIEND_kbv.h>
#include <SoftwareSerial.h>
SoftwareSerial SerialCom(0, 1); // Rx, Tx

MCUFRIEND_kbv tft;

#define BLACK       0x0000
#define DARK_GRAY   0x39C7
#define LIGHT_GRAY  0xE73C
#define BLUE        0x001F
#define RED         0xF800
#define GREEN       0x07E0
#define CYAN        0x07FF
#define MAGENTA     0xF81F
#define YELLOW      0xFFE0
#define WHITE       0xFFFF

#define WIDTH  480
#define HEIGHT   320

#define WHITE_KEY_HEIGHT 120

#define SCALE_INTERVAL_TIME 500

/* Key struct used to store information about 1 key */
struct Key{
  int x = 0; 
  int y = 0;
  int w = 0;
  int h = 0;
  bool isBlack = false;
  bool highlighted = false;
  bool pressed = false;
  int index = 0;
  bool changed = false;
  char letter = ' ';
};

struct Data{
  char event;
  int index;
  int note;
  int noteFloored; 
  int state;
};

/* The letters that should be showed on each key */
char letters[12] = {'C', 'C', 'D', 'D', 'E', 'F', 'F', 'G', 'G', 'A', 'A', 'B'};

/* Chords */
int majChord [] = {0, 4, 7, 0, 0, 0};
int minChord [] = {0, 3, 7, 0, 0, 0};
int augChord [] = {0, 3, 6, 0, 0, 0};
int dimChord [] = {0, 4, 8, 0, 0, 0};
int sus2Chord [] = {0, 2, 7, 0, 0, 0};
int sus4Chord [] = {0, 5, 7, 0, 0, 0};
int perf5Chord [] = {0, 7, 0, 0, 0, 0};
int perf4Chord [] = {0, 5, 0, 0, 0, 0};
int maj7Chord [] = {0, 4, 7, 11, 0, 0};
int min7Chord [] = {0, 3, 7, 11, 0, 0};
int maj9Chord [] = {0, 4, 7, 11, 14, 0};
int min9Chord [] = {0, 3, 7, 10, 14, 0};
int maj11Chord [] = {0, 4, 7, 11, 14, 17};
int min11Chord [] = {0, 3, 7, 10, 14, 17};
int *chordsArray [] = {majChord, minChord, augChord, dimChord, sus2Chord, sus4Chord, perf5Chord, perf4Chord, maj7Chord, min7Chord, maj9Chord, min9Chord, maj11Chord, min11Chord};

/* Scales */
int majorKey [] = {0, 2, 4, 5, 7, 9, 11, 12, 0};       
int minorKey [] = {0, 2, 3, 5, 7, 8, 10, 12, 0};       
int augKey   [] = {0, 3, 4, 7, 8, 11, 12, 0, 0};      
int dimKey   [] = {0, 2, 3, 5, 6, 8, 9, 11, 12};        
int *scalesArray [] = {majorKey, minorKey, augKey, dimKey};   


/* Keys values */

Key *keys = malloc(26 * sizeof(Key));

/* Loop update variables */
unsigned long counter = 0;
unsigned long prevCounter = 0;


void setup(){

  /* START COMMUNCATION TO TEENSY */
  //SerialCom.begin(9600);

  /* START COMMUNICATION TO TOUCHSCREEN */
  Serial.begin(9600);
  uint16_t ID = tft.readID(); //
  if (ID == 0xD3D3) ID = 0x9481; // write-only shield
  tft.begin(ID); 
  
  /* SCREEN STARTUP */
  setBackgoundColor();
  tft.setRotation(1);
  
  initializeKeys(keys);
  paintKeys(keys);
}

char incomingData[10];
char indexTmp[3];
char noteNumberTmp[4];

bool showChord = false;
int selectedChord = 0;
int chordCounter = 0;

bool showScale = false;
int selectedScale = 0;
int scaleCounter = 0;

int rootNote = -1;

void loop(){
  
  if(readIncomingData()){
    handleIncomingData();  
  }
   
  if(showScale){
    if(counter > SCALE_INTERVAL_TIME){
      if(scaleCounter != 8){
        keys[rootNote + scalesArray[selectedScale][scaleCounter]].changed = true;
        keys[rootNote + scalesArray[selectedScale][scaleCounter]].highlighted = true;
        
        scaleCounter++;  
      }

      prevCounter = millis(); 
    }
  }
  else if(!showScale && scaleCounter != 0){
    resetKeys();
    scaleCounter = 0;      
  }

  

  if(checkForUpdates() == true){
    repaintKeys();
  }

  //delay(200);
  //debugMsg();
  
  counter = millis() - prevCounter; 

  delay(1);
}

/* INCOMMING DATA HANDLING */

bool readIncomingData(){ 
  bool gotData = false;
  //Serial.print("INCOMING DATA LEN: ");
  //Serial.println(Serial.available());
  
  while(Serial.available() > 0){
    gotData = true;
    Serial.readBytes(incomingData, 7);
    
    delay(1);
  }

  return gotData;
}

void handleIncomingData(){
  
  
  Data d = splitIncomingData();

  if(d.event == 'R'){
    showScale = false;
    showChord = false;
    resetKeys();
  }
  else if(d.event == 'S'){
    if(d.state == 1){
      showScale = true;
      selectedScale = d.index;
      rootNote = d.noteFloored;

      showChord = false;
    }
    else{
     showScale = false;
     selectedScale = 0; 
    }
  }
  else if(d.event == 'C'){
    if(d.state == 1){
      showChord = true;
      selectedChord = d.index;
      
      showScale = false;

      if(rootNote != -1){
        for(int i = 0; i < 6; i++){
          keys[rootNote + chordsArray[selectedChord][i]].changed = true; 
          keys[rootNote + chordsArray[selectedChord][i]].pressed = true; 
        }  
      }
      
    }
    else{
      showChord = false;  
      
      for(int i = 0; i < 6; i++){
        if(chordsArray[selectedChord][i] != 0){
          keys[rootNote + chordsArray[selectedChord][i]].changed = true; 
          keys[rootNote + chordsArray[selectedChord][i]].pressed = false;   
        }
      }  
     
      selectedChord = 0; 
    }
  }
  else if(d.event == 'N'){ 
    if(d.state == 1){
      rootNote = d.noteFloored;
      keys[d.noteFloored].changed = true;
      keys[d.noteFloored].pressed = true;   

      if(showChord){
        for(int i = 0; i < 6; i++){
          keys[rootNote + chordsArray[selectedChord][i]].changed = true; 
          keys[rootNote + chordsArray[selectedChord][i]].pressed = true; 
        }  
      }
    }
    else if(d.state == 0){
      keys[d.noteFloored].changed = true;
      keys[d.noteFloored].pressed = false; 

      for(int i = 0; i < 6; i++){
        keys[rootNote + chordsArray[selectedChord][i]].changed = true; 
        keys[rootNote + chordsArray[selectedChord][i]].pressed = false; 
      }  
      
      rootNote = -1;
    }
  }
}

Data splitIncomingData(){

  Data data;
  data.event = incomingData[0];

  indexTmp[0] = incomingData[1];
  indexTmp[1] = incomingData[2];
  indexTmp[2] = '\0';
  
  data.index = atoi(indexTmp);

  noteNumberTmp[0] = incomingData[3];
  noteNumberTmp[1] = incomingData[4];
  noteNumberTmp[2] = incomingData[5];
  noteNumberTmp[3] = '\0';
  
  data.note = atoi(noteNumberTmp);
  if(data.note < 25){
    data.noteFloored = data.note;
  }
  else{
    int noteDivided = floor(data.note / 12);
    data.noteFloored = (data.note - (noteDivided * 12)) - 1;  
  }
  
  
  char stateTmp[2] = { incomingData[6], '\0' };
  data.state = atoi(stateTmp);

  delay(1);
  Serial.print("EVENT: ");
  Serial.println(data.event);
  Serial.print("INDEX: ");
  Serial.println(data.index);
  Serial.print("NOTE: ");
  Serial.println(data.note);
  Serial.print("NOTE FlOORED: ");
  Serial.println(data.noteFloored);
  Serial.print("STATE: ");
  Serial.println(data.state);
  Serial.println("-----------------");
  delay(1);

  return data;
}

/* OTHER FUNCTIONS */


void resetKeys(){
  for(int i = 0; i < 25; i++){
    if(keys[i].highlighted){
      keys[i].highlighted = false; 
      keys[i].changed = true;
    }
  }
}



void setStartupPercent(char *msg){
  tft.setTextColor(WHITE);
  tft.setCursor(10, 10);
  tft.println(msg);  
}

void setStartupMessage(char *msg, int row){
  tft.setTextColor(WHITE);
  tft.setCursor(10, 15 * (row + 1));
  tft.println(msg);  
}

void setBackgoundColor(){
  tft.setCursor(0, 0);
  tft.fillRect(0, 0, HEIGHT, WIDTH, BLACK);
}

void paintKeys(Key *keys){

  int textX = 0;
  int textY = 100;
  
  tft.setTextSize(2);
  
  for(int i = 0; i < 25; i++){
    if(keys[i].isBlack == false){
      tft.fillRect(keys[i].x, keys[i].y, keys[i].w, keys[i].h, WHITE);
      tft.drawRect(keys[i].x, keys[i].y, keys[i].w, keys[i].h, LIGHT_GRAY);

      tft.setTextColor(BLACK);
      textX = (keys[i].x + (keys[i].w) / 2) - 5;
      textY = (keys[i].y + keys[i].h) - 20;
      
      tft.setCursor(textX, textY);
      tft.println(keys[i].letter);  
    }
  }

  for(int i = 0; i < 25; i++){
    if(keys[i].isBlack == true){
      tft.fillRect(keys[i].x - 2, keys[i].y, keys[i].w + 4, keys[i].h + 5, DARK_GRAY);
      tft.fillRect(keys[i].x, keys[i].y, keys[i].w, keys[i].h, BLACK);

      tft.setTextColor(WHITE);
      textX = (keys[i].x + (keys[i].w) / 2) - 8;
      textY = (keys[i].y + keys[i].h) - 20;
      
      tft.setCursor(textX, textY);
      tft.println(keys[i].letter);  

      tft.setCursor(textX + 8, textY - 8);
      tft.println('#');
    }
  }

  

  
  
  
  
  
}

void initializeKeys(Key *keys){

  int blackKeys[] = {1, 3, 6, 8, 10, 13, 15, 18, 20, 22};
  int whiteKeyIndex = 0;
  int blackKeyIndex = 0;

  int whiteKeyWidth = (WIDTH / 15);
  int blackKeyWidth = 20;

  int index = 0;
  
  for(int i = 0; i < 25; i++){
    Key newKey;

    newKey.isBlack = false;
    newKey.highlighted = false;
    newKey.pressed = false;
    newKey.changed = false;
    newKey.index = i;
    newKey.letter = letters[index];
    
    index++;
    if(index == 12){
      index = 0;
    }
    
    for(int j = 0; j < 10; j++){
      if(i == blackKeys[j]){
        newKey.isBlack = true;
        break;
      }
    }
    
    if(newKey.isBlack == false){
      newKey.x = whiteKeyWidth * whiteKeyIndex;
      newKey.y = HEIGHT - WHITE_KEY_HEIGHT;
      newKey.h = WHITE_KEY_HEIGHT;
      newKey.w = whiteKeyWidth;

      whiteKeyIndex++;
    }
    else{
      newKey.x = (keys[i - 1].x + whiteKeyWidth) - (blackKeyWidth / 2);
      newKey.y = HEIGHT - WHITE_KEY_HEIGHT;
      newKey.h = WHITE_KEY_HEIGHT / 2;
      newKey.w = blackKeyWidth;
      blackKeyIndex++;
    }
    
    keys[i] = newKey;
  }
  
}

bool checkForUpdates(){
  for(int i = 0; i < 25; i++){
    if(keys[i].changed == true){
      return true; 
    }
  }
  return false;
}

void repaintKeys(){
  for(int i = 0; i < 25; i++){
    if(keys[i].changed == true){
      paintKey(i); 
      keys[i].changed = false; 
    }
  }
}

void paintKey(int keyIndex){
  
  if(keyIndex != 24){  
    if(keys[keyIndex].isBlack){
      paintSingleKey(keys[keyIndex]);
    }
    else{
      paintSingleKey(keys[keyIndex]);
      if(keyIndex != 0){
        if(keys[keyIndex - 1].isBlack){
          paintSingleKey(keys[keyIndex - 1]);
        }
      }
      if(keys[keyIndex + 1].isBlack){
         paintSingleKey(keys[keyIndex + 1]);
      }
    }
  }
}

void paintSingleKey(Key key){
  int textX = 0;
  int textY = 0;
  
  if(key.isBlack){
    if(key.highlighted){
      tft.fillRect(key.x - 2, key.y, key.w + 4, key.h + 5, DARK_GRAY);
      tft.fillRect(key.x, key.y, key.w, key.h, BLUE);    
    }
    else if(key.pressed){
      tft.fillRect(key.x - 2, key.y, key.w + 4, key.h + 5, DARK_GRAY);
      tft.fillRect(key.x, key.y, key.w, key.h, GREEN);  
    }
    else{
      tft.fillRect(key.x - 2, key.y, key.w + 4, key.h + 5, DARK_GRAY);
      tft.fillRect(key.x, key.y, key.w, key.h, BLACK);  
    }
    
    tft.setTextColor(WHITE);
    textX = (key.x + (key.w) / 2) - 8;
    textY = (key.y + key.h) - 20;
    
    tft.setCursor(textX, textY);
    tft.println(key.letter);  

    tft.setCursor(textX + 8, textY - 8);
    tft.println('#');
    
  }
  else{
    if(key.highlighted){
      tft.fillRect(key.x, key.y, key.w, key.h, BLUE);
      tft.drawRect(key.x, key.y, key.w, key.h, DARK_GRAY);  
    }
    else if(key.pressed){
      
      tft.fillRect(key.x, key.y, key.w, key.h, GREEN);
      tft.drawRect(key.x, key.y, key.w, key.h, DARK_GRAY);  
    }
    else{
      tft.fillRect(key.x, key.y, key.w, key.h, WHITE);
      tft.drawRect(key.x, key.y, key.w, key.h, LIGHT_GRAY);  
    }

    tft.setTextColor(BLACK);
    textX = (key.x + (key.w) / 2) - 5;
    textY = (key.y + key.h) - 20;
    
    tft.setCursor(textX, textY);
    tft.println(key.letter);  
  }
}
