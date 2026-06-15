#include <RadioLib.h>
#include <SPI.h>

#define LORA_SCK    18
#define LORA_MISO   19
#define LORA_MOSI   23
#define LORA_CS      5
#define LORA_RESET  14
#define LORA_BUSY   21
#define LORA_DIO1   26

SX1262 radio = new Module(
    LORA_CS,
    LORA_DIO1,
    LORA_RESET,
    LORA_BUSY
);

// ─── Flag ────────────────────────────────
volatile bool receivedFlag = false;

// ─── Interrupt Callback ──────────────────
void setFlag(void) {
    receivedFlag = true;
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Receiver Starting...");

    SPI.begin(LORA_SCK,
              LORA_MISO,
              LORA_MOSI,
              LORA_CS);

    int state = radio.begin(
        433.0,   // Frequency
        125.0,   // Bandwidth
        9,       // SF
        7,       // CR
        0x12,    // Sync Word
        10,      // Power
        8        // Preamble
    );

    if (state == RADIOLIB_ERR_NONE) {
        Serial.println("LoRa Init OK!");
    } else {
        Serial.print("Init Failed: ");
        Serial.println(state);
        while (1);
    }
    radio.setDio1Action(setFlag);

    state = radio.startReceive();
    if (state == RADIOLIB_ERR_NONE) {
        Serial.println("Listening...");
    }
}

void loop() {
    // ─── Flag check karo ─────────────────
    if (receivedFlag) {
        receivedFlag = false;

        String received = "";
        int state = radio.readData(received);

        if (state == RADIOLIB_ERR_NONE) {
            Serial.println("═══════════════════");
            Serial.print("Message : ");
            Serial.println(received);
            Serial.print("RSSI    : ");
            Serial.print(radio.getRSSI());
            Serial.println(" dBm");
            Serial.print("SNR     : ");
            Serial.print(radio.getSNR());
            Serial.println(" dB");
            Serial.println("═══════════════════");

        } else {
            Serial.print("Read Error: ");
            Serial.println(state);
        }

        radio.startReceive();
    }
}