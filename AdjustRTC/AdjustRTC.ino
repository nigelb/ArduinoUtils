/*
 AdjustRTC is a sketch that lets you set the DS3231 Real Time 
 Clock's time. Copyright (C) 2012 Nigel Bajema
 
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <Wire.h>
#include "DS3231.h"

DS3231 RTC; //Create the DS3231 object

char buf[1024];
int pos = 0, _pos1 = 0, _pos2 = 0;
char tmp;

char weekDay[][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

int _year, _month, _day, _hour, _minute, _second, _day_of_week;

DateTime displayTime();

void setup()
{
  Serial.begin(19200);             // the Serial port of Arduino baud rate.
  delay(2000);
  Wire.begin();
  RTC.begin();
  Serial.println("Welcome to the RTC Time Seting Utility.");
  Serial.println("");
  Serial.println("Usage:");
  Serial.println("");  
  Serial.println("\tset <YEAR>, <MONTH>, <DAY>, <HOUR24>, <MINUTE>, <SECOND>, <DAY_OF_WEEK>");  
  Serial.println("");    
  Serial.println("\tExample:");      
  Serial.println("");    
  Serial.print("\t\tCurrent RTC Time: ");  
  DateTime now = displayTime();
  Serial.println(""); 
  Serial.println("\tExample Set Command:");      
  Serial.println("");      
  Serial.print("\t\tset ");
  Serial.print(now.year());
  Serial.print(", ");
  Serial.print(now.month());
  Serial.print(", ");
  Serial.print(now.date());
  Serial.print(", ");
  Serial.print(now.hour());
  Serial.print(", ");
  Serial.print(now.minute());
  Serial.print(", ");
  Serial.print(now.second());
  Serial.print(", ");
  Serial.print(now.dayOfWeek());  
  Serial.println(""); 
  Serial.println(""); 
  Serial.println("\tDays of Week:");
  Serial.println("");
  for(int i = 0; i < 7; i++) 
  {
      Serial.print("\t\t");    
      Serial.print(i, DEC);
      Serial.print(": ");      
      Serial.println(weekDay[i]);      
  }
  Serial.println("");
  Serial.print("#> ");  
}

void loop()
{
  
  if(Serial.available())
    {
      while(Serial.available())
      {

        tmp = Serial.read();
        Serial.write(tmp);
        if(pos == 0 && (tmp == 10 || tmp == 13))
        {
        }
        else{
           buf[pos++] = tmp;
        }
      }

   if((tmp == 10) && pos > 0)
    {
      displayTime();
      Serial.println("==================================="); 
      
      String command(buf);
      command.trim();
      String c2 = command.substring(command.indexOf(" "));
      c2.trim();
      int _pos1 = c2.indexOf(",");
      String t = c2.substring(0, _pos1);
      t.trim();
      _year = t.toInt();

      _pos1++;
      _pos2 = c2.indexOf(",", _pos1);
      t = c2.substring(_pos1, _pos2);
      t.trim();
      _month = t.toInt();
      _pos1 = _pos2;

      _pos1++;      
      _pos2 = c2.indexOf(",", _pos1);
      t = c2.substring(_pos1, _pos2);
      t.trim();
      _day = t.toInt();
      _pos1 = _pos2;      

      _pos1++;
      _pos2 = c2.indexOf(",", _pos1);
      t = c2.substring(_pos1, _pos2);
      t.trim();
      _hour = t.toInt();
      _pos1 = _pos2;      

      _pos1++;
      _pos2 = c2.indexOf(",", _pos1);
      t = c2.substring(_pos1, _pos2);
      t.trim();
      _minute = t.toInt();
      _pos1 = _pos2;      
      
      _pos1++;
      _pos2 = c2.indexOf(",", _pos1);
      t = c2.substring(_pos1, _pos2);
      t.trim();
      _second = t.toInt();   
      _pos1 = _pos2;      
   
      _pos1++;
      _pos2 = c2.indexOf(",", _pos1);
      t = c2.substring(_pos1, _pos2);
      t.trim();
      _day_of_week = t.toInt();       
      _pos1 = _pos2;      
      RTC.adjust(DateTime(_year, _month, _day, _hour, _minute, _second, _day_of_week));
      Serial.print("Year ");
      Serial.println(_year, DEC);
      Serial.print("Month ");      
      Serial.println(_month, DEC);
      Serial.print("Day ");      
      Serial.println(_day, DEC);
      Serial.print("Hour ");      
      Serial.println(_hour, DEC);
      Serial.print("Minute ");      
      Serial.println(_minute, DEC);
      Serial.print("Second ");      
      Serial.println(_second, DEC);
      Serial.print("Day Of Week ");      
      Serial.println(weekDay[_day_of_week]);      
      Serial.println("===================================");
      displayTime();
      Serial.println("");
      Serial.print("#> ");        
    } 
  }
  
}

void clearBufferArray()              // function to clear buffer array
{
  for (int i=0; i<pos;i++)
    { buf[i]=NULL;}                  // clear all index of array with command NULL
    pos = 0;
}


DateTime displayTime () 
{
    DateTime now = RTC.now(); //get the current date-time
    Serial.print("");
    Serial.print("RTC's Current Time: ");
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.date(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.print(" ");
    Serial.print(weekDay[now.dayOfWeek()]);
    Serial.println();
    return now;
}


