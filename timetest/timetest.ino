#include <TimeLib.h>

unsigned long offset_days = 3;    // 3 days
unsigned long t_unix_date1, t_unix_date2;

void setup() {
  Serial.begin(115200);
  t_unix_date1 = 1564398600;
  Serial.print("t_unix_date1: ");
  Serial.println(t_unix_date1);
  offset_days = offset_days * 86400;    // convert number of days to seconds
  t_unix_date2 = 1564398600 + offset_days;
  Serial.print("t_unix_date2: ");
  Serial.println(t_unix_date2);
  printf("Date1: %4d-%02d-%02d %02d:%02d:%02d\n", year(t_unix_date1), month(t_unix_date1), day(t_unix_date1), hour(t_unix_date1), minute(t_unix_date1), second(t_unix_date1));
  printf("Date2: %4d-%02d-%02d %02d:%02d:%02d\n", year(t_unix_date2), month(t_unix_date2), day(t_unix_date2), hour(t_unix_date2), minute(t_unix_date2), second(t_unix_date2));
}

void loop() {



}
