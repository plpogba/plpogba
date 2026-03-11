#include <LiquidCrystal.h>
#define ECHO 8
#define TRIGGER 7
#define RED 3 // define RED pin
#define GREEN 5 // define GREEN pin
#define BLUE 9 // define BLUE pin

LiquidCrystal lcd(4, 6, 10, 11, 12, 13);
void setup() {
lcd.begin(16, 2);
lcd.clear();
pinMode(ECHO,INPUT);
pinMode(TRIGGER,OUTPUT);
pinMode(RED, OUTPUT); 
pinMode(GREEN, OUTPUT);
pinMode(BLUE, OUTPUT);
}
void setColor(int r, int g, int b)
{
analogWrite(RED, r);
analogWrite(GREEN, g);
analogWrite(BLUE, b);
}
void loop() {
long duration, distance;
digitalWrite(TRIGGER,LOW);
delayMicroseconds(2);
digitalWrite(TRIGGER,HIGH);
delayMicroseconds(10);
digitalWrite(TRIGGER,LOW);
//calculate distance
duration = pulseIn(ECHO,HIGH);
distance = duration * 34000 / 1000000 / 2;
lcd.setCursor(4,0);
lcd.print("Distance");
lcd.setCursor(5,1);
lcd.print(distance);
if(distance < 10){ // 한 번 출력된 곳은 자동으로 지워지지 않음
lcd.setCursor(6,1);
lcd.print(" ");
}
if(distance < 10){
  setColor(255,0,0);
}
else if(distance >= 10 && distance < 20){
  setColor(255,255,0);
}
else if(distance>=20 && distance <30){
  setColor(0,128,0);
}
else {
  setColor(0,0,255);
}
lcd.setCursor(10,1);
lcd.print("Cm");
delay(1000);
lcd.clear();
}