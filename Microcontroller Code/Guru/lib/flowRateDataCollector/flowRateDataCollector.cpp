/*
Liquid flow rate sensor -DIYhacking.com Arvind Sanjeev

Measure the liquid/water flow rate using this code. 
Connect Vcc and Gnd of sensor to arduino, and the 
signal line to arduino digital pin 2.
 
 */

byte statusLed    = 13;

byte sensorInterrupt = 2;  // 0 = digital pin 2
byte sensorPin       = 2;

volatile int pulseCount;  

unsigned long oldTime;
unsigned long currentTime;

void setup()
{
  
  // Initialize a serial connection for reporting values to the host
  Serial.begin(115200);
   
  // Set up the status LED line as an output
  pinMode(statusLed, OUTPUT);
  digitalWrite(statusLed, HIGH);  // We have an active-low LED attached
  
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);

  pulseCount        = 0;
  oldTime           = 0;

  // The Hall-effect sensor is connected to pin 2 which uses interrupt 0.
  // Configured to trigger on a FALLING state change (transition from HIGH
  // state to LOW state)
  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  Serial.println("time, pulses per ms");
}

// int oldPulseCount = 0;

/**
 * Main program loop
 */
void loop()
{
   
  if(pulseCount>=1)    // Only process counters once per second
  { 

    int timeTaken = currentTime-oldTime;
    oldTime = currentTime;

    double pulsesPerSec = 1000.0*(double)pulseCount/double(timeTaken);
    pulseCount = 0;
    
    Serial.print(timeTaken); Serial.print(", ");
    Serial.print(oldTime);
    Serial.print(", ");
    Serial.println(pulsesPerSec, 8);
    // Reset the pulse counter so we can start incrementing again
    
    // Enable the interrupt again now that we've finished sending output
    // attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  }
}

/*
Insterrupt Service Routine
 */
void pulseCounter()
{
  // Increment the pulse counter
  pulseCount++;
  currentTime = millis();
}