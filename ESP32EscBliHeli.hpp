//how to use
// ESP32EscBliHeli esc;
// esc.attach(pwmChannel,pinoutput);
// esc.setSpeed(pwmChannel,speed);
//----------//
int debugLed = 0;
class ESP32EscBliHeli{    
    public:
        void attach(short pwmChannel, short outputPin){
            ledcSetup(pwmChannel,50,16);
            ledcAttachPin(outputPin,pwmChannel);
        }
        void setSpeed(short pwmChannel, int value){
            //if use led
            if(debugLed == 1){
                value = map(value, 0, 4095, 0, 65535);    
            }
            //if use esc
            else{
                value = map(value, 0, 4095, 3276, 6885);//3276 && 6553
            }
            ledcWrite(pwmChannel,value);
        }
};