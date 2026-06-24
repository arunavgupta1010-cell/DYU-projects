#include <SoftwareSerial.h>

SoftwareSerial stm(2,3);

const char* commands[] =
{
    "<GET,FREQ>",
    "<GET,SF>",
    "<GET,BW>",
    "<GET,CR>",
    "<GET,POWER>"
};

int cmd_index = 0;

void setup()
{
    Serial.begin(115200);
    stm.begin(9600);

    Serial.println("UNO Ready");
}

void loop()
{
    stm.println(commands[cmd_index]);

    Serial.print("TX -> ");
    Serial.println(commands[cmd_index]);

    unsigned long start = millis();

    while(millis() - start < 1000)
    {
        if(stm.available())
        {
            String response =
                stm.readStringUntil('\n');

            Serial.print("RX <- ");
            Serial.println(response);

            break;
        }
    }

    cmd_index++;

    if(cmd_index >= 5)
        cmd_index = 0;

    delay(2000);
}