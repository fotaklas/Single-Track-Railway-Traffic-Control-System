//TinkerCad URL: https://www.tinkercad.com/things/bwjfd9sKEyU-e3ypna-periballonta-kai-efarmoges/editel?returnTo=https%3A%2F%2Fwww.tinkercad.com%2Fdashboard
// Pin definitions for ultrasonic sensors
// Ascent sensors
const int TRIG_PIN_ASC_1 = 2;
const int ECHO_PIN_ASC_1 = 3;
const int TRIG_PIN_ASC_2 = 4;
const int ECHO_PIN_ASC_2 = 5;

// Descent sensors
const int TRIG_PIN_DESC_1 = 6;
const int ECHO_PIN_DESC_1 = 7;
const int TRIG_PIN_DESC_2 = 8;
const int ECHO_PIN_DESC_2 = 9;

// Pin definitions for LEDs
// Ascent traffic lights
const int ASC_GREEN_LED = 10;
const int ASC_YELLOW_LED = 11;
const int ASC_RED_LED = 12;

// Descent traffic lights
const int DESC_GREEN_LED = 13;
const int DESC_YELLOW_LED = A0;
const int DESC_RED_LED = A1;

// Constants
const int DETECTION_DISTANCE_CM = 300; // Maximum distance to detect an object (in cm)
const unsigned long DEBOUNCE_DELAY = 500; // Debounce delay in milliseconds
const unsigned long BLINK_INTERVAL = 500; // Blinking interval for green light in milliseconds

// Variables for train detection
bool trainInAscent = false;
bool trainInDescent = false;
bool smallObjectDetected = false;
bool trainMovingUp = false;
bool trainMovingDown = false;
bool nothingDetected = true;

// Variables for sensor activation times
unsigned long lastAsc1Time = 0;
unsigned long lastAsc2Time = 0;
unsigned long lastDesc1Time = 0;
unsigned long lastDesc2Time = 0;

// Variables for blinking green light
unsigned long previousMillis = 0;
bool greenLedState = false;

void setup() {
  // Initialize Serial for debugging
  Serial.begin(9600);
  
  // Initialize ultrasonic sensor pins
  pinMode(TRIG_PIN_ASC_1, OUTPUT);
  pinMode(ECHO_PIN_ASC_1, INPUT);
  pinMode(TRIG_PIN_ASC_2, OUTPUT);
  pinMode(ECHO_PIN_ASC_2, INPUT);
  pinMode(TRIG_PIN_DESC_1, OUTPUT);
  pinMode(ECHO_PIN_DESC_1, INPUT);
  pinMode(TRIG_PIN_DESC_2, OUTPUT);
  pinMode(ECHO_PIN_DESC_2, INPUT);
  
  // Initialize LED pins
  pinMode(ASC_GREEN_LED, OUTPUT);
  pinMode(ASC_YELLOW_LED, OUTPUT);
  pinMode(ASC_RED_LED, OUTPUT);
  pinMode(DESC_GREEN_LED, OUTPUT);
  pinMode(DESC_YELLOW_LED, OUTPUT);
  pinMode(DESC_RED_LED, OUTPUT);
  
  // Turn off all LEDs initially
  turnOffAllLEDs();
}

void loop() {
  // Read distances from all sensors
  int distAsc1 = getDistance(TRIG_PIN_ASC_1, ECHO_PIN_ASC_1);
  int distAsc2 = getDistance(TRIG_PIN_ASC_2, ECHO_PIN_ASC_2);
  int distDesc1 = getDistance(TRIG_PIN_DESC_1, ECHO_PIN_DESC_1);
  int distDesc2 = getDistance(TRIG_PIN_DESC_2, ECHO_PIN_DESC_2);
  
  // Debug info
  Serial.print("Distances (cm): ASC1=");
  Serial.print(distAsc1);
  Serial.print(" ASC2=");
  Serial.print(distAsc2);
  Serial.print(" DESC1=");
  Serial.print(distDesc1);
  Serial.print(" DESC2=");
  Serial.println(distDesc2);
  
  // Check for train in ascent direction
  bool asc1Active = (distAsc1 < DETECTION_DISTANCE_CM);
  bool asc2Active = (distAsc2 < DETECTION_DISTANCE_CM);
  
  // Check for train in descent direction
  bool desc1Active = (distDesc1 < DETECTION_DISTANCE_CM);
  bool desc2Active = (distDesc2 < DETECTION_DISTANCE_CM);
  
  // Handle train detection in ascent direction
  handleTrainDetection(asc1Active, asc2Active, lastAsc1Time, lastAsc2Time, trainMovingUp);
  
  // Handle train detection in descent direction
  handleTrainDetection(desc1Active, desc2Active, lastDesc1Time, lastDesc2Time, trainMovingDown);
  
  // Check for train in ascent area (both sensors active)
  trainInAscent = (asc1Active && asc2Active);
  
  // Check for train in descent area (both sensors active)
  trainInDescent = (desc1Active && desc2Active);
  
  // Check for small object (only one sensor active in any area)
  smallObjectDetected = ((asc1Active != asc2Active) || (desc1Active != desc2Active));
  
  // Check if nothing is detected
  nothingDetected = !asc1Active && !asc2Active && !desc1Active && !desc2Active;
  
  // Update traffic lights based on the detected scenario
  updateTrafficLights();
  
  // Small delay 
  delay(100);
}

// Function to measure distance with an ultrasonic sensor
int getDistance(int trigPin, int echoPin) {
  // Clear the trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Set the trigger pin high for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Read the echo pin, convert the time to distance in cm
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;
  
  return distance;
}

// Function to handle train detection and direction
void handleTrainDetection(bool sensor1Active, bool sensor2Active, 
                         unsigned long &lastSensor1Time, unsigned long &lastSensor2Time, 
                         bool &trainMoving) {
  unsigned long currentTime = millis();
  
  // Record the activation time of each sensor
  if (sensor1Active) {
    lastSensor1Time = currentTime;
  }
  
  if (sensor2Active) {
    lastSensor2Time = currentTime;
  }
  
  // If both sensors are active within the debounce period, determine direction
  if (sensor1Active && sensor2Active) {
    // If sensor1 activated before sensor2
    if (lastSensor1Time < lastSensor2Time && 
        (lastSensor2Time - lastSensor1Time) < DEBOUNCE_DELAY) {
      trainMoving = true;
    }
    // If sensor2 activated before sensor1
    else if (lastSensor2Time < lastSensor1Time && 
             (lastSensor1Time - lastSensor2Time) < DEBOUNCE_DELAY) {
      trainMoving = true;
    }
  }
  
  // Reset direction if sensors are inactive for a while
  if (!sensor1Active && !sensor2Active && trainMoving) {
    if (currentTime - max(lastSensor1Time, lastSensor2Time) > DEBOUNCE_DELAY * 5) {
      trainMoving = false;
    }
  }
}

// Function to update traffic lights based on detected scenario
void updateTrafficLights() {
  turnOffAllLEDs();
  
  // Case 5: Nothing detected - blink green lights
  if (nothingDetected) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= BLINK_INTERVAL) {
      previousMillis = currentMillis;
      greenLedState = !greenLedState;
      digitalWrite(ASC_GREEN_LED, greenLedState);
      digitalWrite(DESC_GREEN_LED, greenLedState);
    }
    return;
  }
  
  // Case 4: Trains detected in both directions - all red
  if (trainInAscent && trainInDescent) {
    digitalWrite(ASC_RED_LED, HIGH);
    digitalWrite(DESC_RED_LED, HIGH);
    return;
  }
  
  // Case 3: Small object detected - all yellow
  if (smallObjectDetected) {
    digitalWrite(ASC_YELLOW_LED, HIGH);
    digitalWrite(DESC_YELLOW_LED, HIGH);
    return;
  }
  
  // Case 1: Train moving in ascent direction only
  if (trainInAscent && !trainInDescent) {
    digitalWrite(ASC_GREEN_LED, HIGH);
    digitalWrite(DESC_RED_LED, HIGH);
    return;
  }
  
  // Case 2: Train moving in descent direction only
  if (!trainInAscent && trainInDescent) {
    digitalWrite(ASC_RED_LED, HIGH);
    digitalWrite(DESC_GREEN_LED, HIGH);
    return;
  }
}

// Function to turn off all LEDs
void turnOffAllLEDs() {
  digitalWrite(ASC_GREEN_LED, LOW);
  digitalWrite(ASC_YELLOW_LED, LOW);
  digitalWrite(ASC_RED_LED, LOW);
  digitalWrite(DESC_GREEN_LED, LOW);
  digitalWrite(DESC_YELLOW_LED, LOW);
  digitalWrite(DESC_RED_LED, LOW);
}
