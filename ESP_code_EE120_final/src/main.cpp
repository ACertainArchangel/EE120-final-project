#include <Arduino.h>
#include <ESP8266WiFi.h>

#define NUM_LIGHTS 4

#define IS_ROUTER 1

#define LIGHT_INDEX 0

// Adjacent light indices, comma-separated. Must contain at least one entry.
#define ADJACENT 1

#define LIGHT_PIN 13

#define SENSOR_PIN A0

#define ADRU_RX_PIN 0

#define ADRU_TX_ADD_PIN 1

#define ADRU_TX_REMOVE_PIN 5

#define ESP_RX_ADD_PIN 2

#define ESP_RX_REMOVE_PIN 4

#define ESP_TX_PIN 3

#define NET_SSID "EE120_NET"
#define NET_PASS "ee120pass"
#define NET_PORT 4210
#define MAX_CLIENTS 8

static const byte adjacent[] = { ADJACENT };
static const byte adjacent_count = sizeof(adjacent) / sizeof(adjacent[0]);

#if IS_ROUTER

static byte numbers[NUM_LIGHTS];
static byte numbers_count = 0;

static WiFiServer server(NET_PORT);
static WiFiClient clients[MAX_CLIENTS];

static void addNumber(byte n) {
  for (byte i = 0; i < numbers_count; i++) {
    if (numbers[i] == n) return;
  }
  if (numbers_count < NUM_LIGHTS) {
    numbers[numbers_count++] = n;
  }
}

static void removeNumber(byte n) {
  for (byte i = 0; i < numbers_count; i++) {
    if (numbers[i] == n) {
      for (byte j = i; j + 1 < numbers_count; j++) numbers[j] = numbers[j + 1];
      numbers_count--;
      return;
    }
  }
}

static void broadcastList() {
  String msg = "LIST";
  for (byte i = 0; i < numbers_count; i++) {
    msg += ' ';
    msg += numbers[i];
  }
  msg += '\n';
  for (byte i = 0; i < MAX_CLIENTS; i++) {
    if (clients[i] && clients[i].connected()) {
      clients[i].print(msg);
    }
  }
}

static void handleCommand(const String& cmd) {
  if (cmd.startsWith("ADD ")) {
    addNumber((byte)cmd.substring(4).toInt());
    broadcastList();
  } else if (cmd.startsWith("REMOVE ")) {
    removeNumber((byte)cmd.substring(7).toInt());
    broadcastList();
  } else if (cmd == "LIST") {
    broadcastList();
  }
}

static void networkBegin() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(NET_SSID, NET_PASS);
  server.begin();
  server.setNoDelay(true);
}

static void networkUpdate() {
  if (server.hasClient()) {
    bool placed = false;
    for (byte i = 0; i < MAX_CLIENTS; i++) {
      if (!clients[i] || !clients[i].connected()) {
        if (clients[i]) clients[i].stop();
        clients[i] = server.accept();
        placed = true;
        break;
      }
    }
    if (!placed) {
      WiFiClient reject = server.accept();
      reject.stop();
    }
  }
  for (byte i = 0; i < MAX_CLIENTS; i++) {
    if (clients[i] && clients[i].connected() && clients[i].available()) {
      String line = clients[i].readStringUntil('\n');
      line.trim();
      if (line.length() > 0) handleCommand(line);
    }
  }
}

static void sendAdd(byte n) {
  addNumber(n);
  broadcastList();
}

static void sendRemove(byte n) {
  removeNumber(n);
  broadcastList();
}

#else  // !IS_ROUTER

static WiFiClient uplink;

static void networkBegin() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(NET_SSID, NET_PASS);
}

static void ensureUplink() {
  if (WiFi.status() != WL_CONNECTED) return;
  if (uplink.connected()) return;
  uplink.connect(WiFi.gatewayIP(), NET_PORT);
}

static void networkUpdate() {
  ensureUplink();
}

static void sendAdd(byte n) {
  ensureUplink();
  if (uplink.connected()) {
    uplink.print("ADD ");
    uplink.println(n);
  }
}

static void sendRemove(byte n) {
  ensureUplink();
  if (uplink.connected()) {
    uplink.print("REMOVE ");
    uplink.println(n);
  }
}

#endif

static bool prev_rx_add = false;
static bool prev_rx_remove = false;

void setup() {
  pinMode(ESP_RX_ADD_PIN, INPUT);
  pinMode(ESP_RX_REMOVE_PIN, INPUT);
  networkBegin();
}

void loop() {
  networkUpdate();

  bool rx_add = digitalRead(ESP_RX_ADD_PIN) == HIGH;
  bool rx_remove = digitalRead(ESP_RX_REMOVE_PIN) == HIGH;

  if (rx_add && !prev_rx_add) {
    for (byte i = 0; i < adjacent_count; i++) {
      sendAdd(adjacent[i]);
    }
  }
  if (rx_remove && !prev_rx_remove) {
    sendRemove(LIGHT_INDEX);
  }

  prev_rx_add = rx_add;
  prev_rx_remove = rx_remove;
}

