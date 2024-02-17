// #include "Wire.h"
// #include "I2Cdev.h"
// #include "MPU6050_6Axis_MotionApps612.h"

// /**
//  * # Wiring
//  * | MPU6050 | Arduino   |
//  * | ------- | --------- |
//  * | GND     | GND       |
//  * | VCC     | 3V3       |
//  * | SCL     | SCL       |
//  * | SDA     | SDA       |
//  * | ADD     | see below |
//  * 
//  * ## ADDR pin configuration:
//  * 
//  * ADDR low = 0x68 (default)
//  * ADDR high = 0x69
//  */

// // initialize the two MPUs with their I2C ID
// MPU6050 mpu68(0x68);
// MPU6050 mpu69(0x69);

// uint8_t error_code = 0U;      // return status after each device operation (0 = success, !0 = error)

// void setup() {
//   Wire.begin(7,8);

//   // it seems that we cannot use too high clock rate, due to the long I2C wire.
//   // cannot flash firmware if signal integrity is too bad.
//   // Wire.setClock(400000);
//   Wire.setClock(100000);

//   Serial.begin(115200);
  
//   // initialize device
//   Serial.print("{\"key\": \"/log\", \"value\": \"Initializing device 0x68...\", \"level\": \"DEBUG\"}\n");
//   mpu68.initialize();
//   error_code = mpu68.dmpInitialize();
  
//   // 1 = initial memory load failed
//   // 2 = DMP configuration updates failed
//   // (if it's going to break, usually the code will be 1)
//   if (error_code == 1U) {
//     Serial.print("{\"key\": \"/log\", \"value\": \"device 0x68 initialization failed: initial memory load failed.\", \"level\": \"ERROR\"}\n");
//     while (1) {}
//   }
//   if (error_code == 2U) {
//     Serial.print("{\"key\": \"/log\", \"value\": \"device 0x68 initialization failed: DMP configuration updates failed.\", \"level\": \"ERROR\"}\n");
//     while (1) {}
//   }
  
//   Serial.print("{\"key\": \"/log\", \"value\": \"Initializing device 0x69...\", \"level\": \"DEBUG\"}\n");
//   mpu69.initialize();
//   error_code = mpu69.dmpInitialize();

//   // 1 = initial memory load failed
//   // 2 = DMP configuration updates failed
//   // (if it's going to break, usually the code will be 1)
//   if (error_code == 1U) {
//     Serial.print("{\"key\": \"/log\", \"value\": \"device 0x68 initialization failed: initial memory load failed.\", \"level\": \"ERROR\"}\n");
//     while (1) {}
//   }
//   if (error_code == 2U) {
//     Serial.print("{\"key\": \"/log\", \"value\": \"device 0x68 initialization failed: DMP configuration updates failed.\", \"level\": \"ERROR\"}\n");
//     while (1) {}
//   }

//   // verify connection
//   if (!mpu68.testConnection()) {
//     Serial.print("{\"key\": \"/log\", \"value\": \"device 0x68 connection failed.\", \"level\": \"ERROR\"}\n"); 
//   }
//   if (!mpu69.testConnection()) {
//     Serial.print("{\"key\": \"/log\", \"value\": \"device 0x69 connection failed.\", \"level\": \"ERROR\"}\n");
//   }

//   // supply your own gyro offsets here, scaled for min sensitivity
//   mpu68.setXGyroOffset(0);
//   mpu68.setYGyroOffset(0);
//   mpu68.setZGyroOffset(0);
//   mpu68.setXAccelOffset(0);
//   mpu68.setYAccelOffset(0);
//   mpu68.setZAccelOffset(0);
  
//   mpu69.setXGyroOffset(0);
//   mpu69.setYGyroOffset(0);
//   mpu69.setZGyroOffset(0);
//   mpu69.setXAccelOffset(0);
//   mpu69.setYAccelOffset(0);
//   mpu69.setZAccelOffset(0);

  
//   // Calibration Time: generate offsets and calibrate our MPU6050
//   mpu68.CalibrateAccel(6);
//   mpu68.CalibrateGyro(6);
  
//   mpu69.CalibrateAccel(6);
//   mpu69.CalibrateGyro(6);

//   // calibration procedure will dump garbage on serial, we use a newline to fence it
//   Serial.print("\n");
  
//   // turn on the DMP, now that it's ready
//   Serial.print("{\"key\": \"/log\", \"value\": \"Enabling DMP...\", \"level\": \"DEBUG\"}\n");
//   mpu68.setDMPEnabled(true);
//   mpu69.setDMPEnabled(true);
//   Serial.print("{\"key\": \"/log\", \"value\": \"Device ready.\", \"level\": \"INFO\"}\n");
// }

// void loop() {
//   // Get the Latest packet 
//   uint8_t fifo_buffer68[64]; // FIFO storage buffer
//   if (!mpu68.dmpGetCurrentFIFOPacket(fifo_buffer68)) {
//     return;
//   }

//   uint8_t fifo_buffer69[64]; // FIFO storage buffer
//   if (!mpu69.dmpGetCurrentFIFOPacket(fifo_buffer69)) {
//     return;
//   }
  
//   // orientation/motion vars
//   Quaternion q68;           // [w, x, y, z]         quaternion container
//   Quaternion q69;           // [w, x, y, z]         quaternion container

//   mpu68.dmpGetQuaternion(&q68, fifo_buffer68);
//   mpu69.dmpGetQuaternion(&q69, fifo_buffer69);
  
//   Serial.print("{\"key\": \"/joint/0\", \"value\": [");
//   Serial.print(q68.w);Serial.print(", ");
//   Serial.print(q68.x);Serial.print(", ");
//   Serial.print(q68.y);Serial.print(", ");
//   Serial.print(q68.z);
//   Serial.print("]}\n");
  
//   Serial.print("{\"key\": \"/joint/1\", \"value\": [");
//   Serial.print(q69.w);Serial.print(", ");
//   Serial.print(q69.x);Serial.print(", ");
//   Serial.print(q69.y);Serial.print(", ");
//   Serial.print(q69.z);
//   Serial.print("]}\n");
// }
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps612.h"
#include "ArduinoJson.h"
#include <PubSubClient.h>
#include <WiFi.h>


//nhatlv Setup wifi - mqtt 

//nhatlv
#define ssid "Hoang Hiep"
#define password "12345678"
// #define mqtt_server "test.mosquitto.org"// local broker
// const uint16_t mqtt_port = 1883; //Port của MQTT
#define DASHBOARD_MQTT_SERVER               "172.20.10.5"
#define DASHBOARD_MQTT_PORT                 1883
#define DASHBOARD_MQTT_USER                 "nhatlv"
#define DASHBOARD_MQTT_PASSWORD             "nhatlv2001"
#define DASHBOARD_TOPIC_TEST                "system/hiep"


// #define topic1 "tempp-humm_fix"
WiFiClient espClient;
PubSubClient client(espClient);

// nhatlv
void DashBoard_ConnectToBroker() {
    while (!client.connected()) {
        String clientId = "ESP8266";
        clientId += String(random(0xffff), HEX);
        if (client.connect(clientId.c_str(), DASHBOARD_MQTT_USER, DASHBOARD_MQTT_PASSWORD)) {
            Serial.println("Đã kết nối đến mqtt server:");
            delay(3000);

        } else {

            Serial.println("Đã bị mất kết nối đến mqtt server:");
        }
    }
}


void setup_wifi()
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  // in ra thông báo đã kết nối và địa chỉ IP của ESP8266
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Thong bao tu topic: ");
  // ...
}
//Hàm reconnect thực hiện kết nối lại khi mất kết nối với MQTT Broker
void reconnect()
{
  while (!client.connected()) 
  {
//    String clientId = "broker.hivemq.com";
    if (client.connect("test.mosquitto.org")) //kết nối vào broker
    {
      Serial.println("Đã kết nối lại đến mqtt server:");
    }
    else
    {
      Serial.print("Lỗi:, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}






//
/**
 * # Wiring
 * | MPU6050 | Arduino   |
 * | ------- | --------- |
 * | GND     | GND       |
 * | VCC     | 3V3       |
 * | SCL     | SCL       |
 * | SDA     | SDA       |
 * | ADD     | see below |
 * 
 * ## ADDR pin configuration:
 * 
 * ADDR low = 0x68 (default)
 * ADDR high = 0x69
 */

// initialize the two MPUs with their I2C ID
MPU6050 mpu68(0x68);
MPU6050 mpu69(0x69);

uint8_t error_code = 0U;      // return status after each device operation (0 = success, !0 = error)

void setup() {
  Wire.begin(7,8);

  //nhatlv
  setup_wifi();  
   client.setServer(DASHBOARD_MQTT_SERVER, DASHBOARD_MQTT_PORT);
   client.setCallback(callback);
  DashBoard_ConnectToBroker();

    //nhatlv
  // it seems that we cannot use too high clock rate, due to the long I2C wire.
  // cannot flash firmware if signal integrity is too bad.
  // Wire.setClock(400000);
  Wire.setClock(100000);

  Serial.begin(115200);
  
  // initialize device
  Serial.print("{\"key\": \"/log\", \"value\": \"Initializing device 0x68...\", \"level\": \"DEBUG\"}\n");
  mpu68.initialize();
  error_code = mpu68.dmpInitialize();
  
  // 1 = initial memory load failed
  // 2 = DMP configuration updates failed
  // (if it's going to break, usually the code will be 1)
  if (error_code == 1U) {
    Serial.print("{\"key\": \"/log\", \"value\": \"device 0x68 initialization failed: initial memory load failed.\", \"level\": \"ERROR\"}\n");
    while (1) {}
  }
  if (error_code == 2U) {
    Serial.print("{\"key\": \"/log\", \"value\": \"device 0x68 initialization failed: DMP configuration updates failed.\", \"level\": \"ERROR\"}\n");
    while (1) {}
  }
  
  Serial.print("{\"key\": \"/log\", \"value\": \"Initializing device 0x69...\", \"level\": \"DEBUG\"}\n");
  mpu69.initialize();
  error_code = mpu69.dmpInitialize();

  // 1 = initial memory load failed
  // 2 = DMP configuration updates failed
  // (if it's going to break, usually the code will be 1)
  if (error_code == 1U) {
    Serial.print("{\"key\": \"/log\", \"value\": \"device 0x68 initialization failed: initial memory load failed.\", \"level\": \"ERROR\"}\n");
    while (1) {}
  }
  if (error_code == 2U) {
    Serial.print("{\"key\": \"/log\", \"value\": \"device 0x68 initialization failed: DMP configuration updates failed.\", \"level\": \"ERROR\"}\n");
    while (1) {}
  }

  // verify connection
  if (!mpu68.testConnection()) {
    Serial.print("{\"key\": \"/log\", \"value\": \"device 0x68 connection failed.\", \"level\": \"ERROR\"}\n"); 
  }
  if (!mpu69.testConnection()) {
    Serial.print("{\"key\": \"/log\", \"value\": \"device 0x69 connection failed.\", \"level\": \"ERROR\"}\n");
  }

  // supply your own gyro offsets here, scaled for min sensitivity
  mpu68.setXGyroOffset(0);
  mpu68.setYGyroOffset(0);
  mpu68.setZGyroOffset(0);
  mpu68.setXAccelOffset(0);
  mpu68.setYAccelOffset(0);
  mpu68.setZAccelOffset(0);
  
  mpu69.setXGyroOffset(0);
  mpu69.setYGyroOffset(0);
  mpu69.setZGyroOffset(0);
  mpu69.setXAccelOffset(0);
  mpu69.setYAccelOffset(0);
  mpu69.setZAccelOffset(0);

  
  // Calibration Time: generate offsets and calibrate our MPU6050
  mpu68.CalibrateAccel(6);
  mpu68.CalibrateGyro(6);
  
  mpu69.CalibrateAccel(6);
  mpu69.CalibrateGyro(6);

  // calibration procedure will dump garbage on serial, we use a newline to fence it
  Serial.print("\n");
  
  // turn on the DMP, now that it's ready
  Serial.print("{\"key\": \"/log\", \"value\": \"Enabling DMP...\", \"level\": \"DEBUG\"}\n");
  mpu68.setDMPEnabled(true);
  mpu69.setDMPEnabled(true);
  Serial.print("{\"key\": \"/log\", \"value\": \"Device ready.\", \"level\": \"INFO\"}\n");

  
}

void loop() {
  // Get the Latest packet 
  uint8_t fifo_buffer68[64]; // FIFO storage buffer
  if (!mpu68.dmpGetCurrentFIFOPacket(fifo_buffer68)) {
    return;
  }

  uint8_t fifo_buffer69[64]; // FIFO storage buffer
  if (!mpu69.dmpGetCurrentFIFOPacket(fifo_buffer69)) {
    return;
  }
  
  // orientation/motion vars
  Quaternion q68;           // [w, x, y, z]         quaternion container
  Quaternion q69;           // [w, x, y, z]         quaternion container

  mpu68.dmpGetQuaternion(&q68, fifo_buffer68);
  mpu69.dmpGetQuaternion(&q69, fifo_buffer69);
  
  // Serial.print("{\"key\": \"/joint/0\", \"value\": [");
  // Serial.print(q68.w);Serial.print(", ");
  // Serial.print(q68.x);Serial.print(", ");
  // Serial.print(q68.y);Serial.print(", ");
  // Serial.print(q68.z);
  // Serial.print("]}\n");
  
  // Serial.print("{\"key\": \"/joint/1\", \"value\": [");
  // Serial.print(q69.w);Serial.print(", ");
  // Serial.print(q69.x);Serial.print(", ");
  // Serial.print(q69.y);Serial.print(", ");
  // Serial.print(q69.z);
  // Serial.print("]}\n");

  // nhatlv
    char buffer[256];  // Đảm bảo buffer đủ lớn để chứa chuỗi JSON

  // Tạo một đối tượng JSON
  DynamicJsonDocument doc(256);  // Đảm bảo kích thước đủ lớn cho JSON

  // Đóng gói dữ liệu từ mpu68
  doc["key"] = "/joint/0";
  doc["value"][0] = q68.w;
  doc["value"][1] = q68.x;
  doc["value"][2] = q68.y;
  doc["value"][3] = q68.z;

  // Chuyển JSON thành chuỗi
  serializeJson(doc, buffer);
  client.publish(DASHBOARD_TOPIC_TEST, buffer,false);   
  // In ra Serial (để kiểm tra)
  Serial.println(buffer);

  // Đóng gói dữ liệu từ mpu69
  doc.clear();  // Xóa dữ liệu cũ
  doc["key"] = "/joint/1";
  doc["value"][0] = q69.w;
  doc["value"][1] = q69.x;
  doc["value"][2] = q69.y;
  doc["value"][3] = q69.z;

  // Chuyển JSON thành chuỗi
  serializeJson(doc, buffer);
  client.publish(DASHBOARD_TOPIC_TEST, buffer,false);   

  // In ra Serial (để kiểm tra)
  Serial.println(buffer);
  //


}

