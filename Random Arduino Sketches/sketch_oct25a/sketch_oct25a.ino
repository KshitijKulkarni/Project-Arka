void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A9, INPUT);
}

bool pulseDone = false;

long pulseCount = 0;

void loop() {
  // put your main code here, to run repeatedly:
  int starttime = millis();
  while (millis() - starttime < 100) {
    if (!pulseDone) {
      if (analogRead(A9) < 100) {
        pulseCount++;
        pulseDone = true;
      }
    } else {
      if (analogRead(A9) > 100)
        pulseDone = false;
    }
  }
  Serial.println(pulseCount * 600);
  pulseCount = 0;
}
