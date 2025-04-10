#define triggerPin1 3  // stating which pin we put the trigger
#define echoPin1 2 
#define triggerPin2 5  // stating which pin we put the trigger
#define echoPin2 4      // stating which pin we put the Echo

void setup() {
  Serial.begin(9600);
  
  pinMode(triggerPin1, OUTPUT);  // sets the trigger pin as output
  pinMode(echoPin1, INPUT);      // sets the echo pin as input
  
  pinMode(triggerPin2, OUTPUT);  // sets the trigger pin as output
  pinMode(echoPin2, INPUT);      // sets the echo pin as input
}

void loop() {
  float distance1 = getDistance(triggerPin1, echoPin1);
  float distance2 = getDistance(triggerPin2, echoPin2);

  Serial.print("Distance 1 = ");
  if (distance1 >= 40 || distance1 <= 3) {
    Serial.println("Out of range");
  } else {
    Serial.print(distance1);
    Serial.println(" cm");
  }

  Serial.print("Distance 2 = ");
  if (distance2 >= 40 || distance2 <= 3) {
    Serial.println("Out of range");
  } else {
    Serial.print(distance2);
    Serial.println(" cm");
  }

  delay(500); // Μικρή καθυστέρηση πριν την επόμενη μέτρηση
}

float getDistance(int triggerPin, int echoPin) {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  
  float duration = pulseIn(echoPin, HIGH);
  float distance = (duration / 2) * 0.0344; // distance = (time/2) * speed of sound
  
  return distance;
}
