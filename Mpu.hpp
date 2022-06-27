//note
//on esp pin must be in d21(sda) and d22(scl)
//-------------------------------------------//
// Mpu mpu;
//to init//
// mpu.Init();
// mpu.setAccRange(MPU6050_ACC_RANGE_2G);   //select accel mode
// mpu.setGyrRange(MPU6050_GYR_RANGE_250);  //select gyro mode
//to use//
// mpu.getPitchRollYawAngle();
// real output to be mpu.pitch, mpu.roll, mpu.yaw
#include <Wire.h>
#include <Arduino.h>
#include <math.h>

#ifndef MPU6050_HPP

#define MPU6050_ADDR 0x68
#define MPU6050_GYRO_CONFIG 0x1B
#define MPU6050_ACCEL_CONFIG  0x1C

typedef enum {
  MPU6050_ACC_RANGE_2G,  // +/- 2g (default)
  MPU6050_ACC_RANGE_4G,  // +/- 4g
  MPU6050_ACC_RANGE_8G,  // +/- 8g
  MPU6050_ACC_RANGE_16G // +/- 16g
} mpu6050_acc_range;
float accelRange[4]={16384.0f,8192.0f,4096.0f,2048.0f};

typedef enum {
  MPU6050_GYR_RANGE_250,  // +/- 250 deg/s (default)
  MPU6050_GYR_RANGE_500,  // +/- 500 deg/s
  MPU6050_GYR_RANGE_1000, // +/- 1000 deg/s
  MPU6050_GYR_RANGE_2000  // +/- 2000 deg/s
} mpu6050_gyr_range;
float gyroRange[4]={131.0f,65.5f,32.8f,16.4f};

class Mpu{
    private:
        int accelRangeConfig,gyroRangeConfig;
        unsigned long startTime,currentTime,duration;
        signed short accX, accY, accZ, gyroX, gyroY, gyroZ;
        float gyroXError,gyroYError,gyroZError;

        // write register
        void writeRegister(uint16_t reg, byte value){
            Wire.beginTransmission(MPU6050_ADDR);
            Wire.write(reg); 
            Wire.write(value); 
            Wire.endTransmission(true);
        }









        //get data
        //get accel raw
        void getAccelRaw(){
            Wire.beginTransmission(MPU6050_ADDR);
            Wire.write(0x3B); //register accel_xout_h
            Wire.endTransmission(false); //continue

            Wire.requestFrom(MPU6050_ADDR,6,1); //request 3*2 register

            accX = (Wire.read() << 8 | Wire.read());
            accY = (Wire.read() << 8 | Wire.read());
            accZ = (Wire.read() << 8 | Wire.read());

            Wire.endTransmission(true);
        }
        //normalize accel
        void getAccelNormalize(){
            getAccelRaw();
            this->accXNormal = this->accX / accelRange[accelRangeConfig];
            this->accYNormal = this->accY / accelRange[accelRangeConfig];
            this->accZNormal = this->accZ / accelRange[accelRangeConfig];
        }










        //get gyro raw
        void getGyroRaw(){
            Wire.beginTransmission(MPU6050_ADDR);
            Wire.write(0x43); //register accel_xout_h
            Wire.endTransmission(false); //continue

            Wire.requestFrom(MPU6050_ADDR,6,1); //request 3*2 register

            gyroX = (Wire.read() << 8 | Wire.read());
            gyroY = (Wire.read() << 8 | Wire.read());
            gyroZ = (Wire.read() << 8 | Wire.read());

            Wire.endTransmission(true);
        }
        //normalize gyro
        void getGyroNormalize(){
            getGyroRaw();
            //get gyro data from raw data
            this->gyroXNormal=this->gyroX/gyroRange[gyroRangeConfig];
            this->gyroYNormal=this->gyroY/gyroRange[gyroRangeConfig];
            this->gyroZNormal=this->gyroZ/gyroRange[gyroRangeConfig];

            //fixing gyro by subtract gyroNormal and gyroError
            if(this->callibrateFlag!=1){
                this->gyroXNormal -= 2.595; //get from getGyroError function
                this->gyroYNormal -= -0.20; //get from getGyroError function
                this->gyroZNormal -= -0.02; //get from getGyroError function
            }
        }
        










    public:
        float accXNormal, accYNormal, accZNormal, gyroXNormal, gyroYNormal, gyroZNormal;
        float pitchAngleAcc,rollAngleAcc,yawAngleAcc,gyroXAngle,gyroYAngle,gyroZAngle;
        float pitch, roll, yaw;
    
        //mpu init
        void Init(){
            Wire.begin();
            Wire.beginTransmission(MPU6050_ADDR);
            Wire.write(0X6B); //pwr_mgmt_1
            Wire.write(0); //wake up
            Wire.endTransmission(true);
        }
        // accel config
        void setAccRange(mpu6050_acc_range range){
            writeRegister(MPU6050_ACCEL_CONFIG, range<<3);
            
            this->accelRangeConfig=range;
        }
        // gyro config
        void setGyrRange(mpu6050_gyr_range range){
            writeRegister(MPU6050_GYRO_CONFIG, range<<3);
            
            this->gyroRangeConfig=range;
        }










        //!!IMPORTANT//
        //angle
        void getPitchRollYawAngle(){
            //get accel pitch roll yaw angle
            getAccelNormalize();
            this->pitchAngleAcc = atan2(this->accXNormal,sqrt(pow(this->accZNormal,2)+pow(this->accYNormal,2)))/M_PI*180;
            this->pitchAngleAcc = map(this->pitchAngleAcc,-88.64,88.54,-90,90); //mapping to offset angle
            this->rollAngleAcc = atan2(this->accYNormal,sqrt(pow(this->accZNormal,2)+pow(this->accXNormal,2)))/M_PI*180;
            this->rollAngleAcc = map(this->rollAngleAcc,-88.06,89.87,-90,90); //mapping to offse angle

            //get gyro x y z angle
            getGyroNormalize();

            this->gyroXAngle += this->gyroXNormal * 0.01;
            this->gyroYAngle += this->gyroYNormal * 0.01;
            this->gyroZAngle += this->gyroZNormal * 0.01;

            //calculate difference between gyro and accel
            float gyroYAngleE = this->pitchAngleAcc - (-this->gyroYAngle);  //signed based on mpu6050 value and rotation symbol
            float gyroXAngleE = this->rollAngleAcc - (this->gyroXAngle);    //signed based on mpu6050 value and rotation symbol

            //callibrate gyroAngle with gyro and accel differences
            this->gyroYAngle -= 0.01 * gyroYAngleE; //min because gyroY sign opposite with pitchAcc
            this->gyroXAngle += 0.01 * gyroXAngleE; //pos because gyroX sign opposite with rollAcc

            //yaw shrinking overtime
            this->gyroZAngle = this->gyroZAngle * 0.999;

            //get pitch roll yaw total
            const float accW = 0.05;
            //pitch
            this->pitch = (1-accW) * (-this->gyroYAngle) + (accW) * this->pitchAngleAcc;    //signed based on mpu6050 value and rotation symbol
            //roll
            this->roll = (1-accW) * (this->gyroXAngle) + (accW) * this->rollAngleAcc;       //signed based on mpu6050 value and rotation symbol
            //yaw
            this->yaw = this->gyroZAngle;
            
        }








        //get accel offset
        void getAccelOffset(){
            getAccelNormalize();
            this->pitchAngleAcc = atan2(this->accXNormal,sqrt(pow(this->accZNormal,2)+pow(this->accYNormal,2)))/M_PI*180;
            this->rollAngleAcc = atan2(this->accYNormal,sqrt(pow(this->accZNormal,2)+pow(this->accXNormal,2)))/M_PI*180;
        }
        //get gyro error for callibrate
        int callibrateFlag;
        float totalX,totalY,totalZ,averageX,averageY,averageZ,iC=0,totalEX,totalEY,totalEZ,totalEXT,totalEYT,totalEZT;
        void getGyroError(){
            callibrateFlag = 1;
            getGyroNormalize();

            this->iC++;
            this->gyroXNormal -= this->totalEX;
            this->totalX += this->gyroXNormal;
            this->averageX = this->totalX/iC;
            this->totalEX += this->averageX;
            this->totalEXT += this->totalEX;
            float averageEX = this->totalEXT / iC;

            this->gyroYNormal -= this->totalEY;
            this->totalY += this->gyroYNormal;
            this->averageY = this->totalY/iC;
            this->totalEY += this->averageY;
            this->totalEYT += this->totalEY;
            float averageEY = this->totalEYT / iC;

            this->gyroZNormal -= this->totalEZ;
            this->totalZ += this->gyroZNormal;
            this->averageZ = this->totalZ/iC;
            this->totalEZ += this->averageZ;
            this->totalEZT += this->totalEZ;
            float averageEZ = this->totalEZT / iC;

            Serial.print("|gyroX:");Serial.print(this->gyroXNormal);
            Serial.print("|gyroY:");Serial.print(this->gyroYNormal);
            Serial.print("|gyroZ:");Serial.print(this->gyroZNormal);

            Serial.print("|gyroXC:");Serial.print(this->averageX);
            Serial.print("|gyroYC:");Serial.print(this->averageY);
            Serial.print("|gyroZC:");Serial.print(this->averageZ);

            Serial.print("|gyroXE:");Serial.print(this->totalEX);
            Serial.print("|gyroYE:");Serial.print(this->totalEY);
            Serial.print("|gyroZE:");Serial.print(this->totalEZ);

            Serial.print("|averageXE:");Serial.print(averageEX);
            Serial.print("|averageYE:");Serial.print(averageEY);
            Serial.print("|averageZE:");Serial.println(averageEZ);
        }
};

#endif