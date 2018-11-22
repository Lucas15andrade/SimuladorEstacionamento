#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>


// Set these to run example.
#define FIREBASE_HOST "jogodavelha-e0bcf.firebaseio.com"
#define FIREBASE_AUTH "KpY56HlZz8m163RfjNXQwporGiHZwFf6aEtEsaQo"
#define WIFI_SSID "Calvinho"
#define WIFI_PASSWORD "calvinho15#"

StaticJsonBuffer<200> jsonBuffer;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  while (!Serial) {
    ;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("loop");

  // get value
  /*
    Serial.print("Valor: ");
    Serial.println(Firebase.getString("jogo/"));
    delay(1000);
  */

  /*
     // set string value
    Firebase.pushFloat("jogo", 2.1);
    // handle error
    if (Firebase.failed()) {
        Serial.print("setting /message failed:");
        Serial.println(Firebase.error());
        return;
    }
  */

  //Setar um novo objeto ao firebase
  /*
    JsonObject& obj = jsonBuffer.createObject();
    obj["coluna"] = 3;
    obj["linha"] = 0;
    obj["valor"] = false;

    Firebase.push("jogo", obj);
    if (Firebase.failed()) {
      Serial.print("setting /message failed:");
      Serial.println(Firebase.error());
      return;
    }
  */

  char valorRecebido;
  int distancia;


  if (Serial.available() > 0) {
    //valorRecebido = (byte)Serial.read();
    distancia = Serial.parseInt();
    Serial.print("valor recebido: ");
    Serial.println(distancia);
    enviarDistanciaFirebase(distancia);
  }


  //enviarDistanciaFirebase();

  //delay(5000);
  delay(1000);
}


void enviarDistanciaFirebase(int distancia) {

  /*
    JsonObject& obj = jsonBuffer.createObject();
    obj["coluna"] = 3;
    obj["linha"] = 0;
    obj["valor"] = false;

    Firebase.push("jogo", obj);
    if (Firebase.failed()) {
      Serial.print("setting /message failed:");
      Serial.println(Firebase.error());
      return;
    }
  */

  Firebase.pushInt("distancia", distancia);
  if (Firebase.failed()) {
    Serial.print("setting /message failed:");
    Serial.println(Firebase.error());
    return;
  }
}
