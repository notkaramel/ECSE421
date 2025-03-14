# Exercise 1: A Simple Doorbell

Consider a simple program that controls a two-tone doorbell (which you will model using the buzzer) based on the push button input. This could be done by polling the button once every loop and turning the buzzer on after every button press.

This is a common and easy to implement method. However, consider a slightly more complex example: what if you wanted your program to always catch pulses from a rotary encoder so that it never missed a pulse? It would be very tricky to write a program to do anything else, because the program would constantly need to poll the sensor lines for the encoder in order to catch pulses when they occurred. The sensor may be polled too late and the event could be missed entirely. In this situation, using an interrupt can free the microcontroller to perform other work while not missing the input, thus avoiding the constant polling that would otherwise occur in the user’s main code loop. The Arduino uses interrupts in many of its functions, as we will later see. Generating your own interrupts will allow you to build more powerful applications.

The Arduino has an `attachInterrupt()` function that takes three parameters:
- The interrupt pin number;
- The interrupt service routine;
- The mode that defines how the interrupt should be triggered, e.g., LOW when the pin has a value of 0, or HIGH when the pin has a value of 1.

More information on this function, including its usage and which pins are available for your Uno, is available from https://www.arduino.cc/reference/cs/language/functions/external-interrupts/attachinterrupt/

**Task**: Write a program that rings the buzzer every time the button is pressed, using an interrupt and ISR to control the buzzer. (Edit March 4) You should not be polling the button in the `loop()` method. Implement a two-tone buzz (or feel free to get more creative with your chime!).

**Question**: Will the `delay()` function work inside the ISR (interrupt service routine)? What about `micros()`? It may be helpful to try this out. Give an explanation for your answers.

# Exercise 2: Your Rotary Encoder, Enhanced!

In Lab 1, you implemented a counter using the rotary encoder. However, assuming that you polled the rotary encoder, you likely missed a few increments, and unnecessary wasted a lot of CPU cycles! But no more, now that you know how to use interrupts to free up some resources.

## Direct accessing registers:

Until now, we have been using methods such as pinMode() to interact with the I/O pins. However, there is a more direct way to interact with the pins by accessing the port registers. Direct port manipulation is faster, which can be very important when designing ISRs.

The breakout board allows you to access digital pins 2-8 and analog pins 1-4. These are split between 3 ports:

- Port B: digital pin 8-13
- Port C: analog inputs
- Port D: digital pins 0-7

Each port has three registers, the registers for Port D are given as follows:

- DDRD: determines if pin is INPUT/OUTPUT
- PORTD: controls whether pin is HIGH or LOW
- PIND: reads the state of INPUT pins set to input with pinMode()

The Arduino page linked below provides more details on direct port manipulation: https://docs.arduino.cc/retired/hacking/software/PortManipulation/

As mentioned above, each pin is associated with a particular bit of one of the three ports (B,C,D). The pinout diagram provides the alternative names for the pins. https://content.arduino.cc/assets/Pinout-UNOrev3_latest.pdf

It is important to note that when performing low-level actions, such as direct register access, you must consult the documentation for your specific board/microcontroller. Registers are unique to each architecture and the information provided above applies to the Arduino Uno but might not apply to other Arduino boards.

Note: It is crucial to understand bit math when addressing and writing to registers directly. If you require a refresher on this topic please refer to the following tutorial from Arduino: https://playground.arduino.cc/Code/BitMath/

**Task**: Write a program that outputs a counter on the serial monitor. The counter is initialized at 0 and is incremented by 1 for each clockwise increment of the rotary encoder. Similarly, it is decremented by 1 for each counterclockwise increment. The counter should be reset to 0 when the encoder is pressed down like a button. Use an interrupt for the rotary encoder and do NOT use `pinMode()`.

## Generating a PWM signal

You will now learn to generate a PWM signal from scratch. As an Arduino Uno board does not have a DAC, it needs another way to generate signals that vary in power, to dim an LED for instance.

In class, you have discussed the theory behind PWM and its application in microcontrollers. Arduino, specifically, uses PWM to output analog values using the analogWrite() function. This function is implemented using the built-in timers (as specified in Table 1). PWM can also be implemented on your own to obtain more control than simply using the analogWrite() function.

The “normal” Arduino IDE uses a timer to provide a “clock” to give you a count per millisecond or microsecond. These are commands that you used in the previous lab. However, you can create your own timer interrupts using any of the three internal timers:

|        | Size              | Types of Interrupts                            | Arduino functions using timer                     |
| ------ | ----------------- | ---------------------------------------------- | ------------------------------------------------- |
| Timer0 | 8 bits (0-255)    | Compare match <br> Overflow                    | delay(), millis() <br> analogWrite() for pins 5,6 |
| Timer1 | 16 bits (0-65535) | Compare match <br> Overflow <br> Input capture | servo() <br> analogWrite() for pins 9,10          |
| Timer2 | 8 bits (0-255)    | Compare match <br> Overflow                    | tone() <br> analogWrite() for pins 3,11           |

As an example, Timer1 is a 16-bit timer, meaning that it can store a maximum value of 65535. Once this value is reached, it will overflow to zero. At the Arduino Uno’s clock speed of 16 MHz, an interrupt will occur at every 65536/(16 \* 106) seconds, or every 4 ms. This is not useful if we want to interrupt, for example, once every second. Thus, to control the speed of the timer counter we make use of a prescaler. A prescaler with a value of 1 will increment the timer counter at 16 MHz (same speed), a value of 2 will increment it at 8 MHz, a value of 8 will increment it at 2 MHz, and so on. The prescaler can be set by setting some bits, which we will discuss later.

The types of interrupt represent ways in which the timer can be interrupted. A compare match generates an interrupt when the timer reaches a specific value. An overflow is generated when the timer rolls over from its max value, i.e., 65535 to 0 in the case of Timer1. An input capture generates an interrupt when a specific pin changes values.

The timers can be configured with special timer registers. The main registers, where x represents the designated timer, i.e., an integer from 0 to 2, include:
- *TCNTx*: The timer register. The actual timer value is stored here.
- *OCRx*: Output compare register.
- *ICRx*: Input capture register, available to the 16-bit timer.
- *TIMSKx*: Timer interrupt mask register. To enable or disable timer interrupts.
- *TIFRx*: Timer/counter interrupt flag register. Indicates a pending timer interrupt.

**Question**: Let's say you want an interrupt to occur every 500 ms. Assuming you're using Timer1, what valid combination of OCRx value and prescaler value would allow the interrupt to occur at the desired frequency? Show your calculations.

> Using the PWM signal that we are now generating, we want to be able to dim an LED.

**Task**: Using one of the timers for the PWM and the rotary encoder to control its duty cycle, implement a program letting you control the brightness of an LED by rotating the rotary encoder. Do not use pinMode() or analogWrite().

In your deliverable make sure to discuss the following:

- How you designed the interrupts
- Any challenges you faced
- If you were in a lab with access to more tools how would you test to ensure that your PWM code is working?

**Question**: Because Timer2 has only 8 bits, it overflows much more frequently than Timer1. If you were to implement the blink example from Lab 1 using Timer2, what is the least frequently that the LED could blink? What would the OCRx and prescaler values be? Show your calculations.

# Exercise 3: Test your Reaction Time

You have always wanted to become a Formula 1 driver. Who hasn't?! However, you know that the drivers need very fast reaction times. Therefore, you decide to create a tool to help you measure and work on your reaction speed. You also just learned about interrupts and their benefits in responsiveness, enabling you to be more accurate in your measurements!

You will therefore create a little game using the joystick, the push button, and the LCD. The basic game mechanics are as follows:

- The user is shown a symbol on the screen representing an action, either up, down, left, right, or button.
- The user needs to react accordingly, as quickly as possible, as a timer will count the duration between the display of the symbol and the user reacting.
- (Edit March 4) Set a time limit for the user to answer to as many prompts as possible, at the end of which the game is interrupted.
- At the end, the results are shown on the LCD. These include the total time and the number of wrong answers. Feel free (i.e., not required) to track extra statistics such as which instruction was the slowest/fastest to generate a reaction, which is most missed, etc.
- A button press restarts the game.

**Task**: Implement the reaction time game according to the specifications given above. Use an interrupt for the button and Arduino timer interrupt for the time limit. (Edit March 4) Read from the joystick in a non-blocking manner.

# Exercise 4: Door Safety System, Revisited

We now revisit the door safety system exercise from Lab 2. As this is an embedded system that runs 24/7, we want to minimize its power draw. However, as noted above, constantly polling consumes significant CPU resources and therefore draws power unnecessarily. Tweak your code to make use of interrupts and timers to free up some cycles.

The sensors should trigger interrupts, which should be dealt with in the ISR instead of a switch case. Implement the delays using the timers as well.

**Task**: Starting with your Lab 2 code, refine your design so that it is more efficient. Make use of timers and interrupts.
