// cyberdeck_radio - key matrix firmware
// board: Seeed XIAO ESP32C3
// expander: MCP23017 on the I2C bus (shares the bus with the BNO085, see
// notes at the bottom - IMU code isn't in here yet, just leaving the hook)
//
// matrix is 6 rows x 10 cols, 51 switches actually populated (SW1-SW51).
// every switch has its own diode (SM5059 footprint on the board) so I'm
// not bothering with ghost-key detection, that's a non-issue here.
//
// row pins  -> MCP GPA0-GPA5  (row1-row6)
// col pins  -> MCP GPB0-GPB7  (col1-col8), then col9/col10 ride on GPA6/GPA7
// since GPB only has 8 pins and I needed 10 columns total
//
// NOTE TO SELF: double check the swNumber table below against the actual
// kicad_sch before flashing a real board, I filled it in row-major and
// I'm only like 90% sure that matches how I actually traced it out in
// the pcb layout. if keys report the wrong SW# but scanning otherwise
// works fine, this table is where to look first.
//
// rev history-ish:
//  - v0.1: 4x4 test rig on a breadboard, no diodes, just proving MCP i2c worked
//  - v0.2: this. real 6x10 board, diodes added, ghosting code ripped out
//    because it doesn't apply anymore

#include <Wire.h>
#include <Adafruit_MCP23X17.h>

Adafruit_MCP23X17 mcp;

const uint8_t NUM_ROWS = 6;
const uint8_t NUM_COLS = 10;

// GPA0..GPA5, straightforward
const uint8_t rowPins[NUM_ROWS] = {0, 1, 2, 3, 4, 5};

// col1-col8 on GPB0-GPB7, col9/col10 borrow the two free GPA pins
const uint8_t colPins[NUM_COLS] = {8, 9, 10, 11, 12, 13, 14, 15, 6, 7};

// which physical SW number lives at each row/col intersection.
// 0 means that grid position isn't populated (51 switches across a
// 6x10 grid leaves 9 empty slots, all crammed into the last row here)
const uint8_t swNumber[NUM_ROWS][NUM_COLS] = {
  { 1,  2,  3,  4,  5,  6,  7,  8,  9, 10},
  {11, 12, 13, 14, 15, 16, 17, 18, 19, 20},
  {21, 22, 23, 24, 25, 26, 27, 28, 29, 30},
  {31, 32, 33, 34, 35, 36, 37, 38, 39, 40},
  {41, 42, 43, 44, 45, 46, 47, 48, 49, 50},
  {51,  0,  0,  0,  0,  0,  0,  0,  0,  0}
};

// debounce window - 25ms has felt right on the mechanical switches I'm
// using, bump it up if you get double-fires with whatever switches you
// actually populated the board with
const unsigned long DEBOUNCE_MS = 25;

// tiny settle delay after driving a row low, before trusting the column
// reads. probably overkill for i2c-speed stuff but costs nothing
const unsigned long ROW_SETTLE_US = 50;

// per-key tracking so multiple simultaneous presses across different
// rows don't step on each other. with diodes in place this should give
// true N-key rollover, not just "2 keys ok, 3+ gets weird"
bool keyState[NUM_ROWS][NUM_COLS];
bool lastReading[NUM_ROWS][NUM_COLS];
unsigned long lastChangeTime[NUM_ROWS][NUM_COLS];

void setup() {
  Serial.begin(115200);
  delay(800); // just enough for the serial monitor to catch up on plug-in

  Serial.println("cyberdeck_radio - matrix scanner up");

  // the MCP has occasionally needed a second attempt right after power-on
  // in my testing, not sure if that's a me problem or a hardware settling
  // thing, but the retry costs nothing so it stays in
  if (!mcp.begin_I2C()) {
    delay(200);
    if (!mcp.begin_I2C()) {
      Serial.println("MCP23017 not responding - check I2C addr / wiring, bailing out");
      while (true) {
        delay(500); // stuck here on purpose, no point spamming retries forever
      }
    }
  }

  for (uint8_t r = 0; r < NUM_ROWS; r++) {
    mcp.pinMode(rowPins[r], OUTPUT);
    mcp.digitalWrite(rowPins[r], HIGH); // idle high, pulled low one at a time during scan
  }
  for (uint8_t c = 0; c < NUM_COLS; c++) {
    mcp.pinMode(colPins[c], INPUT_PULLUP);
  }

  Serial.println("scanning...");
}

void loop() {
  for (uint8_t r = 0; r < NUM_ROWS; r++) {
    mcp.digitalWrite(rowPins[r], LOW);
    delayMicroseconds(ROW_SETTLE_US);

    for (uint8_t c = 0; c < NUM_COLS; c++) {
      if (swNumber[r][c] == 0) {
        continue; // nothing physically wired at this intersection, skip it
      }

      bool pressed = (mcp.digitalRead(colPins[c]) == LOW);
      handleKey(r, c, pressed);
    }

    mcp.digitalWrite(rowPins[r], HIGH);
  }

  // deliberately no delay() here - the debounce timers handle timing on
  // a per-key basis, adding a blanket delay would just slow the whole
  // scan down for no benefit
}

void handleKey(uint8_t r, uint8_t c, bool rawState) {
  unsigned long now = millis();

  // reading flipped since last check - restart this key's debounce clock
  if (rawState != lastReading[r][c]) {
    lastChangeTime[r][c] = now;
    lastReading[r][c] = rawState;
  }

  // only commit to a new state once it's held steady past the debounce window
  if (now - lastChangeTime[r][c] >= DEBOUNCE_MS && rawState != keyState[r][c]) {
    keyState[r][c] = rawState;

    // printing SW-number instead of raw row/col because that's what
    // actually matches the silkscreen on the board, easier to sanity
    // check with a multimeter or by just pressing the physical key
    Serial.print(rawState ? "DOWN SW" : "UP   SW");
    Serial.println(swNumber[r][c]);
  }
}

// --------------------------------------------------------------------
// where the IMU hooks in (not wired up yet, just leaving breadcrumbs)
//
// BNO085 (U3 on the schematic) is on the same I2C bus as the MCP23017,
// so it's already electrically reachable from this sketch. to add it:
//   1. #include <Adafruit_BNO08x.h>
//   2. declare a global Adafruit_BNO08x bno;
//   3. bno.begin_I2C() right after mcp.begin_I2C() succeeds in setup()
//   4. enableReport() for whichever sensor report you actually want
//      (rotation vector is the obvious one for a handheld deck, but
//      linear accel or gyro might make more sense depending on what
//      the IMU's actually for on this build)
//   5. read it on its own timer inside loop(), NOT every pass - don't
//      want IMU polling slowing down key scan responsiveness
//
// haven't decided yet if IMU data should ride the same serial stream as
// key events or go out some other way, that's a later-me problem
// --------------------------------------------------------------------
