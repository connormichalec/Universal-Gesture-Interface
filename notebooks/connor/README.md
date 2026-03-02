# Connor's notebook

## 2026-02-13 - developing ideas and creating the proposal

The main objective for the day was to solidify our ideas and work on our proposal. Our project is the "Universal Gesture Interface." The main idea is to develop a "glove" that the user can wear to interface with a computer. The idea is to make the glove as a replacement for standard Human Interface Devices (HID), such as a mouse, with hopes of being more interactive and allowing more expression from the user. 

We have decided to split up our project mainly like so:
* I will work on USB interfacing and HID software on the computer
* Kenobi will work on PCB design and the mechanics of the glove
* Kobe will work on intefacing with the IMU and doing the necessary DSP to get a meaningful signal

## 2026-02-19 - developed testboard schematic

We decided to split up the work this week by having me do the initial PCB schematic, Kenobi do the initial PCB layout, and Kobe formalize a team contract that we agree on. We all have experience with KiCad so drafting the schematic was not difficult. The idea for our first PCB is to be a "testboard" that just has our components on it so we can start development and testing with it without worrying too much of how it will fit in mechanically. The first challenge of this was to pick out the components. We needed an MCU, IMU, Flex sensors, and touch sensors.

The heart of the project will be an [STMF405](https://www.st.com/en/microcontrollers-microprocessors/stm32f405-415.html) We chose it will be good candidate for the DSP needed for the IMU. 

The IMU we will be using is the 9-axis ICM-20948. Unfortunately it requires 1.7V SPI communication meaning I needed to implement level shifters into my schematic. After some experiementing and searching online I came up with this schematic:

<img width="1252" height="872" alt="image" src="https://github.com/user-attachments/assets/c4b8792c-a29a-48f8-bc8a-4afcb032b924" />

## 2026-02-20 - developed testboard schematic V2

After searching for components online, I decided we should change around the components on the testboard so I made a V2 schematic. Mainly because the IMU is out of stock and its complicated to use with the level shifters. I decided to switch it to the LSM6DSL which doesn't require any level shifters and works well with the STM32. 

I also implemented components to test USB communication, and provided the ability to power our testboard over USB. This required implementing a 3V regulator as USB is 5V but the STM32 runs on less than 3.3V. I also had to implement a crystal oscillator for USB. The schematic is here:

<img width="1120" height="839" alt="image" src="https://github.com/user-attachments/assets/fe663200-7c3e-450d-b7d8-0dc595ebaf8d" />

Kenobi will now be the one to design the layout. He has a lot of experience with PCB layout from one of his internships, and we can get it ordered by round 1.
