#include <Arduino.h>
#include <Wire.h>
#include <BleMouse.h>

// pin config for the cyberdeck mouse board
#define I2C_SDA 9
#define I2C_SCL 10
#define MCP_ADDR 0x20 // default i2c addr for mcp23017

// rotary encoder pins
#define ENC_A 2
#define ENC_B 3
#define ENC_SW 4

BleMouse deckMouse("Cyberdeck_Mouse", "Custom", 100);

volatile int scroll_counter = 0;
uint8_t enc_last_state = 0;
uint8_t mcp_portb_cache = 0x01; // keep CS high by default so the sensor doesnt glitch

// quick i2c write/read wrappers
void i2c_set_reg(uint8_t r, uint8_t v) {
  Wire.beginTransmission(MCP_ADDR);
  Wire.write(r);
  Wire.write(v);
  Wire.endTransmission();
}

uint8_t i2c_get_reg(uint8_t r) {
  Wire.beginTransmission(MCP_ADDR);
  Wire.write(r);
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)MCP_ADDR, (uint8_t)1);
  if(Wire.available()) {
    return Wire.read();
  }
  
  // log error if the mcp stops responding
  Serial.println("ERR: I2C read failed!");
  return 0;
}

// bitbang spi over the mcp chip
// gpb0=cs, gpb1=clk, gpb2=mosi, gpb3=miso
void sensor_write(uint8_t reg, uint8_t val) {
  // pull CS low
  mcp_portb_cache &= ~0x01;
  i2c_set_reg(0x15, mcp_portb_cache);
  delayMicroseconds(1);
  
  uint8_t tx_data[2] = {(uint8_t)(reg | 0x80), val};
  
  for(int b = 0; b < 2; b++) {
    for (int i = 7; i >= 0; i--) {
      mcp_portb_cache &= ~0x02; // clk low
      
      if((tx_data[b] >> i) & 1) mcp_portb_cache |= 0x04; // mosi high
      else mcp_portb_cache &= ~0x04; // mosi low
      
      i2c_set_reg(0x15, mcp_portb_cache);
      
      mcp_portb_cache |= 0x02; // clk high (samples on rising edge)
      i2c_set_reg(0x15, mcp_portb_cache);
    }
    if(b == 0) delayMicroseconds(35); // delay between addr and data bytes
  }
  
  // pull CS high when done
  mcp_portb_cache |= 0x01;
  i2c_set_reg(0x15, mcp_portb_cache);
  delayMicroseconds(180);
}

uint8_t sensor_read(uint8_t reg) {
  // pull CS low
  mcp_portb_cache &= ~0x01;
  i2c_set_reg(0x15, mcp_portb_cache);
  delayMicroseconds(1);
  
  uint8_t cmd = reg & 0x7F;
  for (int i = 7; i >= 0; i--) {
    mcp_portb_cache &= ~0x02; // clk low
    
    if((cmd >> i) & 1) mcp_portb_cache |= 0x04; 
    else mcp_portb_cache &= ~0x04; 
    
    i2c_set_reg(0x15, mcp_portb_cache);
    mcp_portb_cache |= 0x02; // clk high
    i2c_set_reg(0x15, mcp_portb_cache);
  }
  
  delayMicroseconds(160); // sensor needs time to fetch data
  
  uint8_t result = 0;
  for (int i = 7; i >= 0; i--) {
    mcp_portb_cache &= ~0x02; // clk low
    i2c_set_reg(0x15, mcp_portb_cache);
    
    mcp_portb_cache |= 0x02; // clk high
    i2c_set_reg(0x15, mcp_portb_cache);
    
    uint8_t pins = i2c_get_reg(0x13); // read GPIOB state
    if(pins & 0x08) result |= (1 << i); // check MISO bit
  }
  
  // CS back high
  mcp_portb_cache |= 0x01;
  i2c_set_reg(0x15, mcp_portb_cache);
  delayMicroseconds(20);
  
  return result;
}

void IRAM_ATTR enc_isr() {
  uint8_t state = (digitalRead(ENC_A) << 1) | digitalRead(ENC_B);
  uint8_t combined = (enc_last_state << 2) | state;

  if (combined == 0b0001 || combined == 0b0111 || combined == 0b1110 || combined == 0b1000) scroll_counter++;
  else if (combined == 0b0010 || combined == 0b1011 || combined == 0b1101 || combined == 0b0100) scroll_counter--;
  
  enc_last_state = state;
}

void setup() {
  Serial.begin(115200);
  delay(1000); // give serial monitor time to connect
  Serial.println("Starting deck mouse firmware...");

  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
  pinMode(ENC_SW, INPUT_PULLUP);

  enc_last_state = (digitalRead(ENC_A) << 1) | digitalRead(ENC_B);
  attachInterrupt(digitalPinToInterrupt(ENC_A), enc_isr, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_B), enc_isr, CHANGE);

  Wire.begin(I2C_SDA, I2C_SCL, 400000);

  // config mcp23017 registers
  // port A: GPA0/1 as output for GND rails, rest input
  i2c_set_reg(0x00, 0b11111100); 
  i2c_set_reg(0x0C, 0b00111100); // internal pullups for the switches
  i2c_set_reg(0x12, 0x00); // drive rows low
  
  // port B: bitbang SPI setup
  i2c_set_reg(0x01, 0b11111000);
  i2c_set_reg(0x0D, 0b00001000); // miso pullup
  i2c_set_reg(0x15, mcp_portb_cache); // init outputs safely

  // pmw3360 startup routine
  mcp_portb_cache &= ~0x01; // drop CS low to wake it up
  i2c_set_reg(0x15, mcp_portb_cache);
  delay(10);
  
  sensor_write(0x3A, 0x5A); // force power up reset
  delay(50);
  
  uint8_t dev_id = sensor_read(0x00);
  if(dev_id != 0x42) {
    Serial.println("ERROR: PMW3360 init failed! Check SPI wiring on the MCP.");
  } else {
    Serial.println("Sensor found, id matches 0x42.");
  }
  
  // flush out the burst registers before starting
  sensor_read(0x02); 
  sensor_read(0x03); 
  sensor_read(0x04); 
  sensor_read(0x05); 
  sensor_read(0x06);
  
  // set approx 1600 cpi for standard desktop use
  sensor_write(0x0F, 0x15);

  deckMouse.begin();
  Serial.println("BLE HID service started.");
}

void loop() {
  if (!deckMouse.isConnected()) {
    delay(50);
    return; // dont poll if we arent connected to anything
  }

  // grab button states from port A
  uint8_t a_pins = i2c_get_reg(0x12);
  bool l_click = !(a_pins & 0x04);
  bool r_click = !(a_pins & 0x08);
  bool fwd_btn = !(a_pins & 0x10);
  bool back_btn = !(a_pins & 0x20);
  bool mid_click = !digitalRead(ENC_SW);

  // send updates
  if (l_click) deckMouse.press(MOUSE_LEFT); else deckMouse.release(MOUSE_LEFT);
  if (r_click) deckMouse.press(MOUSE_RIGHT); else deckMouse.release(MOUSE_RIGHT);
  if (mid_click) deckMouse.press(MOUSE_MIDDLE); else deckMouse.release(MOUSE_MIDDLE);
  if (fwd_btn) deckMouse.press(MOUSE_FORWARD); else deckMouse.release(MOUSE_FORWARD);
  if (back_btn) deckMouse.press(MOUSE_BACK); else deckMouse.release(MOUSE_BACK);

  // check for sensor movement
  uint8_t mot = sensor_read(0x02);
  int16_t dx = 0, dy = 0;
  
  if (mot & 0x80) {
    dx = (int16_t)((sensor_read(0x04) << 8) | sensor_read(0x03));
    dy = (int16_t)((sensor_read(0x06) << 8) | sensor_read(0x05));
  }

  // check wheel scroll queue
  signed char wheel = 0;
  if (scroll_counter != 0) {
    noInterrupts();
    wheel = scroll_counter;
    scroll_counter = 0;
    interrupts();
  }

  if (dx != 0 || dy != 0 || wheel != 0) {
    deckMouse.move(constrain(dx, -127, 127), constrain(dy, -127, 127), wheel);
  }

  delay(5);//DONE!!!!!!!
}