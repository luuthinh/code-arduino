/*
Arduino Water flow meter
YF- S201 Hall Effect Water Flow Sensor
Water Flow Sensor output processed to read in litres/hour
*/
volatile float flow_frequency; // Measures flow sensor pulses
//unsigned int l_hour; // Calculated litres/hour
unsigned char flowsensor = 2; // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;
char inputChar;         
bool stringComplete = false;  
void flow () // Interrupt function
{
   flow_frequency += 2.22;
}
void setup()
{
   pinMode(flowsensor, INPUT);
   digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
   Serial.begin(9600);
   attachInterrupt(0, flow, RISING); // Setup Interrupt
   sei(); // Enable interrupts
   currentTime = millis();
   cloopTime = currentTime;
   flow_frequency= 2222.22;
}
void loop ()
{
  /*
   currentTime = millis();
   // Every second, calculate and print litres/hour
   if(currentTime >= (cloopTime + 1000))
   {
      cloopTime = currentTime; // Updates cloopTime
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      l_hour = (flow_frequency * 60 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      flow_frequency = 0; // Reset Counter
      Serial.print(l_hour, DEC); // Print litres/hour
      Serial.println(" L/hour");
   }
   */
  if (stringComplete && inputChar == 'A') {
    //Serial.println(inputChar);
    flow_frequency= 2222.22;
    int temp = int(flow_frequency/100);
    byte temp_byte = temp;
    //Serial.print(temp);
    //byte temp_high = byte(temp >> 8);
    //byte temp_low = byte(temp) ;
    //byte temp_array[3];
    //temp_array[0]=temp_high;
    //temp_array[1]=temp_low;
    //temp_array[2]='$';
    Serial.write(temp_byte);
    //Serial.print(temp_high,DEC);
    //Serial.print(temp_low,DEC);
    // clear the string:
    inputChar = "";
    stringComplete = false;
  }
    if (stringComplete && inputChar == 'B') {
    //Serial.println(inputChar);
    Serial.println("ok");
    flow_frequency =0;
    // clear the string:
    inputChar = "";
    stringComplete = false;
  }
}
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    //inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '$') {
      stringComplete = true;
      break;
    }
    inputChar = inChar;
  }
}
