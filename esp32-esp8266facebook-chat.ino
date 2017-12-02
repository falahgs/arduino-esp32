

//#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFi.h>
// Update these with values suitable for your network.

const char* ssid = "SSID"; // change to your home wifi name
const char* password = "PASSWORD"; //  change to your home wifi password
const char* mqtt_server = "io.reteiot.com"; // MQTT broker Name 

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

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
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == 'o' && (char)payload[1]== 'n') { // Here we are comparing if ON command is coming from server. 
      
  //  digitalWrite(D7, HIGH);
    digitalWrite(2, HIGH);
   
  } else {
   // digitalWrite(D7, LOW); // else turn off led 
    digitalWrite(2, LOW);
    
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "BoardID";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
       digitalWrite(2, LOW);
      client.subscribe("YourEmail/light"); //  chnage your email id , you can also change facebook-led and keep what ever device name here.
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(2, OUTPUT);     // on board led for mqtt status indication.
//  pinMode(D7, OUTPUT); // our Buzzer is connected to D7
  pinMode(2, OUTPUT); // lED
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  digitalWrite(2, HIGH);
  digitalWrite(2, LOW);
}

void loop() {

  if (!client.connected()) {
     digitalWrite(2, HIGH);
    reconnect();
  }
  client.loop();

}
