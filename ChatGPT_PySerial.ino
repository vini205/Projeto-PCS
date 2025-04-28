const int botao1 = 13;
const int botao2 = 12;
const int botao3 = 14;
const int botao4 = 27;
const int pinLedVerde = 32;
const int pinLedVermelho = 33;

bool stateLedVermelho = false; // Stores the state of the first LED (false = OFF, true = ON)
bool stateLedVerde = false; // Stores the state of the second LED (false = OFF, true = ON)

void setup() {
  Serial.begin(115200);
  pinMode(botao1, INPUT_PULLUP);
  pinMode(botao2, INPUT_PULLUP);
  pinMode(botao3, INPUT_PULLUP);
  pinMode(botao4, INPUT_PULLUP);

  pinMode(pinLedVermelho, OUTPUT); 
  pinMode(pinLedVerde, OUTPUT);

  digitalWrite(pinLedVermelho, LOW); // Turn the first LED OFF initially
  digitalWrite(pinLedVerde, LOW); // Turn the second LED OFF initially
}

void loop() {
  static bool botaoPressionado = false;

  if (!botaoPressionado) {
    if (digitalRead(botao1) == LOW) { Serial.println("BOTAO:1"); botaoPressionado = true; }
    else if (digitalRead(botao2) == LOW) { Serial.println("BOTAO:2"); botaoPressionado = true; }
    else if (digitalRead(botao3) == LOW) { Serial.println("BOTAO:3"); botaoPressionado = true; }
    else if (digitalRead(botao4) == LOW) { Serial.println("BOTAO:4"); botaoPressionado = true; }
  }

  if (digitalRead(botao1) == HIGH && digitalRead(botao2) == HIGH &&
      digitalRead(botao3) == HIGH && digitalRead(botao4) == HIGH) {
    botaoPressionado = false;
  }

  // Verifica comandos da serial
  if (Serial.available()) {
    String comando = Serial.readStringUntil('\n');
    if (comando == "LedVerde=1") digitalWrite(pinLedVerde, HIGH);
    else if (comando == "LedVerde=0") digitalWrite(pinLedVerde, LOW);
    else if (comando == "LedVermelho=1") digitalWrite(pinLedVermelho, HIGH);
    else if (comando == "LedVermelho=0") digitalWrite(pinLedVermelho, LOW);
  }

  delay(50); // um delay menor é suficiente
}

/*const int ledPin1 = 25; // Pin number where the first LED is connected
const int buttonPin1 = 12; // Pin number where the first button is connected

const int ledPin2 = 26; // Pin number where the second LED is connected
const int buttonPin2 = 13; // Pin number where the second button is connected

const int ledPin3 = 27; // Pin number where the third LED is connected
const int buttonPin3 = 14; // Pin number where the third button is connected

bool ledState1 = false; // Stores the state of the first LED (false = OFF, true = ON)
bool ledState2 = false; // Stores the state of the second LED (false = OFF, true = ON)
bool ledState3 = false; // Stores the state of the third LED (false = OFF, true = ON)

bool button2Pressed = false; // Flag to track the state of the second button
bool button3Pressed = false; // Flag to track the state of the third button

unsigned long button3PressTime = 0; // Variable to store the time when button 3 is pressed
const unsigned long longPressDuration = 1000; // Define the duration for a long press (in milliseconds)

void setup() {
  // Start the serial communication
  Serial.begin(115200);

  pinMode(ledPin1, OUTPUT); // Set the first LED pin as an OUTPUT
  pinMode(buttonPin1, INPUT_PULLUP); // Set the first button pin as an INPUT with internal pull-up resistor

  pinMode(ledPin2, OUTPUT); // Set the second LED pin as an OUTPUT
  pinMode(buttonPin2, INPUT_PULLUP); // Set the second button pin as an INPUT with internal pull-up resistor

  pinMode(ledPin3, OUTPUT); // Set the third LED pin as an OUTPUT
  pinMode(buttonPin3, INPUT_PULLUP); // Set the third button pin as an INPUT with internal pull-up resistor

  digitalWrite(ledPin1, LOW); // Turn the first LED OFF initially
  digitalWrite(ledPin2, LOW); // Turn the second LED OFF initially
  digitalWrite(ledPin3, LOW); // Turn the third LED OFF initially
}

void loop() {
  // Check if the first button is pressed (LOW state when pressed due to the internal pull-up resistor)
  if (digitalRead(buttonPin1) == LOW) {
    digitalWrite(ledPin1, HIGH); // Turn the first LED ON when the first button is pressed
  } else {
    digitalWrite(ledPin1, LOW); // Turn the first LED OFF when the first button is released
  }

  // Check if the second button is pressed (LOW state when pressed due to the internal pull-up resistor)
  if (digitalRead(buttonPin2) == LOW) {
    if (!button2Pressed) {
      button2Pressed = true;
      ledState2 = !ledState2; // Toggle the state of the second LED when the second button is pressed
      digitalWrite(ledPin2, ledState2 ? HIGH : LOW);
    }
  } else {
    button2Pressed = false; // Reset the button pressed flag
  }

  // Check if the third button is pressed (LOW state when pressed due to the internal pull-up resistor)
  if (digitalRead(buttonPin3) == LOW) {
    if (!button3Pressed) {
      button3Pressed = true;
      button3PressTime = millis(); // Record the time when the button was pressed
    }
  } else {
    if (button3Pressed) {
      // Calculate the duration of the button press
      unsigned long button3Duration = millis() - button3PressTime;
      if (button3Duration >= longPressDuration) {
        // If the button was pressed for a long duration, toggle the state of the third LED
        ledState3 = !ledState3;
        digitalWrite(ledPin3, ledState3 ? HIGH : LOW);
      }
      button3Pressed = false; // Reset the button pressed flag
    }
  }
}*/