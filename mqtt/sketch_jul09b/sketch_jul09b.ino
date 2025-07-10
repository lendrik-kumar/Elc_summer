#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "DHT.h"

// ----- Wi-Fi Credentials -----
const char* ssid = "EACCESS";
const char* password = "hostelnet";

// ----- MQTT Credentials (HiveMQ Cloud) -----
const char* mqtt_server = "8f6746e9083d405fbbee65151e4af05c.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char* mqtt_user = "sanket1";
const char* mqtt_pass = "Sanket@2006";

// ----- DHT22 Setup -----
#define DHTPIN 14
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ----- Soil Moisture -----
#define SOIL_PIN 34

WiFiClientSecure espClient; // Secure Client
PubSubClient client(espClient);

// ----- Root Certificate for HiveMQ Cloud (Let's Encrypt R3) -----
const char* root_ca = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIFazCCA1OgAwIBAgISA6TuN++psF2V2PYeM+gZ5wXlMA0GCSqGSIb3DQEBCwUA\n" \
"MEoxCzAJBgNVBAYTAlVTMRMwEQYDVQQKEwpMZXQncyBFbmNyeXB0MRwwGgYDVQQL\n" \
"ExN3d3cubGV0c2VuY3J5cHQub3JnMB4XDTIxMDQxMzEyMDExM1oXDTIxMDcxMjEy\n" \
"MDExM1owTjELMAkGA1UEBhMCVVMxEzARBgNVBAoTCkxldCdzIEVuY3J5cHQxHzAd\n" \
"BgNVBAMTFkxldCdzIEVuY3J5cHQgQXV0aG9yaXR5IFIzMIIBIjANBgkqhkiG9w0B\n" \
"AQEFAAOCAQ8AMIIBCgKCAQEAwHHV9H7PZbJgK7sU1HXkG5VNRVxYNL9fQ9tnV0Nf\n" \
"pLqGRdvxRi6Zs2V7j1RBPXlELvl38/LcVjNvW8BdYLUuDfxS62ePt4wcozEGHXLZ\n" \
"t6cSLuRwZ2fGQOe+U5VgwcfPLiQ69Z1EF1vw69jbWv9jOG5z2Npp9I1lKj2bQ9vG\n" \
"Fh99xGfYkNwcGuN+fp2D0ME8Wve5DXK9iNUPYvkkhFnx9BIln1rb4ez+ThjI79Db\n" \
"H4ezQAWpAaDazwFx+5kN1Eg61D3TAvx26Vn8CklG5kDJW/4Cp6rPKijC1NMIMkLZ\n" \
"Gal6FeV7Xv/NpOKt+eHkgc41G7RIbsxSwnq1X9/xKH7UvQIDAQABo4ICODCCAjQw\n" \
"DwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUdRP4ChLRySFFi7pOoYV1XM2OY0Uw\n" \
"HwYDVR0jBBgwFoAUuV2sTLfZo3Jhd2Cl+BzJZ+a2w5YwDgYDVR0PAQH/BAQDAgGG\n" \
"MB0GA1UdJQQWMBQGCCsGAQUFBwMBBggrBgEFBQcDAjAtBgNVHREEJjAkoSognSqG\n" \
"H2h0dHBzOi8vd3d3LmxldHNlbmNyeXB0Lm9yZy9yZXBvc2l0b3J5MIIBAwYKKoZI\n" \
"zj0EAwIEATAfBgNVHSMEGDAWgBS5XaxMt9mjcml3YKX4HMln5rbDlgB1BgNVHR8E\n" \
"bjBsMGqgX6BdhlNodHRwOi8vY3JsLmxldHNlbmNyeXB0Lm9yZy9sZXRzLWVuY3J5\n" \
"cHQtcm9vdC1yMy5jcmwwa6BrBGEwbzB1MQswCQYDVQQGEwJVUzETMBEGA1UEChMK\n" \
"TGV0J3MgRW5jcnlwdDEPMA0GA1UECxMGUm9vdCBDQTEhMB8GA1UEAxMYTGV0J3Mg\n" \
"RW5jcnlwdCBDQSBPcmcgUm9vdDEXMBUGCSqGSIb3DQEJARYIcm9vdEBsZW5jcnlw\n" \
"dC5vcmcwDQYJKoZIhvcNAQELBQADggEBACU+3tzsQ5diFFm0duKf3sPS4vTx3/Vo\n" \
"ULQxzD9v0VqFcRj7kFY/tKHyTKCQ3yHHPCEsvGdRJn9BKVtZQ+c9c07v/jpRIfuO\n" \
"AwR+/FpiWR6rHn4D9qFuPHbs5OTCrYpY/NlvfnqFtrUVFJ+2vGiZVA4kRA6KaUlO\n" \
"3yG30XZRSm4S+8/y2DwclmAZnJtnyCWUHEcE2MA0\n" \
"-----END CERTIFICATE-----\n";

void setup_wifi() {
  delay(100);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client", mqtt_user, mqtt_pass)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  setup_wifi();

  espClient.setCACert(root_ca);  // Set TLS cert
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int soilRaw = analogRead(SOIL_PIN);
  int soilPercent = map(soilRaw, 4095, 1300, 0, 100);  // Adjust based on your soil sensor

  if (!isnan(temp) && !isnan(hum)) {
    char tempStr[10], humStr[10], soilStr[10];
    dtostrf(temp, 4, 2, tempStr);
    dtostrf(hum, 4, 2, humStr);
    dtostrf(soilPercent, 4, 2, soilStr);

    client.publish("esp32/temperature", tempStr);
    client.publish("esp32/humidity", humStr);
    client.publish("esp32/soil", soilStr);

    Serial.println("Data published:");
    Serial.print("Temp: "); Serial.println(tempStr);
    Serial.print("Humidity: "); Serial.println(humStr);
    Serial.print("Soil: "); Serial.println(soilStr);
  } else {
    Serial.println("Failed to read from DHT sensor!");
  }

  delay(5000);
}
