#define NUM_ROWS 8
#define NUM_COLS 8

// Define row and column pins
int rowPins[NUM_ROWS] = {32, 33, 25, 26, 27, 14, 12, 13}; // H1
int colPins[NUM_COLS] = {23, 22, 21, 19, 18,  5, 17, 16}; // H2

// Define musical notes for each grid position
// Empty positions are marked with an empty string ""
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

void setup() {
    Serial.begin(115200);
    
    // Your existing setup code here
    // Initialize row pins as outputs and set to HIGH (inactive)
    for (int r = 0; r < NUM_ROWS; r++) {
        pinMode(rowPins[r], OUTPUT);
        digitalWrite(rowPins[r], HIGH);
    }

    // Initialize column pins as inputs with pull-up resistors
    for (int c = 0; c < NUM_COLS; c++) {
        pinMode(colPins[c], INPUT_PULLUP);
    }
}

void loop() {
    // Create a grid to store key states
    int grid[NUM_ROWS][NUM_COLS] = {0};
    
    // Scan the matrix
    for (int r = 0; r < NUM_ROWS; r++) {
        digitalWrite(rowPins[r], LOW);
        
        for (int c = 0; c < NUM_COLS; c++) {
            if (digitalRead(colPins[c]) == LOW) {
                grid[r][c] = 1;
            }
        }
        
        digitalWrite(rowPins[r], HIGH);
    }
    
    // Print pressed notes
    Serial.println("Pressed Notes:");
    for (int r = 0; r < NUM_ROWS; r++) {
        for (int c = 0; c < NUM_COLS; c++) {
            if (grid[r][c] == 1) {
                Serial.print("Position [");
                Serial.print(r);
                Serial.print("][");
                Serial.print(c);
                Serial.print("]: ");
                Serial.println(noteGrid[r][c]);
            }
        }
    }
    
    Serial.println(); // Add a blank line for readability
    delay(200); // Small delay to prevent spamming the serial monitor
}