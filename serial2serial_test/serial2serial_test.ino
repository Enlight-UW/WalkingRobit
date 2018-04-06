#include<string.h>
char inputs[] = "aaaassssqqqqqwwwwklop";
int i;
int len;
void setup() 
{
    Serial.begin(9600);
    Serial.print("0");
    i = 0;
    len = strlen(inputs);
} 
void loop()
{
   delay(100);
   Serial.write(inputs[i%len]);
   i++;
}
