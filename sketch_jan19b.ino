#include <Stepper.h>

const int STEPS_PER_REV = 2048;
Stepper myStepper(STEPS_PER_REV, 8, 10, 9, 11);

const int JOY_X_PIN = A0;

const int MAX_RPM = 15;
const int DEADZONE = 50;

int center = 512;
int currentRPM = 0;

unsigned long lastStepTime = 0;
const int stepInterval = 5; // ms

void setup() {
  Serial.begin(9600);

  // Auto-calibrate joystick center
  delay(500);
  center = analogRead(JOY_X_PIN);

  myStepper.setSpeed(MAX_RPM);
}

void loop() {
  int joy = analogRead(JOY_X_PIN);
  int offset = joy - center;

  // Dead zone
  if (abs(offset) < DEADZONE) {
    currentRPM = 0;
    return;
  }

  // Map joystick to speed
  int targetRPM = map(
    abs(offset),
    DEADZONE,
    512,
    1,
    MAX_RPM
  );

  int direction = (offset > 0) ? 1 : -1;

  // Smooth acceleration
  if (currentRPM < targetRPM) currentRPM++;
  if (currentRPM > targetRPM) currentRPM--;

  myStepper.setSpeed(currentRPM);

  // Non-blocking step timing
  if (millis() - lastStepTime >= stepInterval) {
    myStepper.step(direction);
    lastStepTime = millis();
  }
}

