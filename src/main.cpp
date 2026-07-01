#include <Arduino.h>

const int triggerPin = D2;
const int leftLedsPin = D8;
const int rightLedsPin = D7;
const int frontLedsPin = D5;
const int speakerPin = D1;

// Phaser sound: a fast, repeating descending frequency sweep.
const unsigned long phaserSweepMs = 220;
const int phaserHighFreq = 2200;
const int phaserLowFreq = 500;
int lastPhaserFreq = -1;

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(triggerPin, INPUT_PULLUP);
  pinMode(leftLedsPin, OUTPUT);
  pinMode(rightLedsPin, OUTPUT);
  pinMode(frontLedsPin, OUTPUT);
  pinMode(speakerPin, OUTPUT);
}

void loop() {
  int triggerState = digitalRead(triggerPin);

  if (triggerState == LOW) {
    digitalWrite(leftLedsPin, HIGH);
    digitalWrite(rightLedsPin, HIGH);
    digitalWrite(frontLedsPin, HIGH);

    // Sweep the tone from high to low every phaserSweepMs, looping
    // continuously for a classic "pew" phaser sound while held.
    unsigned long elapsed = millis() % phaserSweepMs;
    int freq = phaserHighFreq - (int)((phaserHighFreq - phaserLowFreq) * elapsed / phaserSweepMs);
    if (freq != lastPhaserFreq) {
      tone(speakerPin, freq);
      lastPhaserFreq = freq;
    }
  }
  else {
    digitalWrite(leftLedsPin, LOW);
    digitalWrite(rightLedsPin, LOW);
    digitalWrite(frontLedsPin, LOW);
    noTone(speakerPin);
    lastPhaserFreq = -1;     
  }
}