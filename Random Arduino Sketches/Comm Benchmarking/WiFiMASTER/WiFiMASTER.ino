#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress[] = {0xD4, 0x8A, 0xFC, 0xCE, 0xF3, 0x80};

typedef struct packet{
  uint8_t a;
  uint8_t b;
  int32_t bigNUM;
} packet;

packet Packet;
packet RecvPacket;

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // Serial.print("\r\nLast Packet Send Status:\t");
  // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

volatile bool recvd = false;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&RecvPacket, incomingData, sizeof(Packet));
  // Serial.print("Bytes received: ");
  // Serial.println(len);
  // Serial.print("a: ");
  // Serial.println(Packet.a);
  // Serial.print("b: ");
  // Serial.println(Packet.b);
  // Serial.print("bigNUM: ");
  // Serial.println(Packet.bigNUM);
  // Serial.println();
  recvd = true;
}

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(esp_now_send_cb_t(OnDataSent));
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

unsigned long oldTime;
void loop() {
  Packet.a = random(1,20);
  Packet.b = random(1,20);
  Packet.bigNUM = random(-20,20);
  oldTime = micros();
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &Packet, sizeof(Packet));
  // recvd = false;
  while(!recvd);
  recvd = false;
  unsigned long duration = micros() - oldTime;
  Serial.print("Time of flight: ");
  Serial.print(duration/2);
  Serial.println(" microseconds");
  delay(1000);
}
