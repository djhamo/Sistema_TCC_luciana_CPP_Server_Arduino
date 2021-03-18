#include<SoftwareSerial.h>

SoftwareSerial SUART(2, 3);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); //enable Serial Monitor
}

void loop() {
  SUART.begin(115200); //enable SUART Port
  long trandNumber = random(106, 130);
  double irandNumber = random(100, 200);
  double icor = irandNumber / trandNumber;
  Serial.println("Serial Transmit"); 
  Serial.println(trandNumber); 
  SUART.println(trandNumber); 
  Serial.println(icor); 
  SUART.println(icor); 
  delay(1000);
  SUART.end(); //enable SUART Port
}
