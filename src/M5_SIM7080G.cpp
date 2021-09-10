#include "M5_SIM7080G.h"


void M5_SIM7080G::Init(HardwareSerial *serial, uint8_t RX, uint8_t TX) {
    _serial = serial;
    _serial->begin(115200, SERIAL_8N1, RX, TX);
}


String M5_SIM7080G::waitMsg(unsigned long time) {
    String restr;
    unsigned long start = millis();
    while(1){
        if(Serial2.available() || (millis() - start) < time) {
            String str = Serial2.readString();
                restr += str;
        }else{
            break;
        }
    }
    return restr;
}

void M5_SIM7080G::sendMsg(String command) {
    _serial->print(command);
}







