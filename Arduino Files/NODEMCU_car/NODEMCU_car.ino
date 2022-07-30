

#include <ESP8266WiFi.h>

const int trigPin = 12;
const int echoPin = 14;
const int trigPin2 = 13;
const int echoPin2 = 15;

//define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034



long duration;
float distanceCm, data_upload;


void setup()
{

  Serial.begin(115200); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
   pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input 

  Serial.println();

  WiFi.begin("Speed", "3357799511"); //WifiCredentials

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}



const char* host = "maker.ifttt.com";





void loop()
{
  delay(1000);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_VELOCITY/2;

  if(distanceCm<50)
  {
    data_upload=10;
  }
  else
  {
    data_upload=0;
  }

  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin2, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_VELOCITY/2;

  if(distanceCm<50)
  {
    data_upload=data_upload+1;
  }
  else
  {
    data_upload=data_upload+0;
  }

  
  // Prints the distance on the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);

  
  delay(1000);
  
  WiFiClient client;

  Serial.printf("\n[Connecting to %s ... ", host);
  if (client.connect(host, 80))
  {
    Serial.println("connected]");

    Serial.println("[Sending a request]");
    client.print(String("GET /trigger/parking/with/key/bj2U33CLm4TGvTePPE2xf_?value1=") + data_upload + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n" +
                 "\r\n"
                );

    Serial.println("[Response:]");
    while (client.connected() || client.available())
    {
      if (client.available())
      {
        String line = client.readStringUntil('\n');
        Serial.println(line);
      }
    }
    client.stop();
    Serial.println("\n[Disconnected]");
  }
  else
  {
    Serial.println("connection failed!]");
    client.stop();
  }


  
  delay(1000);
}
