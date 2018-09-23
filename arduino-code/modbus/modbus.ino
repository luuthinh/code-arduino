/*
  Modbus-Arduino  (Modbus IP)
  code chạy trên con arduino slave , điều khiển thông qua modbus tcp ip
  sử dungj thư viện modbus TCP
  http://github.com/andresarmento/modbus-arduino
*/
 
#include <SPI.h>
#include <Ethernet.h>
#include <Modbus.h>
#include <ModbusIP.h>

// cam bien luong nuoc
float litter=0;
// gia tri nuoc gioi han
//volatile float litterLimit = 15000;
// gia trin nuoc cai dai
//float litterSet=10000;
volatile float flow_frequency; // Measures flow sensor pulses
//unsigned int l_hour; // Calculated litres/hour
unsigned char flowsensor = 2; // Sensor Input
//Khai báo thanh ghi cho modbus(0-9999)
const int litter_hreg = 100;
const int pump_hreg = 6;
const int solInputWater_coil = 3;


const int solOutputWater_coil = 5;
const int solInputAir_coil = 7;
const int solOutputAir_coil = 8;

//khai bao chân sử dụng
const int pump = 6;
const int solInputWater = 3;
const int solOutputWater = 5;
const int solInputAir = 7;
const int solOutputAir = 8;
//const int led=13;
//ModbusIP object
ModbusIP mb;
long ts;
byte buf[3];
void setup() {
    // Serial
    //Serial1.begin(9600);
    delay(1000);
    Serial.begin(9600);
    delay(1000);
    Serial1.setTimeout(1000); 
    // Địa chỉ MAC của thiết bị
    byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  
    // Địa chỉ IP của thiết bị
    byte ip[] = { 192, 168, 1, 199 };   
    //Config Modbus IP 
    mb.config(mac, ip);
    delay(5000);
    //
   //pinMode(flowsensor, INPUT);
   pinMode(flowsensor,INPUT_PULLUP); // Optional Internal Pull-Up
   attachInterrupt(digitalPinToInterrupt(flowsensor),flow, RISING); // Setup Interrupt
   sei(); // Enable interrupts
    // khai bao ngat doc tin hieu
    // khai báo tín hiệu output 
    pinMode(pump,OUTPUT);
    pinMode(solInputWater,OUTPUT);
    pinMode(solOutputWater,OUTPUT);
    pinMode(solInputAir,OUTPUT);
    pinMode(solOutputAir,OUTPUT);
   // pinMode(led,OUTPUT);
    //digitalWrite(led,LOW);
    // Đăng kí địa chỉ thanh ghi tương ứng với tính năng điều khiển
    //mb.addIreg(SENSOR_IREG);
    mb.addHreg(litter_hreg);
    mb.addHreg(pump_hreg, 0);
    mb.addCoil(solInputWater_coil,0);
    mb.addCoil(solOutputWater_coil,0);
    mb.addCoil(solInputAir_coil,0);
    mb.addCoil(solOutputAir_coil,0);
    // Lấy giá trị time
    ts = millis();
    // Khởi tạo cổng serial
   // Serial.begin(9600);
   // delay(1000);
   // Serial.println("khoi tao thanh cong");
    analogWrite(pump,0);
    digitalWrite(solInputWater, 0);
    digitalWrite(solOutputWater, 0);
    digitalWrite(solInputAir,0);
    digitalWrite(solOutputAir,0);
}

void loop() {
   // Thực hiện đọc trả gía trị modbus
   mb.task();
   //Serial.println(litter);
   //delay(500);
   // Thực hiện lệnh mỗi 0.5s
   if (millis() > ts + 500) {   
       ts = millis();
       analogWrite(pump, mb.Hreg(pump_hreg));
       digitalWrite(solInputWater, mb.Coil(solInputWater_coil));
       digitalWrite(solOutputWater, mb.Coil(solOutputWater_coil));
       digitalWrite(solInputAir, mb.Coil(solInputAir_coil));
       digitalWrite(solOutputAir, mb.Coil(solOutputAir_coil));
       //Serial1.print("A$");
       //delay(50);
       //if(Serial1.available()){
       //buf[0]=Serial1.read();
       Serial.println(buf[0]);
       
       //long temp = long(litter *100);
       mb.Hreg(litter_hreg,int(flow_frequency));
       }
   } 
void flow() // Interrupt function
{
   flow_frequency += 2.22;
}
