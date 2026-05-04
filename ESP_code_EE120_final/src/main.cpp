#include <Arduino.h>
#include <ESP8266WiFi.h>

#define NUM_LIGHTS 4

#define IS_ROUTER 0

#define LIGHT_INDEX 2

// Adjacent light indices, comma-separated. Must contain at least one entry.
#define ADJACENT {1, 3}

#define ARDU_TX_ADD_PIN 12

#define ARDU_TX_REMOVE_PIN 5

#define ESP_RX_ADD_PIN 4

#define ESP_RX_REMOVE_PIN 16

#define ESP_TX_PIN 14

#define NET_SSID "EE120_NET"
#define NET_PASS "ee120pass"
#define NET_PORT 4210
#define MAX_CLIENTS 8

static const byte adjacent[] = ADJACENT;
static const byte adjacent_count = sizeof(adjacent) / sizeof(adjacent[0]);

static void updateSelfPin(const byte* list, byte count) {
  bool present = false;
  for (byte i = 0; i < count; i++) {
    if (list[i] == LIGHT_INDEX) { present = true; break; }
  }
  digitalWrite(ESP_TX_PIN, present ? HIGH : LOW);
}

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
  updateSelfPin(numbers, numbers_count);
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
static byte last_list[NUM_LIGHTS];
static byte last_list_count = 0;

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
  while (uplink.connected() && uplink.available()) {
    String line = uplink.readStringUntil('\n');
    line.trim();
    if (!line.startsWith("LIST")) continue;
    byte parsed[NUM_LIGHTS];
    byte count = 0;
    String rest = line.substring(4);
    rest.trim();
    while (rest.length() > 0 && count < NUM_LIGHTS) {
      int sp = rest.indexOf(' ');
      String tok = (sp < 0) ? rest : rest.substring(0, sp);
      parsed[count++] = (byte)tok.toInt();
      if (sp < 0) break;
      rest = rest.substring(sp + 1);
      rest.trim();
    }
    last_list_count = count;
    for (byte i = 0; i < count; i++) last_list[i] = parsed[i];
    updateSelfPin(parsed, count);
  }
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
static unsigned long last_debug_ms = 0;

static void debugPrint() {
  unsigned long now = millis();
  if (now - last_debug_ms < 500) return;
  last_debug_ms = now;
  Serial.print("RX_ADD=");
  Serial.print(digitalRead(ESP_RX_ADD_PIN));
  Serial.print(" RX_REMOVE=");
  Serial.print(digitalRead(ESP_RX_REMOVE_PIN));
  Serial.print(" LIST=[");
#if IS_ROUTER
  for (byte i = 0; i < numbers_count; i++) {
    if (i) Serial.print(',');
    Serial.print(numbers[i]);
  }
#else
  for (byte i = 0; i < last_list_count; i++) {
    if (i) Serial.print(',');
    Serial.print(last_list[i]);
  }
#endif
  Serial.println("]");
}

void setup() {
  Serial.begin(115200);
  pinMode(ESP_RX_ADD_PIN, INPUT);
  pinMode(ESP_RX_REMOVE_PIN, INPUT);
  pinMode(ESP_TX_PIN, OUTPUT);
  digitalWrite(ESP_TX_PIN, LOW);
  networkBegin();
}

void loop() {
  networkUpdate();
  debugPrint();

  bool rx_add = digitalRead(ESP_RX_ADD_PIN) == HIGH;

  if (rx_add && !prev_rx_add) {
    for (byte i = 0; i < adjacent_count; i++) {
      sendAdd(adjacent[i]);
    }
  }
  if (digitalRead(ESP_RX_REMOVE_PIN) == HIGH) {
    sendRemove(LIGHT_INDEX);
  }

  prev_rx_add = rx_add;
}

