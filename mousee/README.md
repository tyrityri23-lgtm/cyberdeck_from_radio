## DIY Wireless Mouse:

I am building a custom wireless optical mouse. The project is not finished yet. I have completed the computer aided design work, printed circuit board design and overall hardware layout. Now I need to order the parts, 3D print the shell and start putting everything together.

The main controller is a Seeed Studio XIAO ESP32-C3. I chose the XIAO ESP32-C3 because it is a compact microcontroller that supports native Bluetooth Low Energy HID functionality. I used 3D CAD software to design the ergonomic mouse enclosure. I imported a model of my custom printed circuit board into the assembly to make sure everything fits correctly before manufacturing the custom printed circuit board.

## Hardware:

* Seeed Studio XIAO ESP32-C3 microcontroller

* PixArt PMW3360DM-T2QU high-end optical sensor

* Custom mouse printed circuit board designed in KiCad

* AP2112K voltage regulator for the PMW3360 power rail

* SPI interface connection between MCU and optical sensor

* Custom switch matrix using tactile button click switches and rotary encoder input

* MCP23017 I/O expander for input line expansion

* Battery-powered design for wireless Bluetooth operation

## PCB & Sensor Routing:

I did not use an off-the-shelf mouse motherboard. Instead I designed my printed circuit board in KiCad with a custom footprint layout for the switches and sensor. The optical sensor is controlled by the ESP32-C3 over an SPI bus. The PMW3360 requires a 1.8V power rail for its internal logic, so I routed an AP2112K low-dropout regulator on the board to safely step down the supply voltage. I also included test pads on the printed circuit board so I can easily flash and debug firmware during assembly.

## Planned Features:

Some features are works in progress for the custom wireless mouse. The features are:

* Battery charging and power management for the ESP32-C3 wireless mouse

* Dynamic CPI/DPI sensitivity switching stored in sensor registers

* Direct Bluetooth Low Energy HID pairing without needing a USB receiver dongle

* Low-power sleep modes to conserve battery life during inactivity

* Final firmware integration for matrix scanning and optical tracking

## Current Status:

The DIY wireless mouse project is currently in the design stage.

* I have completed the 3D CAD shell model

* I have designed the printed circuit board in KiCad

* I have checked the component fit using a 3D assembly

* I have generated and uploaded the design files

* I still need to order the parts and PCBs for the custom mouse

* I have not started the physical assembly of the wireless mouse

* The firmware code and BLE power management are still being developed

## BMO:

| Item | Quantity | Source / Designer Link | Price (€/$) |
| --- | --- | --- | --- |
| PMW3360DM-T2QU | 1 | [AliExpress Link](https://tr.aliexpress.com/item/1005005318848362.html?gatewayAdapt=glo2tur) | 13 |
| tactile button click switch | 50pcs | [AliExpress Link](https://tr.aliexpress.com/item/1005003792387482.html) | 13 |
| AP2112K | 10pcs | [AliExpress Link](https://tr.aliexpress.com/item/32998839398.html) | 13 |
| resistors | 1 box | [AliExpress Link](https://tr.aliexpress.com/item/1005011772534173.html) | 15 |
| mcp23017 | 1 | [AliExpress Link](https://tr.aliexpress.com/item/1005012543943820.html) | 2 |
| rotery encoder | 5pcs | [AliExpress Link](https://tr.aliexpress.com/item/1005005181295203.html) | 2 |
| esp32 | 3 | [Seeed Studio Link](https://www.seeedstudio.com/Seeed-Studio-XIAO-ESP32C3-3PCS-p-5920.html) | 13 |
| pcb | 8 | [JLCPCB Link](https://cart.jlcpcb.com/quote?businessType=projectFile&fileAccessId=8775813919665467392&bomId&cplId&projectBizKey=610380259701374978) | 23 |

I will continue updating the repository as the custom wireless mouse hardware comes together. Once all the parts arrive and the build begins I will add assembly photos, firmware and documentation covering the process of the mouse build.
