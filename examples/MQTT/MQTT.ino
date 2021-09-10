/*
    Description: 
    Use UNIT CAT-M to connect to the MQTT server, and implement subscription and publishing messages.
    Check the status through Serial and Screen. When the MQTT connection is successful, Click BtnA Public Topic
    Please install library before compiling:  
    M5GFX: https://github.com/m5stack/M5GFX
    M5Stack: https://github.com/m5stack/M5Stack
*/
#include "M5Stack.h"
#include "M5GFX.h"
#include "M5_SIM7080G.h"

M5GFX display;
M5Canvas canvas(&display);

M5_SIM7080G device;

String readstr;

void log(String str) {
    Serial.print(str);
    canvas.print(str);
    canvas.pushSprite(0, 0);
}

void setup()
{
    M5.begin();


    display.begin();

    if (display.isEPD())
    {
        display.setEpdMode(epd_mode_t::epd_fastest);
        display.invertDisplay(true);
        display.clear(TFT_BLACK);
    }
    if (display.width() < display.height())
    {
        display.setRotation(display.getRotation() ^ 1);
    }

    canvas.setColorDepth(1); // mono color
    canvas.createSprite(display.width(), display.height());
    canvas.setTextSize((float)canvas.width() / 160);
    canvas.setTextScroll(true);

    //SIM7020
    device.Init(&Serial2, 16, 17);

    device.sendMsg("AT+CREBOOT\r\n");
    delay(1000);
    while(1){
        device.sendMsg("AT+CSQ\r\n");
        readstr = device.waitMsg(1000);
        log(readstr);
        if(readstr.indexOf("+CSQ: 99,99") ==-1){
            break;
        }
    }
}

void loop()
{

    while(1) {
        device.sendMsg("AT+CNACT=0,1\r\n");
        readstr = device.waitMsg(200);
        log(readstr);

        device.sendMsg("AT+CNACT?\r\n");
        readstr = device.waitMsg(200);
        log(readstr);

        device.sendMsg("AT+SMCONF=\"URL\",\"broker.emqx.io\",\"1883\"\r\n");
        readstr = device.waitMsg(1000);
        log(readstr);

        device.sendMsg("AT+SMCONF=\"KEEPTIME\",60\r\n");
        readstr = device.waitMsg(1000);
        log(readstr);

        device.sendMsg("AT+SMCONF=\"CLEANSS\",1\r\n");
        readstr = device.waitMsg(1000);
        log(readstr);

        device.sendMsg("AT+SMCONF=\"CLIENTID\",\"simmqtt\"\r\n");
        readstr = device.waitMsg(1000);
        log(readstr);

        device.sendMsg("AT+SMCONN\r\n");
        readstr = device.waitMsg(5000);
        log(readstr);

        if(readstr.indexOf("ERROR") == -1) {
            break;
        }
    }

    device.sendMsg("AT+SMSUB=\"sub_topic\",1\r\n");
    readstr = device.waitMsg(1000);
    log(readstr);

    while(1){
        M5.update();
        if(M5.BtnA.wasPressed()) {
            device.sendMsg("AT+SMPUB=\"pub_topic\",5,1,1\r\n");
            delay(100);
            device.sendMsg("hello\r\n");
        }
        readstr = device.waitMsg(0);
        Serial.print(readstr);
        log(readstr);
    }
}
