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

## 2026-03-28 - IMU connectivity

USB connectivity has been established so the next step is to interface with the peripherals. Kobe wrote some basic code for intefacing with the flex sensor and force sensitive resistor but could not get any response over SPI from the IMU. My main goal here was to investigate what was going wrong and try to get communication established.

The issue we were encountering was that the IMU does not seem to respond at all. After some basic code to setup SPI communication, I wrote a basic WHOAMI test which should just read a status register on the IMU to ensure basic communication works. Unfortunately the data line seemed dead. I spent many many hours troubleshooting this. At first I couldn't tell whether it was a firmware or hardware issue, so tried various tests such as changing clock speeds, configurations, probing with oscilliscope, reflowing the component, and even resoldering a completely new board.

I eventually narrowed it down to a hardware issue because the MISO line seemed to not respond at all despite me checking correct MOSI/CS/SCK signals via an oscilliscope probe. After many hours of debugging I gave up.

The following day I decided to spend more hours troubleshooting the issue and eventually discovered via reviewing the datasheet that I was using auxiliary connections on the IMU instead of the main one. I was quite relieved to have found the issue but unfortunately, that meant our PCB was wrong and I would have to redesign it, even after the round 4 deadline had passed, meaning we are probably going to have to order our own PCBs. Before that though, I still wanted to test IMU functionality so I decided to surgically solder to the IMU to try to communicate with it so we could still proceed with our testing without the correct PCB.

This was incredible difficult, and I utilized the smallest wire I could find under the microscope to solder to the bottom of the IMU, taping in place and using wires to connect it directly to the pads on the MCU. You can see below how it was done:
![IMG_6138](https://github.com/user-attachments/assets/58865559-e033-45b8-8fec-f4b6971fdbb1)

While quite janky, I was quite relieved to see that I could now communicate with the chip (as seen below with the WHOAMI serial test) and we could now proceed on to receiving gyro/accelerometer data.


![IMG_6139](https://github.com/user-attachments/assets/e1569d24-cc0d-436b-af2b-0c4b0f6bdcad)

## 2026-03-29 - IMU value reading

With a successful testbench setup, we are now ready to read values off the IMU. The goal for this part is to successfully read accel/gyro data. 

Kobe has already started writing code to interface with it, but upon testing the code, I get 0s for data response. So really, I just have to fix the code that Kobe started. I quickly discovered that Kobe was reading from the registers in a way that works for an older model of the IMU, and changing the process resulted in successful read/writes as seen below.

![IMG_6147](https://github.com/user-attachments/assets/206c3add-11e5-4471-b09e-6af6ca47b2f2)

## 2026-03-30 - PCB Round 4

Upon discovery of the issue with our PCB's IMU connections, I had to quickly make adjustments to the schematic. PCB round 4 was already due so I reached out to Lukas and he said we could still submit if we were fast. I altered the PCB schematic from round 3 to just change the pads we utilized for our IMU. Unfortunately, our mistke meant our round 3 PCB was useless so we would have to rely on our "surgically altered" testboard PCB until round 4 came. 

In the end, I got the schematic altered and sent to Kenobi to change around the layout so we should get our PCB just barely in time.


## 2026-04-18 - Final board assembly attempt 1

With our final board arriving, it was now my job to get it operational so we could begin full scale implementation tests.

The biggest difficulty again was soldering the IMU, requriing careful application with the heat gun and precise plcaement. This was the part that made me most nervous because If I messed up the schematic any more than I had already, we would not be able to get an additional PCB in time.

Overall the soldering went quite poorly at first. The lab was very busy from people finishing up so I had to use soldering irons with tips way to big for the process. After hours of soldering we had a result that was suboptimal as seen below:

<img width="4284" height="5712" alt="IMG_6376" src="https://github.com/user-attachments/assets/7719387a-30b1-41a4-81d9-d1cc20dae004" />

Unfortunately when going to test it, it would not program. This was very frustrating and in the troubleshooting process I ended up burning out the MCU. 

## 2026-04-19 - Final board assembly attempt 2

With the failures of the first attempt, I decided to wait until a time when the lab would be more empty for access to more equipment. I used the last MCU on a blank PCB and starting soldering eveyrthing again from scratch. Much to my relief, the board programmed! The soldering job was a lot better than last time:

<img width="4284" height="5712" alt="IMG_6377" src="https://github.com/user-attachments/assets/8317a7dd-886e-4fdf-9421-3b4f29b47491" />

Unfortunately the IMU did not work at first which made me very nervous, but with a reflow it ended up working successfully!

<img width="4284" height="5712" alt="IMG_6378" src="https://github.com/user-attachments/assets/70aeb682-ac16-4fa2-86c7-41a458e50b02" />

## 2026-04-24 - Keyboard control over HID

Something that we deemed to be an ideal feature to have would be keyboard control over our glove. My goal here was to implement this.

Keyboard control is less useful than mouse control, but could be used in conjugtion with it for certain functions like using the glove to navigate in a game, programming macros, or using the glove to switch through slides in a presentation via programmed arrow keys. It was certainly not as easy to implement keyboard control as it was to implement mouse control. The STM32 offers a HID device class with many default options, including a mouse driver. In order to implment keyboard control, I had to specify a device report "descriptor". The descriptor helps define the payload of the data being sent to the computer and is part of a larger scheme as seen below:

<img width="404" height="327" alt="image" src="https://github.com/user-attachments/assets/684a5ed7-de8e-4ffb-92bf-98c782cd04b7" /> (Zilog.com, see below for link)

 I had to research quite a bit to understand this and utilized the resources below to build an understanding and help implement the keyboard report descriptor. Here are some resources that helped:
 * https://docs.kernel.org/hid/hidintro.html
 * https://www.zilog.com/docs/AN0416.pdf
 * https://community.st.com/t5/stm32-mcus/implementing-a-usb-hid-keyboard-device-from-a-usb-hid-mouse/ta-p/793535

It took quite a bit of troubleshooting to get this working, primarilly getting the HID repotr descriptor right. Eventually I was successful and I had a keyboard iterface that we could utilize for gesture controls.

## 2026-04-27 - HID/serial dual mode

This is the final week we are at the point where we have something that works well, especially on the hardware side, but are trying to polish it into a finished product, especially getting gesture detection fully working. As part of this, we deemed it useful to be able to communicate in HID mode as well as serial mode simaltaenously. In the past, we would have to pre-configure this every time we flashed the board, electing to utilize the USB interface as a serial device or a HID device. Developing a way to communicate over serial as well as acting as a HID device would allow us to better gain live insight into how Kobe's gesture detection algorithm was working and what we can do to imrpove it.

The way this was achieved was by modifying the report descriptor described above. In addition to the report descriptors mouse and keyboard components, I added an additional section which could be used to send a generic data payload. This worked well, and we unfortunately ultimately did not use it in our final demo, worked as solid ground for continuing the project in the future.



