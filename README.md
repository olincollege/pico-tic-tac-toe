# Handheld Tic-Tac-Toe with Raspberry Pi Pico

Kat Canavan, Drew Pang, Miranda Pietraski 

<div align="center">
    <img src="demo/picodemo30fps.gif">
</div>

## Materials

Each unit requires the following hardware:

- [Raspberry Pi Pico W]([Raspberry Pi Documentation - Raspberry Pi Pico and Pico W](https://www.raspberrypi.com/documentation/microcontrollers/raspberry-pi-pico.html))

- Male Headers (to mount the Pico to the base)

- Micro USB cable with (with data transfer capability)

- [Pimoroni RBG Keypad Base]([Pico RGB Keypad Base](https://shop.pimoroni.com/products/pico-rgb-keypad-base?variant=32369517166675))

## Build Instructions

Create a folder called `pico`. From within this directory, clone this repository.

#### SDK

From the pico directory, install the C/C++ SDK Repository:

```
$ git clone https://github.com/raspberrypi/pico-sdk.git --branch master
$ cd pico-sdk
$ git submodule update --init
```

Make an environment variable with this path. If you followed the structure above you can do this with the following, otherwise use an absolute path.

```
export PICO_SDK_PATH=../../pico-sdk
```

Optionally, update the include path for the C/C++ extension in VSCode.

Optionally, install example Pico code from Raspberry Pi:

```
$ cd ..
$ git clone https://github.com/raspberrypi/pico-examples.git --branch master
```

#### Toolchain

Follow the [Official Raspberry Pi Setup Guide](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf) to configure your operating system to build. Replace instructions for the `pico-instructions` repo with *this* repo when applicable.

- Section 9 for Mac/ Windows. 

- Section 2.2 for Linux

#### Build

Create a `build` folder. From there run

```
$ cmake .. -DPICO_BOARD=pico_w
$ make
```

Alternatively, configure VSCode to build [explained here in section 7](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf)

## Load and Run on the Pico

Plug in the Pico to the computer with a micro USB cable while pressing and holding the `BOOTSEL` button. Release the button when the Pico is plugged in. 

Find and copy the UF2 file from `build/src` to the Pico (it should appear as a storage device named *RPI-RP2*).
