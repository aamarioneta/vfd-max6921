#define LED_BUILTIN 2
#define LED_BUILTIN_AUX 16

static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10  = 1;

int DELAY = 10;
int clk = D8;
int load = D1;
int din = D2;
/*
Group 1: Pin 26
Group 2: Pin 25
Group 3: Pin 24
Group 4: Pin 23
*/
/* Pin No         26 25 24 23 22 21 20 19 18 17 12 11 10  9  8  7  6  5  4  3 */
/*   OUTx          0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 */ 
int group1[20] = {01,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00};
int group2[20] = {00,01,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00};
int group3[20] = {00,00,01,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00};
int group4[20] = {00,00,00,01,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00};
/* Pin No / Segments
  10
06  09
  07
08  20 //former 12, pin 12 was always brighter than the others, don't know why
  11
*/
int d[10][20]={
/* Pin No 
   26 25 24 23 22 21 20 19 18 17 12 11 10  9  8  7  6  5  4  3 */
  {00,00,00,00,00,00,01,00,00,00,00,01,01,01,01,00,01,00,00,00},//0
  {00,00,00,00,00,00,01,00,00,00,00,00,00,01,00,00,00,00,00,00},//1
  {00,00,00,00,00,00,00,00,00,00,00,01,01,01,01,01,00,00,00,00},//2
  {00,00,00,00,00,00,01,00,00,00,00,01,01,01,00,01,00,00,00,00},//3
  {00,00,00,00,00,00,01,00,00,00,00,00,00,01,00,01,01,00,00,00},//4
  {00,00,00,00,00,00,01,00,00,00,00,01,01,00,00,01,01,00,00,00},//5
  {00,00,00,00,00,00,01,00,00,00,00,01,01,00,01,01,01,00,00,00},//6
  {00,00,00,00,00,00,01,00,00,00,00,00,01,01,00,00,00,00,00,00},//7
  {00,00,00,00,00,00,01,00,00,00,00,01,01,01,01,01,01,00,00,00},//8
  {00,00,00,00,00,00,01,00,00,00,00,01,01,01,00,01,01,00,00,00} //9
};
int nr = 1200;
int nrclk = 0;
 
void setup() {
  pinMode(din, OUTPUT);
  pinMode(load, OUTPUT);
  pinMode(clk, OUTPUT);
  digitalWrite(clk, LOW);
  digitalWrite(load, LOW);
  digitalWrite(din, LOW);
}
 
void loop() {
  writeDigit(group1, d[(int)((nr / 1000) % 10)]);
  writeDigit(group2, d[(int)((nr / 100) % 10)]);
  writeDigit(group3, d[(int)((nr / 10) % 10)]);
  writeDigit(group4, d[(int)((nr / 1) % 10)]);
  if (nrclk % 20 == 0) nr++;
  nrclk++;
  delayMicroseconds(DELAY);
}

void writeDigit(int group[20], int d[20]) {
  digitalWrite(clk, LOW);
  delayMicroseconds(DELAY);
  for (int i = 19; i >= 0; i--) {
    digitalWrite(din, group[i] | d[i]);
    delayMicroseconds(DELAY);
    digitalWrite(clk, HIGH);
    delayMicroseconds(DELAY);
    digitalWrite(clk, LOW);
  }
  digitalWrite(load, HIGH);
  delayMicroseconds(DELAY);
  digitalWrite(load, LOW);
  delayMicroseconds(DELAY);
}
