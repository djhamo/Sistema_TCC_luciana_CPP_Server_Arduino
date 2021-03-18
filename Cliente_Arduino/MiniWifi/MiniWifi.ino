#include <SoftwareSerial.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID "Amarelinhos"
#define STAPSK  "Luthor$Domi!"
#endif

#define TAMLEITURA  30

const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = "192.168.0.40";
const uint16_t port = 18000;

int cont_leitura_serial = 0;
int pos_b = 0;

String buffer_serial[TAMLEITURA + 1];

SoftwareSerial SUART(4, 5);

void setup() {
  Serial.begin(115200);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  cont_leitura_serial = 0;
  pos_b = 0;

  //Serial de Comunicacao
  SUART.begin(115200);
}

bool leitura_serial(String *msg) {
  String result = "";
  bool saida = false;
  byte n = SUART.available(); //n != 0 means a character has arrived
  if (n != 0) {
    result = "tensao=";
    char x = SUART.read();
    while(x != '\n') {
      result += x;
      x = SUART.read();  
    }
    result += "&corrente=";
    x = SUART.read();
    while(x != '\n') {
      result += x;
      x = SUART.read();  
    }
    saida = true;
  }
  //Serial.println(result);        //show character on Serial Monitor
  (* msg) = result;
  return saida;
}

void wifi_trasmit(const String msg) {
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
    HTTPClient http;    //Declare object of class HTTPClient
 
    http.begin("http://192.168.0.40:18000/post");      //Specify request destination
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");  //Specify content-type header

    
    // Data to send with HTTP POST
    //String httpRequestData = msg;

    // Send HTTP POST request
    int httpCode = http.POST(msg);   //Send the request
    String payload = http.getString();                  //Get the response payload
 
    Serial.println(httpCode);   //Print HTTP return code
    Serial.println(payload);    //Print request response payload
 
    http.end();  //Close connection
 
  } else {
 
    Serial.println("Error in WiFi connection");
 
  }
  
}

void loop() {
  String msg;
  bool l_serial = leitura_serial(&msg);
  if (l_serial) {
    Serial.println("Leitura Serial");
    Serial.println(pos_b);  
    Serial.println(msg);  
    buffer_serial[pos_b] = msg;
    pos_b++;
  }
  
  if (cont_leitura_serial >= TAMLEITURA) {
    SUART.end();
    for (int c = 0; c < pos_b; c++) {
      Serial.println(c);
      String temp = "nome=mostrador_01&" + buffer_serial[c];
      Serial.println(temp);
      wifi_trasmit(temp);
      buffer_serial[c] = " ";  
    }
    pos_b = 0;
    cont_leitura_serial = 0;
    SUART.begin(115200);
  }
  cont_leitura_serial++;
  delay(1000);  //Send a request every 1 seconds
}
