<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/othneildrew/Best-README-Template">
    <img src="images/logo.png" alt="Logo" width="80" height="80">
  </a>

  <h3 align="center">Lab 15: Space Invaders</h3>

  <p align="center">
    Systems Approach to Game Design
    <br />
    <a href="https://github.com/othneildrew/Best-README-Template"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/othneildrew/Best-README-Template">View Demo</a>
  </p>
</p>



<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

The goal of this lab is to integrate all of the lessons learned in the previous
lectures into one project. 

The objectives are to:

* Review course contents
* Integrate components into a complete embedded system
* Use structures to organize data
* Introduce graphics
* Present the timer module and use it to create periodic interrupts
* Build a hand-held game

I've decided to take a slightly different approach. The original lab requires
the use of a slide potentiometer but I have replaced that with a joystsick. 
Additionally, the original lab used a Nokia LCD display while my version of the
lab will use an OLED display. I decided to make these changes so that I can 
begin implementing my own solutions to problems rather than having the instructors
provide some hand-holding. 


### Built With

* [CMake](https://cmake.org)
* [Visual Studio Code](https://code.visualstudio.com)
* [GNU Arm Embedded Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm)
* [LM4Tools](https://github.com/utzig/lm4tools)
* [TIVAWARE](https://www.ti.com/tool/SW-TM4C)
* [OpenOCD](http://openocd.org)
* [GDB](https://www.gnu.org/software/gdb/)


<!-- GETTING STARTED -->
## Getting Started

This is an example of how you may give instructions on setting up your project locally.
To get a local copy up and running follow these simple example steps.

### BOM

* [EK-TM4C123GXL](https://www.ti.com/tool/EK-TM4C123GXL)
* [Analog Joystick](https://www.adafruit.com/product/512)
* LEDs
* [OLED Display](https://www.adafruit.com/product/938)
* Audio Jack
### Prerequisites

#### Build Tools

The first thing you'll need is the required software:

```sh
sudo apt install make cmake arm-none-eabi-gcc arm-none-eabi-newlib gcc-c++ libusb unzip
```

You can also install the latest GNU Arm toolchain directly from their [website](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm).

#### Installing LM4Flash

[LM4Tools](https://github.com/utzig/lm4tools) enables multi-platform development on the TI Stellaris Launchpad boards.

To install the tools run:

```sh
git clone https://github.com/utzig/lm4tools.git
cd lm4tools/lm4flash
make
```

Next add the lm4flash to your `PATH` and get the udev rules for the usb device.

```sh
sudo echo "export PATH=$PATH:$HOME/tiva-projects/lm4tools/lm4flash" &>> ~/.profile

# Note: If you're on a Mac this step is not required
echo 'ATTRS{idVendor}=="1cbe", ATTRS{idProduct}=="00fd", GROUP="users", MODE="0660"' | \
sudo tee /etc/udev/rules.d/99-stellaris-launchpad.rules
```

#### TivaWare

Download the Full Release (.exe) version of the TivaWare software [here](https://www.ti.com/tool/SW-TM4C). You'll have to create an account to download the software.

Once you have the TivaWare software downloaded, extract it like so:

```sh
mkdir ~/tivaware
cd tivaware
mv ~/Downloads/SW-TM4C-VERSION.exe .
unzip SW-TM4C-VERSION.exe .
make
```

### Building the Project

Once you have all the prerequisites set up, you can now build the project.

To do that simply run:

```sh
cd lab15_spaceinvaders
mkdir build
cd build
cmake -DTIVAWARE_PATH="/path/to/tivaware" ..
make
make flash  # To flash the Launchpad (Make sure it's connected)
```

### Debugging (TODO)

### Hardware Assembly (TODO)
