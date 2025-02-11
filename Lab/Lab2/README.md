# Objectives
- Understand the operation of timers
- Understand the fundamentals of Finite State Machines (FSMs)
- Design and optimize FSMs

# Materials 
You will be using the Arduino Uno and the associated sensor kit for this lab, specifically:

- Temperature and humidity sensor
- Photoresistor
- Motion detector (PIR)
- LED
- Joystick
- Relay
- LCD

# Background
In the first lab, you gained familiarity with the Arduino environment and various sensors. In this lab, you will be creating a mini-project with some of the same components to reinforce your understanding of state machine concepts. Recall that you studied two types of state machines from your readings: a Mealy machine, which produces outputs when a transition is taken, and a Moore machine, in which outputs are produced when the machine is in a state. Also recall that for a deterministic state machine, there is at most one transition enabled by each input value for each state. If, for any state, more than one distinct transition with guards can evaluate to true in the same reaction, the state machine is said to be nondeterministic.

When designing a state machine, the first step is generally to translate a description of the problem into a state transition diagram. Based on the diagram, a state transition logic table is constructed whose columns contain inputs, current state, next state, and outputs. A circuit is then designed according to the logic in the table and tested.

As with the first lab, you'll need to connect your sensors to the Arduino. Remember to disconnect power from the Arduino before doing so.