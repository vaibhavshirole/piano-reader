#define NUM_ROWS 8
#define NUM_COLS 8

// Define row and column pins
int rowPins[NUM_ROWS] = {32, 33, 25, 26, 27, 14, 12, 13}; // H1
int colPins[NUM_COLS] = {23, 22, 21, 19, 18,  5, 17, 16}; // H2

//{32,33,25,26,27,14,13,23,22,21,19,18,5,17,16};    // Non-controversial GPIO

void setup() {
  Serial.begin(115200);

  delay(500);

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
  int grid[NUM_ROWS][NUM_COLS] = {0}; // Create a grid to store key states

  // Scan the matrix
  for (int r = 0; r < NUM_ROWS; r++) {
    // Set the current row to LOW
    digitalWrite(rowPins[r], LOW);

    // Check all columns for key presses
    for (int c = 0; c < NUM_COLS; c++) {
      if (digitalRead(colPins[c]) == LOW) {
        grid[r][c] = 1; // Key is pressed
      }
    }

    // Reset the current row to HIGH
    digitalWrite(rowPins[r], HIGH);
  }

  // Print the grid to the serial monitor
  Serial.println("Key Press Grid:");
  for (int r = 0; r < NUM_ROWS; r++) {
    for (int c = 0; c < NUM_COLS; c++) {
      Serial.print(grid[r][c]);
      Serial.print(" ");
    }
    Serial.println();
  }

  Serial.println(); // Add a blank line for readability
  delay(200); // Small delay to prevent spamming the serial monitor
}
