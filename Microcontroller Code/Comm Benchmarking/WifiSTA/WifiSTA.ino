#include <esp_wifi.h>
#include "WiFi.h" 

void setup() {
  Serial.begin(9600);

  esp_err_t ret;

  // ret = esp_wifi_set_mode(WIFI_MODE_STA);
  // ret = esp_wifi_start();

  
  WiFi.mode(WIFI_STA);
  WiFi.STA.begin();

  uint8_t baseMac[6];
  ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
  if (ret == ESP_OK) {
    Serial.println("Here be mac: ");
    Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
                  baseMac[0], baseMac[1], baseMac[2],
                  baseMac[3], baseMac[4], baseMac[5]);
  } else {
    Serial.println("Error came off :(");
  }
}

void loop() {
  // // put your main code here, to run repeatedly:
  // Serial.println("Hi!");
  // delay(1000);
}
