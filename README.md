# Single-Track Railway Traffic Control System

## Project Description

This system is designed to control and regulate train traffic on a single railway track that can be used in both directions (ascending and descending). It utilizes ultrasonic sensors to detect the presence and direction of trains, and LED traffic lights for appropriate signaling.

The project simulates a real railway traffic control system, particularly useful for single tracks in mountainous or challenging routes where bidirectional traffic must be safely controlled.

![Train Control System](https://www.tinkercad.com/things/bwjfd9sKEyU-e3ypna-periballonta-kai-efarmoges/editel)

## Functionality

The system offers:

1. **Train detection** in two directions (ascent and descent)
2. **Determination of movement direction** of trains
3. **Detection of small objects** on the tracks
4. **Visual signaling** with LED traffic lights (green, yellow, red) for each direction
5. **Management of different traffic scenarios** to prevent collisions

## Materials

- 1x Arduino (Uno or compatible)
- 4x Ultrasonic sensors (HC-SR04 or similar)
- 6x LEDs (2x green, 2x yellow, 2x red)
- 6x 220Ω resistors (for LEDs)
- Connection wires
- Breadboard or PCB for construction

## Wiring

### Ultrasonic Sensors
- **Ascent**:
  - Sensor 1: TRIG to pin 2, ECHO to pin 3
  - Sensor 2: TRIG to pin 4, ECHO to pin 5
  
- **Descent**:
  - Sensor 1: TRIG to pin 6, ECHO to pin 7
  - Sensor 2: TRIG to pin 8, ECHO to pin 9

### LEDs
- **Ascent Traffic Light**:
  - Green: pin 10
  - Yellow: pin 11
  - Red: pin 12
  
- **Descent Traffic Light**:
  - Green: pin 13
  - Yellow: pin A0
  - Red: pin A1

## Operation Scenarios

The system manages the following scenarios:

1. **Train in Ascent Direction**: Green for ascent, Red for descent
2. **Train in Descent Direction**: Red for ascent, Green for descent
3. **Small Object Detected**: Yellow in both directions
4. **Trains in Both Directions**: Red in both directions
5. **No Detection**: Blinking green in both directions

## Installation and Usage

1. Connect the components according to the wiring instructions
2. Upload the code to your Arduino
3. Power the Arduino
4. The system will automatically start operating

## Configurable Parameters

You can adjust the following parameters in the code:

- `DETECTION_DISTANCE_CM`: Maximum detection distance (default: 300cm)
- `DEBOUNCE_DELAY`: Delay to avoid false detections (default: 500ms)
- `BLINK_INTERVAL`: Interval for blinking green lights (default: 500ms)

## Code Explanation

- `setup()`: Initializes pins and serial communication
- `loop()`: Main loop that reads sensors and updates traffic lights
- `getDistance()`: Calculates distance from ultrasonic sensor
- `handleTrainDetection()`: Detects trains and determines direction
- `updateTrafficLights()`: Updates traffic lights based on current scenario
- `turnOffAllLEDs()`: Deactivates all LEDs

## TinkerCAD Simulation

You can view and test the circuit in simulation on TinkerCAD:
[TinkerCAD Simulation](https://www.tinkercad.com/things/bwjfd9sKEyU-e3ypna-periballonta-kai-efarmoges/editel)

## Future Improvements

- Add audio notification
- Implement network interface for remote monitoring
- Add data logging functionality
- Integrate GPS module for precise train location

## License

This project is distributed under the MIT License. See the `LICENSE` file for more information.

## Contact

For any questions or suggestions, you can create an issue on GitHub or contact directly.
