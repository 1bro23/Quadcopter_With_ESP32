#ifndef LOCAL_WEBSERVER

#include <WiFi.h>
#include <WebServer.h>
#include <html.h>
#include <style.h>
#include <script.h>

IPAddress MY_IP_SET(192,168,1,100);
IPAddress MY_GATEWAY(192,168,1,1);
IPAddress MY_SUBNET(255,255,255,0);

#define MY_WIFI_PASSWORD "pakyansenkopo"
#define MY_SSID "KONTRAKAN AX/3"

WebServer server(80);

String answear;
int m1,m2,m3,m4;

class MyWebServer{
    public:
        void init(){
            //connecting to wifi
            if(!WiFi.config(MY_IP_SET,MY_GATEWAY,MY_SUBNET)){
                Serial.println("STA failed to configure");
            }
            WiFi.begin(MY_SSID,MY_WIFI_PASSWORD);
            while(WiFi.status()!=WL_CONNECTED){
                Serial.print(".");
                delay(100);
            }
            Serial.println();Serial.print("Connected, your wifi address:");Serial.println(WiFi.localIP());
            //handle home request
            server.on("/",[](){
                MyWebServer MyServer;
                server.send(200,"text/html",html1+style+html2+String(html2_1_throtleValue)+html2_2+script+html3);
            });
            //handle gain speed (press w on keyboard)
            server.on("/gainSpeed",[](){
                //1.125 ms to arming (stable to start engine)
                if(html2_1_throtleValue < 500){
                    html2_1_throtleValue = 500;
                }
                //add 2.5% throtle
                else if(html2_1_throtleValue + 100 < 4095){
                    html2_1_throtleValue += 100;
                }
                // set max throtle
                else{
                    html2_1_throtleValue = 4095;
                }
                // //for callibrating
                // if(html2_1_throtleValue + 100 < 4095){
                //     html2_1_throtleValue += 100;
                // }
                // else{
                //     html2_1_throtleValue = 4095;
                // }
                server.sendHeader("Access-Control-Allow-Origin","*");
                server.send(200,"text/plain",String(html2_1_throtleValue));
            });
            server.on("/decreaseSpeed",[](){
                //devrease 2.5% throtle
                if(html2_1_throtleValue - 100 > 0){
                    html2_1_throtleValue -= 100;
                }
                //set min throtle
                else{
                    html2_1_throtleValue = 0;
                }
                server.sendHeader("Access-Control-Allow-Origin","*");
                server.send(200,"text/plain",String(html2_1_throtleValue));
            });
            server.on("/reset",[](){
                m1 = 0;
                m2 = 0;
                m3 = 0;
                m4 = 0;
                html2_1_throtleValue = 0;
                server.sendHeader("Access-Control-Allow-Origin","*");
                server.send(200,"text/plain","reseted");
            });
            //pid set value
            server.on("/MinProportional",[](){
                KP -= 1;
                server.sendHeader("Access-Control-Allow-Origin","*");
                server.send(200,"text/plain",String(KP));
            });
            server.on("/PlusProportional",[](){
                KP += 1;
                server.sendHeader("Access-Control-Allow-Origin","*");
                server.send(200,"text/plain",String(KP));
            });
            server.on("/MinIntegral",[](){
                KI -= 0.01;
                server.sendHeader("Access-Control-Allow-Origin","*");
                server.send(200,"text/plain",String(KI));
            });
            server.on("/PlusIntegral",[](){
                KI += 0.01;
                server.sendHeader("Access-Control-Allow-Origin","*");
                server.send(200,"text/plain",String(KI));
            });
            server.on("/MinDerivative",[](){
                KD -= 1;
                server.sendHeader("Access-Control-Allow-Origin","*");
                server.send(200,"text/plain",String(KD));
            });
            server.on("/PlusDerivative",[](){
                KD += 1;
                server.sendHeader("Access-Control-Allow-Origin","*");
                server.send(200,"text/plain",String(KD));
            });
            server.on("/rollPos",[](){
                vehicle.desiredRollAngle = -45;
                server.sendHeader("Access-Control-Allow-Origin","*");
                server.send(200,"text/plain",String("rigth"));
            });
            server.on("/rollNeg",[](){
                vehicle.desiredRollAngle = 45;
                server.sendHeader("Access-Control-Allow-Origin","*");
                server.send(200,"text/plain",String("left"));
            });
            server.on("/stable",[](){
                vehicle.desiredRollAngle = 0;
                server.sendHeader("Access-Control-Allow-Origin","*");
                server.send(200,"text/plain",String("stable"));
            });
            server.begin();
        }
        void listen(){
            server.handleClient();
        }
        
};

#endif