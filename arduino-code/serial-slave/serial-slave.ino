#include <SimpleDHT.h>
// khai bao chan doc cam bien nhiet do
int pinDHT11 = 4;
SimpleDHT11 dht11;
byte temperature = 0;
byte humidity = 0;
int err = SimpleDHTErrSuccess;
// chuoi nhan vao
String inputString = "";
// whether the string is complete         
boolean stringComplete = false;  
// cam bien luong nuoc
volatile long litter=0;
// gia tri nuoc gioi han
volatile long litterLimit = 15000;
// gia trin nuoc cai dai
long litterSet=10000;
unsigned long currentTime;
unsigned long cloopTime;
unsigned int flowsensor = 2;
// khai bao chan cho he thong bom nuoc
int pinPump = 8;
int pinNguon = 9;
int pinThoatKhi = 10;
int pinThoatNuoc = 11;
int pinHutKhi = 12;
// khai bao chan den led
int pinLed =13;
void setup() {
  // ngat 10Hz 
  // khoi tao timer 1
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 62500;            // compare match register 16MHz/256/10Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts
  // khai tao chan den led
  pinMode(pinLed,OUTPUT);
  // khai bao chan tin hieu vao
  pinMode(flowsensor, INPUT);
  // khai bao chan tin hien ra
  pinMode(pinPump,OUTPUT);
  pinMode(pinNguon,OUTPUT);
  pinMode(pinThoatKhi,OUTPUT);
  pinMode(pinThoatNuoc,OUTPUT);
  pinMode(pinHutKhi,OUTPUT);
  // khoi tao trang thai ban dau cho pin tin hieu      
  digitalWrite(flowsensor,HIGH);
  digitalWrite(pinPump,LOW);
  digitalWrite(pinNguon,LOW);
  digitalWrite(pinThoatKhi,LOW);
  digitalWrite(pinThoatNuoc,LOW);
  digitalWrite(pinHutKhi,LOW);
  digitalWrite(pinLed,LOW);        
  // khai bao ngat doc tin hieu
  attachInterrupt(0,flow,RISING);
  
  Serial.begin(115200);
  Serial.println("khoi tao cong ket noi thanh cong");
}
ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  // test chu ki timer 1
  //digitalWrite(pinLed,digitalRead(pinLed)^1);
  if (litter > litterLimit) {
      tatBom();
    }  
  ; //luong nuoc sau moi 1s
}
void loop() {
  if(inputString.equals("nhietdo-doam$") && stringComplete == true){
      // xoa command
      //Serial.println(inputString);
      inputString = "";
      stringComplete = false;
      digitalWrite(pinLed,HIGH);
      // thuc hien doc nhiet do do am tu DHT11
      processDHT11();
      digitalWrite(pinLed,LOW);
    }
  else if (inputString.equals("bomNuocNguon$") && stringComplete == true){
      //xoa command
      inputString = "";
      stringComplete = false;
      digitalWrite(pinLed,HIGH);
      delay(100);
      bomNuoc();
      Serial.print("ok");
      digitalWrite(pinLed,LOW);
    }
  else if (inputString.equals("bomKhi$") && stringComplete == true){
      inputString = "";
      stringComplete = false;
      digitalWrite(pinLed,HIGH);
      delay(100);
      bomKhi();
      Serial.print("ok");
      digitalWrite(pinLed,LOW);
    }
  else if (inputString.equals("tatBom$") && stringComplete == true){
      inputString = "";
      stringComplete = false;
      digitalWrite(pinLed,HIGH);
      delay(100);
      tatBom();
      Serial.print("ok");
      digitalWrite(pinLed,LOW);
    }    
  else if (inputString.equals("litter$") && stringComplete == true ) {
      inputString = "";
      stringComplete = false;
      Serial.println(litter);
    }
  else if (inputString.equals("maytengi$") && stringComplete == true){
      inputString = "";
      stringComplete = false;
      Serial.println("arduino1");
    }     
}
/*
ngat nhan serial
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '#') inputString ="";
    if (inChar == '$') {
      stringComplete = true;
    }
  }
}
// xu ly nhiet do do am
void processDHT11(){
   byte temperature = 0;
   byte humidity = 0;
   int err = SimpleDHTErrSuccess;
   if ((err = dht11.read(pinDHT11, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
     Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(1000);
    return;
   }
   //Serial.print("Sample OK: ");
   Serial.print((int)temperature); Serial.print(" *C, "); 
   Serial.print((int)humidity); Serial.println(" H");
  // DHT11 sampling rate is 1HZ.
   delay(1500); 
  }
// ngat chan pin2 doc xung cam bien nuoc
void flow(){
  litter += 2.22;
  } 
void bomNuoc(){ // lay nuoc vao binh chua nuoc
       // mo nguon nuoc va bat may bom lay nuoc
       digitalWrite(pinNguon,HIGH);
       digitalWrite(pinThoatKhi,LOW);
       digitalWrite(pinPump,HIGH);
       // dat lai gia tri tan so cam bien nuoc
       //flow_frequency = 0;
  } 
void bomKhi(){
       digitalWrite(pinNguon,LOW);
       digitalWrite(pinThoatKhi,HIGH);
       digitalWrite(pinPump,HIGH);
  }  
void tatBom(){
       digitalWrite(pinNguon,LOW);
       digitalWrite(pinThoatKhi,LOW);
       digitalWrite(pinPump,LOW);
  }   
