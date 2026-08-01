## Instructions: 

## Step 1: Order the PCB
Send the provided Gerber files (found in the `KiCad` project folder) to your PCB manufacturer.

## Step 2: Solder the Matrix
Begin by soldering the **SM5059** diodes and the mechanical switches to the board. Ensure the diode polarity matches the silkscreen.

## Step 3: Mount the ICs
Solder the **MCP23017** I/O expander and the **XIAO ESP32-C3** microcontroller into their footprints.

## Step 4: Attach Sensors
Wire/solder the **PMW3360** sensor and the **BNO055** gyroscope. Double-check your **I2C/SPI** lines according to the provided diagram.

## Step 5: Connect
Plug the **XIAO ESP32-C3** into your host PC via USB.

## Step 6: Flash
Flash the custom firmware (located in the `/firmware` directory).

## Step 7: Test
Once flashed the board should be recognized by your PC as a **HID Keyboard and Mouse**. Test the keystrokes, the optical sensor tracking, and the gyro input before mounting it in the chassis.

## Step 8: Prep the Chassis
Hollow out the radio chassis, carefully removing old electronics while preserving the structural integrity and aesthetic faceplate.

## Step 9: Print Components
Print the mounting brackets and the custom keyboard plate using the provided CAD files.

## Step 10: Mount PCB
Secure the assembled custom PCB to the 3D-printed faceplate.

## Step 11: Secure SBC
Mount the **Radxa X4** (or your chosen SBC) to the 3D-printed rails inside the radio chassis.

## Step 12: Connect Inputs
Route the USB output of the **XIAO ESP32-C3** to one of the USB ports on the **Radxa X4**.

## Step 13: Wire Display & Power
Route the power supply to the **Radxa X4** and connect your internal display panel via **HDMI/eDP**.

## Step 14: Boot Up
Power on the system. The **Radxa X4** will instantly recognize the custom **ESP32-C3** board as its primary input device.
