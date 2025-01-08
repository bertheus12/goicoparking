#include <LiquidCrystal.h>

// Pin definitions
const int trigPin = 9;
const int echoPin = 10;
const int buttonPin = 2;  // Pushbutton for car exit
const int redLED = 11;
const int greenLED = 12;

// LCD pin definitions
const int rs = 7, en = 8, d4 = 4, d5 = 5, d6 = 6, d7 = 3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Variables
int totalSpaces = 10;  // Total parking spaces
int freeSpaces = 10;   // Free parking spaces
long duration;
float carHeight;

void setup() {
  // Initialize pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  // Initialize LCD
  lcd.begin(16, 2);
  lcd.print("Spaces: ");
  lcd.setCursor(0, 1);
  lcd.print("Parked: ");
}

void loop() {
  // Measure car height with ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  carHeight = duration * 0.034 / 2;

  // Car entry logic (if height detected and space is available)
  if (carHeight > 5 && carHeight < 50 && freeSpaces > 0) {
    freeSpaces--;
    updateLCD(carHeight);
    delay(2000);  // Debounce delay for ultrasonic sensor
  }

  // Car exit logic (button press)
  if (digitalRead(buttonPin) == LOW) {
    if (freeSpaces < totalSpaces) {
      freeSpaces++;
    }
    updateLCD(-1);  // No height for exiting cars
    delay(500);     // Debounce delay for pushbutton
  }

  // LED indicator logic
  if (freeSpaces == 0) {
    digitalWrite(redLED, HIGH);
    digitalWrite(greenLED, LOW);
  } else {
    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, HIGH);
  }
}

// Function to update LCD display
void updateLCD(float height) {
  lcd.setCursor(8, 0);
  lcd.print(freeSpaces);
  lcd.print("  ");  // Clear extra characters
  lcd.setCursor(8, 1);
  lcd.print(totalSpaces - freeSpaces);
  lcd.print("  ");
  if (height > 0) {
    lcd.setCursor(0, 0);
    lcd.print("Height: ");
    lcd.print(height);
    lcd.print("cm ");
  }
}
