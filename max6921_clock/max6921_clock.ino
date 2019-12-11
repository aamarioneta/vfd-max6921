#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "credentials.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, 3600);//gmt+1

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

int DELAY = 2;
int clk = D8;
int load = D1;
int din = D2;

int second = 0;
int hour = 20;
int minute = 22;
int year;
int month;
int dayOfMonth;
int time_1;
int dayOfWeek = 0;

/*
Group 1: Pin 26
Group 2: Pin 25
Group 3: Pin 24
Group 4: Pin 23
*/
/*   OUTx                 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 */ 
/* Pin No                26 25 24 23 22 21 20 19 18 17 12 11 10  9  8  7  6  5  4  3 */
int hourTens[20]      = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int hourOnes[20]      = { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int minuteTens[20]    = { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int minuteOnes[20]    = { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int dayTens[20]       = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int dayOnes[20]       = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
int monthTens[20]     = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0};
int monthOnes[20]     = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0};
int groupSunday[20]   = { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int groupMonday[20]   = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int groupThursday[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int d[10][20]={
/* Pin No 
                         26 25 24 23 22 21 20 19 18 17 12 11 10  9  8  7  6  5  4  3 */
                        { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0},//0
                        { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},//1
                        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0},//2
                        { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0},//3
                        { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0},//4
                        { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0},//5
                        { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0},//6
                        { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},//7
                        { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0},//8
                        { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0} //9
};

/* Pin No / Segments
  10
06  09
  07
08  20 //former 12, pin 12 was always brighter than the others, don't know why
  11
*/

/* Pin No 
                 26 25 24 23 22 21 20 19 18 17 12 11 10  9  8  7  6  5  4  3 */
int day[20]   = {00,00,00,00,01,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00}; //Day
int off[20]   = {00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00};
int allOn[20] = {00,00,00,00,00,00,01,00,00,00,00,01,01,01,01,01,01,00,00,00};

void setup() {
  Serial.begin(115200);
  pinMode(din, OUTPUT);
  pinMode(load, OUTPUT);
  pinMode(clk, OUTPUT);
  digitalWrite(clk, LOW);
  digitalWrite(load, LOW);
  digitalWrite(din, LOW);
  connectWifi();
  getInternetTime();
  time_1 = millis();
}
 
void loop() {
  writeDigit(hourTens, d[(int)(hour / 10)], dayOfWeek == 2 ? day : off);
  writeDigit(hourOnes, d[hour % 10], dayOfWeek == 3 ? day : off);
  writeDigit(minuteTens, d[(int)(minute / 10)], dayOfWeek == 5 ? day : off);
  writeDigit(minuteOnes, d[minute % 10], dayOfWeek == 6 ? day : off);
  writeDigit(dayTens, d[(int)(dayOfMonth / 10)], off);
  writeDigit(dayOnes, d[dayOfMonth % 10], off);
  writeDigit(monthTens, d[(int)(month / 10)], off);
  writeDigit(monthOnes, d[month % 10], off);

  writeDigit(groupSunday, off, dayOfWeek == 0 ? day : off);
  writeDigit(groupMonday, off, dayOfWeek == 1 ? day : off);
  writeDigit(groupThursday, allOn, dayOfWeek == 4 ? day : off);
  
  // update ntp time every minute
  if(millis() > time_1 + 60*1000){
    time_1 = millis();
    getInternetTime();
  }
  delayMicroseconds(DELAY);
}

void getInternetTime() {
  timeClient.update();
  Serial.print("Got ntp time: ");
  Serial.println(timeClient.getFormattedDate());  
  year = timeClient.getFormattedDate().substring(0,4).toInt();
  month = timeClient.getFormattedDate().substring(5,7).toInt();
  dayOfMonth = timeClient.getFormattedDate().substring(8,10).toInt();
  hour = timeClient.getHours();
  minute = timeClient.getMinutes();
  second = timeClient.getSeconds();
  dayOfWeek = timeClient.getDay();
  Serial.print((int)(hour / 10));
  Serial.print(" ");
  Serial.print(hour % 10);
  Serial.print(" ");
  Serial.print((int)(minute / 10));
  Serial.print(" ");
  Serial.print(minute % 10);
  Serial.print(" ");
  Serial.print((int)(dayOfMonth / 10));
  Serial.print(" ");
  Serial.print(dayOfMonth % 10);
  Serial.print(" ");
  Serial.print((int)(month / 10));
  Serial.print(" ");
  Serial.print(month % 10);
  Serial.println("");
}

void writeDigit(int group[20], int d[20], int dayOn[20]) {
  digitalWrite(clk, LOW);
  delayMicroseconds(DELAY);
  for (int i = 19; i >= 0; i--) {
    digitalWrite(din, group[i] | d[i] | dayOn[i]);
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

void connectWifi() {
  WiFi.begin(STASSID, STAPSK);
  WiFi.mode(WIFI_STA);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected to ");
  Serial.println(STASSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
