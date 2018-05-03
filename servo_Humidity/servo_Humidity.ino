#include <Servo.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
//#include <FirebaseArduino.h>

//#define FIREBASE_HOST “com.example.davidt.humiditysensor”
//#define FIREBASE_AUTH “AIzaSyCAcsV6TYvrRlo8dns6Qgy8VugEQJ2mIYM”


Servo myServo; // instanciation du servo

int const potPin = A0;
int potVal;  // variable to read the value from the analog pin
int angle; 
String val;
const char* ssid = "#David"; // mettre ici le SSID de votre WiFi
const char* password = "pass1234"; // mettre le mot de passe de votre WiFi
ESP8266WebServer server(80);

void setup(void) 
{
  Serial.begin(115200);
  
  //initialisation du servo
  pinMode(A0, INPUT);
  pinMode(5, OUTPUT);

  WiFi.begin(ssid, password);
  
  Serial.println("connecting...");

  // on attend d'etre connecte au WiFi avant de continuer
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // on affiche l'adresse IP qui nous a été attribuee
  Serial.println("");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  //Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
  // on definit ce qui doit etre fait lorsque la route "/" est appelee  
  server.on("/bonjour", [](){
    // on recupere le parametre msg dans l'url
    String message=server.arg("msg");
    // on efface l'ecran
      myServo.attach(14);  // relier le servomoteur au port 9 
      myServo.write(0);
      myServo.write(90);
      Serial.println(analogRead(A0));
      val = analogRead(A0);

    // on repond au client
    
    server.send(200, "text/plain", val);
  });

  server.begin();
  Serial.println("HTTP server started");
}


void loop(void)
{
//  Serial.print("Humidité : ");
//  Serial.println(analogRead(A0));
//  delay(2000);
//  if (analogRead(A0) < 50) {
//    digitalWrite(7, HIGH);
//    digitalWrite(6, LOW);
//    digitalWrite(5, LOW);
////    potVal = analogRead(A0);
////    Serial.print("potVal: ");
////  Serial.print(potVal);
////
////  // scale the numbers from the pot
////  angle = map(potVal, 0, 1023, 0, 179);
////
////  // print out the angle for the servo motor
////  Serial.print(", angle: ");
////  Serial.println(angle);
//
//  // set the servo position
//  myServo.attach(14);  // relier le servomoteur au port 9 
//  myServo.write(90);
//
//  Serial.print("Angle  90");
//
//
//  // wait for the servo to get there
//  delay(1);
//
//// 
//
//  } else if (analogRead(A0) >= 50) {
//    digitalWrite(7, LOW);
//    digitalWrite(6, LOW);
//    digitalWrite(5, HIGH);
//
//   
//
//  // set the servo position
//  myServo.attach(14);  // relier le servomoteur au port 9 
//  myServo.write(0);
//
//    Serial.print("Angle 0");
//
//
//  // wait for the servo to get there
//  delay(1);
//
//  }

  server.handleClient();

}




