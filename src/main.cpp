#include <Arduino.h>
#define RED_PIN 32
#define YELLOW_PIN 33
#define GREEN_PIN 25
#define BUTTON_PIN 26
#define BUZZER_PIN 27
// Hard coded enumerator
#define RED_STATE 0
#define RED_YELLOW_STATE 1
#define YELLOW_STATE 2
#define GREEN_STATE 3
#define WAIT_STATE 4
#define BUZZER_ON_STATE 0
#define BUZZER_OFF_STATE 1
#define PRINT_MILLIS 100
#define RED_MILLIS 10000
#define RED_YELLOW_MILLIS 2000
#define YELLOW_MILLIS 2000
#define GREEN_MILLIS 5000
#define WAIT_MILLIS 5000
#define RED_BUZZER_ON_MILLIS 250
#define RED_BUZZER_OFF_MILLIS 250
#define GREEN_BUZZER_ON_MILLIS 500
#define GREEN_BUZZER_OFF_MILLIS 1500
int tl_state;
int buzz_state;
unsigned long print_timer;
unsigned long tl_timer; // Traffic light timer.
unsigned long buzz_timer;

void setTone(int pin)
{
  ledcAttachPin(pin, 0);       // pin, channel
  ledcWriteNote(0, NOTE_F, 4); // channel, frequency, octave
}

void unsetTone(int pin)
{
  ledcDetachPin(pin);
}

void setup()
{
  Serial.begin(9600);
  // Configure LED pins as outputs.
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  // Initial state for states and timers..
  tl_state = RED_STATE;
  buzz_state = BUZZER_OFF_STATE;
  print_timer = millis() + PRINT_MILLIS;
  tl_timer = millis() + RED_MILLIS;
  buzz_timer = millis() + RED_BUZZER_ON_MILLIS;
}

void loop()
{
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
    digitalWrite(YELLOW_PIN, HIGH);
    if (millis() > tl_timer)
    {
      digitalWrite(YELLOW_PIN, LOW);
      tl_timer = millis() + RED_MILLIS;
      tl_state = RED_STATE;
    }
    break;
  case GREEN_STATE:
    digitalWrite(GREEN_PIN, HIGH);
    if (millis() > tl_timer && digitalRead(BUTTON_PIN) == HIGH)
    {
      tl_timer = millis() + WAIT_MILLIS;
      tl_state = WAIT_STATE;
    }
    break;
  case WAIT_STATE:
    digitalWrite(GREEN_PIN, HIGH);
    if (millis() > tl_timer)
    {
      digitalWrite(GREEN_PIN, LOW);
      tl_timer = millis() + YELLOW_MILLIS;
      tl_state = YELLOW_STATE;
    }
    break;
  }

  switch (buzz_state)
  {
  case BUZZER_ON_STATE:
    if (tl_state == YELLOW_STATE || tl_state == RED_YELLOW_STATE)
    {
      unsetTone(BUZZER_PIN);
      buzz_state = BUZZER_OFF_STATE;
      buzz_timer = millis() + RED_BUZZER_OFF_MILLIS;
    }
    else if (millis() > buzz_timer)
    {
      buzz_state = BUZZER_OFF_STATE;
      if (tl_state == GREEN_STATE || tl_state == WAIT_STATE)
      {
        unsetTone(BUZZER_PIN);
        buzz_timer = millis() + GREEN_BUZZER_OFF_MILLIS;
      }
      else // (tl_state == RED_STATE)
      {
        unsetTone(BUZZER_PIN);
        buzz_timer = millis() + RED_BUZZER_OFF_MILLIS;
      }
    }
    break;
  case BUZZER_OFF_STATE:
    if (tl_state == YELLOW_STATE || tl_state == RED_YELLOW_STATE)
    {
      buzz_timer = millis() + RED_BUZZER_OFF_MILLIS;
    }
    else if (millis() > buzz_timer)
    {
      if (tl_state == GREEN_STATE || tl_state == WAIT_STATE)
      {
        buzz_state = BUZZER_ON_STATE;
        setTone(BUZZER_PIN);
        buzz_timer = millis() + GREEN_BUZZER_ON_MILLIS;
      }
      else // (tl_state == RED_STATE)
      {
        buzz_state = BUZZER_ON_STATE;
        setTone(BUZZER_PIN);
        buzz_timer = millis() + RED_BUZZER_ON_MILLIS;
      }
    }
    break;
  }

  if (millis() > print_timer)
  {
    switch (tl_state)
    {
    case RED_STATE:
      Serial.print("RED_STATE ");
      break;
    case RED_YELLOW_STATE:
      Serial.print("RED_YELLOW_STATE ");
      break;
    case YELLOW_STATE:
      Serial.print("YELLOW_STATE ");
      break;
    case GREEN_STATE:
      Serial.print("GREEN_STATE ");
      break;
    }
    switch (buzz_state)
    {
    case BUZZER_ON_STATE:
      Serial.print("BUZZER_ON_STATE ");
      break;
    case BUZZER_OFF_STATE:
      Serial.print("BUZZER_OFF_STATE ");
      break;
    }
    switch (digitalRead(BUTTON_PIN))
    {
    case HIGH:
      Serial.println("BUTTON_PIN_HIGH");
      break;
    case LOW:
      Serial.println("BUTTON_PIN_LOW");
      break;
    }
    print_timer = millis() + PRINT_MILLIS;
  }
}
