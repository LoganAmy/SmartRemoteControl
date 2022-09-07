#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <Servo.h>

/* 修改为你的WIFI SSID和密码 */
#define WIFI_SSID "SSID"
#define WIFI_PASSWD "PASSWD"

// mdns名称
#define MDNS_NAME "youtoo-switch"

// 舵机控制管脚
#define SERVO_PIN D5
// 指示灯
#define LED_PIN D4

ESP8266WebServer server(80);
Servo myservo;

/**
 * @brief WiFi连接
 */
void wifiInit() {
  // WIFI未连接时，LED常亮
  digitalWrite(LED_PIN, LOW);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWD);
  Serial.println("");

  // 等待WiFi连接
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // WIFI连接后，关闭LED
  digitalWrite(LED_PIN, HIGH);
}

/**
 * @brief 触发开关
 */
void handleRoot() {
  digitalWrite(LED_PIN, LOW);
  server.send(200, "text/plain", "Hello from youtoo-switch!");
  delay(10);
  digitalWrite(LED_PIN, HIGH);

  myservo.write(45);
  delay(500);
  myservo.write(90);

  Serial.println("Switch triggered!");
}

void handleNotFound() {
  server.send(404, "text/plain", "404 Not Found\r\n");
}

/**
 * @brief 配置web服务
 */
void serverConfig() {
  if (MDNS.begin(MDNS_NAME)) {
    Serial.println("MDNS responder started");
  }
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void setup(void) {
  // 设置指示灯
  pinMode(LED_PIN, OUTPUT);
  // 设置舵机控制
  myservo.attach(SERVO_PIN);
  // 打开串口，调试信息打印
  Serial.begin(115200);
  //  WIFI连接
  wifiInit();
  // 配置Web服务
  serverConfig();
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}
