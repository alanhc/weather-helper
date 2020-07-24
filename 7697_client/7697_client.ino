#include <DHT.h>
#include <LRTC.h>
#include <LWiFi.h>
DHT dht21_p2(2, DHT21);
char ssid[] = "";
char pass[] = "";
char server[] = "";
void printWifiStatus();
int status = WL_IDLE_STATUS;
WiFiClient client;
String jsonStr = "{\"t\":\"test\",\"tmp\":0,\"h\":0}";
int interval = 30*60*1000;
unsigned long past;
void setup()
{

  Serial.begin(9600);
  dht21_p2.begin();
  LRTC.begin();

  // set the time to 2017/10/2 16:10:30
  //LRTC.set(2020, 7, 23, 15, 03, 30);
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
  }
  Serial.println("Connected to wifi");
  printWifiStatus();


}
char buffer[64];

void loop()
{
  //Serial.print("溫度:");
  double tmp = dht21_p2.readTemperature();
  double h = dht21_p2.readHumidity();
  //Serial.println(tmp);
  //Serial.print("濕度:");
  //Serial.println(h);
  LRTC.get();
  sprintf(buffer, "%ld/%ld/%ld %.2ld:%.2ld:%.2ld",
          LRTC.year(), LRTC.month(), LRTC.day(), LRTC.hour(), LRTC.minute(), LRTC.second());

  //Serial.println(buffer);

  if (millis()-past >  interval ) {
    Serial.println(buffer);
    String data = "";
    data = "{\"t\":\"" + String(buffer) + "\",\"tmp\":" + String(tmp) + ",\"h\":" + String(h) + "}";
    httpSend(data);
    past = millis();
  }

}
void httpSend(String jsonStr) {
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 6000)) {
    //    Serial.println("connected");　　　
    Serial.println("connected");
    client.println("POST /post_json HTTP/1.1");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(jsonStr.length());
    client.println();
    client.print(jsonStr);

  } else {
    Serial.println("connection failed");
  }
}
void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
