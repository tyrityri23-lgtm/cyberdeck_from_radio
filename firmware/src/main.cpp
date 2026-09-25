#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MCP23X17.h>
#include <HijelHID_BLEKeyboard.h>

// ============================================================
// CyberDeck from Radio - keyboard controller bring-up firmware
// Target: Seeed Studio XIAO ESP32-C3
//
// IMPORTANT:
// The repository currently does not expose a verified final matrix
// pin map in the public documentation. The 6x10 mapping below is a
// bring-up/default configuration and MUST be replaced with the exact
// row/column mapping from your final KiCad schematic before claiming
// the keyboard is fully tested.
// ============================================================

constexpr uint8_t MCP23017_ADDRESS = 0x20;
constexpr uint8_t MATRIX_ROWS = 6;
constexpr uint8_t MATRIX_COLS = 10;
constexpr uint32_t DEBOUNCE_MS = 12;

// XIAO ESP32-C3 I2C pins according to Seeed's pin map.
constexpr int I2C_SDA_PIN = D4; // GPIO6
constexpr int I2C_SCL_PIN = D5; // GPIO7

Adafruit_MCP23X17 mcp;
HijelHID_BLEKeyboard keyboard("CyberDeck Keyboard", "CyberDeck from Radio", 100);

// Default bring-up matrix: MCP23017 GPA0-GPA5 = rows,
// GPA6-GPA7 + GPB0-GPB7 = columns.
// Change these arrays to match the actual PCB schematic.
const uint8_t rowPins[MATRIX_ROWS] = {0, 1, 2, 3, 4, 5};
const uint8_t colPins[MATRIX_COLS] = {6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

// Simple logical test keymap for a 60-position matrix.
// Layout is intentionally easy to edit. Replace entries with the
// exact key actions used by the physical keyboard.
const char printableKeymap[MATRIX_ROWS][MATRIX_COLS] = {
    {'q','w','e','r','t','y','u','i','o','p'},
    {'a','s','d','f','g','h','j','k','l',';'},
    {'z','x','c','v','b','n','m',',','.','/'},
    {'1','2','3','4','5','6','7','8','9','0'},
    {'-','=','[',']','\\','\'','`',' ','\n','\b'},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '}
};

bool stableState[MATRIX_ROWS][MATRIX_COLS] = {};
bool lastReading[MATRIX_ROWS][MATRIX_COLS] = {};
uint32_t lastChange[MATRIX_ROWS][MATRIX_COLS] = {};

void selectRow(uint8_t row) {
    // All rows idle HIGH; selected row LOW.
    for (uint8_t r = 0; r < MATRIX_ROWS; ++r) {
        mcp.digitalWrite(rowPins[r], HIGH);
    }
    mcp.digitalWrite(rowPins[row], LOW);
}

void sendKey(uint8_t row, uint8_t col) {
    const char key = printableKeymap[row][col];
    if (!keyboard.isPaired()) {
        return;
    }

    switch (key) {
        case '\n':
            keyboard.tap(KEY_RETURN);
            break;
        case '\b':
            keyboard.tap(KEY_BACKSPACE);
            break;
        case ' ':
            keyboard.tap(KEY_SPACE);
            break;
        default:
            keyboard.write(static_cast<uint8_t>(key));
            break;
    }
}

void scanMatrix() {
    const uint32_t now = millis();

    for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
        selectRow(row);
        delayMicroseconds(80);

        for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
            // Columns use pull-ups, so LOW means a pressed switch.
            const bool pressed = (mcp.digitalRead(colPins[col]) == LOW);

            if (pressed != lastReading[row][col]) {
                lastReading[row][col] = pressed;
                lastChange[row][col] = now;
            }

            if ((now - lastChange[row][col]) >= DEBOUNCE_MS &&
                pressed != stableState[row][col]) {
                stableState[row][col] = pressed;

                if (pressed) {
                    sendKey(row, col);
                } else {
                    // Clear any held key state after a transition.
                    keyboard.releaseAll();
                }
            }
        }
    }
}

void printStatus() {
    static uint32_t lastStatus = 0;
    if (millis() - lastStatus < 2000) {
        return;
    }
    lastStatus = millis();

    Serial.print("CyberDeck keyboard | BLE paired: ");
    Serial.print(keyboard.isPaired() ? "YES" : "NO");
    Serial.print(" | free heap: ");
    Serial.println(ESP.getFreeHeap());
}

void setup() {
    Serial.begin(115200);
    delay(200);

    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

    if (!mcp.begin_I2C(MCP23017_ADDRESS, &Wire)) {
        Serial.println("ERROR: MCP23017 was not detected at address 0x20.");
        while (true) {
            delay(1000);
        }
    }

    for (uint8_t r = 0; r < MATRIX_ROWS; ++r) {
        mcp.pinMode(rowPins[r], OUTPUT);
        mcp.digitalWrite(rowPins[r], HIGH);
    }

    for (uint8_t c = 0; c < MATRIX_COLS; ++c) {
        mcp.pinMode(colPins[c], INPUT_PULLUP);
    }

    keyboard.begin();

    Serial.println("CyberDeck keyboard firmware started.");
    Serial.println("Pair the host with 'CyberDeck Keyboard'.");
    Serial.println("Verify the row/column arrays against the final schematic before full testing.");
}

void loop() {
    scanMatrix();
    printStatus();
    delay(1);
}
