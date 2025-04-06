# Lab 4 - Multitasking

## Materials:
- RGB LED
- SMD RGB LED
- Buzzer 
- Button
- Ultrasonic Sensor

## Prelab Activities 
- Install two libraries:
    - FreeRTOS (`arduino-cli lib install FreeRTOS`)
    - LoopTimer
        - Download the zip files named “loopTimer” 
        - Add the library in your Arduino IDE: Sketch >> Include Library >> Add .ZIP Library
        - Sample code:
        ```c
        #include "loopTimer.h"
        /* ... */
        void setup() {
            Serial.begin(9600);
        /* ... */
        }
        void loop() {
            loopTimer.check(Serial);
        /* ... */
        }
        ```

# Lab Activities
## Activity 1: Task Generation 
- **Exercise 1**: Rewrite the LED 6-color cycle program to execute the ledTask from the loop function. Have the LED change color every 500ms for a total cycle duration of 3 seconds. Using the loopTimer, report the execution time in the serial monitor. 

- **Exercise 2**: Write a new task, buzzTask, to turn on the buzzer for 300 milliseconds every 1 second and call this task from the loop() function instead of the ledTask. Using the loopTimer, report the execution time.


## Activity 2: Multitasking using millis() 
Imagine you are a traffic engineer for the MTQ (also known as Transports Québec) tasked with prototyping a new traffic light. Thanks to your knowledge of embedded systems and multitasking, you decide to create independent tasks for the controls for cars and pedestrians so that your system can be scalable and adaptable to a variety of intersections. These tasks will be described below. Within these tasks, make sure to replace any uses of the delay() function with millis() (as seen in the previous labs). Similar to Activity 1, make sure to include the loopTimer. 

- **Exercise**: Create a simple multitasking program that runs the tasks. Use the an RGB LED for each car and ped signals (i.e., the RGB LED and the SMD LED). Implement an ISR for the button. Using loopTimer, report the execution time of the application. Within each task make sure to print to the serialPort when a task is running.


### `carTask`
- Shows the green light to the cars until the pedTask requests a green phase
- When displaying a green signal, an orange (amber) signal will be displayed for 1 second before switching to red.
- Sets carRequest to false when light turns green
- Sets phase to 'ped' when the car light turns red
- May start a green phase when phase is set to 'car'

### `pedTask`

- Shows the green light to the pedestrians until the carTask requests a green phase
- When displaying a green signal, a blinking red signal will be displayed for 2 seconds before switching to solid red.
- sets pedRequest to false when light turns green
- sets phase to 'car' when the pedestrian light turns red
- May start a green phase when phase is set to 'ped'

### `carDetectTask`

- Detects cars waiting for the green light with the ultrasonic sensor, makes a measurement every second.
- If a car is detected and the car signal is not green, set the carRequest variable to true

### `buttonISR`

- Button for pedestrians to request a green light
- If the pedestrian signal is not green, set the pedRequest variable to true

## Activity 3: Multitasking using FreeRTOS 
- **Exercise**: Create a simple multitasking program that runs the tasks as in Activity 2.

Using loopTimer, report the execution time of the application.