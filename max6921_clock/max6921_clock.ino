#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "credentials.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

#include <Timezone.h>    // https://github.com/JChristensen/Timezone
// Central European Time (Frankfurt, Paris)
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     // Central European Summer Time
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};       // Central European Standard Time
Timezone CE(CEST, CET);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);//gmt+1

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
static const uint8_t SUNDAY = 0;
static const uint8_t MONDAY = 1;
static const uint8_t TUESDAY = 2;
static const uint8_t WEDNESDAY = 3;
static const uint8_t THURSDAY = 4;
static const uint8_t FRIDAY = 5;
static const uint8_t SATURDAY = 6;

int DELAY = 10000;
int clk = D8;
int load = D1;
int din = D2;

int tSecond = 0;
int tHour = 20;
int tMinute = 22;
int tYear;
int tMonth;
int tDayOfMonth;
int time_1;
int millisElapsed;
int dayOfWeek = 0;

/* Pin No                26 25 24 23 22 21 20 19 18 17 12 11 10  9  8  7  6  5  4  3 */
int hourTens[20]      = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int hourOnes[20]      = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0};
int minuteTens[20]    = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0};
int minuteOnes[20]    = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0};
int dayTens[20]       = { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int dayOnes[20]       = { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int monthTens[20]     = { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int monthOnes[20]     = { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int groupSunday[20]   = { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int groupMonday[20]   = { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int groupThursday[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0};
int d[10][20]={
/* Pin No 
                         26 25 24 23 22 21 20 19 18 17 12 11 10  9  8  7  6  5  4  3 */
                        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1},//0
                        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0},//1
                        { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1},//2
                        { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0},//3
                        { 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0},//4
                        { 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0},//5
                        { 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1},//6
                        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0},//7
                        { 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1},//8
                        { 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0} //9
};

/* Pin No / Segments
  10
17  09
  19
03  07 //former 12, pin 12 was always brighter than the others, don't know why
  11
*/

/* Pin No 
                 26 25 24 23 22 21 20 19 18 17 12 11 10  9  8  7  6  5  4  3 */
int tDay[20]   = {01,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00}; //Day
int off[20]   = {00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00};
int allOn[20] = {00,00,00,00,00,00,00,01,00,01,00,00,01,01,00,01,00,01,00,01};

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
  millisElapsed = millis();
}
 
void loop() {
  writeDigit(dayTens, d[(int)(tDayOfMonth / 10)], off);
  writeDigit(dayOnes, d[tDayOfMonth % 10], off);
  writeDigit(monthTens, d[(int)(tMonth / 10)], off);
  writeDigit(monthOnes, d[tMonth % 10], off);

  writeDigit(groupSunday, off, dayOfWeek == SUNDAY ? tDay : off);
  writeDigit(groupMonday, off, dayOfWeek == MONDAY ? tDay : off);

  writeDigit(hourTens, d[(int)(tHour / 10)], dayOfWeek == TUESDAY ? tDay : off);
  writeDigit(hourOnes, d[tHour % 10], dayOfWeek == WEDNESDAY ? tDay : off);
  writeDigit(groupThursday, allOn, dayOfWeek == THURSDAY ? tDay : off);
  writeDigit(minuteTens, d[(int)(tMinute / 10)], dayOfWeek == FRIDAY ? tDay : off);
  writeDigit(minuteOnes, d[tMinute % 10], dayOfWeek == SATURDAY ? tDay : off);

  writeDigit(minuteOnes, off, off);
  
  // update ntp time every hour
  if(millis() > time_1 + 60*60*1000){
    time_1 = millis();
    getInternetTime();
    millisElapsed = millis();
  } else {
    delayMicroseconds(DELAY);
  }

  if(millis() > millisElapsed + 1000){
    millisElapsed = millis();
    tSecond = (tSecond + 1) % 60;
    if(tSecond == 0){
      tMinute = (tMinute + 1) % 60;
      if(tMinute == 0){
        tHour = (tHour + 1) % 24;
        if(tHour == 0){
          dayOfWeek = (dayOfWeek + 1) % 7;
        }
      }
    }
    printTime();
  }
}

void printTime() {
  Serial.print(tHour);
  Serial.print(":");
  Serial.print(tMinute);
  Serial.print(":");
  Serial.println(tSecond);
  
  Serial.print("dayOfWeek:");
  Serial.println(dayOfWeek);
  Serial.print(" ");
  Serial.print(tDayOfMonth);
  Serial.print(".");
  Serial.print(tMonth);
  Serial.print(".");
  Serial.println(tYear);
}

void getInternetTime() {
  timeClient.update();
  Serial.print("Got ntp time: ");
  Serial.println(timeClient.getFormattedTime());
  time_t utc = timeClient.getEpochTime();
  setSummerTime(utc, "Berlin");
}

// given a Timezone object, UTC and a string description, convert and print local time with time zone
void setSummerTime(time_t utc, const char *descr)
{
    char buf[40];
    char m[4];    // temporary storage for month string (DateStrings.cpp uses shared buffer)
    TimeChangeRule *tcr;        // pointer to the time change rule, use to get the TZ abbrev

    time_t t = CE.toLocal(utc, &tcr);
    strcpy(m, monthShortStr(month(t)));
    sprintf(buf, "%.2d:%.2d:%.2d %s %.2d %s %d %s",
        hour(t), minute(t), second(t), dayShortStr(weekday(t)), day(t), m, year(t), tcr -> abbrev);
    Serial.print("Summer Time: ");
    Serial.print(buf);
    Serial.print(' ');
    Serial.println(descr);
    tHour = hour(t);
    tMinute = minute(t);
    tSecond = second(t);
    tYear = year(t);
    tMonth = month(t);
    tDayOfMonth = day(t);    
    dayOfWeek = weekday(t) - 1;
}

void writeDigit(int group[20], int d[20], int dayOn[20]) {
  digitalWrite(clk, LOW);
  for (int i = 19; i >= 0; i--) {
    digitalWrite(din, group[i] | d[i] | dayOn[i]);
    digitalWrite(clk, HIGH);
    digitalWrite(clk, LOW);
  }
  digitalWrite(load, HIGH);
  digitalWrite(load, LOW);
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
