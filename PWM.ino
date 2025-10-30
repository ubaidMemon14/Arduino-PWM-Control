// Sketch A: LED + motor speed control via PWM (MOSFET/N-P transistor)
// LED on PWM pin 9, Motor on PWM pin 10
// Potentiometer for LED -> A0, for motor -> A1

const int ledPin   = 9;    // PWM pin for LED
const int motorPin = 10;   // PWM pin for motor (connect to MOSFET gate/base resistor to transistor)
const int potLed   = A0;   // potentiometer for LED brightness
const int potMotor = A1;   // potentiometer for motor speed

int lastLedValue = -1;
int lastMotorValue = -1;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(motorPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("PWM LED + Motor Speed Control");
}

void loop() {
  // Read pots (0..1023)
  int rawLed  = analogRead(potLed);
  int rawMotor = analogRead(potMotor);

  // Map to PWM range (0..255)
  int pwmLed  = map(rawLed, 0, 1023, 0, 255);
  int pwmMotor = map(rawMotor, 0, 1023, 0, 255);

  // Optional: gentle ramp to avoid sudden jumps
  pwmLed = ramp(lastLedValue, pwmLed, 5);
  pwmMotor = ramp(lastMotorValue, pwmMotor, 10);

  // Write PWM
  analogWrite(ledPin, pwmLed);
  analogWrite(motorPin, pwmMotor);

  // Debug only if values changed (reduces serial spam)
  if (pwmLed != lastLedValue || pwmMotor != lastMotorValue) {
    Serial.print("LED PWM: "); Serial.print(pwmLed);
    Serial.print("  |  Motor PWM: "); Serial.println(pwmMotor);
    lastLedValue = pwmLed;
    lastMotorValue = pwmMotor;
  }

  delay(20); // small delay to reduce noise
}

// Ramp helper: move cur toward target by 'step'
int ramp(int cur, int target, int step) {
  if (cur < 0) return target; // first-time set
  if (abs(target - cur) <= step) return target;
  if (target > cur) return cur + step;
  return cur - step;
}
