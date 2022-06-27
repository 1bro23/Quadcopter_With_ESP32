//how to use
//call class : Neo6m gps;
//gps.init();
//and use : gps.getLatLongCoordinate(); function
//gps.latitudeGPS;
//gps.longitudeGPS;
//--------------------------//

#include <Arduino.h>

class Neo6m{
    public:
        float latitudeGPS, longitudeGPS;
        String latitudeNMEA, longitudeNMEA, orientNS, orientEW;
        int newDataFlag=0;
        void init(){
            Serial2.begin(9600, SERIAL_8N1);
        }

        void getLatLongNMEA(){
            //skip data until $GPGLL line
            if(Serial2.read() == 'L'){
                Serial2.read();
                //read datatype nmea
                this->latitudeNMEA = Serial2.readStringUntil(',');
                this->orientNS = Serial2.readStringUntil(',');
                this->longitudeNMEA = Serial2.readStringUntil(',');
                this->orientEW = Serial2.readStringUntil(',');

                newDataFlag = 1;
            }
            
        }
        void getLatLongCoordinate(){
            //get raw nmea data
            getLatLongNMEA();
            //convert nmea to gps coordinate
            if(newDataFlag == 1){
                String lat = this->latitudeNMEA;
                int latSub1 = floor(lat.toFloat() / 100);
                float latSub2 = (lat.toFloat() - latSub1*100)/60;
                this->latitudeGPS = latSub1 + latSub2;
                if(this->orientNS == "S"){
                    this->latitudeGPS = -this->latitudeGPS;
                }
                String longi = this->longitudeNMEA;
                int longSub1 = floor(longi.toFloat() / 100);
                float longSub2 = (longi.toFloat() - longSub1*100)/60;
                this->longitudeGPS = longSub1 + longSub2;

                newDataFlag = 0;
            }
        }
};