#include <RadioLib.h>
#include <SPI.h>


// ─── Pin Config ──────────────────────────
#define LORA_SCK    18
#define LORA_MISO   19
#define LORA_MOSI   23
#define LORA_CS      5
#define LORA_RESET  14
#define LORA_BUSY   21
#define LORA_DIO1   26

// ─── SX1262 Object ───────────────────────
SX1262 radio = new Module(
    LORA_CS,
    LORA_DIO1,
    LORA_RESET,
    LORA_BUSY
);

int count = 0;

void setup() {
    Serial.begin(115200);
    Serial.println("SX1262 Transmitter Start!");

    // SPI pins set karo
    SPI.begin(LORA_SCK,
              LORA_MISO,
              LORA_MOSI,
              LORA_CS);

    // LoRa init karo
    int state = radio.begin(
        867.0,   // Frequency MHz
        125.0,   // Bandwidth kHz
        9,       // Spreading Factor
        7,       // Coding Rate
        0x12,    // Sync Word
        10,      // TX Power dBm
        8        // Preamble Length
    );

    if (state == RADIOLIB_ERR_NONE) {
        Serial.println("LoRa Init OK!");
    } else {
        Serial.print("LoRa Init Failed: ");
        Serial.println(state);
        while (1);
    }
}

void loop() {
    // Serial se input lo
    if (Serial.available()) {
        String msg = Serial.readString();
        msg.trim();  // Extra spaces hatao

    Serial.print("Sending: ");
    Serial.println(msg);

    // Transmit karo
    int state = radio.transmit(msg);

    if (state == RADIOLIB_ERR_NONE) {
        Serial.println("Sent OK!");
    } else {
        Serial.print("Send Failed: ");
        Serial.println(state);
    }
    }
}
    
