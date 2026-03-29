# Connor's notebook

## 2026-02-13 - developing ideas and creating the proposal

The main objective for the day was to solidify our ideas and work on our proposal. Our project is the "Universal Gesture Interface." The main idea is to develop a "glove" that the user can wear to interface with a computer. The idea is to make the glove as a replacement for standard Human Interface Devices (HID), such as a mouse, with hopes of being more interactive and allowing more expression from the user. 

We have decided to split up our project mainly like so:
* I will work on USB interfacing and HID software on the computer
* Kenobi will work on PCB design and the mechanics of the glove
* Kobe will work on intefacing with the IMU and doing the necessary DSP to get a meaningful signal

## 2026-02-19 - developed testboard schematic

We decided to split up the work this week by having me do the initial PCB schematic, Kenobi do the initial PCB layout, and Kobe formalize a team contract that we agree on. We all have experience with KiCad so drafting the schematic was not difficult. 

Today my objective was to get our first PCB schematic done. The idea for our first PCB is to be a "testboard" that just has our components on it so we can start development and testing with it without worrying too much of how it will fit in mechanically. The first challenge of this was to pick out the components. We needed an MCU, IMU, Flex sensors, and touch sensors.

The heart of the project will be an [STMF405](https://www.st.com/en/microcontrollers-microprocessors/stm32f405-415.html) We chose it will be good candidate for the DSP needed for the IMU. 

The IMU we will be using is the 9-axis ICM-20948. Unfortunately it requires 1.7V SPI communication meaning I needed to implement level shifters into my schematic. After some experiementing and searching online I came up with this schematic:

<img width="1252" height="872" alt="image" src="https://github.com/user-attachments/assets/c4b8792c-a29a-48f8-bc8a-4afcb032b924" />

## 2026-02-20 - developed testboard schematic V2

Todays objective was to refine the testboard PCB schematic due to some issues found. After searching for components online, I decided we should change around the components on the testboard so I made a V2 schematic. Mainly because the IMU is out of stock and its complicated to use with the level shifters. I decided to switch it to the LSM6DSL which doesn't require any level shifters and works well with the STM32. 

I also implemented components to test USB communication, and provided the ability to power our testboard over USB. This required implementing a 3V regulator as USB is 5V but the STM32 runs on less than 3.3V. I also had to implement a crystal oscillator for USB. The schematic is here:

<img width="1120" height="839" alt="image" src="https://github.com/user-attachments/assets/fe663200-7c3e-450d-b7d8-0dc595ebaf8d" />

Kenobi will now be the one to design the layout. He has a lot of experience with PCB layout from one of his internships, and we can get it ordered by round 1.

## 2026-03-10 - developed final board V1

PCBs are taking a while to arrive, its already round 3 and we haven't received our testboards. We decided just to develop the final board without being able to test.

For the final schematic, I am shifting the design away from a testbench to one that we can see in the final project. The biggest change on the schematic side is just adding more support for all of the Flex sensors/Force sensitive resistors. We decided we will just have support for 3 flex sensors one for the index finger, middle finger and thumb, but will have force senstiive resistors for every finger. The reason for this is that we determined the ring and pinky finger would not be used that much in most gestures. We decided to keep all the Force senstiive resistors however since we believe they could be useful to expand the possible surface gestures, so we will leave that option there.

Other than that the only other changes are on the layout side to make it fit well into the glove shape, but that is handled by Kenobi.

## 2026-03-13 - Test board assembly and configuration

We received our test board PCB and parts, and today the main goal for me was to solder it together and perform the initial tests and configuration.

The biggest challenges in this were not only soldering the LQFP-64 STM32F405 but especially the LGA-14 LSM6DSL IMU, which has tiny LGA pads that cannot be soldered with the iron. (I decided to not get a stencil because we found it unnecessary and wanted the challenge. The equipment in the lab is a little lackluster because there is only a few soldering irons that work well, most of them have broken tips; so it was a bit of a challenge to find a setup that would actually work for soldering the components. Once I found a soldering iron that worked well the STM32 was a bit of a challenge to solder because the tip was too big for the pins i was soldering, but I developed a technique of using a blob of solder and brushing it against the pins which worked well and i got a clean connection.

To solder the IMU I tinned the pads of the PCB as well as tinned the component and used a heat gun on it to set it. This seemed to work well, and while it took some patience, I think I got a good connection.

The rest of the components were trivial, and were mainly larger smd components and peripherals. However, I had to sacrifice one of my USB charging cables to solder the usb connection to the board.

Our board is powered off of USB and requirse a regulator to step the 5V to 3.3V so that was there first thing to test. We did a bench supply test first, which showed proper voltage readings throuhgout the board and progressed to plugging it into the laptop which also showed good voltage readings.

While I was working with the hardware, Kobe wrote a simple program in STM32CubeIDE to turn a debug pin on and off on our testboard, so the next test was seeing if we could successfully program it, which worked. We are using an STLINK-V2 clone to do so, and my biggest concern was that I laid out our MCU wrong on the PCB but getting it to program was a huge relief. Overall today was a big success and being able to program it was a big milestone.

![IMG_6111](https://github.com/user-attachments/assets/26456032-674f-4f63-b255-dbb06ccfb808)

## 2026-03-20 - Test USB connectivity

After getting basic MCU operation, my main goal is to setup communication between the MCU and the PC. 

The first challenge in this is to configure the clock settings on the MCU, as USB requires a 48MHz clock signal with little room for error, such that the built in oscillator in the STM32 would not be sufficently precise. I included a 8Mhz crystal oscillator on the PCB for this purpose. I had to configure STM32CubeIDE to utilize the crystal oscillator as its oscillation source, then using the correct prescaling settings, derived a 48Mhz signal for USB. The clock layout is below:

<img width="1155" height="593" alt="image" src="https://github.com/user-attachments/assets/f03af783-a19d-4f2a-a267-778061f9f5e8" />

After some research, As a basic test, I configured the USB interface as a HID device. This successfully showed up in my computers device manager indicating USB connectivity was successful. I then wrote a rudimentary program to act as a mouse device which you can see below:



https://github.com/user-attachments/assets/b1170685-d390-4c15-a29c-faa79d675fc0


This proves the basic functionality of MCU communication with PC. The next steps are now to start communicating with the sensors so we can utilize that data to control the PC in a way such as the mouse demo. I will focus on the hardware level communication with the peripherals (particularlly the IMU), and Kobe will start working on the signal processing that involves. 







