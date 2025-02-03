#include <WiFi.h>
#include <WebSocketsServer.h>

#define NUM_ROWS 8
#define NUM_COLS 8

// Wi-Fi credentials
const char* ssid = "";
const char* password = "";

// Define row and column pins
int rowPins[NUM_ROWS] = {32, 33, 25, 26, 27, 14, 12, 13}; // H1
int colPins[NUM_COLS] = {23, 22, 21, 19, 18,  5, 17, 16}; // H2

// Define musical notes for each grid position
const char* noteGrid[NUM_ROWS][NUM_COLS] = {
    {"X",   "X",    "X",    "C6",   "B5",   "A#5",  "A5",   "G#5"},
    {"G5",  "F#5",  "F5",   "E5",   "D#5",  "D5",   "C#5",  "C5"},
    {"B4",  "A#4",  "A4",   "G#4",  "G4",   "F#4",  "F4",   "E4"},
    {"D#4", "D4",   "C#4",  "C4",   "B3",   "A#3",  "A3",   "G#3"},
    {"G3",  "F#3",  "F3",   "E3",   "D#3",  "D3",   "C#3",  "C3"},
    {"B2",  "A#2",  "A2",   "G#2",  "G2",   "F#2",  "F2",   "E2"},
    {"D#2", "D2",   "C#2",  "C2",   "B1",   "A#1",  "A1",   "G#1"},
    {"G1",  "F#1",  "F1",   "E1",   "D#1",  "D1",   "C#1",  "C1"}
};

// WebSocket server on port 81
WebSocketsServer webSocket(81);

// Function to handle WebSocket events
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    switch (type) {
        case WStype_CONNECTED:
            Serial.printf("Client %u connected\n", num);
            break;
        case WStype_DISCONNECTED:
            Serial.printf("Client %u disconnected\n", num);
            break;
        case WStype_TEXT:
            Serial.printf("Received: %s\n", payload);
            break;
    }
}

void setup() {
    Serial.begin(115200);

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to Wi-Fi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWi-Fi connected.");
    Serial.print("ESP32 IP Address: ");
    Serial.println(WiFi.localIP());

    // Initialize row pins as outputs and set to HIGH (inactive)
    for (int r = 0; r < NUM_ROWS; r++) {
        pinMode(rowPins[r], OUTPUT);
        digitalWrite(rowPins[r], HIGH);
    }

    // Initialize column pins as inputs with pull-up resistors
    for (int c = 0; c < NUM_COLS; c++) {
        pinMode(colPins[c], INPUT_PULLUP);
    }

    // Start WebSocket server
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
}

void loop() {
    webSocket.loop();

    // Create a grid to store key states
    int grid[NUM_ROWS][NUM_COLS] = {0};
    bool anyKeyPressed = false;

    // Scan the matrix
    for (int r = 0; r < NUM_ROWS; r++) {
        digitalWrite(rowPins[r], LOW);
        for (int c = 0; c < NUM_COLS; c++) {
            if (digitalRead(colPins[c]) == LOW) {
                grid[r][c] = 1;
                anyKeyPressed = true;
            }
        }
        digitalWrite(rowPins[r], HIGH);
    }

    // Prepare data to send
    String message = "Pressed Notes: ";
    for (int r = 0; r < NUM_ROWS; r++) {
        for (int c = 0; c < NUM_COLS; c++) {
            if (grid[r][c] == 1) {
                message += noteGrid[r][c];
                message += " ";
            }
        }
    }

    // If no keys were pressed, send a "No keys pressed" message
    if (!anyKeyPressed) {
        message = "No keys pressed";
    }

    // Send the message via WebSocket
    webSocket.broadcastTXT(message);

    delay(500);  // Update frequency (0.5 seconds)
}
