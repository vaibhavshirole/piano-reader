#define NUM_ROWS 8
#define NUM_COLS 8

// Define GPIO pins for rows and columns
int rowPins[NUM_ROWS] = {13, 12, 14, 27, 26, 25, 33, 32};
int colPins[NUM_COLS] = {23, 22,  1,  3, 21, 19, 18,  5};

void setup() {
  Serial.begin(115200);

  // Configure row pins as outputs
  for (int i = 0; i < NUM_ROWS; i++) {
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], HIGH); // Set all rows to HIGH initially
  }

  // Configure column pins as inputs with pull-ups
  for (int i = 0; i < NUM_COLS; i++) {
    pinMode(colPins[i], INPUT_PULLUP);
  }
}

void loop() {
  for (int r = 0; r < NUM_ROWS; r++) {
    // Set the current row to LOW
    digitalWrite(rowPins[r], LOW);

    // Debugging: Log the state of all columns
//    for (int c = 0; c < NUM_COLS; c++) {
//      int state = digitalRead(colPins[c]);
//      Serial.print("Row ");
//      Serial.print(r);
//      Serial.print(", Column ");
//      Serial.print(c);
//      Serial.print(": ");
//      Serial.println(state);
//    }

    // Check all columns for a key press
    for (int c = 0; c < NUM_COLS; c++) {
      if (digitalRead(colPins[c]) == LOW) {
        Serial.print("Key pressed: Row ");
        Serial.print(r);
        Serial.print(", Column ");
        Serial.println(c);
      }
    }

    // Reset the current row to HIGH
    digitalWrite(rowPins[r], HIGH);
  }

  delay(100); // Small delay to stabilize readings
}
