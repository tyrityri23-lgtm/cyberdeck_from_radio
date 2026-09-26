### Step 1: **Order the PCB**
Upload your **Gerber files** (found in the `/kicad` folder) to your preferred PCB fabrication house. 
> **Note:** We recommend standard FR4 1.6mm thickness for the best structural integrity.

### Step 2: **Solder SMD Passives**
Solder the **100nF decoupling capacitors** and required **pull-up resistors** first. Keep your iron temperature stable to avoid lifting the pads on the PCB.

### Step 3: **Mount Core Components**
Install the **Seeed Studio XIAO ESP32-C3** and the **PMW3360 breakout board** into their footprints. Ensure the **MCU** is perfectly flat against the board surface.

### Step 4: **Wire Inputs**
Solder your primary microswitches (e.g., **Kailh GM 8.0**) and the **TTC Gold** mechanical scroll wheel encoder. Route the wiring to the board pads with minimal slack to prevent internal cable crowding.

### Step 5: **Integrate Power**
Connect your **LiPo battery** to the **inline slide switch** and then to the board power pads. 
> **WARNING:** Always verify voltage polarity with a multimeter before finalizing the battery connection to prevent damage.

### Step 6: **USB Connectivity**
Connect the **XIAO ESP32-C3** to your workstation using a reliable **USB-C data cable**.

### Step 7: **Firmware Flashing**
Launch **Arduino IDE** or **PlatformIO**. Carefully verify your **pin definitions** align with your specific PCB layout, then compile and flash the firmware.

### Step 8: **Functional Verification**
Observe the **Serial Monitor** output. Confirm that the **PMW3360** successfully streams movement data and that every microswitch click registers within your debugging software.

### Step 9: **Enclosure Fabrication**
Slice and print the **top and bottom shell** components using the provided `.stl` files from the CAD directory. 

### Step 10: **Install Optics**
Seat the **PMW3360 plastic lens** assembly into the sensor. 
> **Critical:** The lens *must* be perfectly centered with the aperture in the bottom shell, or tracking will feel jittery or inconsistent.

### Step 11: **Internal Mounting**
Secure the **PCB** into the bottom shell. Use **double-sided foam tape** to anchor the **LiPo battery** into its specific cut-out pocket.

### Step 12: **Final Assembly**
Close the housing shells, ensuring the **scroll wheel** rotates freely without rubbing against the top casing. Secure the enclosure using your machine screws.
