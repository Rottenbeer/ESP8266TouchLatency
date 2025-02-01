#include <Arduino.h>

#include <U8x8lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

// Output PINS
#define TOUCH_ELECTRODE D5

// Interrupt PINS
#define STOP_PIN D6

IRAM_ATTR void stop(void);
void drawHeadline(void);

U8X8_SSD1306_128X64_NONAME_HW_I2C display(U8X8_PIN_NONE);

volatile unsigned long started = 0;
volatile unsigned long stopped = 0;
volatile int first_stop = 0;

void setup()
{
  Serial.begin(115200);
  delay(100);

  display.begin();
  drawHeadline();

  Serial.println("\nLet's get started!\n");
  display.println("Let's go!\n");
  pinMode(TOUCH_ELECTRODE, OUTPUT);

  pinMode(STOP_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(STOP_PIN), stop, RISING);

  delay(1000);
  digitalWrite(TOUCH_ELECTRODE, LOW);
}

IRAM_ATTR void stop()
{
  if (first_stop == 1)
  {
    stopped = millis();
    first_stop = 0;
  }
}

void drawHeadline(void)
{
  display.setCursor(0, 0);
  display.setFont(u8x8_font_amstrad_cpc_extended_f);
  delay(100);
  display.clear();

  display.inverse();
  display.print("Touch-O-Mat 3000");
  display.noInverse();
  display.setCursor(0, 2);
}

void loop()
{
  drawHeadline();
  Serial.println("Starting measurement. \n");
  display.println("Get ready... \n");
  delay(500);
  display.println("Touched... \n");
  first_stop = 1;
  digitalWrite(TOUCH_ELECTRODE, HIGH);
  started = millis();
  delay(1000);
  drawHeadline();
  digitalWrite(TOUCH_ELECTRODE, LOW);
  Serial.printf("started %lu, stopped %lu, delay %lu ms\n", started, stopped, stopped - started);
  unsigned long latency = stopped - started;
  if (latency > 500)
  {
    latency = 0;
  }
  display.printf("Start %lu\nStop  %lu\nDelay %lu ms \n", started, stopped, latency);

  Serial.println("Reset...\n");
  delay(2000);
}