# Introduction: Embedded System (Cont.)

- Flexibility of hardware design
  - Embedded system uses **specialized** hardware to save on power comsumption
  - Hardwired design: save cost at scale, very expensive at small scale (ASIC, FPGA)
  - FLOPS: Performance efficiency
  - (Cyber) Security: when everything is edged in the hardware, making it harder for external attacker to program malware

- Pareto optimality
  - (a) point: energy vs memory - "indifferent" means designing a trade-off 
  - (b) front: "dominated design points" - kinda like a front

- Thursday exercise: come up with 4 embedded systesm in regular contact
| Device | Most important design decision / Design issues | Tradeoff(s) |
| --- | ---| --- |
| Fridge | Ability to control and stablize temperature and moisture | idk |
| Card-access door scanner | Scan RFID efficiently and open the door. Must be compact & sensitive |  idk |
| Kettle | Boil water without overboiling (can stop at the right time, error-free operation) | idk |
| Oven | Regulate inner temperature at desired level | idk |

## Exercise
- "... start-up company, wrist-wearable heart rate monitor for medical patients. Intel ... 200,000 Pentium III CPUs"
- Question: **Should you use the P3 chips?**

- Dependabiity for the commercial success:
| Criterion | Importance | Design |
| --- | --- | --- |
| Reliability | <span style="color:red"> High </span> |  |
| Maintainability | High | 
| Availability | High | 
| Safety | Low | P3 is HUGE and might need cooling |
| Security | Low | 

- Efficiency:
| Criterion | |
| --- | --- |
| Energy |
| Run-time efficiency | 
| Code size |
| Weight |
| Cost | 

- **Real-time constraints**: Not completing computations within a given timeframe can result in a serious loss of the quaity providied
- A **hard** realtime constraints is made in place to prevent catastrophic outcome
