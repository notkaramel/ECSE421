ECSE 421 - Lab 3: Interrupts & I/O

# Exercise 1: A Simple Doorbell

**Task**: Write a program that rings the buzzer every time the button is pressed, using an interrupt and ISR to control the buzzer. Implement a two-tone buzz (or feel free to get more creative with your chime!).

**Question**: Will the `delay()` function work inside the ISR (interrupt service routine)? What about `micros()`? It may be helpful to try this out. Give an explanation for your answers.
- **ANSWER:** It is mentioned in https://www.arduino.cc/reference/cs/language/functions/external-interrupts/attachinterrupt/ that:

> Notes and Warnings
>
> Inside the attached function, `delay()` won’t work and the value returned by `millis()` will not increment. Serial data received while in the function may be lost. You should declare as volatile any variables that you modify within the attached function. See the section on ISRs below for more information.

With some testing, I observed that `micros()` and `millis()` print out the time of the interrupt event (I had `CHANGE` as the interrupt trigger), without being slowing down due to the tone or the push button. `delay()` also has no effect regardless of how long the wait duration is

# Exercise 2: Your Rotary Encoder, Enhanced!
## Direct accessing registers
**Task**: Write a program that outputs a counter on the serial monitor. 

## Generating a PWM signal
**Question**: Let's say you want an interrupt to occur every 500 ms. Assuming you're using Timer1, what valid combination of OCRx value and prescaler value would allow the interrupt to occur at the desired frequency? Show your calculations.
- **ANSWER**: 
    - For 500ms interrupt interval, 

**Task**: Using one of the timers for the PWM and the rotary encoder to control its duty cycle, implement a program letting you control the brightness of an LED by rotating the rotary encoder. Do not use pinMode() or analogWrite().

**Question**: Because Timer2 has only 8 bits, it overflows much more frequently than Timer1. If you were to implement the blink example from Lab 1 using Timer2, what is the least frequently that the LED could blink? What would the OCRx and prescaler values be? Show your calculations.

- **ANSWER**: 

# Exercise 3: Test your Reaction Time

# Exercise 4: Door Safety System, Revisited
