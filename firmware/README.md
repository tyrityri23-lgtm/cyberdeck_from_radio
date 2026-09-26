# CyberDeck Keyboard Firmware

This firmware runs on the **Seeed Studio XIAO ESP32-C3** and handles the custom keyboard controller.

## What it does

- starts I2C communication with the MCP23017;
- configures the MCP23017 as a keyboard-matrix scanner;
- debounces key transitions in software;
- translates matrix positions into keyboard actions; and
- exposes the controller as a **BLE HID keyboard**.

The XIAO ESP32-C3 has Bluetooth LE, while the ESP32-C3 USB peripheral is fixed-function USB Serial/JTAG/CDC rather than a configurable USB HID device. For a wired USB HID implementation, use a USB-HID-capable ESP32-S2/S3-class controller instead. citeturn656599view0turn643326search0turn643326search2

## Very important: verify the matrix mapping

The current public repository instructions do not show the final row/column pin assignment. Therefore `src/main.cpp` contains a **bring-up mapping** using 6 rows × 10 columns across the 16 MCP23017 GPIOs.

Before calling this firmware the final firmware, open the final KiCad schematic and replace:

```cpp
const uint8_t rowPins[MATRIX_ROWS] = {0, 1, 2, 3, 4, 5};
const uint8_t colPins[MATRIX_COLS] = {6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
```

with the actual row/column mapping from your PCB.

Then replace the test keymap with the real physical layout.

## Arduino IDE setup

1. Install the latest Arduino IDE.
2. Install the Espressif `arduino-esp32` board package.
3. Select **XIAO_ESP32C3** as the board.
4. Install these libraries through the Arduino Library Manager:
   - `Adafruit MCP23017 Arduino Library`
   - `HijelHID_BLEKeyboard`
5. Open `src/main.cpp` as the main sketch.
6. Connect the XIAO ESP32-C3 with a data-capable USB-C cable.
7. Select the XIAO's serial port.
8. Upload the firmware.

Seeed's current XIAO ESP32-C3 documentation identifies D4/GPIO6 as SDA and D5/GPIO7 as SCL, and describes the board as having Bluetooth LE support. citeturn656599view0

## First test

With the PCB connected:

1. Open Serial Monitor at `115200`.
2. Confirm the message `CyberDeck keyboard firmware started.` appears.
3. Confirm there is no `MCP23017 was not detected` error.
4. On the host computer, open Bluetooth settings.
5. Pair with `CyberDeck Keyboard`.
6. Open a text editor.
7. Press one physical key.
8. Confirm the corresponding character appears.
9. Repeat for every key.
10. Test keys that must be held or act as modifiers after the basic matrix is working.

## Next firmware milestone

After the matrix mapping is confirmed, the next firmware revision should add:

- a complete modifier-aware keymap;
- Caps Lock / Num Lock state handling where needed;
- BNO055 motion support if it is installed;
- PMW3360 mouse support if it is installed; and
- a battery-status/reporting strategy if the hardware exposes a reliable measurement point.
