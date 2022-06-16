
#include <Arduino.h>

class M5_SIM7080G
{
    private:
        HardwareSerial *_serial;
    public:
        void Init(HardwareSerial *serial = &Serial2, uint8_t RX = 16, uint8_t TX = 17);
        bool checkStatus();
        String waitMsg(unsigned long time);
        void sendMsg(String command);
        String getMsg();
        String send_and_getMsg(String str);
};