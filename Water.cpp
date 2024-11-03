#define TRIG_PIN 5 // TRIG pin
#define ECHO_PIN 4 // ECHO pin
#define BUZZER_PIN D3

float filterArray[20]; // array to store data samples from sensor
float distance; // store the distance from sensor
float new_distance;
float water_intake;
float height;

void setup() {
  // begin serial port
  Serial.begin (9600);

  // configure the trigger and echo pins to output mode
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // set the buzzer pin as an output
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  // 1. TAKING MULTIPLE MEASUREMENTS AND STORE IN AN ARRAY
  for (int sample = 0; sample < 20; sample++) {
    filterArray[sample] = ultrasonicMeasure();
    delay(30); // to avoid ultrasonic interference
  }

  // 2. SORTING THE ARRAY IN ASCENDING ORDER
  for (int i = 0; i < 19; i++) {
    for (int j = i + 1; j < 20; j++) {
      if (filterArray[i] > filterArray[j]) {
        float swap = filterArray[i];
        filterArray[i] = filterArray[j];
        filterArray[j] = swap;
      }
    }
  }

  // 3. FILTERING NOISE
  // + the five smallest samples are considered as noise -> ignore it
  // + the five biggest samples are considered as noise -> ignore it
  // ----------------------------------------------------------------
  // => get average of the 10 middle samples (from 5th to 14th)
  double sum = 0;
  for (int sample = 5; sample < 15; sample++) {
    sum += filterArray[sample];
  }

  distance = sum / 10;

  float level = distance;

  // print the value to Serial Monitor
  Serial.print("distance: ");
  Serial.print(level);
  Serial.println(" cm");

  // Taking new measurement
  for (int sample = 0; sample < 20; sample++) {
    filterArray[sample] = ultrasonicMeasure();
    delay(30); // to avoid ultrasonic interference
  }

  // SORTING THE ARRAY IN ASCENDING ORDER
  for (int i = 0; i < 19; i++) {
    for (int j = i + 1; j < 20; j++) {
      if (filterArray[i] > filterArray[j]) {
        float swap = filterArray[i];
        filterArray[i] = filterArray[j];
        filterArray[j] = swap;
      }
    }
  }

  double sum1 = 0;
  for (int sample = 5; sample < 15; sample++) {
    sum1 += filterArray[sample];
  }

  new_distance = sum1 / 10;

  float level_new = new_distance;
  Serial.print("new distance: ");
  Serial.print(level_new);
  Serial.println(" cm");

  // Comparing new and old distance
  if (level_new >= level - 0.5 && level_new <= level + 0.5) {
    digitalWrite(BUZZER_PIN, HIGH); // Turn on the buzzer
    delay(5000); // Keep the buzzer on for 5 seconds
    digitalWrite(BUZZER_PIN, LOW); // Turn off the buzzer
    delay(15000); // Wait for 15 seconds before the next iteration
  }
    height = level - level_new; // Calculate the change in water level
    // Assuming a cylindrical container, calculate water intake in milliliters
    // Height is in centimeters, radius is assumed to be 3.5 cm (adjust as needed)
    water_intake = (22.0 / 7) * 3.5 * 3.5 * height; 
    Serial.print("Water Intake : ");
    Serial.print(water_intake);
    Serial.println(" ml");
}

float ultrasonicMeasure() {
  // generate 10-microsecond pulse to TRIG pin
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // measure duration of pulse from ECHO pin
  float duration_us = pulseIn(ECHO_PIN, HIGH);

  // calculate the distance
  float distance_cm = 0.017 * duration_us;

  return distance_cm;
}
