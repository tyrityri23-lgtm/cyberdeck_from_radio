# CyberDeck from Radio — Build Instructions

These instructions describe the complete project workflow. Update any hardware names or connections that differ from the final physical build before submitting.

## 1. Prepare the electronics

Verify the final PCB against the KiCad schematic before soldering. Confirm the component reference designators, diode orientation, connector locations and I2C/SPI connections.

## 2. Assemble the keyboard PCB

1. Install the switch diodes with polarity matching the PCB silkscreen and schematic.
2. Install the mechanical switches.
3. Install the MCP23017 I/O expander.
4. Install the XIAO ESP32-C3 or the final controller selected by the PCB design.
5. Install any sensor hardware that is present in the final schematic.
6. Inspect all solder joints for bridges and cold joints.

## 3. Program the controller

The firmware is in `firmware/`. The current XIAO ESP32-C3 implementation uses BLE HID for keyboard input. This is intentional: ESP32-C3 USB is fixed-function USB Serial/JTAG/CDC rather than configurable USB HID.

### First firmware bring-up

1. Install Arduino IDE or PlatformIO.
2. Install the Espressif ESP32 board support.
3. Install the `Adafruit MCP23017 Arduino Library`.
4. Install `HijelHID_BLEKeyboard`.
5. Open `firmware/src/main.cpp`.
6. Confirm the I2C pins match the XIAO ESP32-C3 wiring: D4/GPIO6 is SDA and D5/GPIO7 is SCL.
7. Compare `rowPins[]` and `colPins[]` with the actual final KiCad schematic.
8. Change the keymap to match the physical keyboard.
9. Compile and upload.
10. Open Serial Monitor at 115200.
11. Confirm the MCP23017 is detected.
12. Pair the host computer with `CyberDeck Keyboard`.
13. Test every key.

## 4. Prepare the radio enclosure

1. Remove obsolete electronics while keeping the original enclosure structurally sound.
2. Measure the internal mounting surfaces.
3. Confirm the final CAD dimensions before cutting or drilling the physical case.
4. Make the required display, connector, ventilation and mounting openings.

## 5. Print and install the mechanical parts

Print the final keyboard plate, keyboard case, mounting brackets and any internal rails/bosses from the verified CAD.

Check every printed part against the electronics before permanent installation.

## 6. Mount the electronics

Install the keyboard PCB, controller, main computer, display and power system according to the final CAD assembly. Use proper mounting hardware rather than leaving heavy components loose inside the enclosure.

## 7. Cable management

Route USB, display, power and sensor wires along deliberate paths. Secure cables so they cannot touch fans, heatsinks, sharp case edges or moving mechanical parts.

## 8. System test

Before closing the enclosure, test:

- main computer boots;
- display works;
- keyboard controller powers up;
- MCP23017 is detected;
- BLE keyboard pairs;
- every key works;
- mouse/input hardware works if installed;
- sensors work if installed; and
- the power system powers the required loads without unexpected resets.

## 9. Close the enclosure

After all tests pass, close the case and verify that no wire is trapped or under excessive strain.

## 10. Produce final evidence

Take photos of:

1. the full assembled cyberdeck;
2. the powered-on device;
3. the open enclosure showing the internal electronics;
4. the custom PCB; and
5. the CAD/final assembly render.

Use the best full-device image as the GitHub cover image.

## 11. Export STEP files

Export the final verified mechanical assembly and major mechanical parts as STEP and place them in `cad/step/`.
