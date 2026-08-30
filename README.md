# PicoLink
An RP2040-based project for VGA graphics generation and USB composite device implementation.

## Table of Contents

  - [PicoLink](#picolink)
  - [Table of Contents](#table-of-contents)
  - [About The Project](#about-the-project)
  - [Features](#features)
  - [Project Workflow](#project-workflow)
  - [Hardware Used](#hardware-used)
  - [Breadboard Testing](#breadboard-testing)
  - [PCB Model](#pcb-model)
  - [Software Used](#software-used)
  - [Future Work](#future-work)
  - [Contributors](#contributors)
  - [Resources](#resources)
  - [Acknowledgements](#acknowledgements)

## About The Project
**PicoLink** is an RP2040-based project focused on **VGA graphics generation** and **USB communication**. It uses **PIO and DMA** for efficient VGA output and implements a **USB composite device with HID and CDC interfaces**.

## Features

- VGA output using RP2040
- Custom VGA graphics library
- Increased color support
- PIO-based VGA signal generation
- DMA-assisted data transfer
- Custom PCB design
- Space Invaders game
- Pacman game
## Project Workflow

#### Research

* Understanding VGA standards, timing, and signal generation
* Studying the RP2040's PIO and DMA peripherals
* Reviewing existing VGA libraries and RP2040 implementations

#### VGA Graphics Development

* Implementing VGA signal generation using PIO
* Developing a custom VGA graphics library
* Implementing framebuffer-based graphics
* Increasing color support and optimizing memory usage
* Testing VGA output on a monitor

#### PCB Design

* Designing the PicoLink PCB around the RP2040
* Designing the VGA interface and USB connections
* Routing the PCB while considering signal integrity and hardware constraints
* Finalizing the PCB for manufacturing
#### USB Development


* Implementing USB HID functionality
* Implementing USB CDC communication
* Creating and understanding the required USB descriptors
* Integrating HID and CDC as a composite USB device

#### Game Development & Testing

* Developing **Sngpace Invaders & Pacman** using the custom VGA graphics library
* Integrating the game with the PicoLink hardware
* Testing VGA output, USB communication, and overall system performance
* Debugging and optimizing the final implementation


## Hardware Used

|     Components        |          Description          |
| --------------------- | ----------------------------- |
| [RP2040-Mitrocontroller](https://robu.in/product/raspberry-pi-pico/?gad_source=1&gad_campaignid=19974686076&gbraid=0AAAAADvLFWc_9GsTRABB0vDtC2DQhwOhp&gclid=CjwKCAjwqJXUBhBNEiwA8BgG7vailV2Hy5Y_-Ko3vAaZAEXPT5CyTGmnKPxanKmfuWqEnSS08VfShBoCNk0QAvD_BwE)           | Microcontroller |
| [15-Pin VGA Connector](https://robu.in/product/x0615ft7b1-xkb-15p-female-d-sub-3-d-sub-vga-connectors-rohs/)  | VGA-connector |
| [33O ohm Resistors](https://robu.in/product/330-ohm-1-4w-1206-surface-mount-chip-resistor-pack-of-100/?gad_source=1&gad_campaignid=17427802703&gbraid=0AAAAADvLFWfZ2mYmii2USnEMQ98Oe9FQ_&gclid=CjwKCAjwqJXUBhBNEiwA8BgG7kQjg_4zEpnGfV_3AsnYPpfzbiUg2oPFK-0IBOd3eiSTI_ayuLDsIhoC2dwQAvD_BwE/) | Resistor |

## Breadboard Testing
- View of Breadboard VGA Connections:   
<p align="left">
  <img src="https://raw.githubusercontent.com/ashmita0307/PicoLink/Ashmita/assets/Breadboard_vga_connections.png" alt="Breadboard VGA Connections" width="600">
</p>

## PCB Model

- View of the PCB Model front and back:       
<table>
  <tr>
    <td align="center">
      <img src="https://github.com/ashmita0307/PicoLink/blob/Ashmita/assets/PicoLink_3D_Front.png"
           alt="PCB Model Front"
           width="600">
    </td>
    <td align="center">
      <img src="https://github.com/ashmita0307/PicoLink/blob/Ashmita/assets/PicoLink_3D_Back.png"
           alt="PCB Model Back"
           width="500">
    </td>
  </tr>
</table>

- View of the PCB Routing - front and back:      

<table>
  <tr>
    <td align="center">
      <img src="https://github.com/ashmita0307/PicoLink/blob/Ashmita/assets/Picolink_B.Cu.png"
           alt="PCB Model Front"
           width="600">
    </td>
    <td align="center">
      <img src="https://github.com/ashmita0307/PicoLink/blob/Ashmita/assets/Picolink_F.Cu.png"
           alt="PCB Model Back"
           width="600">
    </td>
  </tr>
</table>


## Software Used 

- PCB Design - [KiCad 10.0.5](https://www.kicad.org/)
- Software development kit for RP2040 - [Raspberry Pi Pico SDK](https://github.com/raspberrypi/pico-sdk) 


## Future Work
- [ ] Develop a more memory-efficient rendering system
- [ ] Develop more games/examples using the custom graphics library

## Contributors
- [Ashmita Subramanian](https://github.com/ashmita0307)
- [Mohd Aasim](https://github.com/Mohdaasim18)

## Resources

- [razorArnov Github Repository](https://github.com/razorArnov/PICO-VGA-BOARD.git)
- [Phil's lab Kicad Playlist](https://www.youtube.com/watch?v=O-zNn5k5Bn4)
- [Mikeshah's C Programming playlist](https://www.youtube.com/playlist?list=PLvv0ScY6vfd8M-Mi_Vyrg7KgISTW3Sklt)



## Acknowledgements 
- [SRA VJTI](https://sravjti.in/) Eklavya 2026
- Special thanks to our mentors [Omkar Nanajkar](https://github.com/nomkar24), [Moksh Panicker](https://github.com/moxybaba), and all the seniors at SRA, VJTI for their constant support and guidance throughout the project.
