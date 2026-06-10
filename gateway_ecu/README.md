<div align="center">

# 🛡️ Drowsy Guard Autopilot System — Gateway ECU

### Central Communication & Command Routing Controller

[![Platform](https://img.shields.io/badge/Platform-STM32-blue?style=flat-square&logo=stmicroelectronics)](https://www.st.com/)
[![Language](https://img.shields.io/badge/Language-C%20%28STM32%20HAL%29-brightgreen?style=flat-square)](https://www.st.com/en/embedded-software/stm32cube-mcu-mpu-packages.html)
[![Protocol](https://img.shields.io/badge/Protocol-I2C%20%7C%20UART-orange?style=flat-square)]()
[![License](https://img.shields.io/badge/License-MIT-lightgrey?style=flat-square)]()
[![Project](https://img.shields.io/badge/Project-Graduation%20Project-purple?style=flat-square)]()

> **Gateway ECU** — The brain of the DGAS distributed network. Receives high-level commands, resolves routing decisions, and orchestrates all vehicle control subsystems through a coordinated I2C master architecture.

</div>

---

## 📌 Table of Contents

- [Project Overview](#-project-overview)
- [Role of the Gateway ECU](#-role-of-the-gateway-ecu)
- [Key Features](#-key-features)
- [System Responsibilities](#-system-responsibilities)
- [Hardware Overview](#-hardware-overview)
- [Communication Architecture](#-communication-architecture)
- [DGAS ECU Network Diagram](#-dgas-ecu-network-diagram)
- [Command Routing Table](#-command-routing-table)
- [ECU Address Map](#-ecu-address-map)
- [Firmware Workflow](#-firmware-workflow)
- [State Machine](#-state-machine)
- [Repository Structure](#-repository-structure)
- [Images](#-images)
- [Technologies Used](#-technologies-used)
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

## 🎯 Role of the Gateway ECU

The **Gateway ECU** is the central coordinator of the entire DGAS distributed architecture. It is the single point of command entry and the communication bridge between all vehicle control ECUs.

Its responsibilities span three layers:

| Layer | Role |
|---|---|
| **Command Reception** | Receives high-level commands over UART from an upstream controller or operator interface |
| **Routing & Decision** | Parses command codes and determines the correct target ECU |
| **Transmission** | Packages and dispatches commands to slave ECUs via I2C using DMA-assisted, non-blocking transfers |

Without the Gateway ECU, individual subsystems (Steering, Motion, Lighting) have no awareness of each other or any incoming directives.

---

## ✨ Key Features

- **Central I2C Master** — Manages all communication to downstream slave ECUs over a shared I2C bus
- **UART Command Interface** — Receives structured commands from an upstream system or operator terminal
- **Non-Blocking I2C Transmissions** — Uses interrupt-driven (`HAL_I2C_Master_Transmit_IT`) transfers to maintain system responsiveness
- **DMA-Assisted Communication** — Offloads data transfers to the DMA controller to reduce CPU overhead
- **Multi-ECU Routing Logic** — Translates command codes into ECU-targeted I2C transactions
- **Bus Arbitration Guard** — Prevents I2C collisions using a `ready_to_send` semaphore flag
- **UART Diagnostics** — Provides real-time status messages for debugging and system observability
- **State Machine Architecture** — Implements a clean, predictable command flow using discrete states
- **Scalable Design** — New ECU nodes can be added with minimal firmware changes

---

## 🔧 System Responsibilities

The Gateway ECU is responsible for the following operations within the DGAS system:

- **Receiving** all incoming system or operator commands through the UART interface
- **Parsing** command codes to determine the appropriate vehicle subsystem
- **Selecting** the target ECU (Steering, Motion, or Lighting) based on the command prefix
- **Validating** command parameters before transmission (e.g., angle bounds of ±90°)
- **Transmitting** ECU-specific command payloads over the I2C bus
- **Monitoring** I2C bus availability to prevent concurrent transmission conflicts
- **Reporting** transmission success or failure status via UART diagnostic messages
- **Resetting** to an idle listening state after each complete command cycle

---

## 🖥️ Hardware Overview

| Component | Specification |
|---|---|
| **Microcontroller** | STM32 (STM32F4xx Series) |
| **Communication — Primary** | I2C1 @ 100 kHz (Master Mode, 7-bit Addressing) |
| **Communication — Secondary** | USART2 @ 115200 baud (8N1, TX/RX) |
| **DMA** | DMA1 Stream 6 (I2C1 TX) |
| **Clock Source** | HSI PLL — 84 MHz System Clock |
| **GPIO** | Status LED (LD2), User Button (B1) |

---

## 📡 Communication Architecture

The Gateway ECU operates as the **I2C Master** in the DGAS network. All downstream ECUs are configured as I2C slaves and listen for addressed commands from the Gateway.

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

**Communication Stack:**

- **Physical Layer:** I2C standard mode, 100 kHz clock, open-drain signaling
- **Transport Layer:** STM32 HAL I2C driver with interrupt callbacks
- **Acceleration:** DMA1 Stream 6 offloads TX data transfers
- **Diagnostics:** All state transitions and results echo to UART terminal

---

## 🌐 DGAS ECU Network Diagram

```
╔══════════════════════════════════════════════════════════════╗
║                  DGAS Distributed ECU Network                ║
╠══════════════════════════════════════════════════════════════╣
║                                                              ║
║   [ Drowsiness Detection Module ]                            ║
║              │                                               ║
║           UART ↓ (Commands: 110 / 120 / 130 + Value)         ║
║              │                                               ║
║   ┌──────────▼─────────────┐                                 ║
║   │      GATEWAY ECU       │  ← Central Master Node          ║
║   │   STM32 | Custom PCB   │                                 ║
║   │   I2C Master @ 100kHz  │                                 ║
║   └──┬─────────┬───────────┘                                 ║
║      │  I2C    │  I2C Bus (Shared)                           ║
║   ┌──┘         └──────────────────────┐                      ║
║   │                                   │                      ║
║   ▼  Addr 0x08                        │                      ║
║   ┌──────────────┐   ▼ Addr 0x62      │ ▼ Addr 0x60          ║
║   │ STEERING ECU │   ┌─────────────┐  │ ┌──────────────┐     ║
║   │  Servo Motor │   │ MOTION ECU  │  │ │ LIGHTING ECU │     ║
║   │  Angle Ctrl  │   │  Speed/Dir  │  │ │ Hazard/Warn  │     ║
║   └──────────────┘   └─────────────┘  └─┴──────────────┘     ║
║                                                              ║
╚══════════════════════════════════════════════════════════════╝
```

> **Note:** The physical vehicle network layout is documented in [`gateway_ecu/layouts/Vehicle_Network.png`](gateway_ecu/layouts/Vehicle_Network.png) and a real hardware photo is available at [`gateway_ecu/photos/Real_Vehicle_Network.png`](gateway_ecu/photos/Real_Vehicle_Network.png).

---

## 🗺️ Command Routing Table

The Gateway ECU uses a two-phase command protocol. The first transmission selects the target subsystem; the second provides the operational value.

| Command Code | Target Subsystem | ECU | Subsequent Value |
|:---:|---|---|---|
| `110` | Steering Mode | Steering ECU (0x08) | Steering angle in degrees (`-90` to `+90`) |
| `120` | Motion Mode | Motion ECU (0x62) | Motion command value (subsystem-defined) |
| `130` | Lighting Mode | Lighting ECU (0x60) | Lighting command value (subsystem-defined) |
| Other | Unknown | — | Error response; system returns to idle |

**Example Command Sequence (Steering):**
```
TX → 110\r\n     → Gateway responds: "Servo Mode"
TX → 45\r\n      → Gateway sends angle 45° to Steering ECU via I2C
RX ← "Angle Sent"
RX ← "I2C TX Done"  (on callback)
```

---

## 📋 ECU Address Map

| ECU Node | I2C Address (7-bit) | I2C Address (Shifted 8-bit) | Function |
|---|:---:|:---:|---|
| **Steering ECU** | `0x08` | `0x10` | Controls steering servo motor angle |
| **Lighting ECU** | `0x60` | `0xC0` | Controls hazard lights and warning signals |
| **Motion ECU** | `0x62` | `0xC4` | Controls vehicle speed and direction |

> All addresses are 7-bit values. The STM32 HAL driver shifts them left by 1 bit automatically on transmission (`addr << 1`).

---

## ⚙️ Firmware Workflow

The Gateway ECU firmware implements a sequential, interrupt-safe command processing loop:

```
  ┌───────────────────────────────────┐
  │     System Initialization         │
  │  GPIO / DMA / UART / I2C / Clock  │
  └─────────────────┬─────────────────┘
                    │
                    ▼
  ┌───────────────────────────────────┐
  │     Send Ready Message via UART   │
  │     "Ready for command\r\n"       │
  └─────────────────┬─────────────────┘
                    │
                    ▼
  ┌───────────────────────────────────┐  ◄─────────────────────────┐
  │    UART Receive (Blocking)        │                            │
  │    Read byte-by-byte until \r\n   │                            │
  └─────────────────┬─────────────────┘                            │
                    │                                              │
                    ▼                                              │
  ┌───────────────────────────────────┐                            │
  │     Parse Integer from Buffer     │                            │
  └─────────────────┬─────────────────┘                            │
                    │                                              │
         ┌──────────▼──────────┐                                   │
         │    State == 0?      │                                   │
         └──┬──────────────────┘                                   │
            │ Yes                                                  │
            ▼                                                      │
  ┌─────────────────────────────┐                                  │
  │  Switch on Command Code     │                                  │
  │  110 → Steering State (1)   │                                  │
  │  120 → Motion State (2)     │                                  │
  │  130 → Lighting State (3)   │                                  │
  │  else → Unknown Command     │                                  │
  └──────────────┬──────────────┘                                  │
                 │ State set, awaiting value                       │
                 ▼                                                 │
  ┌─────────────────────────────┐                                  │
  │   Receive Value (next cmd)  │                                  │
  └──────────────┬──────────────┘                                  │
                 │                                                 │
                 ▼                                                 │
  ┌──────────────────────────────┐                                 │
  │   Validate & Transmit I2C    │                                 │
  │   HAL_I2C_Master_Transmit_IT │                                 │
  └──────────────┬───────────────┘                                 │
                 │                                                 │
                 ▼                                                 │
  ┌─────────────────────────────┐                                  │
  │  MasterTxCpltCallback fires │                                  │
  │  ready_to_send = 1          │                                  │
  │  "I2C TX Done" via UART     │                                  │
  └──────────────┬──────────────┘                                  │
                 │                                                 │
                 └─────────────────────────────────────────────────┘
                              Return to Idle
```

---

## 🔄 State Machine

The firmware uses a lightweight integer-based state machine to manage command processing safely and predictably.

### States

| State | ID | Description |
|---|:---:|---|
| **Idle** | `0` | The system is listening on UART, awaiting the first command code (`110`, `120`, or `130`). No ECU is selected. |
| **ECU Selection** | `1 / 2 / 3` | A valid command code has been received. The firmware records which ECU is targeted and sets `awaiting_value = 1`. |
| **Command Reception** | (within state 1/2/3) | The system waits for the follow-up numeric value (e.g., steering angle, motion command). |
| **I2C Transmission** | (triggered) | The value is validated and passed to `HAL_I2C_Master_Transmit_IT()`. The `ready_to_send` flag is cleared to block concurrent transmissions. |
| **Transmission Complete** | (callback) | `HAL_I2C_MasterTxCpltCallback()` fires, sets `ready_to_send = 1`, reports "I2C TX Done" via UART, and returns the system to State 0 (Idle). |

### State Diagram

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

---

## 📁 Repository Structure

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
└── README.md                     # This document
```

> **Note:** Only top-level firmware source files are listed above. Internal STM32CubeMX-generated folder structures are not expanded here.

---

## 🖼️ Images

| Image | Description | Path |
|---|---|---|
| **Vehicle Network Diagram** | Logical architecture diagram of the full DGAS ECU network | ![Vehicle_Network](./gateway_ecu/layouts/Vehicle_Network.png) |
| **Real Vehicle Network** | Hardware photograph of the physical I2C ECU network | ![Real_Vehicle_Network](./gateway_ecu/photos/Real_Vehicle_Network.png) |

---

## 🛠️ Technologies Used

| Category | Technology / Tool |
|---|---|
| **Microcontroller** | STM32F4xx |
| **Firmware Framework** | STM32 HAL (Hardware Abstraction Layer) |
| **IDE / Toolchain** | STM32CubeIDE / GCC ARM Embedded |
| **Communication Protocols** | I2C (Master, 100 kHz), USART (115200 baud) |
| **DMA** | DMA1 — I2C TX channel offload |
| **Code Generation** | STM32CubeMX |
| **Language** | C (Embedded) |
| **PCB Design** | Custom PCB (hardware-specific) |

---

## 🚀 Future Improvements

The current Gateway ECU firmware provides a solid, functional foundation. The following enhancements are planned for future iterations of the DGAS system:

### 1. Automatic Integration with Drowsiness Detection Subsystem
Currently, commands are injected manually via UART. A future revision will establish a direct hardware or software interface between the drowsiness detection module (camera/ML pipeline) and the Gateway ECU, enabling fully autonomous command injection without human intervention.

### 2. Autonomous Decision-Making Logic
The Gateway ECU will be upgraded to host decision logic capable of determining which ECUs to activate, in what sequence, and with what parameters — based on vehicle state, sensor readings, and safety protocols. This transforms the ECU from a passive router into an active decision-maker.

### 3. Fault-Tolerant Communication
Future firmware will implement I2C NACK detection, retry mechanisms, bus reset procedures, and timeout handling. If a slave ECU fails to acknowledge, the system will attempt recovery and log fault codes rather than silently failing.

### 4. ECU Health Monitoring
A heartbeat or polling mechanism will be introduced to periodically query each slave ECU for a status response. This allows the Gateway to detect offline or malfunctioning nodes and trigger safety fallbacks accordingly.

---

## 📄 License

This project is part of a graduation project submission and is made available for educational and portfolio purposes.

```
MIT License — See LICENSE file for full terms.
```

---

<div align="center">

**Drowsy Guard Autopilot System (DGAS)**
*Gateway ECU — Graduation Project*

Built with precision. Designed for safety.

</div>
