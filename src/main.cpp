#include <Arduino.h>


#ifdef ATtiny
constexpr uint8_t PIN_LEFT_LEDS = PIN_PB0;
#else 
constexpr uint8_t PIN_TRIGGER = D2;
constexpr uint8_t PIN_LEFT_LEDS = D8;
constexpr uint8_t PIN_RIGHT_LEDS = D7;
constexpr uint8_t PIN_FRONT_LEDS = D5;
constexpr uint8_t PIN_SPEAKER = D1;
#endif 

// Phaser sound: a fast, repeating descending frequency sweep.
constexpr unsigned long phaserSweepMs = 220;
constexpr int phaserHighFreq = 2200;
constexpr int phaserLowFreq = 500;
int lastPhaserFreq = -1;

// Each trigger pull fires a single shot of this length; the trigger must be
// released and pressed again before another shot will fire.
constexpr unsigned long shotDurationMs = 440;
int lastTriggerState = HIGH;
bool shotActive = false;
unsigned long shotStartMs = 0;


#ifdef ATtiny
void setup()
{
  // initialize LED digital pin as an output.
  pinMode(LEFT_LEDS_PIN, OUTPUT);
}

void loop()
{
  // turn the LED on (HIGH is the voltage level)
  digitalWrite(LEFT_LEDS_PIN, HIGH);
  // wait for a second
  delay(500);
  // turn the LED off by making the voltage LOW
  digitalWrite(LEFT_LEDS_PIN, LOW);
   // wait for a second
  delay(500);
}
#else
void setup() {
  pinMode(PIN_TRIGGER, INPUT_PULLUP);
  pinMode(PIN_LEFT_LEDS, OUTPUT);
  pinMode(PIN_RIGHT_LEDS, OUTPUT);
  pinMode(PIN_FRONT_LEDS, OUTPUT);
  pinMode(PIN_SPEAKER, OUTPUT);
}

void loop() {
  int triggerState = digitalRead(PIN_TRIGGER);

  // Start a new shot only on the press edge (button was released last loop,
  // pressed now). Holding the trigger down does not retrigger the shot.
  if (triggerState == LOW && lastTriggerState == HIGH) {
    shotActive = true;
    shotStartMs = millis();
  }
  lastTriggerState = triggerState;

  if (triggerState == HIGH) {
    shotActive = false;
  }

  if (shotActive && millis() - shotStartMs >= shotDurationMs) {
    shotActive = false;
  }

  if (shotActive) {
    digitalWrite(PIN_LEFT_LEDS, HIGH);
    digitalWrite(PIN_RIGHT_LEDS, HIGH);
    digitalWrite(PIN_FRONT_LEDS, HIGH);

    // Sweep the tone from high to low every phaserSweepMs for a classic
    // "pew" phaser sound over the shot's duration.
    unsigned long elapsed = (millis() - shotStartMs) % phaserSweepMs;
    int freq = phaserHighFreq - (int)((phaserHighFreq - phaserLowFreq) * elapsed / phaserSweepMs);
    if (freq != lastPhaserFreq) {
      tone(PIN_SPEAKER, freq);
      lastPhaserFreq = freq;
    }
  }
  else {
    digitalWrite(PIN_LEFT_LEDS, LOW);
    digitalWrite(PIN_RIGHT_LEDS, LOW);
    digitalWrite(PIN_FRONT_LEDS, LOW);
    noTone(PIN_SPEAKER);
    lastPhaserFreq = -1;
  }
}
#endif