#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library
#include <SD.h>
#include <SPI.h>
#include "BMP.h"
#include "I2C.h"
#include "FifoCamera.h"
#include <EEPROM.h>
#define EEPROM_SIZE 1
const int VSYNC = 2;  //vertical sync
#define SIOD A4   //SDA
#define SIOC A5   //SCL

const int RRST = 17;  //read reset
const int WRST = 16;  //write reset
const int RCK = 3;   //read clock
const int WR = 9;    //write flag
//OE -> GND     (output enable always on since we control the read clock)
//PWDN not nonnected
//HREF not connected
//STR not connected
//RST -> 3.3V
/*
const int D0 = 13;
const int D1 = 12;/26
const int D2 = 14;//12
const int D3 = 27;//25
const int D4 = 26;//14
const int D5 = 25;//33
const int D6 = 35;//27
const int D7 = 34;//32
*/
#define D0 A0
#define D1 A1
#define D2 A2
#define D3 A3
const int D4 = 4;
const int D5 = 5;
const int D6 = 6;
const int D7 = 7;
/*
const int TFT_CS = 2;   //chip select
const int TFT_RST = 0;  //connected to EN on LOLIN32
const int TFT_DC = 15; */ //data/command pin
//DIN <- MOSI 23
//CLK <- SCK 18
//SD MISO <- 19
const int SD_CS = 10;  //SD card pin on your shield
const int Shutter = 8;

I2C<SIOD, SIOC> i2c;
FifoCamera<I2C<SIOD, SIOC>, RRST, WRST, RCK, WR, D0, D1, D2, D3, D4, D5, D6, D7> camera(i2c);

//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

//#define QQVGA
#define QQQVGA

#ifdef QQVGA
const int XRES = 160;
const int YRES = 128;
#endif
#ifdef QQQVGA
const int XRES = 80;
const int YRES = 60;
#endif
unsigned char bmpHeader[BMP::headerSize];
const int BYTES_PER_PIXEL = 2;
const int frameSize = XRES * YRES * BYTES_PER_PIXEL;
unsigned char frame[frameSize];
//SD CARD
File outFile;

//char str[] = "/TEST11.BMP";
//Button
int ShPinState, BeforeShState = 0, ShFlag = 0, pictureNumber = 0;

void Initial() {
  i2c.init();
  camera.init();
#ifdef QQVGA
  camera.QQVGARGB565();
#endif
#ifdef QQQVGA
  camera.QQQVGARGB565();
#endif
}
void SD_Initial() {
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, LOW);
  delay(1);
  digitalWrite(SD_CS, HIGH);
  delay(1);
  SD.begin(SD_CS);
  delay(1);
  if (!SD.begin(SD_CS)) {
    Serial.println("err strtng SD");
    while (1)
      ;  //If failed, stop here
  }
}
void setup() {
  Serial.begin(115200);
  Serial.println("Initialization...");
  //pinMode(Shutter, INPUT_PULLUP);
  pinMode(Shutter, INPUT);

  //EEPROM.begin(EEPROM_SIZE);

  BMP::construct16BitHeader(bmpHeader, XRES, YRES);

  Initial();

  pinMode(VSYNC, INPUT);
  Serial.println("start");
  //tft.initR(INITR_BLACKTAB);
  //tft.fillScreen(0);
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS,HIGH);
  SD_Initial();
}

void readframe() {
  while (!digitalRead(VSYNC))
    ;
  while (digitalRead(VSYNC))
    ;
  camera.stopCapture();
  camera.prepareCapture();
  while (!digitalRead(VSYNC))
    ;
  camera.startCapture();
}

/*
void displayRGB565() {
  tft.setAddrWindow(0, 0, YRES - 1, XRES - 1);
  int i = 0;
  for (int x = 0; x < XRES; x++)
    for (int y = 0; y < YRES; y++) {
      i = (y * XRES + x) << 1;
      tft.pushColor(frame[i] | (frame[i + 1] << 8));
      //tft.pushColor(((frame[i] | (frame[i + 1] << 8)) >> 1) & 0b111101111101111); //dimming to test for tft error
    }
}

void testTFT()  //a small tft test output showing errors on my tft with bright colors
{
  tft.setAddrWindow(0, 0, 31, 63);
  int i = 0;
  for (int y = 0; y < 64; y++)
    for (int x = 0; x < 32; x++)
      tft.pushColor(x | y << 5);
}

void displayY8() {
  tft.setAddrWindow(0, 0, YRES - 1, XRES - 1);
  int i = 0;
  for (int x = 0; x < XRES; x++)
    for (int y = 0; y < YRES; y++) {
      i = y * XRES + x;
      unsigned char c = frame[i];
      unsigned short r = c >> 3;
      unsigned short g = c >> 2;
      unsigned short b = c >> 3;
      tft.pushColor(r << 11 | g << 5 | b);
    }
}*/

void frameToSerial() {
  int i = 0;
  Serial.println("var frame=[");
  for (int y = 0; y < YRES; y += 1) {
    i = y * XRES;
    for (int x = 0; x < XRES; x += 1) {
      //Serial.print(frame[i + x], HEX);
      Serial.print(frame[i + x]);
      Serial.print(',');
    }
    Serial.println();
  }
  Serial.println("];");
}
void GrabImage() {

  //Create the File
  pictureNumber = EEPROM.read(0) + 1;
  String path = "/picture" + String(pictureNumber) + ".bmp";
  //Serial.println("Picture file name: %s\n", path.c_str());
  outFile = SD.open(path.c_str(), FILE_WRITE);
  //outFile = SD.open(str, FILE_WRITE);
  if (!outFile) {
    Serial.println("err opng file");
    return;
  };

  //char data1,data2;
  for (int i = 0; i < BMP::headerSize; i++) {
    outFile.write(bmpHeader[i]);
    //data1 +=bmpHeader[i];
    //Serial.write(bmpHeader[i]);
  }
  //outFile.write(data1);
  //Serial.println(data1);
  for (int i = 0; i < XRES * YRES * BYTES_PER_PIXEL; i++) {
    outFile.write(frame[i]);
    //data2 +=frame[i];
    //Serial.write(frame[i]);
    //delay(1);
  }
  //delay(2000);
  //outFile.write(data2);
  //Serial.println(data2);
  //Close the file

  outFile.close();
  if (SD.exists(path.c_str())) {
    Serial.println(" Save OK ");
  } else {

    Serial.println("File doesn't save.");
  }
  //EEPROM.write(0, pictureNumber);
  //EEPROM.commit();
}
void loop() {

  while (!digitalRead(VSYNC))
    ;
  while (digitalRead(VSYNC))
    ;
  camera.prepareCapture();
  camera.startCapture();
  while (!digitalRead(VSYNC))
    ;
  camera.stopCapture();
  /*
  while(!digitalRead(VSYNC));
  while(digitalRead(VSYNC));
  camera.stopCapture();
  camera.prepareCapture();
  while(!digitalRead(VSYNC));
  camera.startCapture();
   */
  //color
  //camera.readFrame(frame, XRES, YRES, BYTES_PER_PIXEL);
  //displayRGB565();

  //testTFT();

  //b/w
  //camera.readFrameOnlySecondByte(frame, XRES, YRES);
  //displayY8();
  //Serial.println(digitalRead(Shutter));
  ShPinState = digitalRead(Shutter);
  //Serial.println(ShPinState);

  if (ShPinState == 1 && BeforeShState == 0) {
    ShFlag = 1;
    if (ShFlag == 1) {
      Serial.println("Take a Picture");
      SD_Initial();
      readframe();
      GrabImage();
      Initial();
      Serial.println("Save to SD Card");
    }
  }
  BeforeShState = ShPinState;
  ShFlag = 0;
}