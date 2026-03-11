
int button = 2;  
int red = 5;
int yellow = 6;
int green = 7;
int val = 0;
int valx=0;
int valy=0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  // set baud rate as 9600
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(yellow, OUTPUT);  
  pinMode(2,INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  valx = analogRead(A0);  // read analog
  valy = analogRead(A1);
  val = analogRead(A2);
  button = digitalRead(2);
  Serial.print(" X: ");
  Serial.print(valx);
  Serial.print(" Y: ");
  Serial.print(valy);
  Serial.print(" : ");
  Serial.print(val);
  Serial.print(" button: ");
  Serial.println(button);
  if(valx >550 ){
    analogWrite(red,val/4);
    digitalWrite(yellow,LOW);
  }
  else if(valx<450){
    analogWrite(yellow,val/4);
    digitalWrite(red,LOW);
  }
  else if(valy<450){
    analogWrite(green,val/4);
  }
  else if(valy>550){
    analogWrite(red,val/4);
    analogWrite(yellow,val/4);
    analogWrite(green,val/4);
  }
  else{
    digitalWrite(red,LOW);
    digitalWrite(yellow,LOW);
    digitalWrite(green,LOW);
  }
  if (button == 0){
    analogWrite(red,val/4);
    analogWrite(yellow,val/4);
    analogWrite(green,val/4);
    delay(500);
    digitalWrite(red,LOW);
    digitalWrite(yellow,LOW);
    digitalWrite(green,LOW);
  }
  delay(500);
}
