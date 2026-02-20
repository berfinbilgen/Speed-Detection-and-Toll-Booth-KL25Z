# Speed-Detection-and-Toll-Booth-KL25Z


A radar-inspired speed detection and highway toll booth demo built with **Embedded C** on the **FRDM-KL25Z**.  
The system uses an **ultrasonic sensor** for distance/time measurement, a **servo motor** for toll gate control, and an **LCD** to display speed and system messages. A **potentiometer** sets the speed limit via the MCU’s **ADC**, enabling an interactive driver experience based on their speed.

## Demo Video
- YouTube: https://www.youtube.com/watch?v=nUOA8T0kiXM

## Hardware Used
- FRDM-KL25Z (NXP KL25Z)
- Ultrasonic sensor (HC-SR04 style)
- Servo motor (gate control)
- LCD (character LCD)
- Potentiometer (speed limit input via ADC)

## Features
- Speed limit selection via ADC (potentiometer)
- Time-trial mode & average speed estimation using ultrasonic measurements
- LCD messages for speed, limit, payment status, and system state
- Servo control for gate open/close (PWM)
- Button interrupts for mode transitions

## Project Structure
```text
src/
  main.c     -> Application state machine & main loop
  utils.h    -> Drivers and utilities (ADC, PWM, LCD, ultrasonic, interrupts)
