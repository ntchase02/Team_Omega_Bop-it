//Nicholas Chase
//Thomas Gallagher-Teske

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  Serial.begin(9600);
 
  pinMode(5, INPUT);
}
 int buttonState = LOW;
// the loop function runs over and over again forever
void loop() {

  
   buttonState = digitalRead(5);
  
  
  if (buttonState == HIGH )
  {
  digitalWrite(6, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(6, LOW);    // turn the LED off by making the voltage LOW
  delay(500);
  digitalWrite(7, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(7, LOW);    // turn the LED off by making the voltage LOW
  delay(500);                       // wait for a second
   digitalWrite(8, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(8, LOW);    // turn the LED off by making the voltage LOW
  delay(500);// wait for a second
  }
}
