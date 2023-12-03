#include <Arduino.h>

#define TOUCH_ELECTRODE D7
#define PHOTORES_PIN A0
#define START_PIN D1
#define STOP_PIN D2

IRAM_ATTR void start(void);
IRAM_ATTR void stop(void);

volatile unsigned long started1 = 0;
volatile unsigned long stopped = 0;
volatile int first= 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Hello Flo");
  pinMode(TOUCH_ELECTRODE, OUTPUT);

  pinMode(START_PIN, INPUT);
  pinMode(STOP_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(START_PIN), start, RISING);
  attachInterrupt(digitalPinToInterrupt(STOP_PIN), stop, RISING);
}

IRAM_ATTR void start() {
  if (first == 1) {
    started1 = millis();
    first = 0;
  }
}

IRAM_ATTR void stop() {
  stopped = millis();
}

void loop() {
  digitalWrite(TOUCH_ELECTRODE, LOW);
  delay(100);

  Serial.println("Starting measurement. ");
  first = 1;
  digitalWrite(TOUCH_ELECTRODE, HIGH);
  delay(1000);
  Serial.printf("started %lu, stopped %lu, delay %lu ms\n", started1, stopped, stopped-started1);

  Serial.println("Reset...");
  digitalWrite(TOUCH_ELECTRODE, LOW);
  delay(2500);
}