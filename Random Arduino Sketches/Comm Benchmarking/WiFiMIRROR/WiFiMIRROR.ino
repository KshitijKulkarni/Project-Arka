
#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress[] =  {0x80, 0x65, 0x99, 0x41, 0x24, 0x12};

// Structure example to receive data
// Must match the sender structure
typedef struct packet {
  uint8_t a;
  uint8_t b;
  int32_t bigNUM;
} packet;

packet Packet;



void SendData() {

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&Packet, sizeof(Packet));
  // Serial.println(result == ESP_OK ? "Sent" : "ehh died");
}

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&Packet, incomingData, sizeof(Packet));
  // Serial.print("Bytes received: ");
  // Serial.println(len);
  // Serial.print("a: ");
  // Serial.println(Packet.a);
  // Serial.print("b: ");
  // Serial.println(Packet.b);
  // Serial.print("bigNUM: ");
  // Serial.println(Packet.bigNUM);
  // Serial.println();

  SendData();
}


esp_now_peer_info_t peerInfo;
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  memset(&peerInfo, 0, sizeof(peerInfo));
  peerInfo.ifidx = WIFI_IF_STA; 
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

void loop() {
}