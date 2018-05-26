//© Clément Burellier, David Tran 2018
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FirebaseArduino.h>


#define FIREBASE_HOST "humiditysensor-buket.firebaseio.com"
#define FIREBASE_AUTH "kwzSTxm0pH4W9SaS2IkLNFzAA3bfMaaowiLlWzNo"

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
 
  Serial.println("HTTP server started");
  //Connexion à firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

}


void loop(void)
{
  // pin de signal
  int in1 = 14;

  int arrosage;
  int minimum = Firebase.getInt("minimum");
  int maximum = Firebase.getInt("maximum");
  int automatic = Firebase.getInt("automatic");
  int en_cours = Firebase.getInt("en_cours");
  
  int humidity = analogRead(0);
  pinMode(A0, INPUT);
  pinMode(in1, OUTPUT);

    // set value
  Firebase.setInt("humidity", humidity);

  if (automatic == 1) { //Mode automatique
    if (humidity <= minimum) //Si taux humidité <= mini, arrosage
    {
    while (humidity <= maximum) {
       digitalWrite(in1, HIGH);
       Serial.println("Arrosage"); 
       Firebase.setInt("en_cours", 1);
       //Valeur de test : simulation arrosage
       //humidity = humidity +1;
       humidity = analogRead(0);
       Firebase.setInt("humidity", humidity);
       
       Serial.println(humidity);
      }
       digitalWrite(in1, LOW);
       Firebase.setInt("en_cours", 0);
      }
  } else { //Mode manuel
    if (en_cours == 1) {
       digitalWrite(in1, HIGH); 
    } else {
      digitalWrite(in1, LOW);
    }
  }

 // handle error
 if (Firebase.failed()) {
     Serial.print("setting /humidity failed:");
     Serial.println(Firebase.error());  
     return;
 }
 delay(1000);

 Serial.print("pushed: /humidity/");
 Serial.println(humidity);

 server.handleClient();
}




