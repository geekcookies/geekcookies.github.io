

/***************************
 * Librerie e File
 * https://github.com/ThingPulse/esp8266-oled-ssd1306
 * https://github.com/sparkfun/SparkFun_BME280_Arduino_Library
 **************************/
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <JsonListener.h>
#include "SSD1306Wire.h"
#include "OLEDDisplayUi.h"
#include <stdint.h>
#include "SparkFunBME280.h"
#include "WorldClockClient.h"
#include "icons.h"
#include "fonts.h"

/***************************
 * Inizio Impostazioni
 **************************/
// WIFI
const char* WIFI_SSID = "IL TUO SSID WIFI";
const char* WIFI_PWD = "LA PASSWORD";

// Setup intervallo aggiornamento
const int UPDATE_INTERVAL_SECS = 1 * 60; // Un aggiornamento di tutti i dati una volta al minuto

// Display Settings
const int I2C_DISPLAY_ADDRESS = 0x3c;
// Pin I2C da utilizzare
// sda-pin=D2 and sdc-pin=D1
const int SDA_PIN = D2;
const int SDC_PIN = D1;

SSD1306Wire  display(I2C_DISPLAY_ADDRESS, SDA_PIN, SDC_PIN);
OLEDDisplayUi ui     ( &display );

// Variabili per lettura sensore
String Temp;
String Hum;
String Press;

//Riferimento globale al sensore
BME280 mySensor;

/***************************
 * Fine Impostazioni
 **************************/
String timeZoneIds [] = {"Europe/Rome"}; // Fuso orario da visualizzare
WorldClockClient worldClockClient("it", "IT", "E, dd. MMMMM yyyy", 1, timeZoneIds); // Parametri per il formato data e ora (restituisce anche la data nella lingua prescelta)


// flag che cambia ogni minuto grazie al ticker
bool readyForUpdate = false;

String lastUpdate = "--";

Ticker ticker;

// Visualizzazione durante l'aggiornameno dati
void updateData(OLEDDisplay *display) {
  drawProgress(display, 50, "Sincronizzazione...");
  worldClockClient.updateTime();
  drawProgress(display, 70, "Lettura Temperatura...");
  Temp = String(mySensor.readTempC());
  drawProgress(display, 80, "Lettura Umidita'...");
  Hum = String(mySensor.readFloatHumidity());
  drawProgress(display, 90, "Lettura Pressione...");
  Press = String(int(mySensor.readFloatPressure()/100));
  drawProgress(display, 100, "Fatto...");
  readyForUpdate = false;
  delay(1000);
}

void drawProgress(OLEDDisplay *display, int percentage, String label) { // Routine che disegna la progress bar
  display->clear();
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_10);
  display->drawString(64, 10, label);
  display->drawProgressBar(10, 28, 108, 12, percentage);
  display->display();
}

void drawClock(OLEDDisplay *display, int x, int y, int timeZoneIndex, String city, const char* icon) { // Routine che visualizza il frame dell'orologio
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_10);
  display->drawString(x + 55, y + 5, "Ora Esatta");
  display->setFont(Crushed_Plain_36);
  display->drawXbm(x, y, 50, 50, icon);
  display->drawString(x + 55, y + 15, worldClockClient.getHours(timeZoneIndex) + ":" + worldClockClient.getMinutes(timeZoneIndex));
}

void drawTemp(OLEDDisplay *display, int x, int y, const char* icon, String Lettura) { // Routine genera il frame della temperatura
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_10);
  display->drawString(x + 55, y + 5, "Temperatura C°");
  display->setFont(Crushed_Plain_36);
  display->drawXbm(x, y, 55, 50, icon);
  display->drawString(x + 55, y + 15, Lettura);
}

void drawHum(OLEDDisplay *display, int x, int y, const char* icon,String Lettura) { // Routine che genera il frame dell'umidità
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_10);
  display->drawString(x + 55, y + 5, "Umidita %");
  display->setFont(Crushed_Plain_36);
  display->drawXbm(x, y, 55, 50, icon);
  display->drawString(x + 55, y + 15, Lettura);
}

void drawPress(OLEDDisplay *display, int x, int y, const char* icon,String Lettura) { // Routine che genera il frame della pressione
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_10);
  display->drawString(x + 55, y + 5, "Pressione");
  display->setFont(Crushed_Plain_36);
  display->drawXbm(x, y, 50, 50, icon);
  display->drawString(x + 55, y + 15, Lettura);
}

    // vedere http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
    // su come creare immagini in formato XBM da utilizzare nel file icons.h

  
// Richiamo dei vari generatori dei frame
void drawFrame1(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  drawClock(display, x, y, 0, "Rome",  clock_bits);
}

void drawFrame2(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  drawTemp(display, x, y, temp_bits,Temp);
}

void drawFrame3(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  drawHum(display, x, y, hygro_bits,Hum);
}

void drawFrame4(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  drawPress(display, x, y, atm_bits,Press);
}


void setReadyForTimeUpdate() { //questa funzione viene richiamata dal ticker quando e' tempo di aggiornare i dati
  Serial.println("Setting readyForUpdate to true");
  readyForUpdate = true;
}

// questo array contiene il richiamo a tutti frame da visualizzare
FrameCallback frames[] = {drawFrame1, drawFrame2, drawFrame3, drawFrame4};
int numberOfFrames = 4; //frame totali

void setup() {
  // settaggi per BME280, vedere gli esempi nella libreria per i dettagli
  mySensor.settings.commInterface = I2C_MODE;
  mySensor.settings.I2CAddress = 0x76;
  mySensor.settings.runMode = 3; //Normal mode
  mySensor.settings.tStandby = 7;
  mySensor.settings.filter = 0;
  mySensor.settings.tempOverSample = 5;
  mySensor.settings.pressOverSample = 5;
  mySensor.settings.humidOverSample = 5;
  
  Serial.begin(115200);
  Serial.print("Program Start\n");
  
  //Richiamo .begin() per caricare i settaggi del sensore
  delay(10);  //Tempo per far stabilizzare il sensore. BME280 richiede 2ms per partire.
  Serial.println(mySensor.begin(), HEX);

  Serial.print("\n\n");
    
  Serial.println();  
   Serial.println("inizializzo il display");

 // inizializzo il display
  display.init();
  display.clear();
  display.display();
 //display.flipScreenVertically(); // se volessi girarlo di 180 gradi
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setContrast(255);
  
  // inizializzo il wifi
  WiFi.begin(WIFI_SSID, WIFI_PWD);

  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) { // nel mentre la connessione si stabilizza visualizzo il nostro logo 
    delay(500);
    Serial.print(".");
    display.clear();
    display.drawString(10, 0, "Connecting to ...");
    display.drawXbm(35, 5, geek_width, geek_height, geek_bits);;
    display.display();
    counter++;
  }

  ui.setTargetFPS(30);

  // Si puo' usare
  // TOP, LEFT, BOTTOM, RIGHT
  ui.setIndicatorPosition(BOTTOM); // posizione deli indicatori dei frame

  // Definisce come l'indicatore deve spostarsi.
  ui.setIndicatorDirection(LEFT_RIGHT);

  // Tipo di transizione possibile
  // SLIDE_LEFT, SLIDE_RIGHT, SLIDE_TOP, SLIDE_DOWN
  ui.setFrameAnimation(SLIDE_LEFT);

  // Set dei frame
  ui.setFrames(frames, numberOfFrames);

  // Inizializzazione della UI
  ui.init();

  Serial.println("");

  updateData(&display);

  ticker.attach(UPDATE_INTERVAL_SECS, setReadyForTimeUpdate);

}

void loop() {
   
  if (readyForUpdate && ui.getUiState()->frameState == FIXED) {
    updateData(&display);
  }

  int remainingTimeBudget = ui.update();

  if (remainingTimeBudget > 0) {
       delay(remainingTimeBudget);
  }

}
