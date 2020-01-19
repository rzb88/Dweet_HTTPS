// um Daten abzufragen:https://dweet.io/get/latest/dweet/for/YOURTHING
// Daten senden: https://dweet.io/dweet/for/YOURTHING?Hello=World

// https://github.com/rzb88/TestVB/upload/master

// Auszahlreichen Sketchen gesammelter Code, der von mir zum laufen gebracht wurde. 


#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <OneWire.h>                                                                    //Temperatur vom DS18B20
#include <DallasTemperature.h>
float Temperatur;

#define ONE_WIRE_BUS D1                                                                 // DS18B20 Data Pin
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress Sensor1;

String arrayVariableNames[] = {"Temp"};                                                 // so kann man jeder Variable einen Index zuordnen
float arrayVariableValues[] = {0.00};                                                   // so kann man jeden "Messwert" als z.B. float instanzieren


void setup() {
  Serial.begin(115200);   																// Serielle Ausgabe instanzieren und auf 115200 Baud einstellen
  Serial.println();
  Serial.println(F("WiFi startet!"));
  WiFi.mode(WIFI_STA);                                                                  // WiFi auf Station
  WiFi.begin("WIFI", "PASSWORD");                                      					// WiFi Zugangsdaten

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");                                                                  // jede 500ms einen Punkt zum Serial-Monitor schicken
  }

  Serial.println(F("\n\r mit WiFi verbunden!"));                                        // Hier ist man mit seinem WiFi verbunden
  
  sensors.begin();                                                                      // DS18B20 zum funktionieren überreden
  sensors.setResolution(12);                                                            // Messwerte auf 12bit stellen
  
  if (!sensors.getAddress(Sensor1, 0)) Serial.println("Sensor-Adresse konnte nicht aufgelöst werden");
  Serial.println();
  Serial.print(sensors.getDeviceCount(), DEC);                                          // Anzahl an verbundenen DS18B20 ermitteln und ausgeben
  Serial.println(" DS18B20 gefunden! ");
  Serial.print("Adresse: ");
  printAddress(Sensor1);                                                                // Adresse des/der gefundenen DS18B20 ausgeben
  Serial.println();
  Serial.print("Sensor-Auflösung: ");
  Serial.print(sensors.getResolution());                                                // die zuvor eingestellte Messwertvorgabe ausgeben (z.B. 12 bit)
  Serial.println(" bit");
  Serial.print("Wartezeit: 120 Sekunden");
  Serial.println(" Sekunden");
  Serial.println("-> Starte Messung und Ubertragung");
  delay(1000);
  
}

void printAddress(DeviceAddress deviceAddress)                                          // Funktion zur Ermittlung aller DS18B20 Temperatur-Sensoren
{
  for (uint8_t i = 0; i < 8; i++)                      
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);                                                // Alle Adressen gefundener DS18B20 anzeigen (jeder Sensor = eigene Adresse)
  }
}


void dweetdata() {                                                                      // Funktion ruft Webseite mit Credentials (Messwerte)
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);      // SSL-WiFi-Client instanzieren
  client->setInsecure();                                                                // Client auf "unsicher" setzen (man braucht keinen Fingerprint des Servers)
  HTTPClient https;

  if (https.begin(*client, "https://dweet.io/dweet/for/spiderlab?" + arrayVariableNames[0] + arrayVariableValues[0])) {  // HTTPS-Request mit Messwerten deployen
    int httpCode = https.GET();

    if (httpCode > 0) {
      Serial.printf("[HTTPS] GET... code: %d\n", httpCode);                             
      if (httpCode == HTTP_CODE_OK) {
          String payload = https.getString();
          Serial.println(String("[HTTPS] Received payload: ") + payload);               // hier kommt die Payload unseres requests auf die serielle Konsole
      }
    } else {
      Serial.printf("[HTTPS] GET... Fehler: %s\n\r", https.errorToString(httpCode).c_str());   // Fehler
    }

    https.end();                                                                        // Request beenden
  } else {
    Serial.printf("[HTTPS] keine Verbindung zum  Server\n\r");                          // Fehler
  }
}

void loop() {
  sensors.requestTemperatures();                                                        // DS18B20 zum Temperatur-messen überreden
  Temperatur = sensors.getTempCByIndex(0), DEC ;                                        // gemessene Temperatur in Celsius anfragen und in der Variable "Temperatur" ablegen
  arrayVariableNames[0] = "DS18B20=";                                                   // dem am Anfang instanzierten Namen-Array einen Eintrag verpassen
  arrayVariableValues[0] = Temperatur;                                                  // ""      ""      ""    ""    Werte-Array einen Eintrag verpassen
  dweetdata();                                                                          // Funktion um Messwerte zu deployen aufrufen
  Serial.println("Pause für 120 Sekunden");                                             // Serielle Ausgabe
  delay(120000);                                                                        // Um Serverlast gering  zu halten, Daten nur alle 120 Sekunden droppen
}
