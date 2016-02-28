/*
  Software serial multple serial test
 
 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.
 
 The circuit: 
 * RX is digital pin 10 (connect to TX of other device)
 * TX is digital pin 11 (connect to RX of other device)
 
 Note:
 Not all pins on the Mega and Mega 2560 support change interrupts, 
 so only the following can be used for RX: 
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69
 
 Not all pins on the Leonardo support change interrupts, 
 so only the following can be used for RX: 
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).
 
 created back in the mists of time
 modified 25 May 2012
 by Tom Igoe
 based on Mikal Hart's example
 
 This example code is in the public domain.
 
 */
#include "SoftwareSerial.h"

const char cmdstr_rdX   []={ 0x68, 0x04, 0x00, 0x01, 0x0d};
const char cmdstr_rdY   []={ 0x68, 0x04, 0x00, 0x02, 0x06};
const char cmdstr_rdXY  []={ 0x68, 0x04, 0x00, 0x04, 0x08};
const char cmdstr_rdZero[]={ 0x68, 0x04, 0x00, 0x0d, 0x11};
const char cmdstr_stZero[]={ 0x68, 0x05, 0x00, 0x05, 0x00, 0x0A};

SoftwareSerial Ser2Lvt(10, 11); // RX, TX

void sencCMD(const char * str)
{
        //flush the data in RX buffer.
        uint8_t i=0;
        while( Ser2Lvt.available() > 0 )
        {
              Ser2Lvt.read();
              i++;
        }
        Serial.print("flush data length = ");
        Serial.println(i,DEC);
        // send command
    	i = str[1]+1;
	while(i--)
	{
		Ser2Lvt.write(*(str++));
		delay(1);
	}
}



unsigned char dataXYstr[6];
unsigned char rcvStr[20];

uint8_t readCMD()
{
	uint8_t i=0;
        //waiting for data back
        while( !(Ser2Lvt.available() > 0 ) )
        {;}
        
        //read data
	while( Ser2Lvt.available() > 0 )
	{
		rcvStr[i]=Ser2Lvt.read();
                i++;
                delay(10);
                if (i>20)
               {  
                  //data too long
                  return 0;
               }
	}

         	dataXYstr[0] = rcvStr[4];
    		dataXYstr[1] = rcvStr[5];
    		dataXYstr[2] = rcvStr[6];
    		dataXYstr[3] = rcvStr[7];  
    		dataXYstr[4] = rcvStr[8];
    		dataXYstr[5] = rcvStr[9];

	return 1;
}

void loop()
{
	sencCMD(cmdstr_rdXY);
	if(readCMD())
	{
		uint8_t i=0;
		while(i<6)
		{
			Serial.print(dataXYstr[i++], HEX);
			Serial.print(",");
		}
		Serial.println();
	}
	else
	{
	}

	delay(2000);
}



void setup()  
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) 
  {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  Ser2Lvt.begin(9600);
}
