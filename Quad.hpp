#include <Mpu.hpp>
#include <MyPorcessing.hpp>
#include <Adafruit_BMP085.h>
#include <Qmc5883l.hpp>

#ifndef QUAD_HPP

//pid gain
// #define KP 1.2
// #define KI 0.01
// #define KD 7.5
float KP = 20, KI = 0, KD = 8;
//some angle value
// #define STABLE_ANGLE 0
//max roll
#define MAX_ROLL 250

Mpu mpu;
MyProcessing alt;
Adafruit_BMP085 bmp;
Qmc5883l compass;

class Quad{
    public:
        //motor output
        signed int m1Roll,m2Roll,m3Roll,m4Roll;
        signed int m1Pitch,m2Pitch,m3Pitch,m4Pitch;
        signed int m1Yaw,m2Yaw,m3Yaw,m4Yaw;
        signed int m1AltH,m2AltH,m3AltH,m4AltH;
        //pitch roll yaw mapped to 0 <-> 4095
        signed int pitchMapped, rollMapped, yawMapped;
        //current altitude
        float startAltitude;
        //pid
        float rollP,rollI,rollD;
        float pitchP, pitchI, pitchD;
        float yawP, yawI, yawD;
        //previous error
        float previousRollE, previousPitchE, previousYawE;
        //pid start flag
        short pFlag, iFlag, dFlag;
        //gyro smooth
        float gyroXNormalSmooth, gyroYNormalSmooth, gyroZNormalSmooth;
        //target Angle
        float desiredRollAngle, desiredPitchAngle, desiredYawAngle;

        void init(){
            //init mpu
            mpu.Init();
            mpu.setAccRange(MPU6050_ACC_RANGE_2G);   //select accel mode
            mpu.setGyrRange(MPU6050_GYR_RANGE_250);  //select gyro mode



            // init bmp
            bmp.begin(3);
            //callibrate altitude
            float totalAltitude;
            int n = 30;
            for(int i = 0; i < n; i++){
                totalAltitude += bmp.readAltitude();
            }
            this->startAltitude = totalAltitude / n;


            //init compass
            compass.init();
        }
        //make quad stable
        void calm(){
            //pid value const
            float KPRoll = KP * 0.87577639751, KIRoll = KI * 0.87577639751, KDRoll = KD * 0.87577639751;
            float KPPitch = KP, KIPitch = KI, KDPitch = KD;
            float KPYaw = 10, KIYaw = 0, KDYaw = 2;
            //-----------------------------
            //pitch pos is forward
            //roll pos is titl to the left
            //yaw pos is ccw
            //-----------------------------
            //get pitch roll yaw angle
            mpu.getPitchRollYawAngle();
            //-----------------------------
            //this for roll action
            //get roll error
            float rollE = this->desiredRollAngle - mpu.roll;
            //its proportional
            this->rollP = KPRoll * rollE;
            //its integral
            this->rollI += KIRoll * rollE;
            //its derivative
            this->rollD = KDRoll * mpu.gyroXNormal;
            this->previousRollE = rollE;
            //get total roll speed
            float totalRollSpeed = -this->rollP + (-this->rollI) + (this->rollD);
            //assign speed to each motor on roll action
            this->m1Roll = totalRollSpeed;
            this->m2Roll = -totalRollSpeed;
            this->m3Roll = -totalRollSpeed;
            this->m4Roll = totalRollSpeed;
            //-----------------------------
            //this for pitch action
            //get pitch error
            float pitchE = this->desiredPitchAngle - mpu.pitch;
            //its proportional
            this->pitchP = KPPitch * pitchE;
            //its integral
            this->pitchI += KIPitch * pitchE;
            //its derivative
            this->pitchD = KDPitch * -mpu.gyroYNormal;
            this->previousPitchE = pitchE;
            //get total pitch speed
            float totalPitchSpeed = -this->pitchP + (-this->pitchI) + (this->pitchD);
            //assign speed to each motor on pitch action
            this->m1Pitch = totalPitchSpeed;
            this->m2Pitch = totalPitchSpeed;
            this->m3Pitch = -totalPitchSpeed;
            this->m4Pitch = -totalPitchSpeed;
            //-----------------------------
            //get yaw error
            float yawE = this->desiredYawAngle - mpu.yaw;
            //its proportional
            this->yawP = KPYaw * yawE;
            //its integral
            this->yawI += KIYaw * yawE;
            //its derivative
            this->yawD = KDYaw * mpu.gyroZNormal;
            this->previousYawE = yawE;
            //get total yaw speed
            float totalYawSpeed = -this->yawP + (-this->yawI) + (this->yawD);
            // //diactived yaw
            // totalYawSpeed = 0;
            //assign speed to each motor on yaw action
            this->m1Yaw = -totalYawSpeed;
            this->m2Yaw = totalYawSpeed;
            this->m3Yaw = -totalYawSpeed;
            this->m4Yaw = totalYawSpeed;
        }


        unsigned long previous2;
        int throtle;
        void altHold(){
            //get throtle speed by subtract start alt with current alt
            if(micros() - this->previous2 >= 200000){
                this->previous2 = micros();
                Serial.print("trigger");
                float altDif = this->startAltitude - alt.smoothLikeButter(bmp.readAltitude());
                this->throtle = altDif * 300;
            }
            //apply speed to esc speed
            gainSpeed(this->m1AltH, throtle);
            gainSpeed(this->m2AltH, throtle);
            gainSpeed(this->m3AltH, throtle);
            gainSpeed(this->m4AltH, throtle);
        }




        void setCompassDirection(float angle){
            //set offset angle input
            if(angle > 360){
                angle = 360;
            }
            else if(angle < 0){
                angle = 0;
            }
            //get compass direction;
            compass.getAngle();
            //subtract current position with target angle
            float speedForYaw;
            if(angle + 180 <= compass.angle){
                speedForYaw = compass.angle - (angle + 360);
            }
            else{
                speedForYaw = compass.angle - angle;
            }
            //mapping speedForYaw
            speedForYaw = map(speedForYaw, -180, 180, -4095, 4095);
            //counter yaw pos
            gainSpeed(this->m2Yaw, speedForYaw);
            gainSpeed(this->m4Yaw, speedForYaw);

            decreaseSpeed(this->m1Yaw, speedForYaw);
            decreaseSpeed(this->m3Yaw, speedForYaw);
        }






        //tools
        void gainSpeed(signed int &m,signed int speed){
            m += speed;
            //set offset
            if(m > 4095){
                m = 4095;
            }
            else if(m < 0){
                m = 0;
            }
        }
        void decreaseSpeed(signed int &m,signed int speed){
            m -= speed;
            //set offset
            if(m > 4095){
                m = 4095;
            }
            else if(m < 0){
                m = 0;
            }
        }
};

#endif