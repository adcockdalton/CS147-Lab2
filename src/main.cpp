#include <Arduino.h>
#define RED_PIN 32
#define YELLOW_PIN 33
#define GREEN_PIN 25
// Hard coded enumerator
#define RED_STATE 0
#define RED_YELLOW_STATE 1
#define YELLOW_STATE 2
#define GREEN_STATE 3
#define RED_MILLIS 10000
#define RED_YELLOW_MILLIS 2000
#define YELLOW_MILLIS 2000
#define GREEN_MILLIS 5000
int tl_state;           // Traffic light state.
unsigned long tl_timer; // Traffic light timer.

void setup()
{
  // Configure LED pins as outputs.
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  // Initial state for states and timers..
  tl_state = RED_STATE;
  tl_timer = millis() + RED_MILLIS;
}

void loop()
{
  // TL state machine
  switch (tl_state)
  {
  case RED_STATE:
    digitalWrite(RED_PIN, HIGH);
    if (millis() > tl_timer)
    {
      digitalWrite(RED_PIN, LOW);
      tl_timer = millis() + RED_YELLOW_MILLIS;
      tl_state = RED_YELLOW_STATE;
    }
    break;
  case RED_YELLOW_STATE:
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(YELLOW_PIN, HIGH);
    if (millis() > tl_timer)
    {
      digitalWrite(RED_PIN, LOW);
      digitalWrite(YELLOW_PIN, LOW);
      tl_timer = millis() + GREEN_MILLIS;
      tl_state = GREEN_STATE;
    }
    break;
  case YELLOW_STATE:
    if (tl_timer < millis())
    {
      tl_timer = millis() + RED_MILLIS;
      tl_state = RED_STATE;
    }
    break;
  case GREEN_STATE:
    digitalWrite(GREEN_PIN, HIGH);
    if (tl_timer < millis() /*&& button pressed*/)
    {
      digitalWrite(GREEN_PIN, LOW);
      tl_timer = millis() + YELLOW_MILLIS;
      tl_state = YELLOW_STATE;
    }
    break;
  }

  // TODO: Detect touch - button pressed.

  switch (tl_state)
  {
  case RED_STATE:

    break;
  case GREEN_STATE:

    break;
  }
}