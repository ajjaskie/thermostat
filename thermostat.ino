float tempCal = 6;
float setPoint = 67.0;
float lowLimit = 2.0;
float highLimit = 2.0;
long minimumCycleLengthMinutes = 8;
long minimumBetweenCyclesMinutes = 12;

int tempPin = 0;
int heatPin = 2;
int ledPin = 13;
bool heating = false;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(heatPin, OUTPUT);
}

void loop() {
  float temperature = getTemperature(tempPin);
  Serial.println(temperature);

  if(heating) {
    if(shouldStopHeat(temperature)) {
      Serial.println("Stopping heat");
      digitalWrite(heatPin, LOW);
      digitalWrite(ledPin, LOW);
      heating = false;
      long timeBetweenCycles = minToMs(minimumBetweenCyclesMinutes);
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
      long cycleTime = minToMs(minimumCycleLengthMinutes);
      Serial.print("Heat on. Waiting ");
      Serial.print(cycleTime);
      Serial.println("ms");
      delay(cycleTime);
    }
  }

  delay(5000);
}

long minToMs(long minutes) {
  return(minutes * 60 * 1000);
}

bool shouldStartHeat(int temperature) {
  return (temperature < (setPoint - lowLimit));
}

bool shouldStopHeat(int temperature) {
  return (temperature > (setPoint + highLimit));
}

float getTemperature(int pin) {
  float temperature = analogRead(pin) * .004882814;
  temperature = (((temperature - .5) * 100) * 1.8) + 32 + tempCal;
  return(temperature);
}
