#include <Arduino.h>

// put function declarations here:
const int pwm = A0;
const int button1 = 9;
const int button2 = 10;

unsigned long lastDebounceTime1 = 0;
unsigned long lastDebounceTime2 = 0;
unsigned long debounceDelay = 50; // the debounce time; increase if the output flickers

int buttonState1 = LOW;
int lastButtonState1 = LOW;

int buttonState2 = LOW;
int lastButtonState2 = LOW;

int previousPotentioValMap = -1;

int potentioVal;

void setup() {
  Serial.begin(9600);
  Serial.println("begin");
  pinMode(pwm, INPUT);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
}

void debounceButton(int buttonPin, int &buttonState, int &lastButtonState, unsigned long &lastDebounceTime) {
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH) {
        if (buttonPin == button1) {
          Serial.println("Left");
        } else if (buttonPin == button2) {
          Serial.println("Right");
        }
      }
    }
  }

  lastButtonState = reading;
}

void potentioPWM() {
  const int threshold = 5; // Define a threshold value
  potentioVal = analogRead(pwm);
  int potentioValMap;
  potentioValMap = map(potentioVal, 0, 803, 0, 255);
  
  // Check if the change exceeds the threshold
  if (abs(potentioValMap - previousPotentioValMap) > threshold) {
    Serial.println(potentioValMap);
    previousPotentioValMap = potentioValMap; // Update the previous value
  }
  
  delay(150);
}

void loop() {
  debounceButton(button1, buttonState1, lastButtonState1, lastDebounceTime1);
  debounceButton(button2, buttonState2, lastButtonState2, lastDebounceTime2);
  potentioPWM();
}
