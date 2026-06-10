<div align="center">

# 🚗 Drowsy Guard Autopilot System
## Steering ECU — Firmware & Hardware

[![Platform](https://img.shields.io/badge/Platform-STM32%20Microcontroller-blue?style=for-the-badge&logo=stmicroelectronics)](https://www.st.com/en/microcontrollers-microprocessors/stm32-32-bit-arm-cortex-mcus.html)
[![Protocol](https://img.shields.io/badge/Protocol-I2C%20%7C%20Slave-green?style=for-the-badge)](https://en.wikipedia.org/wiki/I%C2%B2C)
[![Language](https://img.shields.io/badge/Language-C%20%7C%20STM32%20HAL-orange?style=for-the-badge&logo=c)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Control](https://img.shields.io/badge/Control-Closed--Loop%20%7C%20PWM%20%2B%20ADC-red?style=for-the-badge)]()
[![Hardware](https://img.shields.io/badge/PCB-Custom%20Design-purple?style=for-the-badge)](./steering_ecu/layouts/)
[![Status](https://img.shields.io/badge/Status-Active%20Development-brightgreen?style=for-the-badge)]()
[![License](https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge)](./LICENSE)

<br/>

> **Part of the Drowsy Guard Autopilot System (DGAS) — A Graduation Project**
>
> *An intelligent, distributed automotive safety system designed to protect lives when the driver can no longer control the vehicle.*

</div>

---

## 📖 Table of Contents

- [Project Overview](#-project-overview)
- [Role of the Steering ECU](#-role-of-the-steering-ecu)
- [Key Features](#-key-features)
- [System Responsibilities](#-system-responsibilities)
- [Hardware Overview](#-hardware-overview)
- [Steering Mechanism](#-steering-mechanism)
- [Engineering Design Evolution](#-engineering-design-evolution)
- [Communication Architecture](#-communication-architecture)
- [Closed-Loop Control Explanation](#-closed-loop-control-explanation)
- [Steering Control Workflow](#-steering-control-workflow)
- [Repository Structure](#-repository-structure)
- [Hardware Layout](#-hardware-layout)
- [Technologies Used](#-technologies-used)
- [Challenges and Solutions](#-challenges-and-solutions)
- [Future Improvements](#-future-improvements)
- [License](#-license)

---

## 🧠 Project Overview

The **Drowsy Guard Autopilot System (DGAS)** is a distributed, real-time automotive safety platform developed as a graduation engineering project. It is designed to continuously monitor the driver for signs of **drowsiness or loss of consciousness**. Upon detection, DGAS autonomously takes control of the vehicle to prevent accidents by:

- 🔍 Detecting drowsiness through dedicated sensor ECUs
- 🚦 Activating warning signals and hazard lighting sequences
- 🛣️ Safely steering the vehicle toward the **emergency lane**
- 🛑 Performing a **controlled, gradual stop**
- 📡 Coordinating all actions across multiple ECUs via a **Master Gateway**

DGAS is built on a **modular multi-ECU architecture**, where each Electronic Control Unit (ECU) is responsible for a specific vehicle subsystem. All ECUs communicate over a shared **I2C bus** managed by the Master/Gateway ECU.

---

## 🎯 Role of the Steering ECU

Within the DGAS architecture, the **Steering ECU** is the dedicated controller responsible for all vehicle directional control during **autonomous emergency lane maneuvers**.

When the system detects driver incapacitation, the Master/Gateway ECU initiates a safe-stop sequence and dispatches steering angle commands to guide the vehicle toward the emergency lane. The Steering ECU is the sole actuator responsible for executing these directional adjustments — it receives a target angle over I2C, continuously reads the current wheel position via potentiometer feedback, and drives the steering motor until the commanded angle is precisely achieved. This ECU is directly responsible for:

- Receiving and decoding target steering angle commands from the Gateway ECU
- Continuously measuring actual steering position via ADC-based potentiometer feedback
- Computing the position error and selecting motor drive direction automatically
- Driving the steering motor with PWM speed control until the target angle is reached
- Enforcing steering angle limits and performing safety checks throughout operation

---

## ✨ Key Features

- **Closed-Loop Steering Control** — Continuous feedback from a potentiometer-based position sensor ensures the vehicle always reaches the exact commanded steering angle
- **ADC-Based Position Measurement** — Real-time analog-to-digital conversion of potentiometer voltage provides accurate current angle calculation at all times
- **Interrupt-Driven I2C Reception** — STM32 HAL-based TWI interrupt handling ensures responsive, non-blocking reception of steering angle commands from the Gateway ECU
- **PWM Motor Speed Control** — Variable duty cycle PWM output drives the DC gearbox motor, enabling proportional speed adjustment relative to the current position error
- **Bidirectional Steering Control** — H-bridge direction control allows the steering motor to drive left or right automatically based on the sign of the computed error
- **Automatic Error Correction** — The control loop continuously corrects the steering position until the target angle is achieved, then halts the motor
- **Steering Angle Limiting & Safety Checks** — Software-enforced angle limits prevent mechanical over-travel and protect the steering mechanism during operation
- **UART Debugging & Diagnostics** — Serial diagnostic output provides real-time visibility into angle commands, feedback values, error magnitudes, and motor states during development and testing
- **STM32 HAL-Based Implementation** — Built entirely on the STM32 Hardware Abstraction Layer for clean, portable, and maintainable embedded firmware

---

## 🔧 System Responsibilities

| Responsibility | Description |
|---|---|
| Target Angle Reception | Receives target steering angle commands from the Gateway ECU via I2C interrupt |
| Position Feedback | Reads potentiometer voltage through ADC to determine current steering angle |
| Error Computation | Calculates the signed difference between target and current angle |
| Motor Direction Selection | Automatically selects CW or CCW motor direction based on error sign |
| PWM Speed Control | Drives motor at a PWM duty cycle proportional to the position error magnitude |
| Target Convergence | Continuously corrects steering position until the error is within the accepted tolerance |
| Angle Limiting | Enforces minimum and maximum steering angle boundaries to protect hardware |
| Safety Checks | Validates received commands and prevents execution of out-of-range angles |
| UART Diagnostics | Outputs real-time steering state information for debugging and system monitoring |

---

## 🔩 Hardware Overview

The Steering ECU is built around an **STM32 microcontroller** interfaced with a **DC gearbox motor**, an **H-bridge motor driver circuit**, and a **potentiometer-based position sensor**. The custom control circuit integrates all signal conditioning, PWM drive, and feedback acquisition on a compact hardware platform.

| Parameter | Value |
|---|---|
| Microcontroller | STM32 (ARM Cortex-M series) |
| Firmware Framework | STM32 HAL (Hardware Abstraction Layer) |
| Steering Actuator | DC Gearbox Motor |
| Motor Driver | H-Bridge Direction Control Circuit |
| Position Sensor | Potentiometer (analog, wiper-coupled to steering shaft) |
| Feedback Interface | ADC (Analog-to-Digital Converter) |
| Speed Control | PWM output from STM32 timer peripheral |
| Communication | I2C (STM32 HAL I2C in Slave mode) |
| Debug Interface | UART serial output |
| PCB | Custom steering control circuit |

---

## 🔄 Steering Mechanism

The steering subsystem translates high-level angle commands into precise physical wheel movement through a combination of motor actuation and real-time position feedback.

**Core components:**

- **DC Gearbox Motor** — Provides the mechanical torque required to turn the steering shaft. The gearbox reduces motor speed and multiplies output torque, enabling controlled, precise angular movement even under load.
- **H-Bridge Driver** — Controls motor current direction, allowing the firmware to command bidirectional rotation (left turn / right turn) by toggling the bridge logic outputs.
- **Potentiometer** — Mechanically coupled to the steering shaft. As the wheels turn, the potentiometer wiper position changes proportionally, producing an analog voltage that represents the current steering angle.
- **ADC** — Samples the potentiometer voltage and converts it to a digital value. The firmware maps this value to an angle in degrees for use in the control loop.
- **PWM Output** — Timer-generated PWM from the STM32 controls motor speed. The duty cycle is adjusted dynamically — higher error produces higher speed for fast correction; lower error reduces speed for smooth convergence near the target.

---

## 🔧 Engineering Design Evolution

The Steering ECU went through a deliberate design evolution during the course of the project, driven by a hardware availability constraint encountered during the final integration and testing phase.

**Original Design — Digital Servo**

The initial steering design was based on a **35 kg·cm Digital Metal Gear Servo**, which offered a self-contained position-control solution with built-in feedback and a simple PWM command interface. This approach was selected for its ease of integration and compact form factor.

**Design Adaptation — DC Motor + Potentiometer Closed-Loop System**

During final project preparation, the servo unit became unavailable due to hardware failure. Rather than treating this as a project setback, the team implemented an **engineering redesign** that yielded a more transparent and configurable steering control system:

| Aspect | Original Design | Final Design |
|---|---|---|
| Actuator | 35 kg·cm Digital Servo | DC Gearbox Motor |
| Feedback | Internal (closed inside servo) | External Potentiometer via ADC |
| Control Loop | Handled internally by servo | Implemented in firmware (explicit) |
| Speed Control | Fixed internal controller | PWM duty cycle (firmware-tunable) |
| Tunability | Limited | Full software control over gains and limits |
| Observability | Black-box | Full diagnostic visibility via UART |

**Engineering Outcome**

The redesigned system exposed the full control loop at the firmware level, providing the team with complete visibility into position error, motor direction selection, and convergence behavior — aspects that were opaque in the original servo-based approach. This adaptation demonstrates practical embedded systems engineering: identifying a constraint, evaluating alternatives, and implementing a solution that meets or exceeds the original functional requirements.

---

## 📡 Communication Architecture

The Steering ECU operates as a **dedicated I2C slave** on the DGAS vehicle bus. All steering angle commands originate from the **Master/Gateway ECU** and are addressed to this node's slave address.

```
┌──────────────────────────────────────────────────────────────────┐
│                     DGAS System Bus (I2C)                        │
│                                                                  │
│  ┌──────────────┐      SDA/SCL      ┌──────────────────────┐     │
│  │  Master /    │ ◄──────────────── │    Steering ECU      │     │
│  │  Gateway ECU │ ─────────────────►│    I2C Slave         │     │
│  └──────────────┘                   └──────────────────────┘     │
│         │                                      │                 │
│         │                       ┌──────────────┴──────────┐      │
│         │                       ▼                         ▼      │
│         │                 ┌───────────┐           ┌────────────┐ │
│         │                 │  DC Motor │           │    ADC     │ │
│         │                 │  H-Bridge │           │ Feedback   │ │
│         │                 └─────┬─────┘           └─────┬──────┘ │
│         │                       │                       │        │
│         │                       ▼                       ▼        │
│         │               Steering Shaft ◄──── Potentiometer       │
│  ┌──────┴──────┐                                                 │
│  │  Other ECUs │  (Lighting, Motion, Sensing...)                 │
│  └─────────────┘                                                 │
└──────────────────────────────────────────────────────────────────┘
```

**Communication Protocol Summary:**

| Parameter | Value |
|---|---|
| Bus Type | I2C |
| ECU Role | Slave |
| Driver | STM32 HAL I2C |
| Data Format | Target steering angle value per transaction |
| Reception Method | Interrupt-driven (HAL I2C Slave RX callback) |
| Clock Source | Configured and controlled by Master/Gateway ECU |

---

## 🔁 Closed-Loop Control Explanation

The Steering ECU implements a **position closed-loop control system** — the most fundamental and safety-critical control architecture in the DGAS steering subsystem.

```
                    Target Angle (from Gateway ECU)
                              │
                              ▼
                    ┌─────────────────┐
                    │  Error Compute  │◄─────────────────────┐
                    │  e = θ_target   │                      │
                    │    − θ_current  │                      │
                    └────────┬────────┘                      │
                             │                               │
                             ▼                               │
                    ┌─────────────────┐                      │
                    │ Direction Select│                      │
                    │  e > 0 → CW     │                      │
                    │  e < 0 → CCW    │                      │
                    └────────┬────────┘                      │
                             │                               │
                             ▼                               │
                    ┌─────────────────┐                      │
                    │  PWM Duty Cycle │                      │
                    │  ∝ |error|      │                      │
                    └────────┬────────┘                      │
                             │                               │
                             ▼                               │
                    ┌─────────────────┐                      │
                    │   DC Motor +    │                      │
                    │   H-Bridge      │                      │
                    └────────┬────────┘                      │
                             │                               │
                             ▼                               │
                    ┌─────────────────┐                      │
                    │ Steering Shaft  │                      │
                    │ (Physical Turn) │                      │
                    └────────┬────────┘                      │
                             │                               │
                             ▼                               │
                    ┌─────────────────┐                      │
                    │  Potentiometer  │                      │
                    │  ADC Readback   │──── θ_current ───────┘
                    └─────────────────┘
                    
              Loop repeats until |e| ≤ tolerance → Motor STOP
```

The control loop runs continuously after each I2C command is received. The motor is driven proportionally to the magnitude of the position error. As the steering shaft approaches the target angle, the error diminishes, motor speed reduces, and the system converges smoothly to the commanded position. When the error falls within the defined tolerance band, the motor is stopped and the system awaits the next command.

---

## ⚙️ Steering Control Workflow

The firmware follows a structured **receive → measure → compute → actuate → converge** execution cycle for every steering command processed.

```
                        ┌─────────────────────┐
                        │       Power ON      │
                        └──────────┬──────────┘
                                   │
                        ┌──────────▼───────────┐
                        │   HAL Init / MX Init │
                        │   - I2C Slave Config │
                        │   - ADC Init         │
                        │   - PWM Timer Init   │
                        │   - UART Init        │
                        └──────────┬───────────┘
                                   │
                        ┌──────────▼───────────┐
                        │  Await I2C Command   │
                        │  (HAL Interrupt)     │
                        └──────────┬───────────┘
                                   │
                        ┌──────────▼───────────┐
                        │  Receive Target      │
                        │  Steering Angle (θt) │
                        │  Validate Range      │
                        └──────────┬───────────┘
                                   │
        ┌──────────────────────────▼──────────────────────────────┐
        │                   Control Loop                          │
        │                                                         │
        │  ┌───────────────────────────────────────────────────┐  │
        │  │  1. ADC Read → Potentiometer Voltage              │  │
        │  │  2. Map ADC value → Current Angle (θc)            │  │
        │  │  3. Compute Error: e = θt − θc                    │  │
        │  │  4. Check: |e| ≤ tolerance? → STOP motor, exit    │  │
        │  │  5. Select direction: e > 0 → CW, e < 0 → CCW     │  │
        │  │  6. Set PWM duty cycle ∝ |e|                      │  │
        │  │  7. Drive motor via H-Bridge                      │  │
        │  │  8. UART log: θt, θc, e, direction, PWM           │  │
        │  │  9. Repeat from step 1                            │  │
        │  └───────────────────────────────────────────────────┘  │
        └─────────────────────────────────────────────────────────┘
                                   ▲
              ┌────────────────────┴─────────────────────────┐
              │       I2C HAL Interrupt Callback             │
              │                                              │
              │  1. HAL_I2C_SlaveRxCpltCallback fires        │
              │  2. Decode received angle byte/value         │
              │  3. Validate against angle limits            │
              │  4. Store as new target angle θt             │
              │  5. Trigger control loop execution           │
              └──────────────────────────────────────────────┘
```

**Step-by-step description:**

**1 — Initialization**
On power-up, STM32 HAL initializes all peripherals: I2C slave interface, ADC channel for potentiometer feedback, PWM timer for motor speed control, and UART for diagnostics. The system enters an idle state awaiting the first command from the Gateway ECU.

**2 — I2C Angle Reception**
When the Gateway ECU transmits a target steering angle to this slave, the STM32 HAL I2C interrupt callback fires. The received value is decoded, validated against the configured angle limits, and stored as the new target angle `θt`.

**3 — ADC Feedback Acquisition**
At each iteration of the control loop, the ADC samples the potentiometer wiper voltage. The raw ADC count is scaled and mapped to a physical steering angle in degrees, yielding the current angle `θc`.

**4 — Position Error Computation**
The signed position error is computed as `e = θt − θc`. The sign of `e` determines the required motor direction; the magnitude of `e` determines the required motor speed.

**5 — Motor Actuation**
The H-bridge direction control pins are set according to the sign of `e`. The PWM duty cycle is set proportional to `|e|`, ensuring rapid correction when far from target and smooth deceleration as the target is approached.

**6 — Target Angle Convergence**
The loop repeats continuously. When `|e|` falls within the defined tolerance band, the PWM output is disabled, the motor halts, and the system returns to idle — ready for the next steering command from the Gateway ECU.

---

## 📁 Repository Structure

```
steering_ecu/
│
├── layouts/
│   └── Screenshot 2026-06-06 184228.png       # Steering hardware layout image
│
├── firmware/
│   ├── main.c                                 # Entry point, main loop & control logic
│   ├── stm32f4xx_it.c                         # Interrupt handlers
│   ├── stm32f4xx_hal_msp.c                    # HAL MSP peripheral initialization
│   └── ...                                    # HAL drivers, ADC, PWM, I2C, UART modules
│
└── README.md                                  # This file
```

---

## 🖼️ Hardware Layout

### Steering Control Hardware

> Physical layout of the Steering ECU showing the STM32 controller, H-bridge motor driver circuit, DC gearbox motor connection, and potentiometer feedback wiring.

![Steering Hardware Layout](./layouts/Screenshot%202026-06-06%20184228.png)

---

## 🛠️ Technologies Used

| Category | Technology |
|---|---|
| Microcontroller | STM32 (ARM Cortex-M series) |
| Firmware Framework | STM32 HAL (Hardware Abstraction Layer) |
| Firmware Language | Embedded C (C99) |
| IDE / Toolchain | STM32CubeIDE / STM32CubeMX / ARM GCC |
| Communication Protocol | I2C (HAL Slave mode, interrupt-driven) |
| Position Sensing | Potentiometer + STM32 ADC |
| Motor Speed Control | PWM (STM32 Timer peripheral) |
| Motor Direction Control | H-Bridge driver circuit |
| Steering Actuator | DC Gearbox Motor |
| Debug Interface | UART serial output |
| PCB | Custom steering control circuit |

---

## 🧩 Challenges and Solutions

| Challenge | Solution |
|---|---|
| **Servo hardware failure** during final integration phase | Redesigned the steering mechanism using a DC gearbox motor with explicit closed-loop potentiometer feedback, delivering a more transparent and configurable control system |
| **Angle calibration accuracy** — mapping ADC counts to physical steering degrees | Implemented a calibration routine that maps the ADC full-scale range to the physical steering travel limits, ensuring consistent and repeatable angle control |
| **Motor overshoot** near the target angle | Applied proportional PWM scaling: duty cycle reduces as `|e|` decreases, smoothing the approach to the target and eliminating mechanical overshoot |
| **I2C command latency** vs. continuous control loop timing | Adopted interrupt-driven I2C reception that updates the target angle asynchronously, keeping the control loop running without blocking on communication events |
| **Mechanical backlash** in the gearbox affecting position accuracy | Introduced a dead-band tolerance window in the error check, preventing motor chatter around the target position while maintaining acceptable positional accuracy |
| **UART diagnostic overhead** in a time-critical loop | Configured UART transmission as non-blocking with selective logging, ensuring diagnostic output does not interfere with control loop timing |

---

## 🔮 Future Improvements

- [ ] **PID Controller Implementation** — Replace the current proportional-only control with a full PID controller to improve steady-state accuracy, reduce settling time, and reject mechanical disturbances
- [ ] **CAN Bus Migration** — Upgrade from I2C to CAN bus for higher noise immunity and reliability over the longer cable runs present in real automotive steering systems
- [ ] **Steering Angle Sensor Upgrade** — Replace the potentiometer with a contactless magnetic encoder (e.g., AS5600) for improved durability, resolution, and immunity to mechanical wear
- [ ] **Fault Detection & Reporting** — Implement motor stall detection, ADC out-of-range monitoring, and I2C timeout detection with fault codes reported back to the Gateway ECU
- [ ] **Steering Return-to-Center** — Add an automatic return-to-center command to restore straight-ahead position after the emergency maneuver completes
- [ ] **Torque Feedback Estimation** — Estimate steering load torque via motor current sensing to detect obstacles or mechanical resistance during autonomous maneuvers
- [ ] **Watchdog Timer Integration** — Add hardware WDT supervision for autonomous recovery from software hang states — essential in a safety-critical steering application
- [ ] **AUTOSAR-style Software Layering** — Refactor firmware into MCAL / BSW / Application layers to improve portability, testability, and compliance with automotive software standards

---

## 📄 License

```
MIT License

Copyright (c) 2025 DGAS Team

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
```

---

<div align="center">

**Drowsy Guard Autopilot System — Steering ECU**

*Built with precision. Designed for safety.*

[![GitHub](https://img.shields.io/badge/GitHub-DGAS%20Project-black?style=flat-square&logo=github)]()

</div>
