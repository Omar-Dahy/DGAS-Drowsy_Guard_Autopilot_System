<div align="center">

# 🚗 Drowsy Guard Autopilot System (DGAS)

### A Distributed Automotive Safety Platform — Graduation Project

[![Project](https://img.shields.io/badge/Project-Graduation%20Project-purple?style=for-the-badge)]()
[![Phase](https://img.shields.io/badge/Status-Phase%201%20Complete%20%7C%20Phase%202%20Planned-brightgreen?style=for-the-badge)]()
[![Platform](https://img.shields.io/badge/Platform-AVR%20%7C%20STM32-blue?style=for-the-badge&logo=stmicroelectronics)]()
[![Protocol](https://img.shields.io/badge/Protocol-I2C%20%7C%20UART%20%7C%20nRF24L01-green?style=for-the-badge)]()
[![License](https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge)]()

*An intelligent, distributed automotive safety system designed to protect lives when the driver can no longer control the vehicle.*

</div>

---

## 📌 Table of Contents

- [Project Overview](#-project-overview)
- [Project Phases](#-project-phases)
- [Contributors](#-contributors)
- [Sponsorship and Support](#-sponsorship-and-support)
- [System Concept](#-system-concept)
- [System Architecture](#-system-architecture)
- [ECU Address Map](#-ecu-address-map)
- [Gateway ECU](#1-gateway-ecu--central-command-router)
- [Steering ECU](#2-steering-ecu--closed-loop-directional-control)
- [Motion ECU](#3-motion-ecu--propulsion-control)
- [Lighting ECU](#4-lighting-ecu--vehicle-lighting-control)
- [Driver Cabin Controller](#5-driver-cabin-controller--testing--simulation-bridge)
- [Wireless Button-Controlled LEDs](#6-wireless-button-controlled-leds-nrf24l01--early-prototype)
- [Repository Structure](#-repository-structure)
- [Technologies Used](#-technologies-used)
- [Future Improvements (Phase 2 Roadmap)](#-future-improvements-phase-2-roadmap)
- [License](#-license)

---

## 🧠 Project Overview

The **Drowsy Guard Autopilot System (DGAS)** is a distributed, real-time automotive safety platform developed as a graduation engineering project. It is designed to continuously monitor the driver for signs of **drowsiness or loss of consciousness**. Upon detection, DGAS autonomously takes control of the vehicle to prevent accidents by:

- 🔍 Detecting drowsiness through dedicated sensor modules
- 🚦 Activating warning signals and hazard lighting sequences
- 🛣️ Safely steering the vehicle toward the **emergency lane**
- 🛑 Performing a **controlled, gradual stop**
- 📡 Coordinating all actions across multiple ECUs via a **Master Gateway**

DGAS is built on a **modular multi-ECU architecture**, where each Electronic Control Unit (ECU) is responsible for a specific vehicle subsystem. All ECUs communicate over a shared **I2C bus** managed by the Gateway ECU, which itself receives high-level commands over UART from an upstream drowsiness-detection module (or, during testing, from the Driver Cabin Controller / manual interfaces).

DGAS is a **hardware-first project** — every subsystem is backed by a real, custom-designed PCB, real embedded firmware, and tested physical hardware, rather than simulation alone.

---

## 🗂️ Project Phases

This repository represents **Phase 1** of the DGAS project. Because DGAS is a graduation project with a scope large enough to involve **four international/academic sponsors**, the work has been deliberately split into two major phases spanning two academic years.

### 🔹 Phase 1 — Foundation & Subsystem Validation (Year 1) — ✅ Completed

Phase 1 focused on building and validating each subsystem independently before full system integration:

- Design and fabrication of custom PCBs for the Lighting, Motion, and Gateway ECUs
- Embedded firmware for each ECU (AVR-based Lighting and Motion ECUs, STM32-based Steering and Gateway ECUs)
- A working I2C communication architecture with the Gateway ECU acting as master and the Lighting, Motion, and Steering ECUs acting as slaves
- A closed-loop steering control system (redesigned from a servo-based approach to a DC motor + potentiometer feedback approach after a hardware failure)
- A Driver Cabin Controller test rig (Python bridge between Arduino and STM32) used to manually exercise steering, motion, and lighting commands during development
- An early wireless prototype (nRF24L01-based button/LED system) used to validate basic wireless command transmission concepts
- Initial documentation of the command sets, communication protocols, and system architecture for every ECU

### 🔹 Phase 2 — Integration, Autonomy & Refinement (Year 2) — 📅 Planned

Phase 2 will build on the validated subsystems from Phase 1 and focus on full-system integration and autonomous operation:

- Integration of the **drowsiness/driver-monitoring module** (camera/ML pipeline) directly with the Gateway ECU, removing the need for manual UART command injection
- Development of **autonomous decision-making logic** within the Gateway ECU — deciding which ECUs to activate, in what sequence, and with what parameters based on real sensor data
- Migration of the inter-ECU bus from **I2C to CAN bus** for automotive-grade noise immunity and longer cable runs
- **Fault-tolerant communication**: NACK detection, retries, bus reset, and timeout handling across all ECUs
- **ECU health monitoring** via heartbeat/polling from the Gateway ECU
- Replacing the proportional-only steering control loop with a full **PID controller**, and upgrading the potentiometer feedback to a contactless magnetic encoder
- **PWM-based dimming and Daytime Running Lights (DRL)** for the Lighting ECU, plus fault detection on lighting outputs
- **Closed-loop speed control** (encoders + PID) and smooth acceleration profiles for the Motion ECU
- **AUTOSAR-style software layering** (MCAL / BSW / Application) across all firmware for portability and maintainability
- **Watchdog timer (WDT) supervision** across all ECUs for autonomous recovery from software hang states
- Full vehicle-level integration testing of the emergency-stop, lane-change, and parking sequence end-to-end

---

## 👥 Contributors

1. Omar Dahy
2. Ayman Abohamed
3. Loay Elzayat
4. Ahmed Shawada
5. Ahmed Reda
6. Amr Ali

---

## 🤝 Sponsorship and Support

This project was developed with the support of four sponsoring organizations:

| Kafr El-Sheikh University | Kinnovia |
| :---: | :---: |
| Academic supervision and laboratory resources | Embedded systems guidance and mentorship |

| Siemens | ITIDA |
| :---: | :---: |
| Provided AI hardware platform for driver drowsiness detection | National technology and innovation support |

---

## 🧩 System Concept

DGAS continuously monitors the driver for signs of drowsiness. When drowsiness is detected, the Gateway ECU coordinates the Lighting, Motion, and Steering subsystems to safely guide the vehicle toward the emergency lane and bring it to a controlled stop.

**Design principles:**

- Distributed control with clear separation of concerns
- Modular, independently testable ECU-based architecture
- Real PCB design, fabrication, assembly, and firmware — not simulation
- Scalable architecture that allows new ECU nodes to be added with minimal changes
- Hardware-first engineering with documented design evolution and trade-offs

---

## 📡 System Architecture

The Gateway ECU is the central coordinator of the entire DGAS network. It receives high-level commands over UART (from the drowsiness-detection module, or manually during testing), determines the correct target subsystem, and dispatches single-byte (or value-based) commands over I2C to the appropriate slave ECU.

```
┌─────────────────────────────────┐
│         Upstream System         │
│  (Drowsiness Detection Module)  │
└──────────────┬──────────────────┘
               │  UART (115200 baud)
               ▼
┌─────────────────────────────────┐
│          Gateway ECU            │
│        (I2C Master Node)        │
│       STM32 — Custom PCB        │
└──────┬─────────────┬────────────┘
       │   I2C Bus   │
  ┌────┘             └──────────────────────┐
  │                  │                      │
  ▼                  ▼                      ▼
┌────────────┐  ┌────────────┐  ┌────────────────┐
│ Steering   │  │  Motion    │  │   Lighting     │
│    ECU     │  │    ECU     │  │     ECU        │
│  Addr 0x08 │  │  Addr 0x62 │  │   Addr 0x60    │
└────────────┘  └────────────┘  └────────────────┘
```

**Communication stack:**

- **Physical layer:** I2C standard mode, 100 kHz clock, open-drain signaling, plus UART (115200 baud) for command entry and diagnostics
- **Transport layer:** STM32 HAL I2C driver (Gateway, Steering) and AVR TWI peripheral (Lighting, Motion), all interrupt-driven
- **Acceleration:** DMA1 Stream 6 offloads I2C TX data transfers on the Gateway ECU
- **Diagnostics:** All state transitions and results are echoed to a UART terminal

### Two-Phase Command Protocol (Gateway ECU)

The Gateway ECU uses a two-phase command protocol: the first transmission selects the target subsystem, and the second provides the operational value.

| Command Code | Target Subsystem | ECU | Subsequent Value |
|:---:|---|---|---|
| `110` | Steering Mode | Steering ECU (0x08) | Steering angle in degrees (`-90` to `+90`) |
| `120` | Motion Mode | Motion ECU (0x62) | Motion command value (subsystem-defined) |
| `130` | Lighting Mode | Lighting ECU (0x60) | Lighting command value (subsystem-defined) |
| Other | Unknown | — | Error response; system returns to idle |

**Example command sequence (steering):**
```
TX → 110\r\n     → Gateway responds: "Servo Mode"
TX → 45\r\n      → Gateway sends angle 45° to Steering ECU via I2C
RX ← "Angle Sent"
RX ← "I2C TX Done"  (on callback)
```

---

## 🗺️ ECU Address Map

| ECU Node | I2C Address (7-bit) | I2C Address (Shifted 8-bit) | Function |
|---|:---:|:---:|---|
| **Steering ECU** | `0x08` | `0x10` | Controls steering motor angle (closed-loop) |
| **Lighting ECU** | `0x60` | `0xC0` | Controls hazard lights and warning signals |
| **Motion ECU** | `0x62` | `0xC4` | Controls vehicle speed and direction |

> All addresses are 7-bit values. The STM32 HAL driver shifts them left by 1 bit automatically on transmission (`addr << 1`).

---

## 1. Gateway ECU — Central Command Router

### Role

The **Gateway ECU** is the brain of the DGAS distributed network. It receives high-level commands, resolves routing decisions, and orchestrates all vehicle control subsystems through a coordinated I2C master architecture. Without the Gateway ECU, the Steering, Motion, and Lighting ECUs have no awareness of each other or of any incoming directives.

Its responsibilities span three layers:

| Layer | Role |
|---|---|
| **Command Reception** | Receives high-level commands over UART from an upstream controller or operator interface |
| **Routing & Decision** | Parses command codes and determines the correct target ECU |
| **Transmission** | Packages and dispatches commands to slave ECUs via I2C using DMA-assisted, non-blocking transfers |

### Key Features

- Central I2C master managing all communication to downstream slave ECUs
- UART command interface for receiving structured commands
- Non-blocking I2C transmissions via `HAL_I2C_Master_Transmit_IT`
- DMA-assisted communication to reduce CPU overhead
- Multi-ECU routing logic translating command codes into ECU-targeted I2C transactions
- Bus arbitration guard using a `ready_to_send` semaphore flag to prevent collisions
- UART diagnostics for real-time status and debugging
- Clean, predictable state-machine-based command flow
- Scalable design — new ECU nodes can be added with minimal firmware changes

### Hardware Overview

| Component | Specification |
|---|---|
| Microcontroller | STM32 (STM32F4xx Series) |
| Communication — Primary | I2C1 @ 100 kHz (Master Mode, 7-bit Addressing) |
| Communication — Secondary | USART2 @ 115200 baud (8N1, TX/RX) |
| DMA | DMA1 Stream 6 (I2C1 TX) |
| Clock Source | HSI PLL — 84 MHz System Clock |
| GPIO | Status LED (LD2), User Button (B1) |

### State Machine

The firmware uses a lightweight integer-based state machine:

| State | ID | Description |
|---|:---:|---|
| **Idle** | `0` | Listening on UART, awaiting a command code (`110`, `120`, or `130`). No ECU selected. |
| **ECU Selection** | `1 / 2 / 3` | A valid command code was received; the target ECU is recorded and `awaiting_value = 1`. |
| **Command Reception** | (within state 1/2/3) | Waiting for the follow-up numeric value (e.g., steering angle, motion command). |
| **I2C Transmission** | (triggered) | The value is validated and passed to `HAL_I2C_Master_Transmit_IT()`; `ready_to_send` is cleared. |
| **Transmission Complete** | (callback) | `HAL_I2C_MasterTxCpltCallback()` fires, sets `ready_to_send = 1`, reports "I2C TX Done" via UART, and returns to State 0 (Idle). |

```
         ┌──────────────────────────────────────────────────────┐
         │                                                      │
  ──────►│           IDLE (State 0)                             │◄───────┐
         │      Listening on UART                               │        │
         └──────────┬───────────────────────────────────────────┘        │
                    │                                                    │
         cmd = 110/120/130                                               │
                    │                                                    │
                    ▼                                                    │
         ┌──────────────────────┐                                        │
         │  ECU SELECTION       │                                        │
         │  State 1 / 2 / 3     │                                        │
         └──────────┬───────────┘                                        │
                    │                                                    │
          awaiting next value                                            │
                    │                                                    │
                    ▼                                                    │
         ┌──────────────────────┐                                        │
         │  COMMAND RECEPTION   │  ──── invalid value ────► UART error   │
         │  Validate parameter  │                                        │
         └──────────┬───────────┘                                        │
                    │                                                    │
            bus free? (ready_to_send == 1)                               │
                    │                                                    │
                    ▼                                                    │
         ┌──────────────────────┐                                        │
         │  I2C TRANSMISSION    │                                        │
         │  Transmit_IT (async) │                                        │
         │  ready_to_send = 0   │                                        │
         └──────────┬───────────┘                                        │
                    │                                                    │
          TxCpltCallback fires                                           │
                    │                                                    │
                    ▼                                                    │
         ┌──────────────────────┐                                        │
         │  TX COMPLETE         │ ───────────────────────────────────────┘
         │  ready_to_send = 1   │
         │  "I2C TX Done" UART  │
         └──────────────────────┘
```

### Repository Structure

```
gateway_ecu/
├── layouts/
│   └── Vehicle_Network.png       # Logical ECU network topology diagram
├── firmware/
│   ├── main.c                    # Application entry point, main loop, and state machine
│   ├── stm32f4xx_it.c            # Interrupt service routines
│   ├── stm32f4xx_hal_msp.c       # HAL MSP initialization (peripheral low-level init)
│   └── ...                       # Additional HAL and STM32CubeMX generated files
├── photos/
│   └── Real_Vehicle_Network.png  # Physical hardware photograph of the vehicle network
└── README.md
```

---

## 2. Steering ECU — Closed-Loop Directional Control

### Role

The **Steering ECU** is the dedicated controller responsible for all vehicle directional control during autonomous emergency lane maneuvers. When the system detects driver incapacitation, the Gateway ECU dispatches a target steering angle. The Steering ECU receives this angle over I2C, continuously reads the current wheel position via potentiometer feedback, and drives the steering motor until the commanded angle is precisely achieved.

Responsibilities:

- Receiving and decoding target steering angle commands from the Gateway ECU
- Continuously measuring actual steering position via ADC-based potentiometer feedback
- Computing the position error and automatically selecting motor drive direction
- Driving the steering motor with PWM speed control until the target angle is reached
- Enforcing steering angle limits and performing safety checks throughout operation

### Key Features

- Closed-loop position control using potentiometer feedback
- ADC-based position measurement, mapped to degrees
- Interrupt-driven I2C reception via STM32 HAL (HAL I2C Slave RX callback)
- PWM motor speed control with duty cycle proportional to position error
- Bidirectional H-bridge control (automatic CW/CCW based on error sign)
- Automatic error correction until the target angle is reached, then motor halt
- Steering angle limiting and safety checks to prevent mechanical over-travel
- UART diagnostics: real-time angle commands, feedback values, error magnitude, motor state
- Built entirely on STM32 HAL for portability and maintainability

### Hardware Overview

| Parameter | Value |
|---|---|
| Microcontroller | STM32 (ARM Cortex-M series) |
| Firmware Framework | STM32 HAL |
| Steering Actuator | DC Gearbox Motor |
| Motor Driver | H-Bridge Direction Control Circuit |
| Position Sensor | Potentiometer (analog, wiper-coupled to steering shaft) |
| Feedback Interface | ADC |
| Speed Control | PWM output from STM32 timer peripheral |
| Communication | I2C (STM32 HAL I2C in Slave mode) |
| Debug Interface | UART serial output |
| PCB | Custom steering control circuit |

### Steering Mechanism

- **DC Gearbox Motor** — provides the mechanical torque required to turn the steering shaft, with the gearbox reducing speed and multiplying torque for precise control under load
- **H-Bridge Driver** — controls motor current direction, allowing bidirectional rotation (left/right) by toggling bridge logic outputs
- **Potentiometer** — mechanically coupled to the steering shaft, produces a voltage proportional to the current steering angle
- **ADC** — samples the potentiometer voltage and converts it to a digital value, mapped to an angle in degrees
- **PWM Output** — duty cycle is adjusted dynamically: higher error produces higher speed for fast correction, lower error reduces speed for smooth convergence

### Engineering Design Evolution

The Steering ECU went through a deliberate design evolution driven by a hardware availability constraint during final integration and testing.

**Original design — digital servo:** the initial steering design was based on a 35 kg·cm digital metal-gear servo, offering a self-contained position-control solution with built-in feedback and a simple PWM command interface, chosen for ease of integration and compact form factor.

**Design adaptation — DC motor + potentiometer closed-loop system:** during final preparation, the servo unit became unavailable due to hardware failure. Rather than treating this as a setback, the team implemented an engineering redesign that yielded a more transparent and configurable control system:

| Aspect | Original Design | Final Design |
|---|---|---|
| Actuator | 35 kg·cm Digital Servo | DC Gearbox Motor |
| Feedback | Internal (closed inside servo) | External Potentiometer via ADC |
| Control Loop | Handled internally by servo | Implemented explicitly in firmware |
| Speed Control | Fixed internal controller | PWM duty cycle (firmware-tunable) |
| Tunability | Limited | Full software control over gains and limits |
| Observability | Black-box | Full diagnostic visibility via UART |

**Engineering outcome:** the redesigned system exposed the full control loop at the firmware level, giving the team complete visibility into position error, motor direction selection, and convergence behavior — aspects opaque in the original servo-based approach. This demonstrates practical embedded systems engineering: identifying a constraint, evaluating alternatives, and implementing a solution that meets or exceeds the original functional requirements.

### Closed-Loop Control Explanation

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

The control loop runs continuously after each I2C command is received. The motor is driven proportionally to the magnitude of the position error. As the steering shaft approaches the target angle, the error diminishes, motor speed reduces, and the system converges smoothly to the commanded position. When the error falls within the defined tolerance band, the motor stops and the system awaits the next command.

### Steering Control Workflow

Each steering command is processed through a **receive → measure → compute → actuate → converge** cycle:

1. **Initialization** — STM32 HAL initializes the I2C slave interface, the ADC channel for potentiometer feedback, the PWM timer for motor speed control, and UART for diagnostics. The system enters idle, awaiting the first command.
2. **I2C angle reception** — when the Gateway ECU transmits a target steering angle, the HAL I2C interrupt callback fires. The value is decoded, validated against angle limits, and stored as the new target angle `θt`.
3. **ADC feedback acquisition** — at each loop iteration, the ADC samples the potentiometer voltage and maps it to a current angle `θc`.
4. **Position error computation** — `e = θt − θc`; the sign determines direction, the magnitude determines speed.
5. **Motor actuation** — the H-bridge direction pins are set per the sign of `e`, and PWM duty cycle is set proportional to `|e|`, giving rapid correction far from target and smooth deceleration near it.
6. **Target angle convergence** — the loop repeats until `|e|` falls within tolerance, at which point PWM is disabled, the motor halts, and the system returns to idle awaiting the next command.

### Challenges and Solutions

| Challenge | Solution |
|---|---|
| Servo hardware failure during final integration | Redesigned using a DC gearbox motor with explicit closed-loop potentiometer feedback, yielding a more transparent and configurable system |
| Angle calibration accuracy (ADC counts → physical degrees) | Implemented a calibration routine mapping ADC full-scale range to physical steering travel limits |
| Motor overshoot near target angle | Proportional PWM scaling — duty cycle reduces as `|e|` decreases, eliminating mechanical overshoot |
| I2C command latency vs. continuous control loop timing | Interrupt-driven I2C reception updates the target angle asynchronously without blocking the control loop |
| Mechanical backlash in the gearbox | Dead-band tolerance window prevents motor chatter around the target position |
| UART diagnostic overhead in a time-critical loop | Non-blocking UART transmission with selective logging |

### Repository Structure

```
steering_ecu/
├── layouts/
│   └── Screenshot 2026-06-06 184228.png       # Steering hardware layout image
├── firmware/
│   ├── main.c                                 # Entry point, main loop & control logic
│   ├── stm32f4xx_it.c                         # Interrupt handlers
│   ├── stm32f4xx_hal_msp.c                    # HAL MSP peripheral initialization
│   └── ...                                    # HAL drivers, ADC, PWM, I2C, UART modules
└── README.md
```

---

## 3. Motion ECU — Propulsion Control

### Role

The **Motion ECU** is the dedicated propulsion controller for all vehicle movement operations during both normal operation and autonomous emergency parking sequences. It receives motion command bytes over I2C and immediately drives the motor outputs to execute the commanded maneuver. It is directly responsible for:

- Translating high-level motion commands into real-time motor control signals
- Managing bidirectional drive for forward propulsion and reverse maneuvers
- Executing clean, controlled deceleration and stop sequences
- Supporting independent per-motor control for differential steering assistance

### Key Features

- Interrupt-driven I2C reception (non-blocking TWI)
- Dual motor control via the Cytron MDD10A dual-channel driver
- Bidirectional drive (forward/reverse) for both motors
- Dedicated "apply speed setting" command
- Independent per-motor direction and stop commands for differential maneuvers
- Grouped stop command for emergency halt of both motors
- Startup diagnostic LED blink to confirm initialization and MCU health
- Modular firmware: clean separation of application logic, motor services, and I2C driver layers
- Custom PCB integrating the AVR MCU and motor driver interface on one board

### Hardware Overview

| Parameter | Value |
|---|---|
| Microcontroller | AVR (ATmega series) |
| PCB | Custom designed, v1.0.0 |
| Motor Driver | Cytron MDD10A — Dual Channel 10A DC |
| Communication | I2C (TWI peripheral) |
| Operating Mode | I2C Slave |
| Slave Address | `0x62` |
| Interrupt Mode | TWI Interrupt-Driven |
| Debug Interface | GPIO Debug LED |
| Motor Outputs | Dual DC Motor channels (Motor 1 & Motor 2) |
| Motor Driver Supply | 5V – 30V operating range |
| Motor Control Mode | PWM speed + direction signal |

### Motor Driver — Cytron MDD10A

| Specification | Value |
|---|---|
| Channels | 2 (Dual Channel) |
| Continuous Current per Channel | 10A |
| Operating Voltage | 5V – 30V DC |
| Control Interface | PWM + Direction signal |
| Motor Types Supported | Brushed DC Motors |
| Protection Features | Overcurrent, thermal shutdown |

Why this driver was chosen: a dual-channel package eliminates the need for two separate driver boards, 10A per channel gives sufficient headroom for the DGAS test platform's drive motors without thermal stress, the PWM + DIR interface maps directly onto AVR timer PWM outputs for clean firmware integration, the 5–30V range accommodates different motor supply configurations during development, and built-in overcurrent/thermal protection is essential for a safety-focused autonomous vehicle application.

### Motion Command Reference Table

All commands are single-byte values sent by the Gateway ECU to slave address `0x62`.

**Speed & group commands**

| Command | Hex Code | Description |
|---|---|---|
| Apply Speed Setting | `0x81` | Applies the current speed configuration to active motor outputs |
| Both Motors Clockwise | `0x82` | Drives both motors forward |
| Both Motors Counter-Clockwise | `0x83` | Drives both motors in reverse |
| Stop Both Motors | `0x84` | Immediately halts both motors |

**Motor 1 commands**

| Command | Hex Code | Description |
|---|---|---|
| Motor 1 Clockwise | `0x85` | Drives Motor 1 forward |
| Motor 1 Counter-Clockwise | `0x86` | Drives Motor 1 in reverse |
| Stop Motor 1 | `0x87` | Immediately halts Motor 1 |

**Motor 2 commands**

| Command | Hex Code | Description |
|---|---|---|
| Motor 2 Clockwise | `0x88` | Drives Motor 2 forward |
| Motor 2 Counter-Clockwise | `0x89` | Drives Motor 2 in reverse |
| Stop Motor 2 | `0x8A` | Immediately halts Motor 2 |

### Firmware Workflow

1. **Initialization** — `application_initialize()` configures the motor driver service layer, registers the TWI interrupt handler, sets the slave address to `0x62`, and enables global interrupts. The debug LED blinks at startup to confirm the firmware is running.
2. **I2C command reception (ISR)** — when the Gateway ECU addresses this slave and sends a command byte, the TWI peripheral fires an interrupt. The ISR reads `TWSR` to validate the transaction, latches the byte from `TWDR` into `rx_buffer`, and blinks the debug LED once.
3. **Command decoding (main loop)** — the `while(1)` loop polls `rx_buffer`; a non-zero value is evaluated via `switch` against the known opcode table.
4. **Motor control execution** — the motor service layer translates each decoded command into PWM and direction signals for the Cytron MDD10A, with each channel receiving an independent direction pin state and duty cycle.
5. **Buffer clear & ready state** — after execution, `rx_buffer` is cleared to signal readiness for the next command.
6. **Unknown commands** — any unrecognized opcode falls through to a safe no-op; motor outputs retain their last commanded state.

### Repository Structure

```
motion_ecu/
├── schematics/
│   └── Schematic_DGAS-ECUs-MOTION.png          # Full electrical schematic
├── layouts/
│   └── PCB_DGAS_MOTION_ECU_V1.0.0_2.png        # PCB layout (Gerber preview)
├── firmware/
│   ├── main.c                                   # Entry point, main loop & I2C ISR
│   └── ...                                      # Motor driver, I2C, and service modules
├── photos/
│   ├── PCB_3D.jpg                               # 3D render of the PCB
│   └── photo_2026-06-06_21-55-20.png            # Photograph of the manufactured PCB
└── README.md
```

---

## 4. Lighting ECU — Vehicle Lighting Control

### Role

The **Lighting ECU** is the dedicated controller for all vehicle lighting functions during both normal operation and autonomous emergency parking sequences. When the system detects driver incapacitation, the Gateway ECU orchestrates a safe-stop procedure, and the Lighting ECU receives lighting commands over I2C and executes them in real time — ensuring the vehicle clearly communicates its state to other road users through hazard warning indicators, emergency flashing modes, brake light intensity control, and front directional lighting.

### Key Features

- Interrupt-driven I2C reception, keeping the MCU responsive while awaiting commands
- Full lighting control suite: individual and grouped control of front lights, brake lights, and hazard lights
- Emergency hazard flash mode for critical safety scenarios
- Brake light intensity management (full-intensity emergency braking vs. default standby)
- Startup diagnostic LED blink (x3) to confirm successful boot
- LDR (Light Dependent Resistor) integration for ambient light awareness
- Custom PCB designed for reliable automotive-grade integration
- Modular service architecture — lighting and LDR services are independently initialized

### Hardware Overview

| Parameter | Value |
|---|---|
| Microcontroller | AVR (ATmega series) |
| PCB | Custom designed, v1.0.0 |
| Communication | I2C (TWI peripheral) |
| Operating Mode | I2C Slave |
| Slave Address | `0x60` |
| Interrupt Mode | TWI Interrupt-Driven |
| Debug Interface | GPIO — PC7 Debug LED |
| Lighting Outputs | Front Lights (L/R), Brake Lights, Hazard Lights (L/R) |
| Sensing Input | LDR (Light Dependent Resistor) |

### Command Reference Table

All commands are single-byte values sent by the Gateway ECU to slave address `0x60`.

**Front lights**

| Command | Hex Code | Description |
|---|---|---|
| Front Right Light ON | `0x80` | Activates the right front light |
| Front Right Light OFF | `0x81` | Deactivates the right front light |
| Front Left Light ON | `0x82` | Activates the left front light |
| Front Left Light OFF | `0x83` | Deactivates the left front light |
| All Front Lights ON | `0x84` | Activates both front lights |
| All Front Lights OFF | `0x85` | Deactivates both front lights |

**Brake lights**

| Command | Hex Code | Description |
|---|---|---|
| Brake Light Full Intensity | `0x86` | Activates full-brightness brake light (emergency stop) |
| Brake Light Default State | `0x87` | Restores brake light to default standby state |

**Hazard lights**

| Command | Hex Code | Description |
|---|---|---|
| Right Hazard ON | `0x88` | Activates right hazard indicator |
| Right Hazard OFF | `0x89` | Deactivates right hazard indicator |
| Left Hazard ON | `0x8A` | Activates left hazard indicator |
| Left Hazard OFF | `0x8B` | Deactivates left hazard indicator |
| All Hazards ON | `0x8C` | Activates all hazard indicators |
| All Hazards OFF | `0x8D` | Deactivates all hazard indicators |
| Emergency Hazard Mode ON | `0x8E` | Activates emergency flashing pattern |
| Emergency Hazard Mode OFF | `0x8F` | Deactivates emergency flashing pattern |

### Firmware Workflow

1. **Initialization** — lighting services, brake defaults, LDR services, and the I2C slave peripheral are configured. The debug LED blinks three times to confirm successful boot.
2. **I2C interrupt service routine** — when the Gateway ECU sends a command byte to `0x60`, the TWI peripheral fires an interrupt. The ISR reads `TWSR`, validates the condition, stores the byte in `rx_buffer`, and blinks the debug LED once.
3. **Main loop dispatch** — the `while(1)` loop continuously checks `rx_buffer`. When a recognized command is present, the corresponding lighting service function is called and `rx_buffer` is cleared.
4. **Unknown commands** — any unrecognized byte falls through to a no-op `default` case.

### Repository Structure

```
lighting_ecu/
├── schematics/
│   └── Schematic_DGAS-ECUs-Lighting.png       # Full electrical schematic
├── layouts/
│   └── PCB_DGAS_LIGHTING_ECU_V1.0.0.png       # PCB layout (Gerber preview)
├── firmware/
│   ├── application.c                           # Main application firmware
│   ├── application.h                           # Application header
│   └── ...                                     # Lighting & LDR service modules
├── photos/
│   ├── PCB_3D.jpg                              # 3D render of the PCB
│   └── PCB_Real.png                            # Photograph of the manufactured PCB
└── README.md
```

---

## 5. Driver Cabin Controller — Testing & Simulation Bridge

A Python-based bridge between an Arduino controller and the STM32 Gateway ECU, used during Phase 1 to manually exercise the steering, motion, and lighting subsystems on the driver cabin test rig.

### Hardware Setup

| Port | Device | Direction |
|------|--------|-----------|
| COM7 | Arduino | Read only (receives controller input) |
| COM10 | STM32 | Write only (sends commands) |

Both run at **115200 baud**.

### How It Works

1. The Arduino reads a physical controller and sends JSON packets over serial.
2. The Python script parses the packets and translates them into STM32 (Gateway ECU) commands.
3. The STM32 drives the cabin hardware (steering, motion, lighting).

**JSON packet fields:**

- `driver_state` — `"D"` triggers emergency stop, `"A"` is normal operation
- `rx` — Steering input → servo/steering angle command
- `buttons_high` — Motion buttons (forward, brake, etc.)
- `buttons_low` — Lighting buttons (5 toggleable lights)

### Features

- **Servo/steering control** — maps `rx` value to a steering angle command (`send_servo_angle`)
- **Motion control** — controls drive and braking (`send_motion`)
- **Lighting control** — toggles up to 5 independent light zones (`send_lighting`)
- **Emergency stop** — immediately halts motion and triggers a safety signal
- **Manual GUI** — Tkinter window with `-15 / 0 / +15` degree buttons for testing

### Files

| File | Description |
|------|-------------|
| `Driver_Cabin_script.py` | Main script |
| `Driver_Cabin_photo.jpg` | Hardware photo |

### Requirements

```bash
pip install pyserial
```

`tkinter` is included in the Python standard library.

### Running

```bash
python Driver_Cabin_script.py
```

Make sure COM7 and COM10 are available and the devices are connected before launching.

---

## 6. Wireless Button-Controlled LEDs (nRF24L01) — Early Prototype

A simple two-board wireless system used early in Phase 1 to validate the basic wireless command-transmission concept later considered for inter-ECU or remote-control communication: a transmitter reads button presses and sends them over an nRF24L01 radio module, and a receiver lights up LEDs based on the received value.

### Files

| File | Description |
|------|--------------|
| `Tx_firmware/tx_code.ino` | Transmitter — reads 3 buttons, sends values wirelessly |
| `Rx_firmware/rx_code.ino` | Receiver — controls 2 LEDs based on received values |
| `wireless/wireless_connection.png` | Wiring/connection diagram |

### Hardware

- 2x Arduino boards
- 2x nRF24L01 radio modules (CE → pin 9, CSN → pin 10, SPI)
- TX side: 3 buttons (pins 3, 4, 5, `INPUT_PULLUP`)
- RX side: 2 LEDs (pins A0, A1)

Both modules use radio address `"00002"`.

### How It Works

1. The TX board continuously reads the state of 3 buttons.
2. When a button state changes, it sends a value over the radio:
   - Button 1 → `5`
   - Button 2 → `10`
   - Button 3 → `15`
3. The RX board listens for incoming values and controls the LEDs:
   - `1` → LED1 ON
   - `0` → LED1 OFF
   - `3` → LED2 ON
   - `2` → LED2 OFF

> ⚠️ Note: TX sends `0/5/10/15`, while RX checks for `0/1/2/3`. Make sure the value mapping matches between TX and RX for the LEDs to respond correctly.

### Requirements

- Arduino IDE
- `RF24` library (by TMRh20) — install via Library Manager

### Uploading

1. Upload `tx_code.ino` to the transmitter Arduino.
2. Upload `rx_code.ino` to the receiver Arduino.
3. Power both boards and press the buttons on the TX side.

---

## 📁 Repository Structure

```
DGAS/
├── lighting_ecu/
│   ├── schematics/
│   ├── layouts/
│   ├── firmware/
│   ├── photos/
│   └── README.md
│
├── motion_ecu/
│   ├── schematics/
│   ├── layouts/
│   ├── firmware/
│   ├── photos/
│   └── README.md
│
├── steering_ecu/
│   ├── layouts/
│   ├── firmware/
│   └── README.md
│
├── gateway_ecu/
│   ├── layouts/
│   ├── firmware/
│   ├── photos/
│   └── README.md
│
├── driver_cabin_controller/
│   ├── Driver_Cabin_script.py
│   ├── Driver_Cabin_photo.jpg
│   └── README.md
│
├── wireless/
│   ├── Tx_firmware/
│   │   └── tx_code.ino
│   ├── Rx_firmware/
│   │   └── rx_code.ino
│   ├── wireless_connection.png
│   └── README.md
│
├── photos/
│   ├── system_overview.jpg
│   └── sponsors/
│       ├── kafr_el_sheikh_university.png
│       ├── kinnovia.png
│       ├── siemens.png
│       └── itida.png
│
├── integration/
│   ├── system_diagrams/
│   ├── power_distribution/
│   └── notes/
│
├── README.md   <-- This file (all-inclusive, both phases)
└── NOTES.md    <-- Personal comments and design notes
```

---

## 🛠️ Technologies Used

| Category | Technology |
|---|---|
| Microcontrollers | AVR (ATmega series) — Lighting & Motion ECUs; STM32 (ARM Cortex-M, STM32F4xx) — Steering & Gateway ECUs |
| Firmware Languages | Embedded C (C99), STM32 HAL |
| IDE / Toolchains | AVR-GCC / Atmel Studio / MPLAB; STM32CubeIDE / STM32CubeMX / ARM GCC; Arduino IDE |
| Communication Protocols | I2C / TWI (inter-ECU bus), USART/UART (command entry & diagnostics), nRF24L01 wireless (prototype) |
| Motor Driver | Cytron MDD10A — Dual Channel 10A DC |
| Position Sensing | Potentiometer + ADC (Steering ECU) |
| Motor Speed Control | PWM (AVR & STM32 timer peripherals) |
| Motor Direction Control | H-Bridge driver circuit |
| Sensing | LDR (Lighting ECU) |
| PCB Design | EDA Tool (KiCad / Altium / EasyEDA) |
| Host Tooling | Python (PySerial, Tkinter) for the Driver Cabin Controller |
| DMA | DMA1 — I2C TX channel offload (Gateway ECU) |

---

## 🔮 Future Improvements (Phase 2 Roadmap)

### Gateway ECU
- Automatic integration with the drowsiness detection subsystem (camera/ML pipeline), removing manual UART command injection
- Autonomous decision-making logic: deciding which ECUs to activate, in what sequence, and with what parameters
- Fault-tolerant communication: I2C NACK detection, retries, bus reset, and timeout handling
- ECU health monitoring via heartbeat/polling

### Steering ECU
- PID controller to replace the proportional-only control loop
- CAN bus migration for higher noise immunity and reliability
- Replace potentiometer with a contactless magnetic encoder (e.g., AS5600)
- Fault detection: motor stall, ADC out-of-range, I2C timeout, reported to the Gateway ECU
- Steering return-to-center command after emergency maneuvers
- Torque feedback estimation via motor current sensing
- Watchdog timer integration
- AUTOSAR-style software layering

### Motion ECU
- CAN bus migration
- Closed-loop speed control via encoders and a PID controller
- Motor fault detection: stall, overcurrent, fault reporting
- Smooth acceleration/deceleration ramp profiles
- Configurable named speed tiers (slow, cruise, emergency)
- EEPROM-based speed/motion profile storage
- Watchdog timer integration
- AUTOSAR-style software layering

### Lighting ECU
- CAN bus migration
- PWM-based brightness/dimming control
- Fault detection and reporting on lighting outputs (open/short circuit)
- EEPROM-based lighting configuration profiles
- AUTOSAR-style software layering
- Unit testing framework via a hardware abstraction layer (HAL)
- Watchdog timer integration
- Daytime Running Lights (DRL) support

---

## 📄 License

This project is part of a graduation project submission and is made available for educational and portfolio purposes.

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

**Drowsy Guard Autopilot System (DGAS)**

*Phase 1 of 2 — Graduation Project*

Built with precision. Designed for safety.

</div>
