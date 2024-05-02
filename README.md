# Handheld Tic-Tac-Toe with Raspberry Pi Pico

Kat Canavan, Drew Pang, Miranda Pietraski

Inspired by the handheld games we played growing up, we decided to add a new twist to a timeless classic by making a bluetooth-enabled 4x4 tic-tac-toe game with the Raspberry Pi Pico and Pimoroni RGB keypad base.

<div align="center">
    <img src="demo/picodemo30fps.gif">
</div>

### Features

- **Onboard Computing**: Our system is self-sufficient, handling all computations internally. The only cable you'll need is for power.

- **Bright LED Display**: Play in any lighting condition- even outside!

- **Effortless Connectivity**: With automatic pairing and a Bluetooth range of up to 30 meters, you're free to move around without losing connection.

- **Instant Response**: Experience real-time interaction. The moment you press a button, the action is mirrored on both devices.

- **Endless Play**: Once the buttons flash red, blue, or green (for a tie), the board automatically resets for the next round so you can enjoy endless competitive rematches.

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

Follow the [Official Raspberry Pi Setup Guide](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf) to configure your operating system to build. Replace instructions for the `pico-instructions` repo with _this_ repo when applicable.

- Section 9 for Mac/ Windows.

- Section 2.2 for Linux

#### Build

Create a `build` folder. From there run

```
$ cmake .. -DPICO_BOARD=pico_w
$ make
```

Alternatively, configure VSCode to build [explained here in section 7](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf)

## Load and Run on the Picos

**Option 1:** Use the flashing script

Build and install `picotool`, available [here](https://github.com/raspberrypi/picotool). Note that using `picotool` with the flash script requires building from source, adding to path, and setting neccesary udev rules. Full details are available in the `picotool` README.

Plug in both Picos to the computer with a micro USB cable, and run `./flash.sh` from the root directory.

**Option 2:** Manual flashing

Plug in both Picos to the computer with micro USB cables while pressing and holding the `BOOTSEL` buttons. Release the buttons when the Picos is plugged in.

Find and copy the UF2 files from `build/src` to the Pico (it should appear as a storage device named *RPI-RP2*). The default file names are `player_0.uf2` and `player_1.uf2`.
