#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <stdio.h>

#define OUT 2//输出控制端口

 // Update these with values suitable for your network.

//const char* ssid = "SDE-SDA";//WIFI名
//const char* password = "sdesda778899";//密码

const char* ssid = "DDD";//WIFI名
const char* password = "598556898";//密码

// const char* ssid = "ChinaNet-CGmk";//WIFI名
// const char* password = "3apetj73";//密码
 
 const char* mqtt_server = "183.230.40.39";//onenet服务器
 
 const char* DEV_ID = "1178041968"; //设备ID
 const char* DEV_PRO_ID = "624813"; //产品ID
 const char* DEV_KEY = "BH7FdQ0pYkKvVTK1inTN2zEKlMM="; //API KEY
//7mJdKq1rwNbNX7a3V=Sf96cR6Ak=
//evgMXJnnq3wJtBAIJKqYd0R5P5XLqLeIcR+bliGpnv0=

 WiFiClient espClient;
 PubSubClient client(espClient);
 int ii = 0;
void sayToClient();

 //char DataStreams[] = "LIGHT";                // 数据流 LIGHT -- Stream "LIGHT"
 String comdata = "";//字符串函数
 long lastMsg = 0;
 char msg[50];
 int value = 0;
 char tmp[50];
 char d[3];
 



//wifi初始化
 void setup_wifi() {

   delay(10);
   // We start by connecting to a WiFi network
   Serial.println();
   Serial.print("Connecting to ");
   Serial.println(ssid);

   WiFi.begin(ssid, password);

   while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
   }

   randomSeed(micros());

   Serial.println("");
   Serial.println("WiFi connected");
   Serial.println("IP address: ");
   Serial.println(WiFi.localIP());
 }

 void callback(char* topic, byte* payload, unsigned int length) {
//   Serial.print("Message arrived [");
//   Serial.print(topic);
//   Serial.print("] ");
   for (int i = 0; i < length; i++) {
     Serial.print((char)payload[i]);
   }
  digitalWrite(OUT,LOW);//上电打开输出
  delay(100); 
  digitalWrite(OUT,HIGH);//上电打开输出
//   Serial.println();
/*
   // Switch on the LED if an 1 was received as first character
   if ((char)payload[0] == '1') {
     digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
     digitalWrite(OUT, LOW); 
     delay(1000);
     digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
     digitalWrite(OUT, HIGH);
     // but actually the LED is on; this is because
     // it is acive low on the ESP-01)
   } else {
     digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
     digitalWrite(OUT, HIGH);
   }*/

 }

 void reconnect() {
   // Loop until we're reconnected
   while (!client.connected()) {
//     Serial.print("Attempting MQTT connection...");
     if (client.connect(DEV_ID,DEV_PRO_ID,DEV_KEY)) {
//       Serial.println("connected");
       delay(5);
     } else {
//       Serial.print("failed, rc=");
 //      Serial.print(client.state());
//       Serial.println(" try again in 5 seconds");
       // Wait 5 seconds before retrying
       delay(5000);
     }
   }
 }

void sayToClient()  //将串口数据上报
  {
    snprintf(tmp,sizeof(tmp),"%S",comdata.c_str()); 
    uint16_t streamLen= strlen(tmp);
    
    d[0]='\x03';
    d[1] = (streamLen >> 8);
    d[2] = (streamLen & 0xFF);
    snprintf(msg,sizeof(msg),"%c%c%c%s",d[0],d[1],d[2],tmp);
    client.publish("$dp", (uint8_t*)msg,streamLen+3,false);

  }

 
 void setup() {
   pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
   pinMode(OUT,OUTPUT);//设置输出
  digitalWrite(OUT,HIGH);//上电打开输出
   Serial.begin(115200);
   setup_wifi();
   client.setServer(mqtt_server, 6002);
   client.connect(DEV_ID,DEV_PRO_ID,DEV_KEY);
   client.setCallback(callback);
   ii=0;
 }

 void loop() {
   if (!client.connected()) {
     reconnect();
   }
   client.loop();
   {

     delay(1000);


       while (Serial.available() > 0)//循环串口是否有数据
    {
        comdata += char(Serial.read());  //每次读一个char字符，并相加
        delay(2);
         ESP.wdtFeed(); 
    }
    if (comdata.length() > 0)
    {
//        Serial.println(comdata); //打印接收到的字符

         sayToClient(); 
         comdata = "";//缓存清零        
    }
 
        delay(1000);
   }
   
 }
