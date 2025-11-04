int inputPin = 2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(inputPin, INPUT);
}

bool pulseDone = false;

long pulseCount = 0;

void loop() {
  // put your main code here, to run repeatedly:
  int starttime = millis();
  while (millis() - starttime < 100) {
    if (!pulseDone) {
      if (digitalRead(inputPin) == HIGH) {
        pulseCount++;
        pulseDone = true;
      }
    } else {
      if (digitalRead(inputPin) == LOW)
        pulseDone = false;
    }
  }
  Serial.println(pulseCount * 600);
  pulseCount = 0;
}
