#include <Wire.h>
#include <Adafruit_MCP23X17.h>

Adafruit_MCP23X17 mcp1; // my mcp expander board

// pin setup for matrix rows and cols
int rPins[4] = {0,1,2,3}; 
int col_pins[] = {8, 9, 10, 11}; 

void setup() {
  Serial.begin(115200);
  delay(1000); // give serial time to open or it misses first print
  
  Serial.println("--- cyberdeck key matrix test start ---");

  // init i2c
  if (!mcp1.begin_I2C()) {
    Serial.println("ERROR: mcp not found!! check wires / address");
    while(true) { 
      // stuck here if wired wrong
      delay(500); 
    }
  }
  
  // set rows output HIGH default
  for (int x = 0; x < 4; x++) {
    mcp1.pinMode(rPins[x], OUTPUT);
    mcp1.digitalWrite(rPins[x], HIGH);
  }

  // set cols as input pullup
  for (int y = 0; y < 4; y++) {
    mcp1.pinMode(col_pins[y], INPUT_PULLUP);
  }
  
  Serial.println("ready to scan keypresses...");
}

void loop() {
  // loop thru each row 
  for (int r = 0; r < 4; r++) {
    
    // drop current row LOW
    mcp1.digitalWrite(rPins[r], LOW);

    // check cols for connection
    for (int c = 0; c < 4; c++) {
      
      int state = mcp1.digitalRead(col_pins[c]);
      
      if (state == LOW) {
        // key is down!
        Serial.print("pressed -> row ");
        Serial.print(r);
        Serial.print(" | col ");
        Serial.println(c);
        
        // simple debouncing delay
        delay(180); 
        // Serial.println("debug: loop active"); 
      }
    }

    // reset row HIGH before next iteration
    mcp1.digitalWrite(rPins[r], HIGH);
  }
}