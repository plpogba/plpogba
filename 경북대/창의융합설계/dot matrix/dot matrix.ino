#include <FrequencyTimer2.h>

#define A  { \
    {0, 0, 0, 1, 1, 0, 0, 0}, \
    {0, 0, 1, 1, 1, 1, 0, 0}, \
    {0, 0, 1, 1, 1, 1, 0, 0}, \
    {0, 1, 1, 0, 0, 1, 1, 0}, \
    {0, 1, 1, 1, 1, 1, 1, 0}, \
    {1, 1, 1, 1, 1, 1, 1, 1}, \
    {1, 1, 0, 0, 0, 0, 1, 1}, \
    {1, 1, 0, 0, 0, 0, 1, 1}  \
} 

#define D { \
    {1, 1, 1, 1, 1, 0, 0, 0},\
    {1, 1, 0, 0, 1, 1, 0, 0},\
    {1, 1, 0, 0, 0, 1, 1, 0},\
    {1, 1, 0, 0, 0, 1, 1, 0},\
    {1, 1, 0, 0, 0, 1, 1, 0},\
    {1, 1, 0, 0, 0, 1, 1, 0},\
    {1, 1, 0, 0, 1, 1, 0, 0},\
    {1, 1, 1, 1, 1, 0, 0, 0},\
}
 
#define U { \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 0, 1, 1, 1, 1, 0, 0}  \
} 
 
#define I { \
    {0, 0, 1, 1, 1, 1, 0, 0},\
    {0, 0, 0, 1, 1, 0, 0, 0},\
    {0, 0, 0, 1, 1, 0, 0, 0},\
    {0, 0, 0, 1, 1, 0, 0, 0},\
    {0, 0, 0, 1, 1, 0, 0, 0},\
    {0, 0, 0, 1, 1, 0, 0, 0},\
    {0, 0, 0, 1, 1, 0, 0, 0},\
    {0, 0, 1, 1, 1, 1, 0, 0},\
}

#define N { \
    {0, 1, 0, 0, 0, 0, 1, 0},\
    {0, 1, 1, 0, 0, 0, 1, 0},\
    {0, 1, 0, 1, 0, 0, 1, 0},\
    {0, 1, 0, 1, 0, 0, 1, 0},\
    {0, 1, 0, 0, 1, 0, 1, 0},\
    {0, 1, 0, 0, 1, 0, 1, 0},\
    {0, 1, 0, 0, 0, 1, 1, 0},\
    {0, 1, 0, 0, 0, 0, 1, 0},\
}

#define O { \
    {0, 0, 0, 1, 1, 0, 0, 0}, \
    {0, 0, 1, 0, 0, 1, 0, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 0, 1, 0, 0, 1, 0, 0}, \
    {0, 0, 0, 1, 1, 0, 0, 0}  \
} 
 
#define HEART { \
    {0, 0, 1, 0, 0, 1, 0, 0},\
    {0, 1, 0, 1, 1, 0, 1, 0},\
    {1, 0, 0, 0, 0, 0, 0, 1},\
    {1, 0, 0, 0, 0, 0, 0, 1},\
    {1, 0, 0, 0, 0, 0, 0, 1},\
    {0, 1, 0, 0, 0, 0, 1, 0},\
    {0, 0, 1, 0, 0, 1, 0, 0},\
    {0, 0, 0, 1, 1, 0, 0, 0},\
}

byte col = 0;       // COL을 0으로 초기화
byte leds[8][8];    // 현재 출력해야 할 LED 모양 업로드하는 배열

// 맨 처음 PINS[0]은 사용하지 않기때문에 -1로 설정. 1~16번까지의 핀을 PIN에 연결
int pins[17]= {-1, 5, 4, 3, 2, 14, 15, 16, 17, 13, 12, 11, 10, 9, 8, 7, 6};

// 행 0~7번까지 핀 연결해 주기
int cols[8] = {pins[13], pins[3], pins[4], pins[10], pins[6], pins[11], pins[15], pins[16]};

// 열 0~ 7번까지 핀 연결해 주기
int rows[8] = {pins[9], pins[14], pins[8], pins[12], pins[1], pins[7], pins[2], pins[5]};

const int numPatterns = 7;      // 총 사용할 패턴 수
byte patterns[numPatterns][8][8] = { A, D, U, I, N, O, HEART };   // 위에서 정의한 led 모양을 patterns에 입력해 주기

int pattern = 0;          

void setup() {
  // 1~16번 까지의 핀을 출력으로 설정
  for (int i = 1; i <= 16; i++) {
    pinMode(pins[i], OUTPUT);
  }

  // 행 0~7번까지를 high로 
  for (int i = 0; i < 8; i++) {
    digitalWrite(cols[i], HIGH );
  }
  // 열 0~7번 까지를 low로
  for (int i = 0; i < 8; i++) {
    digitalWrite(rows[i], HIGH);
  }

  clearLeds();   // led 초기화
  
  FrequencyTimer2::disable();
  FrequencyTimer2::setPeriod(2000);
  FrequencyTimer2::setOnOverflow(display);  // leds를 보여주기 위해서 setOnOverFlow를 사용
  
  setPattern(pattern);      
}

void loop() {

    pattern = ++pattern % numPatterns;
    slidePattern(pattern, 100);
}

void clearLeds() {  // led 다 초기화
  for (int i = 0; i < 8; i++) 
    for (int j = 0; j < 8; j++) 
      leds[i][j] = 0;  
}

void setPattern(int pattern) {        // LED 배열에 PATTREN 입력하기
  for (int i = 0; i < 8; i++) 
    for (int j = 0; j < 8; j++) 
      leds[i][j] = patterns[pattern][i][j];   
}

// Interrupt routine
void display() {
  digitalWrite(cols[col], HIGH);  // 이전 행들 다 꺼지게끔 해주기.
  col++;
  if (col == 8) {
    col = 0;
  }
  for (int row = 0; row <= 7; row++) {
    if (leds[col][7 - row] == 1) {        // 위의 배열에서 1일때 해당되는 위치 불빛 켜주기
      digitalWrite(rows[row], HIGH);
    }
    else {                          // 위 배열에서 0일때 해당되는 위치 불빛 꺼주기
      digitalWrite(rows[row], LOW);
    }
  }
  digitalWrite(cols[col], LOW);     // 다음 패턴을 위해 led 다 꺼주기
}

void slidePattern(int pattern, int del) {
  for (int l = 0; l < 8; l++) {
    for (int i = 7; i >0 ; i--) {
      for (int j = 0; j < 8; j++) {
        leds[j][i] = leds[j][i-1];
      }
    }
    for (int j = 0; j < 8; j++) {
      leds[j][0] = patterns[pattern][j][l];
    }
    delay(del);
  }
}
