const int buttonPin = 2;
const int ledPin =  4;

int buttonState = 0;
int i;

void setup()
{
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop()
{
  delay(500);
  Serial.print("iteration:   ");
  Serial.println(++i);
  buttonState = digitalRead(buttonPin);
  //buttonState = HIGH;

  if (buttonState == HIGH)
  {
    digitalWrite(ledPin, HIGH);
  } 
  else
  {
    digitalWrite(ledPin, LOW);
  }
    Serial.println(buttonState);
}
