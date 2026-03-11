#define CDS A0  // set analog interface #A0 to connect cds sensor
int val = 0;    // define variable val
int red = 5;
int yellow = 6;
int green = 7;

void setup() {
  Serial.begin(9600);  // set baud rate as 9600
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(yellow, OUTPUT);
}
void loop() {
  val = analogRead(CDS);  // read analog
  Serial.println(val);
  if (val < 500) {
    digitalWrite(red, HIGH);
    digitalWrite(green, LOW);
    digitalWrite(yellow, LOW);
  } 
  else if (val >= 500 && val < 800) {
    digitalWrite(yellow, HIGH);
    digitalWrite(green, LOW);
    digitalWrite(red, LOW);
  }
  else {
    digitalWrite(green, HIGH);
    digitalWrite(red, LOW);
    digitalWrite(yellow, LOW);
  }
  delay(1000);  // delay 1.0s
}