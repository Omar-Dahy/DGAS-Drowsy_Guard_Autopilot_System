<div align="center">

<br/>

# 🛡️ Drowsy Guard Autopilot System
### DGAS — System Integration Repository

<br/>

[![System](https://img.shields.io/badge/System-Distributed%20Automotive%20Safety-critical?style=for-the-badge)](.)
[![AI](https://img.shields.io/badge/AI-Driver%20Monitoring%20%7C%20Lane%20Detection-blueviolet?style=for-the-badge)]()
[![ECUs](https://img.shields.io/badge/ECUs-Gateway%20%7C%20Steering%20%7C%20Motion%20%7C%20Lighting-blue?style=for-the-badge)]()
[![Wireless](https://img.shields.io/badge/Wireless-NRF24L01%20%7C%20~1km%20Range-green?style=for-the-badge)]()
[![Platform](https://img.shields.io/badge/Platform-STM32%20%7C%20AVR%20%7C%20Python-orange?style=for-the-badge)]()
[![Status](https://img.shields.io/badge/Status-Graduation%20Project-brightgreen?style=for-the-badge)]()
[![License](https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge)](./LICENSE)

<br/>

> *An intelligent, distributed automotive safety platform that continuously monitors the driver for signs of drowsiness or loss of consciousness — and autonomously performs a safe emergency parking maneuver when human control is no longer possible.*

<br/>

---

</div>

## 📖 Table of Contents

- [Project Overview](#-project-overview)
- [System Integration Overview](#-system-integration-overview)
- [System Architecture](#-system-architecture)
- [Driver Cabin Simulation Station](#-driver-cabin-simulation-station)
- [Vehicle Control Station](#-vehicle-control-station)
- [Wireless Communication](#-wireless-communication)
- [JSON Communication Protocol](#-json-communication-protocol)
- [AI Components](#-ai-components)
- [Driver Monitoring Pipeline](#-driver-monitoring-pipeline)
- [Lane Detection Pipeline](#-lane-detection-pipeline)
- [Autonomous Emergency Parking Workflow](#-autonomous-emergency-parking-workflow)
- [ECU Network Architecture](#-ecu-network-architecture)
- [Hardware Components](#-hardware-components)
- [Vision System](#-vision-system)
- [Power Distribution](#-power-distribution)
- [System Diagrams](#-system-diagrams)
- [Repository Structure](#-repository-structure)
- [Images](#-images)
- [Technologies Used](#-technologies-used)
- [Challenges and Engineering Decisions](#-challenges-and-engineering-decisions)
- [Future Improvements](#-future-improvements)
- [Contributors](#-contributors)
- [License](#-license)

---

## 🧠 Project Overview

The **Drowsy Guard Autopilot System (DGAS)** is a distributed, real-time automotive safety platform engineered as a graduation project. It addresses one of the most dangerous conditions in road safety: a driver who has lost the ability to control a vehicle due to drowsiness or sudden loss of consciousness.

DGAS does not merely alert the driver — it autonomously intervenes. Upon detecting an incapacitated driver, the system takes full control of the vehicle and executes a structured emergency parking sequence, steering the vehicle to the emergency lane and bringing it to a controlled stop.

**Core capabilities:**

- Real-time **driver state monitoring** via camera-based AI
- **Autonomous emergency lane maneuver** guided by lane detection AI
- **Distributed ECU architecture** for robust, modular vehicle control
- **Long-range wireless telemetry** bridging the driver cabin simulation to the vehicle
- **Remote manual override** via PS4 controller during normal operation

This repository serves as the **central system integration layer** for the complete DGAS project, encompassing architecture documentation, integration logic, system diagrams, and cross-subsystem references.

---

## 🔗 System Integration Overview

DGAS spans two physical environments connected over a wireless link and integrates five major engineering domains:

```
╔══════════════════════════════════════════════════════════════════════╗
║                  DGAS — System Integration Map                       ║
╠══════════════════════════════════════════════════════════════════════╣
║                                                                      ║
║  ┌─────────────────────────────────┐                                 ║
║  │   DRIVER CABIN SIMULATION       │                                 ║
║  │                                 │                                 ║
║  │  ┌──────────┐  ┌─────────────┐  │                                 ║
║  │  │ Driver   │  │ PS4         │  │                                 ║
║  │  │ Camera   │  │ Controller  │  │                                 ║
║  │  └────┬─────┘  └──────┬──────┘  │                                 ║
║  │       │               │         │                                 ║
║  │  ┌────▼───────────────▼──────┐  │                                 ║
║  │  │   Python Dashboard App    │  │                                 ║
║  │  │  · Drowsiness AI          │  │                                 ║
║  │  │  · JSON Command Builder   │  │                                 ║
║  │  │  · Connection Manager     │  │                                 ║
║  │  └────────────┬──────────────┘  │                                 ║
║  │               │ JSON Telemetry  │                                 ║
║  └───────────────┼─────────────────┘                                 ║
║                  │                                                   ║
║  ┌───────────────▼─────────────────┐                                 ║
║  │   WIRELESS BRIDGE               │                                 ║
║  │                                 │                                 ║
║  │  Laptop → Arduino UNO →         │                                 ║
║  │  NRF24L01 ~~~~~ NRF24L01 →      │                                 ║
║  │  Arduino UNO → Vehicle Laptop   │                                 ║
║  └───────────────┬─────────────────┘                                 ║
║                  │                                                   ║
║  ┌───────────────▼─────────────────┐                                 ║
║  │   VEHICLE CONTROL STATION       │                                 ║
║  │                                 │                                 ║
║  │  ┌──────────────────────────┐   │                                 ║
║  │  │  Vehicle Computer        │   │                                 ║
║  │  │  · Lane Detection AI     │   │                                 ║
║  │  │  · Decision Engine       │   │                                 ║
║  │  │  · Gateway Communication │   │                                 ║
║  │  └────────────┬─────────────┘   │                                 ║
║  │               │ UART            │                                 ║
║  │  ┌────────────▼─────────────┐   │                                 ║
║  │  │     GATEWAY ECU          │   │                                 ║
║  │  │   STM32 — I2C Master     │   │                                 ║
║  │  └──┬──────────┬────────────┘   │                                 ║
║  │     │  I2C     │  Bus           │                                 ║
║  │  ┌──┘────────┬┘───────┬────┐    │                                 ║
║  │  │           │             │    │                                 ║
║  │  ▼           ▼             ▼    │                                 ║
║  │ [Steering] [Motion]  [Lighting] │                                 ║
║  │   ECU       ECU        ECU      │                                 ║
║  └─────────────────────────────────┘                                 ║
║                                                                      ║
╚══════════════════════════════════════════════════════════════════════╝
```

| Domain | Technology | Repository |
|---|---|---|
| Driver Monitoring AI | Python, Camera, CV Model | `driver-monitoring` |
| Lane Detection AI | Python, Astra Pro Camera, CV Model | `lane-detection` |
| Python Dashboard | Python Desktop App, PS4 Controller | `cabin-simulator` |
| Gateway ECU | STM32, UART, I2C Master | `gateway-ecu` |
| Steering ECU | STM32, Closed-Loop PWM, ADC | `steering-ecu` |
| Motion ECU | AVR, Cytron MDD10A, I2C Slave | `motion-ecu` |
| Lighting ECU | AVR, I2C Slave, Hazard/Brake Control | `lighting-ecu` |
| Wireless Bridge | Arduino UNO, NRF24L01 | `wireless` |
| **System Integration** | **Architecture & Documentation** | **← This Repository** |

---

## 🏗️ System Architecture

DGAS is organized into two physical stations, linked by a long-range wireless communication subsystem and unified through a centralized ECU network.

### High-Level Architecture

```
┌──────────────────────────────────────────────────────────────────────┐
│                        DGAS Architecture                             │
├─────────────────────────────┬────────────────────────────────────────┤
│  DRIVER CABIN               │  VEHICLE SIDE                          │
│                             │                                        │
│  ┌─────────────┐            │          ┌─────────────────────────┐   │
│  │  Driver     │            │          │  Vehicle Computer       │   │
│  │  Camera     │───────┐    │          │                         │   │
│  └─────────────┘       │    │          │  ┌───────────────────┐  │   │
│                        │    │          │  │  Lane Detection   │  │   │
│  ┌─────────────┐       ▼    │          │  │  AI               │  │   │
│  │  PS4        │  ┌───────┐ │          │  └───────────────────┘  │   │
│  │  Controller │─►│Python │ │          │  ┌───────────────────┐  │   │
│  └─────────────┘  │ App   │ │  ~~~~~~  │  │  Decision Engine  │  │   │
│                   └───┬───┘ │  NRF24   │  └─────────┬─────────┘  │   │
│                       │     │          │            │ UART       │   │
│  ┌─────────────┐      │     │          └────────────┼────────────┘   │
│  │  Arduino +  │◄─────┘     │                       │                │
│  │  NRF24L01   │            │          ┌────────────▼────────────┐   │
│  └─────────────┘            │          │     Gateway ECU         │   │
│                             │          │  STM32 — I2C Master     │   │
│                             │          └──────┬──────┬───────────┘   │
│                             │              I2C│      │Bus            │
│                             │       ┌─────────┘      └─────────┐     │
│                             │       ▼                          ▼     │
│                             │  ┌─────────┐  ┌─────────┐  ┌─────────┐ │
│                             │  │Steering │  │ Motion  │  │Lighting │ │
│                             │  │  ECU    │  │  ECU    │  │  ECU    │ │
│                             │  │STM32    │  │  AVR    │  │  AVR    │ │
│                             │  │0x08     │  │  0x62   │  │  0x60   │ │
│                             │  └─────────┘  └─────────┘  └─────────┘ │
└─────────────────────────────┴────────────────────────────────────────┘
```

### Operational Modes

| Mode | Description | Control Authority |
|---|---|---|
| **Normal Driving** | Driver operates vehicle via PS4 controller and Python dashboard | Human (PS4 + Dashboard) |
| **Monitoring** | AI continuously evaluates driver state in background | Monitoring AI (passive) |
| **Alert** | Drowsiness indicators trigger warnings before intervention | System (warnings only) |
| **Autonomous Emergency** | Driver incapacitation confirmed — system takes full control | DGAS Autonomous |

---

## 🖥️ Driver Cabin Simulation Station

The **Driver Cabin Simulation Station** is a Python desktop application that simulates a vehicle cockpit interface. It serves as the human-machine interface during normal operation and as the drowsiness data source during the monitoring phase.

### Application Layout

```
┌────────────────────────────────────────────────────────────────────┐
│                    DGAS — Cabin Simulator                          │
├───────────────────────────────┬────────────────────────────────────┤
│         LEFT PANEL            │           RIGHT PANEL              │
│                               │                                    │
│  ┌───────────────────────┐    │    ┌───────────────────────────┐   │
│  │ PS4 Controller        │    │    │ JSON Command Payload      │   │
│  │ Button Visualization  │    │    │                           │   │
│  │                       │    │    │  {                        │   │
│  │  [ △ ] [ ○ ] [ □ ]   │    │    │    "driver_state": ...,   │   │
│  │     [ × ]             │    │    │    "steering": ...,       │   │
│  │  L1/L2    R1/R2       │    │    │    "throttle": ...,       │   │
│  │  D-PAD    STICKS      │    │    │    "connection": ...      │   │
│  └───────────────────────┘    │    │  }                        │   │
│                               │    └───────────────────────────┘   │
│  ┌───────────────────────┐    │                                    │
│  │ Control Bars          │    │    ┌───────────────────────────┐   │
│  │                       │    │    │  [Connect]  [Disconnect]  │   │
│  │  Throttle  ████░░░░   │    │    │  [Activate Camera]        │   │
│  │  Steering  ░░░████░   │    │    └───────────────────────────┘   │
│  │  Brake     ░░░░░░██   │    │                                    │
│  └───────────────────────┘    │    ┌───────────────────────────┐   │
│                               │    │  Live Camera Feed         │   │
│  ┌───────────────────────┐    │    │                           │   │
│  │ Vehicle Control       │    │    │  ┌─────────────────────┐  │   │
│  │ Indicators            │    │    │  │                     │  │   │
│  │                       │    │    │  │  Driver Frame +     │  │   │
│  │  Speed:  ██░░░░░░░    │    │    │  │  State Overlay      │  │   │
│  │  State:  ALERT        │    │    │  │  [DROWSY DETECTED]  │  │   │
│  └───────────────────────┘    │    │  └─────────────────────┘  │   │
│                               │    └───────────────────────────┘   │
└───────────────────────────────┴────────────────────────────────────┘
```

### Key Features

| Feature | Description |
|---|---|
| Driver Camera Support | Activates and streams the driver-facing camera for AI monitoring |
| Drowsiness AI Integration | Real-time driver state classification overlaid on the camera feed |
| PS4 Controller Integration | Full gamepad input capture mapped to vehicle control commands |
| Real-Time Dashboard | Live visualization of control inputs, state indicators, and telemetry |
| JSON Command Generation | Packages all inputs into structured JSON telemetry packets |
| Vehicle Connection Manager | Connect/Disconnect to the vehicle wireless receiver on demand |
| Camera Visualization | Live frame display with annotated driver state classification |

---

## 🚗 Vehicle Control Station

The **Vehicle Control Station** is the vehicle-side computing environment. It receives incoming telemetry from the cabin simulation, processes it through the AI decision pipeline, and issues precise commands to the ECU network.

### Responsibilities

```
┌─────────────────────────────────────────────────────────┐
│              Vehicle Control Station                    │
│                                                         │
│  ┌───────────────┐     JSON      ┌──────────────────┐   │
│  │  Wireless     │─────────────► │  Command Parser  │   │
│  │  Receiver     │               └────────┬─────────┘   │
│  │  (NRF24L01)   │                        │             │
│  └───────────────┘                        ▼             │
│                               ┌───────────────────────┐ │
│  ┌───────────────┐            │  Decision Engine      │ │
│  │  Astra Pro    │            │                       │ │
│  │  Depth Camera │──────────► │  · Normal Mode        │ │
│  └───────────────┘            │  · Emergency Mode     │ │
│                               │  · Override Logic     │ │
│  ┌───────────────┐            └──────────┬────────────┘ │
│  │  Lane         │                       │              │
│  │  Detection AI │──────────────────────►│              │
│  └───────────────┘                       │              │
│                                          │ UART         │
│                               ┌──────────▼────────────┐ │
│                               │    Gateway ECU        │ │
│                               └───────────────────────┘ │
└─────────────────────────────────────────────────────────┘
```

### Key Features

| Feature | Description |
|---|---|
| Wireless Telemetry Reception | Receives JSON packets from cabin simulator via NRF24L01 |
| Driver State Processing | Interprets incoming drowsiness classification from the cabin AI |
| Lane Detection AI | Runs real-time lane tracking using the Astra Pro depth camera |
| Decision Making Engine | Determines operational mode (normal / emergency) from combined inputs |
| Motion Coordination | Sequences ECU commands to produce smooth, controlled vehicle movement |
| Gateway ECU Communication | Dispatches parsed commands to Gateway ECU over UART |

---

## 📡 Wireless Communication

### Architecture

DGAS uses a dual-Arduino, dual-NRF24L01 wireless bridge to transmit driver telemetry from the cabin simulation to the vehicle computer.

```
┌─────────────────────────────────────────────────────────────────────┐
│                  Wireless Communication Bridge                      │
│                                                                     │
│  ┌─────────────────┐                       ┌─────────────────────┐  │
│  │  TRANSMIT SIDE  │                       │   RECEIVE SIDE      │  │
│  │                 │                       │                     │  │
│  │  Driver Laptop  │                       │  Vehicle Laptop     │  │
│  │       │         │                       │       ▲             │  │
│  │       │ Serial  │                       │       │ Serial      │  │
│  │       ▼         │                       │       │             │  │
│  │  Arduino UNO    │                       │  Arduino UNO        │  │
│  │       │         │                       │       ▲             │  │
│  │       │ SPI     │                       │       │ SPI         │  │
│  │       ▼         │                       │       │             │  │
│  │  NRF24L01       │    ~~~ RF Link ~~~    │  NRF24L01           │  │
│  │  (Transmitter)  │ ────────────────────► │  (Receiver)         │  │
│  └─────────────────┘                       └─────────────────────┘  │
│                                                                     │
│  Link Range: Up to approximately 1 km under suitable conditions     │
│  Protocol:   NRF24L01 proprietary RF — 2.4 GHz band                 │
│  Direction:  Unidirectional (Cabin → Vehicle)                       │
└─────────────────────────────────────────────────────────────────────┘
```

### Communication Specifications

| Parameter | Value |
|---|---|
| Module | NRF24L01+ (Long Range variant) |
| Frequency Band | 2.4 GHz ISM |
| Maximum Range | ~1 km (line of sight, suitable conditions) |
| Protocol | NRF24L01 ShockBurst™ |
| Host Interface | SPI |
| Bridge Controller | Arduino UNO (× 2) |
| Data Format | JSON packets (ASCII) |
| Direction | Cabin → Vehicle (unidirectional telemetry) |

---

## 📦 JSON Communication Protocol

All telemetry between the Driver Cabin and Vehicle Control Station is encoded as structured JSON packets. This enables extensibility, human readability during development, and straightforward parsing on the vehicle side.

### Packet Structure

```json
{
  "driver_state": {
    "status": "DROWSY | ALERT | UNCONSCIOUS",
    "confidence": 0.93
  },
  "controller": {
    "left_stick_x": 0.42,
    "left_stick_y": -0.18,
    "right_stick_x": 0.00,
    "throttle": 0.75,
    "brake": 0.00,
    "buttons": {
      "cross": false,
      "circle": false,
      "triangle": false,
      "square": false,
      "l1": false,
      "r1": false
    }
  },
  "vehicle_command": {
    "steering_angle": 12,
    "speed": 0.75,
    "direction": "FORWARD"
  },
  "connection": {
    "status": "CONNECTED",
    "timestamp": 1700000000
  },
  "system_status": {
    "camera_active": true,
    "ai_active": true,
    "emergency_mode": false
  }
}
```

### Field Reference

| Field Group | Key Fields | Description |
|---|---|---|
| `driver_state` | `status`, `confidence` | AI classification output from cabin monitoring |
| `controller` | sticks, throttle, brake, buttons | PS4 controller input values |
| `vehicle_command` | `steering_angle`, `speed`, `direction` | Derived vehicle control commands |
| `connection` | `status`, `timestamp` | Session state and packet timestamp |
| `system_status` | `camera_active`, `ai_active`, `emergency_mode` | System health flags |

---

## 🤖 AI Components

DGAS integrates two independent AI pipelines — one on the driver side for monitoring, one on the vehicle side for navigation.

```
┌──────────────────────────────────────────────────────────────┐
│                      DGAS AI Architecture                    │
├──────────────────────────────┬───────────────────────────────┤
│   DRIVER MONITORING AI       │   LANE DETECTION AI           │
│   (Cabin Simulation Side)    │   (Vehicle Side)              │
│                              │                               │
│  Input: Camera Frame         │  Input: Astra Pro Feed        │
│  Output: Driver State        │  Output: Lane Boundaries      │
│                              │                               │
│  · Drowsiness Detection      │  · Lane Line Tracking         │
│  · Fatigue Estimation        │  · Road Boundary Mapping      │
│  · Consciousness Assessment  │  · Vehicle Path Estimation    │
│  · Alert / Warning Trigger   │  · Emergency Lane Targeting   │
│                              │                               │
└──────────────────────────────┴───────────────────────────────┘
```

---

## 👁️ Driver Monitoring Pipeline

The Driver Monitoring AI operates continuously on the cabin simulation laptop, analyzing each camera frame to classify the driver's state.

```
Camera Frame
     │
     ▼
┌────────────────────────┐
│   Face Detection       │ ── Localizes face region in frame
└────────────┬───────────┘
             │
             ▼
┌────────────────────────┐
│   Landmark Extraction  │ ── Eye aspect ratio, head pose, mouth state
└────────────┬───────────┘
             │
             ▼
┌────────────────────────┐
│   State Classifier     │ ── Model inference on extracted features
└────────────┬───────────┘
             │
     ┌───────┴────────┐
     ▼                ▼
  ALERT            DROWSY / UNCONSCIOUS
     │                │
     │                ▼
     │         ┌─────────────────────┐
     │         │  Trigger Emergency  │
     │         │  Protocol           │
     │         └─────────────────────┘
     ▼
 Normal Operation
```

**Output states:**

| State | Description | System Response |
|---|---|---|
| `ALERT` | Driver is attentive and responsive | Normal driving mode continues |
| `DROWSY` | Fatigue indicators detected | Warning signals activated |
| `UNCONSCIOUS` | Driver is unresponsive | Autonomous emergency mode engaged |

---

## 🛣️ Lane Detection Pipeline

The Lane Detection AI runs on the vehicle-side computer, processing depth and visual data from the Astra Pro camera to identify road geometry and guide the emergency lane maneuver.

```
Astra Pro Camera
     │
     ├─ RGB Frame
     │       │
     │       ▼
     │  ┌──────────────────────┐
     │  │  Lane Line Detection │ ── Edge detection, Hough transform
     │  └──────────┬───────────┘
     │             │
     └─ Depth Map  │
             │     │
             ▼     ▼
     ┌──────────────────────┐
     │  Road Boundary Map   │ ── 2D lane boundary estimation
     └──────────┬───────────┘
                │
                ▼
     ┌──────────────────────┐
     │    Path Estimator    │ ── Target trajectory computation
     └──────────┬───────────┘
                │
                ▼
     Steering Angle Command → Gateway ECU → Steering ECU
```

---

## 🚨 Autonomous Emergency Parking Workflow

When the Driver Monitoring AI confirms an incapacitated driver, DGAS executes a structured emergency parking sequence coordinated across all ECU nodes.

```
┌──────────────────────────────────────────────────────────────────────┐
│                AUTONOMOUS EMERGENCY PARKING SEQUENCE                 │
└──────────────────────────────────────────────────────────────────────┘

Phase 1 — Detection
─────────────────────────────────────────────────────────────────────
  Driver Monitoring AI
        │
        ▼ DROWSY / UNCONSCIOUS confirmed
  Emergency Flag Set in JSON Payload
        │
        ▼
  Vehicle Computer receives emergency state

Phase 2 — Warning Activation
─────────────────────────────────────────────────────────────────────
  Decision Engine → Gateway ECU (UART: 130)
        │
        ▼ Lighting ECU (I2C: 0x60)
  ┌─────────────────────────────────┐
  │  All Hazard Lights ON  (0x8C)   │
  │  Emergency Flash Mode  (0x8E)   │
  └─────────────────────────────────┘

Phase 3 — Deceleration
─────────────────────────────────────────────────────────────────────
  Decision Engine → Gateway ECU (UART: 120)
        │
        ▼ Motion ECU (I2C: 0x62)
  ┌─────────────────────────────────┐
  │  Controlled Deceleration        │
  │  Speed Ramp-Down Sequence       │
  │  Brake Light Full Intensity     │
  │  (Lighting ECU: 0x86)           │
  └─────────────────────────────────┘

Phase 4 — Lane Maneuver
─────────────────────────────────────────────────────────────────────
  Lane Detection AI → Path Estimator
        │
        ▼ Computed target angle (e.g., -35°)
  Decision Engine → Gateway ECU (UART: 110)
        │
        ▼ Steering ECU (I2C: 0x08)
  ┌─────────────────────────────────┐
  │  Closed-Loop Steering Control   │
  │  Potentiometer Feedback ADC     │
  │  PWM Motor Drive to Target      │
  │  Error Correction Loop Active   │
  └─────────────────────────────────┘

Phase 5 — Safe Stop
─────────────────────────────────────────────────────────────────────
  Motion ECU: All Motors STOP (0xA6 / 0xA7)
  Steering ECU: Wheels Centered
  Lighting ECU: Hazards remain active

  ┌─────────────────────────────────┐
  │     VEHICLE SAFELY STOPPED      │
  │     Emergency Lane Reached      │
  │     Hazard Lights Blinking      │
  └─────────────────────────────────┘
```

---

## ⚡ ECU Network Architecture

All vehicle actuator ECUs communicate over a shared **I2C bus** with the **Gateway ECU** acting as the I2C Master. Each slave ECU has a unique 7-bit address and responds to single-byte command opcodes.

### ECU Network Diagram

```
╔══════════════════════════════════════════════════════════════════════╗
║                   DGAS Distributed ECU Network                       ║
╠══════════════════════════════════════════════════════════════════════╣
║                                                                      ║
║   [ Vehicle Computer ]                                               ║
║        │                                                             ║
║     UART (115200 baud)                                               ║
║        │                                                             ║
║   ┌────▼───────────────────────────────────────────┐                 ║
║   │               GATEWAY ECU                      │                 ║
║   │   STM32F4 — I2C Master @ 100 kHz               │                 ║
║   │   DMA-Assisted Non-Blocking Transmissions      │                 ║
║   │   Bus Arbitration Guard (ready_to_send flag)   │                 ║
║   └──────────────┬────────────────┬────────────────┘                 ║
║                  │   I2C Bus      │   (SDA / SCL)                    ║
║       ┌──────────┘───┌────────────┘───┌                              ║
║       ▼              ▼                ▼                              ║
║  ┌──────────┐  ┌──────────┐   ┌─────────────┐                        ║
║  │STEERING  │  │ MOTION   │   │  LIGHTING   │                        ║
║  │  ECU     │  │  ECU     │   │    ECU      │                        ║
║  │          │  │          │   │             │                        ║
║  │ STM32    │  │  AVR     │   │   AVR       │                        ║
║  │ Addr:    │  │ Addr:    │   │  Addr:      │                        ║
║  │ 0x08     │  │ 0x62     │   │  0x60       │                        ║
║  │          │  │          │   │             │                        ║
║  │ Servo    │  │ Cytron   │   │ Front/Brake │                        ║
║  │ Motor    │  │ MDD10A   │   │ Hazard LEDs │                        ║
║  │ Closed-  │  │ Dual DC  │   │ LDR Sensor  │                        ║
║  │ Loop ADC │  │ Motor    │   │             │                        ║
║  └──────────┘  └──────────┘   └─────────────┘                        ║
╚══════════════════════════════════════════════════════════════════════╝
```

### ECU Address Map

| ECU | I2C Address (7-bit) | Platform | Primary Function |
|---|:---:|---|---|
| **Gateway ECU** | Master | STM32F4xx | Command routing, I2C arbitration, UART bridge |
| **Steering ECU** | `0x08` | STM32 | Closed-loop steering angle control |
| **Motion ECU** | `0x62` | AVR (ATmega) | DC motor speed and direction |
| **Lighting ECU** | `0x60` | AVR (ATmega) | Hazard, brake, and front lighting |

### Gateway Command Routing

| UART Command | Routed To | ECU Address | Function |
|:---:|---|:---:|---|
| `110` + angle value | Steering ECU | `0x08` | Set steering angle (−90° to +90°) |
| `120` + motion code | Motion ECU | `0x62` | Set speed, direction, or stop |
| `130` + lighting code | Lighting ECU | `0x60` | Set lighting state or flash mode |

---

## 🔩 Hardware Components

### System-Level Bill of Materials

| Component | Specification | Role |
|---|---|---|
| Gateway ECU MCU | STM32F4xx (84 MHz, Custom PCB) | Central I2C Master, UART bridge |
| Steering ECU MCU | STM32 (Custom PCB) | Closed-loop steering control |
| Motion ECU MCU | AVR ATmega (Custom PCB) | DC motor drive |
| Lighting ECU MCU | AVR ATmega (Custom PCB) | Vehicle lighting control |
| Motor Driver | Cytron MDD10A — 2×10A DC | Dual-channel propulsion motor drive |
| Wireless Module | NRF24L01+ Long Range (×2) | Cabin-to-vehicle telemetry |
| Wireless Bridge | Arduino UNO (×2) | Serial-to-RF gateway |
| Depth Camera | Astra Pro (Orbbec) | Lane detection perception |
| Position Sensor | Rotary Potentiometer | Steering angle feedback (ADC) |
| Ambient Sensor | LDR (Light Dependent Resistor) | Adaptive lighting awareness |
| Driver Camera | Standard USB / Integrated | Driver monitoring feed |
| Controller | Sony PS4 DualShock 4 | Manual vehicle control input |

### Custom PCB Summary

Each actuator ECU in DGAS uses a purpose-designed PCB:

| ECU | PCB Version | Key Interfaces |
|---|---|---|
| Gateway ECU | v1.0.0 | UART (USART2), I2C1, DMA1 |
| Steering ECU | v1.0.0 | I2C Slave, ADC, PWM (TIM), H-Bridge |
| Motion ECU | v1.0.0 | I2C Slave (0x62), PWM, Cytron MDD10A |
| Lighting ECU | v1.0.0 | I2C Slave (0x60), GPIO (LEDs), LDR ADC |

> PCB schematics, layouts, 3D renders, and manufactured board photos are maintained in each ECU's dedicated repository under `schematics/`, `layouts/`, and `photos/`.

---

## 🎥 Vision System

### Astra Pro Depth Camera (Vehicle Side)

The **Orbbec Astra Pro** serves as the primary perception sensor for the vehicle-side lane detection AI.

| Parameter | Specification |
|---|---|
| Model | Orbbec Astra Pro |
| Depth Range | 0.6 m – 8 m |
| Sensing Technology | Structured light depth sensing |
| Output | RGB + Depth stream |
| Interface | USB 3.0 |
| AI Use | Lane detection, road boundary mapping |

### Driver Monitoring Camera (Cabin Side)

| Parameter | Specification |
|---|---|
| Type | USB or integrated webcam |
| Placement | Driver-facing, within cabin simulation station |
| AI Use | Facial landmark detection, drowsiness classification |
| Output | RGB video stream |

---

## ⚡ Power Distribution

> 📁 Detailed power distribution schematics and wiring documentation are maintained in [`./power_distribution/`](./power_distribution/).

### Power Architecture Overview

```
[ Vehicle Battery / Power Supply ]
          │
          ├──► Vehicle Laptop (12V or USB-C)
          │
          ├──► Cytron MDD10A Motor Driver (5V–30V)
          │         │
          │         └──► Drive Motors (Motor 1 & Motor 2)
          │
          ├──► ECU Power Rail (5V regulated)
          │         │
          │         ├──► Gateway ECU (STM32)
          │         ├──► Steering ECU (STM32)
          │         ├──► Motion ECU (AVR)
          │         └──► Lighting ECU (AVR)
          │
          └──► Lighting Loads (Front, Brake, Hazard)

[ Cabin Laptop ]
          │
          └──► Arduino UNO TX (USB)
                    │
                    └──► NRF24L01 (3.3V from Arduino)
```

> ⚠️ **Placeholder:** Detailed per-rail voltage levels, current budgets, and protection circuitry are documented in [`./power_distribution/`](./power_distribution/). Complete this section with measured current draw values and fuse ratings upon system validation.

---

## 📊 System Diagrams

All architectural, communication, and wiring diagrams are maintained in [`./system_diagrams/`](./system_diagrams/).

| Diagram | Description | File |
|---|---|---|
| Full System Architecture | End-to-end DGAS integration diagram | `system_diagrams/full_architecture.png` |
| ECU Network Topology | I2C bus layout and ECU addressing | `system_diagrams/ecu_network.png` |
| Wireless Communication Flow | NRF24L01 bridge data flow | `system_diagrams/wireless_flow.png` |
| Emergency Parking Sequence | Phase-by-phase autonomous parking flow | `system_diagrams/emergency_sequence.png` |
| Power Distribution | Voltage rails and current paths | `system_diagrams/power_distribution.png` |
| AI Pipeline | Driver monitoring and lane detection flow | `system_diagrams/ai_pipeline.png` |

> **Note:** Populate `./system_diagrams/` with exported diagrams from your schematic and design tools. Recommended formats: PNG (≥1920×1080) or SVG for vector clarity.

---

## 📁 Repository Structure

```
dgas-integration/
│
├── integration/
│   ├── system_overview.md          # Narrative system integration description
│   ├── interface_contracts.md      # Inter-subsystem API and protocol definitions
│   └── startup_sequence.md        # System boot and initialization order
│
├── notes/
│   ├── engineering_decisions.md    # Design rationale and tradeoff documentation
│   ├── testing_log.md             # Integration test records and results
│   └── known_issues.md            # Active issues and resolution tracking
│
├── power_distribution/
│   ├── power_architecture.md       # Power rail design and current budget
│   ├── wiring_diagram.png          # Full wiring schematic
│   └── fuse_and_protection.md      # Protection circuit documentation
│
├── system_diagrams/
│   ├── full_architecture.png       # End-to-end system architecture
│   ├── ecu_network.png             # ECU I2C network topology
│   ├── wireless_flow.png           # Wireless communication data flow
│   ├── emergency_sequence.png      # Autonomous parking sequence diagram
│   ├── ai_pipeline.png             # Driver monitoring + lane detection flow
│   └── power_distribution.png      # Power distribution diagram
│
└── README.md                       # This file — central documentation
```

---

## 🖼️ Images

### Python Dashboard Screenshots

> 📸 **Placeholder** — Add screenshots of the DGAS Python dashboard application showing:

| Screenshot | Description | Path |
|---|---|---|
| Dashboard — Normal Mode | Full UI with PS4 inputs, JSON payload, and camera feed | `images/dashboard_normal.png` |
| Dashboard — Emergency Mode | Emergency state active, red overlays, drowsy state visible | `images/dashboard_emergency.png` |
| JSON Payload View | Close-up of the live-generated JSON command packet | `images/dashboard_json.png` |
| Camera Feed — Alert | Camera view with "ALERT" driver state annotation | `images/camera_alert.png` |
| Camera Feed — Drowsy | Camera view with "DROWSY" driver state annotation | `images/camera_drowsy.png` |

---

### System Architecture Diagrams

> 📐 **Placeholder** — Export and add the following from your design tools:

| Diagram | Description | Path |
|---|---|---|
| Full System Architecture | Complete DGAS end-to-end integration | `images/arch_full_system.png` |
| ECU Network | I2C bus topology with all node addresses | `images/arch_ecu_network.png` |
| AI Pipeline Overview | Both AI components and their data flows | `images/arch_ai_pipeline.png` |

---

### Communication Flow Diagrams

> 📡 **Placeholder** — Add annotated communication flow diagrams:

| Diagram | Description | Path |
|---|---|---|
| Normal Mode Flow | JSON telemetry from cabin to ECUs during normal driving | `images/flow_normal_mode.png` |
| Emergency Mode Flow | Signal chain from drowsiness detection to ECU commands | `images/flow_emergency_mode.png` |
| Wireless Bridge Detail | NRF24L01 packet flow between Arduino nodes | `images/flow_wireless_bridge.png` |

---

### Hardware Setup Photos

> 📷 **Placeholder** — Add real hardware photographs:

| Photo | Description | Path |
|---|---|---|
| Full System Setup | Both stations assembled and connected | `images/hw_full_system.jpg` |
| ECU Network Board | All four ECUs mounted and wired | `images/hw_ecu_network.jpg` |
| Vehicle Platform | Vehicle chassis with mounted hardware | `images/hw_vehicle_platform.jpg` |
| Astra Pro Mount | Depth camera mounted on vehicle | `images/hw_astra_pro.jpg` |
| Wireless Bridge | Arduino + NRF24L01 modules on both ends | `images/hw_wireless_bridge.jpg` |
| Power Distribution | Power rails, wiring, and fuse block | `images/hw_power_distribution.jpg` |

---

## 🛠️ Technologies Used

### Software

| Category | Technology |
|---|---|
| Dashboard Application | Python 3.x (Desktop) |
| Driver Monitoring AI | Python, OpenCV, [Drowsiness Model — specify] |
| Lane Detection AI | Python, OpenCV, [Lane Model — specify] |
| Depth Camera SDK | Orbbec Astra SDK / OpenNI2 |
| PS4 Controller | Python `inputs` / `pygame` library |
| JSON Serialization | Python `json` standard library |
| Serial Communication | Python `pyserial` |

### Firmware

| Category | Technology |
|---|---|
| Gateway ECU | STM32 HAL (C), UART, I2C Master, DMA |
| Steering ECU | STM32 HAL (C), I2C Slave, ADC, PWM, H-Bridge |
| Motion ECU | Embedded C (AVR-GCC), I2C TWI Slave, PWM |
| Lighting ECU | Embedded C (AVR-GCC), I2C TWI Slave, GPIO |
| Build Toolchain | AVR-GCC, STM32CubeIDE, Atmel Studio |

### Hardware

| Category | Technology |
|---|---|
| Communication Protocol | I2C (100 kHz), UART (115200 baud), SPI (NRF24L01) |
| Wireless | NRF24L01+ 2.4 GHz RF Transceiver |
| Motor Drive | Cytron MDD10A Dual 10A DC Motor Driver |
| Perception | Orbbec Astra Pro Structured-Light Depth Camera |
| PCB Design | EasyEDA / KiCad (custom boards per ECU) |

---

## 🔬 Challenges and Engineering Decisions

### I2C Bus Design
**Challenge:** Coordinating four ECU nodes on a shared I2C bus without contention or timing conflicts.

**Decision:** The Gateway ECU implements a `ready_to_send` semaphore flag managed by the I2C completion callback (`HAL_I2C_MasterTxCpltCallback`). This prevents overlapping transmissions and ensures sequential command delivery to slave nodes.

---

### Steering Control — Open-Loop vs Closed-Loop
**Challenge:** An open-loop PWM drive for the steering motor could not guarantee precise angular positioning due to mechanical load variations and backlash.

**Decision:** The Steering ECU implements a full **closed-loop position controller** using a rotary potentiometer as an ADC-based angle sensor. The control loop continuously measures position error and drives the motor until the error falls within an acceptable deadband, then halts. This ensures repeatable, accurate steering angle execution regardless of mechanical variation.

---

### Wireless Protocol Selection
**Challenge:** Reliable, long-range communication between cabin and vehicle without infrastructure dependency.

**Decision:** NRF24L01+ modules were selected for their long-range capability (~1 km LoS), low latency, and straightforward SPI integration with Arduino. An Arduino UNO on each end acts as a serial-to-RF bridge, decoupling the radio protocol from the application layer on both laptops.

---

### Heterogeneous MCU Architecture
**Challenge:** Mixing STM32 (ARM Cortex-M) and AVR platforms within the same I2C network.

**Decision:** I2C is platform-agnostic at the electrical level. The Gateway ECU (STM32 HAL) acts as master to both AVR-based slaves (Lighting and Motion ECUs) with no protocol translation required. Each platform implements its own TWI/I2C peripheral independently.

---

### JSON as Telemetry Format
**Challenge:** Defining a flexible, human-readable protocol that supports evolution over the project lifecycle.

**Decision:** JSON was selected for its extensibility, built-in Python support, and ease of debugging during development. The trade-off (higher byte count vs binary protocols) was accepted given the low telemetry frequency and the value of transparency during prototyping.

---

> 📝 **Placeholder:** Add additional challenge entries from team notes in [`./notes/engineering_decisions.md`](./notes/engineering_decisions.md) as the project matures.

---

## 🔮 Future Improvements

### System-Level

- [ ] **CAN Bus Migration** — Replace I2C with CAN bus across all ECUs for improved noise immunity, longer cable runs, and real automotive-grade reliability
- [ ] **Bidirectional Telemetry** — Add vehicle-to-cabin feedback (speed, heading, ECU health) to the wireless link for a complete closed-loop remote interface
- [ ] **MQTT Over Wi-Fi** — Optionally replace NRF24L01 with Wi-Fi + MQTT for integration with mobile applications and cloud monitoring dashboards
- [ ] **Hardware Watchdog Supervisors** — Implement WDT across all ECUs for autonomous recovery from software hang states — critical for a safety system
- [ ] **System Health Dashboard** — Real-time monitoring of ECU states, I2C bus health, and AI confidence metrics via a dedicated diagnostic panel

### AI Components

- [ ] **On-Device Inference** — Deploy the driver monitoring model on an embedded accelerator (e.g., Coral Edge TPU, NVIDIA Jetson) to eliminate laptop dependency
- [ ] **Sensor Fusion** — Combine camera, steering angle, and speed data for more robust driver state estimation
- [ ] **Adaptive Lane Detection** — Extend lane detection to handle adverse weather, faded markings, and construction zones

### ECU Firmware

- [ ] **AUTOSAR-style Layering** — Refactor all ECU firmware into MCAL / BSW / Application layers for cross-platform portability
- [ ] **Fault Detection and Reporting** — Each ECU to report fault codes back to the Gateway via I2C for system-level fault management
- [ ] **PWM Dimming for Lighting** — Replace binary lighting control with PWM-based dimming for smooth transitions and DRL support
- [ ] **Closed-Loop Speed Control (Motion ECU)** — Add motor encoders and a PID controller for precise, load-independent speed regulation

---

## 👥 Contributors

> 📝 **Placeholder** — Add team member names, roles, and GitHub profiles here.

| Name | Role | GitHub |
|---|---|---|
| [Name] | System Architect / Integration Lead | [@username]() |
| [Name] | AI Engineer — Driver Monitoring | [@username]() |
| [Name] | AI Engineer — Lane Detection | [@username]() |
| [Name] | Embedded Engineer — Gateway & Steering ECU | [@username]() |
| [Name] | Embedded Engineer — Motion & Lighting ECU | [@username]() |
| [Name] | Python Dashboard & Wireless Bridge | [@username]() |

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
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
```

---

<div align="center">

**Drowsy Guard Autopilot System — System Integration**

*Engineered for the moments when human control is no longer possible.*

[![GitHub](https://img.shields.io/badge/GitHub-DGAS%20Project-black?style=flat-square&logo=github)]()

</div>
