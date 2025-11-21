# Reality Protocol: The Veritas Anchor

> **"In an age of AI hallucinations and digital spoofing, Physics is the only source of truth."**

## 📖 Overview
**Reality Protocol** is a context-aware hardware agent designed to verify physical reality. While digital feeds (cellular data, social media, cameras) can be spoofed or hallucinated, **Time-of-Flight (ToF)** measurements via Ultra-Wideband (UWB) are immutable.

This repository contains the firmware for the **Veritas Anchor**—an autonomous sensor node that establishes a "Circle of Truth" around critical assets or rescue teams.

## 🏗️ Architecture
The system uses a **"Trust but Verify"** loop:
1.  **Digital Context:** An AI Agent (Claude) monitors web data for alerts (e.g., "Crowd Panic").
2.  **Physical Audit:** The Veritas Anchor (ESP32) pings local tags using UWB physics.
3.  **Consensus:** If Digital = Panic but Physical = Calm, the system flags a **"Digital Hallucination"** and prevents false alarms.

## ⚡ Hardware Stack
* **Microcontroller:** ESP32-WROVER-E
* **Sensor A (Spatial):** 2x Reyax RYUW122 (UWB Modules)
* **Sensor B (Vibration):** MPU6050 (Gyro/Accelerometer)
* **Visualization:** LCD1602 (I2C)
* **Platform:** Freenove Ultimate Starter Kit

## 🔌 Wiring Configuration (ESP32-WROVER)

| Component | Pin Label | ESP32 GPIO | Notes |
| :--- | :--- | :--- | :--- |
| **UWB TX** | RX | GPIO 18 | Remapped for WROVER |
| **UWB RX** | TX | GPIO 4 | Remapped for WROVER |
| **UWB VDD** | VDD | 3.3V | **DO NOT USE 5V** |
| **LCD SDA** | SDA | GPIO 21 | I2C Data |
| **LCD SCL** | SCL | GPIO 22 | I2C Clock |
| **LCD VCC** | VDD | 5V | Required for backlight |

## 🚀 How to Run
1.  Flash `RealityProtocol.ino` to the ESP32.
2.  Power the Remote Tag (Battery or USB).
3.  The LCD will display real-time cm-precision distance.
