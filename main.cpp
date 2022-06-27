#include <Arduino.h>
// #include <Quad.hpp>
// #include <ESP32EscBliHeli.hpp>
// #include <Neo6m.hpp>

// //esc output
// ESP32EscBliHeli esc;
// //quadcopter environment
// Quad vehicle;
// //neo6m gps;
// Neo6m gps;

// float previuos = 0;

// void setup() {
//   Serial.begin(115200);
//   //esc init (motor output resolution is 0 <-> 4095)
//   esc.attach(0,32);
//   esc.attach(2,33);
//   esc.attach(4,25);
//   esc.attach(5,26);
//   //just init
//   vehicle.init();
//   // //gps init
//   // gps.init();
// }

// void loop(){
//   // make it stable (+2.6ms)
//   vehicle.calm();
//   //make it stay on that height (+16ms)
//   vehicle.altHold();
//   // stay to compass direction(+1.5ms)
//   vehicle.setCompassDirection(0);
//   // spin the rotor
//   esc.setSpeed(0,vehicle.m1);
//   esc.setSpeed(2,vehicle.m2);
//   esc.setSpeed(4,vehicle.m3);
//   esc.setSpeed(5,vehicle.m4);
  
//   // get gps cordinate(0.4ms+0.1ms*loop)
//   if(Serial2.available()){
//     for(int i = 0; i < 2; i++){
//       if(Serial2.read()=='L'){
//         gps.getLatLongCoordinate();
//       }
//     }
//   }
//   Serial.print("|Compass:");Serial.print(compass.angle);
//   Serial.print("|Pitch:");Serial.print(mpu.pitch);
//   Serial.print("|Roll:");Serial.print(mpu.roll);
//   Serial.print("|Yaw:");Serial.print(mpu.yaw);
//   Serial.print("|ThrothleAlt:");Serial.print(vehicle.throtle);
//   Serial.print("|GPS:");Serial.print(gps.latitudeGPS,6);
//   Serial.print(" ");Serial.print(gps.longitudeGPS,6);
//   Serial.print("|m1:");Serial.print(vehicle.m1);
//   Serial.print("|m2:");Serial.print(vehicle.m2);
//   Serial.print("|m3:");Serial.print(vehicle.m3);
//   Serial.print("|m4:");Serial.print(vehicle.m4);

//   //measure time each loop
//   Serial.print("|interval ");Serial.println(micros() - previuos);
//   previuos = micros();
// }
// ----------------------------------
// #include <Qmc5883l.hpp>

// Qmc5883l compass;

// void setup(){
//   Serial.begin(115200);
//   compass.init();
// }
// void loop(){
//   compass.getAngle();
//   Serial.println(compass.angle);
//   // compass.getOffset();
// }
//----------------------------------
// #include <Neo6m.hpp>

// Neo6m gps;

// void setup(){
//   Serial.begin(115200);
//   gps.init();
// }
// void loop(){
//   gps.getLatLongCoordinate();
//   if(Serial2.available()){
//     Serial.print("|NMEA:");Serial.print(gps.latitudeNMEA);
//     Serial.print(" ");Serial.print(gps.orientNS);
//     Serial.print(" ");Serial.print(gps.longitudeNMEA);
//     Serial.print(" ");Serial.print(gps.orientEW);

//     Serial.print("|Coordinate:");Serial.print(gps.latitudeGPS,6);
//     Serial.print(" ");Serial.println(gps.longitudeGPS,6);
//   }
//   delay(1000);
  // if(Serial2.available()){
  //   Serial.print(char(Serial2.read()));
  // }
// }
//----------------------------------
// void setup(){
//   Serial.begin(115200);
//   Serial2.begin(9600,SERIAL_8N2);
// }
// void loop(){
//   if(Serial2.available()){
//     Serial2.findUntil();
//     Serial.print(Serial2.read());
//   }
//   delay(1000);
// }
// ----------------------------------
// volatile int interrupts;
// int totalInterrupts;

// hw_timer_t * timer = NULL;
// portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

// void IRAM_ATTR onTime() {
// 	portENTER_CRITICAL_ISR(&timerMux);
// 	interrupts++;
//   Serial.println("_________________");
// 	portEXIT_CRITICAL_ISR(&timerMux);
// }

// void setup() {

// 	Serial.begin(115200);

// 	// Configure Prescaler to 80, as our timer runs @ 80Mhz
// 	// Giving an output of 80,000,000 / 80 = 1,000,000 ticks / second
// 	timer = timerBegin(0, 80, true);                
// 	timerAttachInterrupt(timer, &onTime, true);   
// 	// Fire Interrupt every 1m ticks, so 1s
// 	timerAlarmWrite(timer, 1000000, true);		
// 	timerAlarmEnable(timer);
// }
// int toogle = 0;
// void loop() {
// 	if (interrupts > 0) {
// 		portENTER_CRITICAL(&timerMux);
// 		interrupts--;
// 		portEXIT_CRITICAL(&timerMux);
// 		totalInterrupts++;
// 		Serial.print("totalInterrupts");
// 		Serial.println(totalInterrupts);
// 	}
//   Serial.println("alealeale");
//   if(toogle==0){
//     timerAlarmDisable(timer);
//     timerDetachInterrupt(timer);
//     timerEnd(timer);
//     toogle = 1;
//   }else{
//   	timer = timerBegin(0, 80, true);                
//     timerAttachInterrupt(timer, &onTime, true);
//     timerAlarmWrite(timer, 1000000, true);		
//     timerAlarmEnable(timer);
//     toogle = 0;
//   }
//   delay(5000);
// }
// ----------------------------------

#include <Quad.hpp>
#include <ESP32EscBliHeli.hpp>

float previuos = 0;

ESP32EscBliHeli esc;
Quad vehicle;

#include <MyWebServer.hpp>
MyWebServer MyServer;

void setup(){
  Serial.begin(115200);
  //quad init
  vehicle.init();
  //server
  MyServer.init();
  //output
  esc.attach(0,32);
  esc.attach(2,33);
  esc.attach(4,25);
  esc.attach(5,26);
}
void loop(){
  //server
  MyServer.listen();
  //make vehicle stable
  vehicle.calm();
  //no arming
  if(html2_1_throtleValue <500){
      esc.setSpeed(0,0);
      esc.setSpeed(2,0);
      esc.setSpeed(4,0);
      esc.setSpeed(5,0);  
  }
  //arming
  else if(html2_1_throtleValue == 500){
      esc.setSpeed(0,html2_1_throtleValue);
      esc.setSpeed(2,html2_1_throtleValue);
      esc.setSpeed(4,html2_1_throtleValue);
      esc.setSpeed(5,html2_1_throtleValue);
  }
  //stabilizing
  else{
    //sum speed
    int m1T = html2_1_throtleValue + vehicle.m1Roll + vehicle.m1Pitch + vehicle.m1Yaw;
    int m2T = html2_1_throtleValue + vehicle.m2Roll + vehicle.m2Pitch + vehicle.m2Yaw;
    int m3T = html2_1_throtleValue + vehicle.m3Roll + vehicle.m3Pitch + vehicle.m3Yaw;
    int m4T = html2_1_throtleValue + vehicle.m4Roll + vehicle.m4Pitch + vehicle.m4Yaw;
    //set offset
    //-----------------------------
    if(m1T - html2_1_throtleValue > MAX_ROLL){
      m1T = html2_1_throtleValue + MAX_ROLL;
    }
    else if(m1T - html2_1_throtleValue < -MAX_ROLL){
      m1T = html2_1_throtleValue - MAX_ROLL;
    }
    if(m2T - html2_1_throtleValue > MAX_ROLL){
      m2T = html2_1_throtleValue + MAX_ROLL;
    }
    else if(m2T - html2_1_throtleValue < -MAX_ROLL){
      m2T = html2_1_throtleValue - MAX_ROLL;
    }
    if(m3T - html2_1_throtleValue > MAX_ROLL){
      m3T = html2_1_throtleValue + MAX_ROLL;
    }
    else if(m3T - html2_1_throtleValue < -MAX_ROLL){
      m3T = html2_1_throtleValue - MAX_ROLL;
    }
    if(m4T - html2_1_throtleValue > MAX_ROLL){
      m4T = html2_1_throtleValue + MAX_ROLL;
    }
    else if(m4T - html2_1_throtleValue < -MAX_ROLL){
      m4T = html2_1_throtleValue - MAX_ROLL;
    }
    //-----------------------------
    if(m1T < 100){
      m1T = 100;
    }
    else if(m1T > 4095){
      m1T = 4095;
    }
    if(m2T < 100){
      m2T = 100;
    }
    else if(m2T > 4095){
      m2T = 4095;
    }
    if(m3T < 100){
      m3T = 100;
    }
    else if(m3T > 4095){
      m3T = 4095;
    }
    if(m4T < 100){
      m4T = 100;
    }
    else if(m4T > 4095){
      m4T = 4095;
    }
    //-----------------------------
    //set motor speed
    esc.setSpeed(0,m1T);
    esc.setSpeed(2,m2T);
    esc.setSpeed(4,m3T);
    esc.setSpeed(5,m4T);
    //debug
    Serial.print("|m1:");Serial.print(m1T);
    Serial.print("|m2:");Serial.print(m2T);
    Serial.print("|m3:");Serial.print(m3T);
    Serial.print("|m4s:");Serial.print(m4T);
  }
  
  // //calibrate esc
  // //set motor speed
  //   esc.setSpeed(0,html2_1_throtleValue);
  //   esc.setSpeed(2,html2_1_throtleValue);
  //   esc.setSpeed(4,html2_1_throtleValue);
  //   esc.setSpeed(5,html2_1_throtleValue);
  //debug value
  Serial.print("|pitchAcc:");Serial.print(mpu.pitchAngleAcc);
  Serial.print("|rollAcc:");Serial.print(mpu.rollAngleAcc);
  Serial.print("|pitchG:");Serial.print(mpu.gyroYNormal);
  Serial.print("|rollG:");Serial.print(mpu.gyroXNormal);
  Serial.print("|pitchT:");Serial.print(mpu.pitch);
  Serial.print("|rollT:");Serial.print(mpu.roll);
  // Serial.print("|yawG:");Serial.print(mpu.gyroZNormal);
  Serial.print("|angle:");Serial.print(mpu.roll);
  //measure time each loop
  Serial.print("|interval ");Serial.println(micros() - previuos);
  previuos = micros();
}