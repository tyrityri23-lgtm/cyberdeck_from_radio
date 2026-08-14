 ## Cyberdeck:

I am building a cyberdeck inside a radio chassis. The project is not finished yet. I have completed the computer aided design work, printed circuit board design and overall hardware layout. Now I need to order the parts print the case and start putting everything

The main computer is a Radxa X4. I chose the Radxa X4 because it is a single-board computer that can run PC operating systems and applications. I used FreeCAD to design the enclosure. I imported a model of my custom printed circuit board into the assembly to make sure everything fits correctly before manufacturing the custom printed circuit board.

## Hardware:

* Radxa X4 single-board computer

* Custom keyboard printed circuit board

* ESP32-C3 keyboard controller

* MCP23017 I/O expander for the keyboard matrix

* gyroscope for future motion-based features

* Internal display mounted in a vintage radio enclosure

* Battery-powered design but the power management is still in development

## Keyboard:

I did not use an off-the-shelf keyboard. Instead I designed my printed circuit board with a custom key matrix. The keyboard is controlled by an ESP32-C3 with an MCP23017 I/O expander. I also included spare connections on the printed circuit board so I can add extra hardware later without redesigning the printed circuit board.

## Planned Features:

Some features are works in progress including the Radxa X4 cyberdeck. The features are:

* Battery charging and power management for the Radxa X4 cyberdeck

* Motion-based -theft alarm using the onboard gyroscope of the Radxa X4 cyberdeck

* Software integration between the hardware and the Radxa X4 cyberdeck

* Final firmware for the keyboard controller of the Radxa X4 cyberdeck

## Current Status:

The Radxa X4 cyberdeck project is currently in the design stage.

* I have completed the computer aided design model

* I have designed the printed circuit board

* I have checked the component fit using a 3D assembly

* I have uploaded the design files

* I still need to order the parts, for the Radxa X4 cyberdeck
 I have not started the assembly of the Radxa X4 cyberdeck

* The software and power management of the Radxa X4 cyberdeck are still being developed


## BMO: 

| Item | Quantity | Source / Designer Link | Price (€/$) |
| --- | --- | --- | --- |
| Diodes | 1 (100pcs) | [AliExpress Link](https://tr.aliexpress.com/item/1005003194692663.html) | 1.15 |
| Power Bank | 1 | [AliExpress Link](https://tr.aliexpress.com/item/1005008381830946.html) | 20.60 |
| Jumper Cable | 1 (40pcs) | [AliExpress Link](https://tr.aliexpress.com/item/1005002349042216.html) | 31.82 |
| HDMI Cable | 1 | [AliExpress Link](https://tr.aliexpress.com/item/1005008741655638.html) | 1.58 |
| Breadboard | 1 | [AliExpress Link](https://tr.aliexpress.com/item/1005007108273878.html) | 15.42 |
| Capacitors | 1 kit | [AliExpress Link](https://tr.aliexpress.com/item/1005012392473854.html) | 18.09 |
| Resistors | 1 kit | [AliExpress Link](https://tr.aliexpress.com/item/1005011772534173.html) | 14.18 |
| Keys | 60pcs | [AliExpress Link](https://tr.aliexpress.com/item/1005012309085499.html) | 24.86 |
| Gyroscope | 1 | [AliExpress Link](https://tr.aliexpress.com/item/1005005499172228.html) | 18.99 |
| MCP23017-E/SP DIP-28 | 1 | [AliExpress Link](https://tr.aliexpress.com/item/1005005363863079.html) | 13.25 |
| Keycaps | 1 kit | [AliExpress Link](https://tr.aliexpress.com/item/1005009592262575.html) | 16.27 |
| Mouse | 1 | [Amazon Link](https://www.amazon.com/gp/product/B0F6ML5Y5P/) | 27.00 |
| Screen | 1 | [Amazon Link](https://www.amazon.com/gp/product/B084L8Z9G9/) | 46.62 |
| Antenna | 1 | [Amazon Link](https://www.amazon.com/gp/product/B01GDN1T4S/) | 60.03 |
| Seeed Studio XIAO ESP32-C3 | 1 | [Seeed Studio Link](https://www.seeedstudio.com/Seeed-XIAO-ESP32C3-p-5431.html) | 5.00 |
| LattePanda IOTA Palm-sized x86 Single Board Computer (Intel N150, 8GB RAM / 64GB eMMC) + heatsink | 1 | [DFRobot Link](https://www.dfrobot.com/product-2989.html) | 141.00 |
| PCB | 1 | [JLCPCB Link](https://cart.jlcpcb.com/quote) | 44.00 |
## Images:

<img width="845" height="689" alt="Screenshot 2026-08-09 134827" src="https://github.com/user-attachments/assets/f1f6f3d5-0553-4225-a1b4-e288111a76a7" /><img width="731" height="656" alt="Screenshot 2026-08-09 163537" src="https://github.com/user-attachments/assets/fae8f205-5589-404e-b866-5e631d702d0d" />
<img width="360" height="396" alt="Screenshot 2026-08-11 180309" src="https://github.com/user-attachments/assets/ccb64242-fa0c-4cad-9366-1ba1e62bfc21" />
<img width="1072" height="743" alt="Screenshot 2026-07-25 012635" src="https://github.com/user-attachments/assets/c84bded2-c1dc-4723-af62-4393990d22b7" />
<img width="569" height="544" alt="Screenshot 2026-07-26 022314" src="https://github.com/user-attachments/assets/fa600441-92cb-40ad-903a-456ce3f47fdd" />
<img width="743" height="472" alt="Screenshot 2026-07-26 023957" src="https://github.com/user-attachments/assets/628c5bc8-b71a-4abe-a1c8-908c02ec114b" />
<img width="1183" height="815" alt="Screenshot 2026-07-18 144718" src="https://github.com/user-attachments/assets/61549089-bdd0-4000-b339-4b76b837e232" />



I will continue updating the repository as the Radxa X4 cyberdeck hardware comes together. Once all the parts arrive and the build begins I will add assembly photos, firmware and documentation covering the process of the Radxa X4 cyberdeck.
