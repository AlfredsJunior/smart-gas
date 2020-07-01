
#include <ESP8266WiFi.h>                                                // esp8266 library
//#include <FirebaseESP8266.h>// firebase library
#include <DHT.h>
#include <FirebaseArduino.h>
//#include <Adafruit_Sensor.h>


#include "pitches.h"

#define FIREBASE_HOST "gas-leak-9383f.firebaseio.com"                         // the project name address from firebase id
#define FIREBASE_AUTH "mMneR7EUZFDC5Aw5TXaTXUJT77j5eJq8fM70eBRb"                    // the secret key generated from firebase
#define WIFI_SSID "HTC"                                          // input your home or public wifi name 
#define WIFI_PASSWORD "1234567890"                                    //password of wifi ssid
#define DHTPIN 2    // what digital pin we're connected to
#define DHTTYPE DHT11
#define LED_STATUS 2
float h,t,f ; 

String fireStatus = "";                                                     // led status received from firebase
String ledStatus = ""; 
int led = 13;  
int smokeA0 = 16;
// Your threshold value
int sensorThres = 400;
int g;

const int digitalPin=7;  //the D0 attach to pin7
int Astate=0;
boolean  Dstate=0;
DHT dht(DHTPIN, DHTTYPE);
//const int DHT11_PIN= 8;//Humiture sensor attach to pin7

/*******************************************/
const int buzzerPin = 5;//the buzzer pin attach to
int fre;//set the variable to store the frequence value
/*******************************************/
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};



 
void setup() {
  Serial.begin(9600);
  pinMode(smokeA0, INPUT);
 // pinMode (digitalPin,INPUT);
   pinMode(buzzerPin,OUTPUT);
 // pinMode(LED_BUILTIN, OUTPUT);      
  pinMode(led, OUTPUT);   
   dht.begin();              
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                      //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                                      //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                                       // connect to firebase
//  Firebase.setString("LED_STATUS", '0');                                          //send initial string of led status
}

void loop() {
  collect();
  
  // set value 
 
}
void fireerror(){
 // handle error 
 if (Firebase.failed()) { 
     Serial.print("setting /number failed:"); 
     Serial.println(Firebase.error());   
     return; 
 } 
  }

void collect(){
  /*mq2
   Astate=analogRead(analogPin);//read the value of A0
  Serial.println(Astate); //peint 
  Dstate=digitalRead(digitalPin);//read the value of D0
  Serial.println(Dstate);//print
  */
  g = analogRead(smokeA0);

  Serial.print("Pin A0: ");
  Serial.println(g);
  // Checks if it has reached the threshold value

  delay(100);
 
  //end mq2

   // READ dht
     
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  h= dht.readHumidity();
  // Read temperature as Celsius (the default)
t= dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
 f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");
  
   delay(2000);
   //Firebase.setFloat("t", t); 
   updateReading(t,h,g);
   if( g>sensorThres || t>35 || h>75 ) //if the value of D0 is HIGH
  {
   alert();

  }

  }
  void alert() {
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(buzzerPin, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(5);
  }
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  //delay(1000); 
  
}
void updateReading(float t,float h ,int g){
 // String fireTemp = String(t) + String("°C");  
  Firebase.setFloat("Temperature", t); 
 //fireerror();
 // String fireHumid = String(h) + String("%");  
//fireerror();
Firebase.setFloat("Humidity", h);                                  //setup path and send readings
 //Firebase.setString("Temperature", fireTemp);  
//   Firebase.setInt("t",t);                     //Here the varialbe"FB1","FB2","FB3" and "FB4" needs to be the one which is used in our Firebase and MIT App Inventor
 //Firebase.pushFloat("h", h); 
//fireerror();
//Firebase.setInt("h",h); fireerror();
 //String state= String(g)   ;
Firebase.setInt("mq2",g); 
fireerror();
  }
  
