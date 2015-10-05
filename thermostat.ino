#define aref_voltage 3.3

// Calibration offset. Compare readings in serial console against
// a decent thermometer.
float tempCal = 2.2;

// The temperature you want to maintain, and the range above and
// below.
float setPoint = 67.0;
float lowLimit = 2.0;
float highLimit = 2.0;

// Cycle times for your furnace. Gas furnaces (what I have) don't
// like to be cycled on and off for short times, so I added some 
// delays here.
unsigned long minimumCycleLengthMinutes = 8;
unsigned long minimumBetweenCyclesMinutes = 12;

// Connected to a TMP36 powered by 3.3V pin which also is
// connected to AREF so I can use a reference voltage. This
// helps temperature remain reasonably accurate with various
// power sources.
int tempPin = 0;

// Connected to a transistor that switches a relay to switch
// my furnance's W and R wires.
int heatPin = 2;
int ledPin = 13;

bool heating = false;

void setup() {
  Serial.begin(9600);
  analogReference(EXTERNAL);
  pinMode(ledPin, OUTPUT);
  pinMode(heatPin, OUTPUT);
}

void loop() {
  delay(1000);
  float temperature = getTemperature(tempPin);
  Serial.println(temperature);

  if(heating) {
    if(shouldStopHeat(temperature)) {
      Serial.println("Stopping heat");
      digitalWrite(heatPin, LOW);
      digitalWrite(ledPin, LOW);
      heating = false;
      unsigned long timeBetweenCycles = minToMs(minimumBetweenCyclesMinutes);
      Serial.print("Heat off. Waiting ");
      Serial.print(timeBetweenCycles);
      Serial.println("ms");
      delay(timeBetweenCycles);
    }
  } else {
    if(shouldStartHeat(temperature)) {
      Serial.println("Starting heat");
      digitalWrite(heatPin, HIGH);
      digitalWrite(ledPin, HIGH);
      heating = true;
      unsigned long cycleTime = minToMs(minimumCycleLengthMinutes);
      Serial.print("Heat on. Waiting ");
      Serial.print(cycleTime);
      Serial.println("ms");
      delay(cycleTime);
    }
  }

  delay(5000);
}

unsigned long minToMs(unsigned long minutes) {
  return(minutes * 60 * 1000);
}

bool shouldStartHeat(int temperature) {
  return (temperature < (setPoint - lowLimit));
}

bool shouldStopHeat(int temperature) {
  return (temperature > (setPoint + highLimit));
}

float getTemperature(int pin) {
  // Multiply analog read by reference voltage and divide by 1024
  // to normalize temperature output from TMP36.
  float temperature = (analogRead(pin) * aref_voltage) / 1024.0;

  // Convert 5mV/degree C reading to degrees Farenheit, and apply
  // calibration offset.
  temperature = (((temperature - .5) * 100) * 1.8) + 32 + tempCal;
  
  return(temperature);
}

