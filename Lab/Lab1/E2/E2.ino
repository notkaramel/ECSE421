/*
Exercise 2: Using the Rotary Encoder, the Buzzer, and the Microphone
The sensor kit contains a rotary encoder. Connect it to your Arduino board. A rotary encoder works differently from a potentiometer. You can read more about them here: https://en.wikipedia.org/wiki/Incremental_encoder. Pins "CLK" and "DT" act as the signal pins. You may need to look at their raw outputs first.

In the setup() function of the Arduino, configure the "CLK" and "DT", and "SW" pins as inputs https://www.arduino.cc/en/Reference/pinMode 

- TODO: Task: Write a program that outputs a counter on the serial monitor. The counter is initialized at 0 and is incremented by 1 for each clockwise increment of the rotary encoder. Similarly, it is decremented by 1 for each counterclockwise increment. The counter should be reset to 0 when the encoder is pressed down like a button.

In addition to simple binary values of the push-button switch, we can measure continuous values such as the temperature of a room, or the value of a knob. The Arduino Uno has six analog inputs, which convert an external voltage from an analog signal module, between 0 and 5 V, to a digital value, expressed to 10 bits of precision. The microphone is such an analog signal module. 

Now, connect the passive buzzer and the microphone to your Arduino. The passive buzzer is the one without the sticker attached to it. In the setup() function of the Arduino, configure the buzzer pin as an output and the microphone pin as an input.

- TODO: Task: Use the rotary encoder to select the buzzing frequency of the buzzer. Using the serial plotter and the microphone, find the approximate frequency at which the buzzer appears to be the loudest from the microphone's point of view.

- TODO: Task: Repeat the previous task with the other microphone module. How does the sensitivity change?

- TODO: Question: What is the smallest value of change in voltage that the analog inputs can report?  Explain your reasoning.
*/

void setup()
{

}

void loop()
{

}