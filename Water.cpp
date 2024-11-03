#include <NewPing.h>

#define TRIGGER_PIN D1
#define ECHO_PIN D2
#define BUZZER_PIN D3
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters).

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(9600);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  // Measure distance using the ultrasonic sensor
  int distance = sonar.ping_cm();
  
  // Print the measured distance to the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  for(int i=0;i<30;i++){
    delay(1000);
  }

  int new_distance = sonar.ping_cm();
  // Print the measured distance to the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(new_distance);
  Serial.println(" cm");
  if(new_distance==distance){
    digitalWrite(BUZZER_PIN, HIGH); // Turn on the buzzer
    delay(5000); // Keep the buzzer on for 10 seconds
    digitalWrite(BUZZER_PIN, LOW); // Turn off the buzzer
    delay(5000); // Wait for 10 seconds before turning it on again
  }
}
