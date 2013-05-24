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

#include <Arduino.h>
#include <Wire.h>
#include "DS3231.h"


DS3231 RTC; //Create the DS3231 object


char weekDay[][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

DateTime displayTime();
String* command;

int set(int argc, char** argv);
int get(int argc, char** argv);

struct cmnd
{
	char* command;
	int (*callback)(int argc, char** argv);
};

static cmnd cli[] = {
		{"set", &set},
		{"get", &get},
		{NULL, NULL},
};

void prompt()
{
	Serial.println();
	Serial.print("#> ");
}

void setup()
{
  Serial.begin(57600);             // the Serial port of Arduino baud rate.
  delay(200);
  command = new String();

  Wire.begin();
  RTC.begin();

  Serial.println("Welcome to the RTC Time Setting Utility.");
  Serial.println("");
  Serial.println("Usage:");
  Serial.println("");  
  Serial.println("\tset <YEAR> <MONTH> <DAY> <HOUR24> <MINUTE> <SECOND> <DAY_OF_WEEK>");
  Serial.println("");    
  Serial.println("\tExample:");      
  Serial.println("");    
  Serial.print("\t\t");
  DateTime now = displayTime();
  Serial.println(""); 
  Serial.println("\tExample Set Command:");      
  Serial.println("");      
  Serial.print("\t\tset ");
  Serial.print(now.year());
  Serial.print(" ");
  Serial.print(now.month());
  Serial.print(" ");
  Serial.print(now.date());
  Serial.print(" ");
  Serial.print(now.hour());
  Serial.print(" ");
  Serial.print(now.minute());
  Serial.print(" ");
  Serial.print(now.second());
  Serial.print(" ");
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
  prompt();
}



void loop()
{
	if(Serial.available())
	{
		char read_v = (char)(Serial.read());
		if(read_v != 10 && read_v != 13)
		{
			if(read_v == 127){
				if(command->length() > 0){
					String *tmp = command;
					command = new String(command->substring(0, command->length()-1));
					delete tmp;
					Serial.print("\b \b");
				}
			}else{
				command->concat(read_v);
				Serial.print(read_v);
			}
		}else
		{
			Serial.println();
			Serial.println();
			command->trim();
			unsigned int length = command->length();
			char _command[length + 1];
			int argc = 1;
			for(unsigned int i = 0; i < length; i++)
			{
				_command[i] = (*command)[i];
				if((*command)[i] == ' ')
				{
					argc++;
					_command[i] = 0;
				}
			}

			_command[length] = 0;
			char* argv[argc];
			int pos = 0;
			for (int i = 0; i < argc; i++)
			{
				argv[i] = _command + pos;
				pos += strlen(argv[i]) + 1;
			}
			bool found = false;
			for(int commands_pos = 0; cli[commands_pos].callback != NULL; commands_pos++)
			{
				if(strcmp(cli[commands_pos].command, argv[0]) == 0)
				{
					found = true;
					cli[commands_pos].callback(argc, argv);
					prompt();
				}
			}
			if(!found)
			{
				Serial.print("\t The command \"");
				Serial.print(argv[0]);
				Serial.println("\" could not be found.");
				prompt();
			}
			delete command;
			command = new String();
		}
	}
}

int set(int argc, char** argv)
{
	if(argc != 8)
	{
		Serial.print("\tset command expected 7 parameters, found: ");
		Serial.println(argc -1);
		return 2;
	}
	int count = 1;
	RTC.adjust(
			DateTime(
					atoi(argv[count++]),
					atoi(argv[count++]),
					atoi(argv[count++]),
					atoi(argv[count++]),
					atoi(argv[count++]),
					atoi(argv[count++]),
					atoi(argv[count++])
			)
	);
	Serial.print("\tSuccessfully set the clock. ");
	displayTime();
	return 0;
}

int get(int argc, char** argv)
{
	Serial.print("\t");
	displayTime();
	return 0;
}



DateTime displayTime () 
{
    DateTime now = RTC.now(); //get the current date-time
    Serial.print("");
    Serial.print("RTC's Current Time: ");
    Serial.print(now.year(), DEC);
    Serial.print('/');
    if(now.month() < 10){Serial.print(0, DEC);}
    Serial.print(now.month(), DEC);
    Serial.print('/');
    if(now.date() < 10){Serial.print(0, DEC);}
    Serial.print(now.date(), DEC);
    Serial.print(' ');
    if(now.hour() < 10){Serial.print(0, DEC);}
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    if(now.minute() < 10){Serial.print(0, DEC);}
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    if(now.second() < 10){Serial.print(0, DEC);}
    Serial.print(now.second(), DEC);
    Serial.print(" ");
    Serial.print(weekDay[now.dayOfWeek()]);
    Serial.println();
    return now;
}


