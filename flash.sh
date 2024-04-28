#!/usr/bin/bash

build_folder="./build/src/"
player_0_u2f="tic_tac_toe_main_server.uf2"
player_1_u2f="tic_tac_toe_main_client.uf2"
first_pico_addr=$(lsusb | grep 2e8a | awk '{print $4}' | awk -v FS="" -v OFS="" '{ $NF = "" ; print }' | sed -n 1p)
second_pico_addr=$(lsusb | grep 2e8a | awk '{print $4}' | awk -v FS="" -v OFS="" '{ $NF = "" ; print }' | sed -n 2p)

# Flash player 0
if [ "$1" == "0" ]; then
  echo "Flashing player 0"
  picotool load "${build_folder}${player_0_u2f}" -f --address "${first_pico_addr}"
  exit 0
# Flash player 1
elif [ "$1" == "1" ]; then
  echo "Flashing player 1" -f --address "${first_pico_addr}"
  picotool load "${build_folder}${player_1_u2f}" -f --address "${first_pico_addr}"
  exit 0
# Else, flash both
elif [ "$1" == "" ]; then
  echo "Flashing player 0 & 1"
  picotool load "${build_folder}${player_0_u2f}" -f --address "${first_pico_addr}"
  picotool load "${build_folder}${player_1_u2f}" -f --address "${second_pico_addr}"
  exit 0
else
  echo "Unknown argument"
  exit 1
fi
