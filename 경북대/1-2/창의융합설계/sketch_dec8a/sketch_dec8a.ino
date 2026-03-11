void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(8, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(8)) {
    delay(1000);
    if(digitalRead(8))
      Serial.println("1");
  }
  else
    Serial.println("0");
  delay(1000);
}