//how to use
//Wire wire
//wire.init();
//wire.getAngle();
//wire.angle; <- value
//profit!!!
#include <Wire.h>

#ifndef QMC5883L_HPP

#define QMC5883L_ADDR 0x0D
#define QMC5883L_SELECT_MODE 0x09
#define QMC5883L_DATA_START 0x00
#define QMC5388L_MODE (QMC5388L_512 << 6 | QMC5388L_2G << 4 | QMC5388L_200HZ << 2 | QMC5388L_CONTINUOUS)
// put data after callibrate here

// this for 2G range
#define xmax 5225
#define xmin -5730
#define ymax 8357
#define ymin -1582
#define zmax 587
#define zmin -9800

// this for 8G range
// #define xmax 4366
// #define xmin 1701
// #define ymax 3065
// #define ymin 646
// #define zmax 3562
// #define zmin 625

//get offset
#define xOff ((xmax + xmin)/2)
#define yOff ((ymax + ymin)/2)
#define zOff ((zmax + zmin)/2)


typedef enum{
    QMC5388L_STANBY,
    QMC5388L_CONTINUOUS
}qmc5883l_mode;

typedef enum{
    QMC5388L_10HZ,
    QMC5388L_50HZ,
    QMC5388L_100HZ,
    QMC5388L_200HZ
}qmc5883l_odr;

typedef enum{
    QMC5388L_2G,
    QMC5388L_8G
}qmc5883l_rng;

typedef enum{
    QMC5388L_512,
    QMC5388L_256,
    QMC5388L_128,
    QMC5388L_64,
}qmc5883l_osr;

class Qmc5883l{
    public:
        signed short x, y, z, stat; //raw data
        //just init
        void init(){
            Wire.begin();
            Wire.beginTransmission(QMC5883L_ADDR);
            Wire.write(QMC5883L_SELECT_MODE);
            Wire.write(QMC5388L_MODE);                  //set to continuous mode, from datasheet
            Wire.endTransmission(true);
        }
        //get raw data
        void getData(){
            //set pointer
            Wire.beginTransmission(QMC5883L_ADDR);
            Wire.write(QMC5883L_DATA_START);
            Wire.endTransmission(false);
            //read data
            Wire.requestFrom(QMC5883L_ADDR,6,1);
            this->x = Wire.read() | Wire.read() << 8;
            this->y = Wire.read() | Wire.read() << 8;
            this->z = Wire.read() | Wire.read() << 8;
            Wire.endTransmission(true);
        }





        //get offset of x y z (for callibration)
        signed short xMax = -30000, yMax = -30000, zMax = -30000, xMin = 30000, yMin = 30000, zMin = 30000;
        void getOffset(){
            getData();
            //x
            if(this->xMax < this->x){
                this->xMax = this->x;
            }
            if(this->xMin > this->x){
                this->xMin = this->x;
            }
            //y
            if(this->yMax < this->y){
                this->yMax = this->y;
            }
            if(this->yMin > this->y){
                this->yMin = this->y;
            }
            //z
            if(this->zMax < this->z){
                this->zMax = this->z;
            }
            if(this->zMin > this->z){
                this->zMin = this->z;
            }
            
            Serial.print("|Xmax:");Serial.print(this->xMax);
            Serial.print("|xMin:");Serial.print(this->xMin);
            Serial.print("|ymax:");Serial.print(this->yMax);
            Serial.print("|yMin:");Serial.print(this->yMin);
            Serial.print("|zmax:");Serial.print(this->zMax);
            Serial.print("|zMin:");Serial.println(this->zMin);

        }





        //and subtract raw data with offset to get calibrated data
        void getDataCallibrated(){
            getData();
            //callibrate x y z
            this->x -= xOff;
            this->y -= yOff;
            this->z -= zOff;
        }
        //get angle from raw data
        float angle;
        void getAngle(){
            getDataCallibrated();
            this->angle = atan2(this->y , this->x) / M_PI * 180;
            if(this->angle < 0){
                this->angle += 360.0;
            }
        }
};

#endif