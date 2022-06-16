/*
    Description: 
    Use UNIT CAT-M to connect to the MQTT server, and implement subscription and publishing messages.
    Check the status through Serial and Screen. When the MQTT connection is successful, Click BtnA Public Topic
    Please install library before compiling:  
    M5GFX: https://github.com/m5stack/M5GFX
    M5Stack: https://github.com/m5stack/M5Stack

    This example is run on M5StackCore.
*/
#include "M5Stack.h"
#include "M5GFX.h"
#include "M5_SIM7080G.h"

M5GFX display;
M5Canvas canvas(&display);

M5_SIM7080G device;

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

    canvas.setColorDepth(1);
    canvas.createSprite(display.width(), display.height());
    canvas.setTextSize((float)canvas.width() / 160);
    canvas.setTextScroll(true);

    /* Init SIM7080G */
    device.Init(&Serial2, 16, 17);

    /* Reboot SIM7080G */
    log("Reboot SIM7080G..");
    while (device.send_and_getMsg("AT+CREBOOT\r\n").indexOf("OK") == -1) {
        log("..");
        delay(1000);
    }

    /* Open GNSS power */
    log("\nTurn GNSS power on..");
    while (device.send_and_getMsg("AT+CGNSPWR=1\r\n").indexOf("OK") == -1) {
        log("..");
        delay(1000);
    }

    /* Ready to print GNSS info */
    log("\nReady to print GNSS info!\n");
    delay(2000);
}

void loop()
{
    /* Print GNSS info every 1s */
    log(device.send_and_getMsg("AT+CGNSINF\r\n"));
    delay(1000);
}
