void setup(){
    pinMode(13, OUTPUT);
    Serial.begin(9600);
}

void loop(){
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(100);
}
