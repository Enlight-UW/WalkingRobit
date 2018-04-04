// Basic Bluetooth sketch HC-06_01
// Connect the Hc-06 module and communicate using the serial monitor
//
// The HC-06 defaults to AT mode when first powered on.
// The default baud rate is 9600
// The Hc-06 requires all AT commands to be in uppercase. NL+CR should not be added to the command string
//
 
 
#include <SoftwareSerial.h>
SoftwareSerial BTserial(13, 12); // RX | TX
// Connect the HC-06 TX to the Arduino RX on pin 2. 
// Connect the HC-06 RX to the Arduino TX on pin 3 through a voltage divider.
// 
 
 
void setup() 
{
    Serial.begin(9600);
    Serial.print("0");
 
    // HC-06 default serial speed is 9600
    // Needs neither CTRL+R nor Newline
    //BTserial.begin(9600);
    //HC-05 default speed is 38400
    //Needs both CTRL+R + Newline 
    BTserial.begin(9600);  
} 
void loop()
{
 
    // Keep reading from HC-06 and send to Arduino Serial Monitor
    if (BTserial.available())
    {  
        byte in_byte = BTserial.read();
        Serial.write(in_byte+1);
    }
 
    // Keep reading from Arduino Serial Monitor and send to HC-06
    if (Serial.available())
    {
        byte in_byte = Serial.read();
        Serial.println("BT sending = ");
        Serial.println(in_byte);
        BTserial.write(in_byte);
    }
 
}
