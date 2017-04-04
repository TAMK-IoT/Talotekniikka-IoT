#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1115 ads; //16bittinen adc (default gain 2/3 +/- 6,144V, 1bit = 0,1875mV

//Arduinon MAC-osoite
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xFE };
//IPAddress server(10,10,206,147);
IPAddress server;
EthernetClient ethClient;
PubSubClient client(ethClient);

//Muuttujat
int ardIDint;
int sensorIDint;
int laite=1;
int luku=0;
int kehystunnus;
float kerroin = 0.0001875; //AD-muuntimen muuntokerroin
byte DIP;
byte Dstate;
int osoite = 0;



//Yhdistys serveriin

void reconnect() {
  //Kun ei ole yhteyttä
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Yritys yhdistää
    //Clientin nimi
    if (client.connect(laite)) {
      Serial.println("connected");
      // Kun saatu yhteys, lähetetään ilmoitus
      //TOPIC/VIESTI
      char viesti[100];
      sprintf(viesti, "Mega %0d online", laite);
      client.publish("vastaus",viesti);     
      //Tilaus tarvittaviin topikkeihin
      client.subscribe("kysely");
      
      //Jos yhteys ei onnistu
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Delay uudelle yritykselle
      delay(5000);
    }
  }
}

//callback viestin lähetykselle
void callback(char* topic, byte* payload, unsigned int length) {
  //Tulosta tilattuihin topikkeihin tullut viesti
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
    for (int i=0;i<length;i++)
      {
        Serial.print((char)payload[i]);
      }
  Serial.println("");


  ardIDint = ((payload[5]-48)*10)+(payload[6]-48);
  sensorIDint = ((payload[7]-48)*10)+(payload[8]-48);
  kehystunnus = ((payload[0]-48)*10)+(payload[1]-48);
  char tempp[5];
  for(int i =0; i<5; i++)
    {
      tempp[i]=payload[i]-48;
    }



  Serial.println("");
  Serial.println("-----------------------------------");
  Serial.println("Debug");
  Serial.print("Kehystunnus: ");
  Serial.println(kehystunnus);
  Serial.print("Tunniste: ");
  for(int i = 2; i < 5; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println("");  
  Serial.print("Arduinon ID, jolta tieto pyydetaan: ");
  Serial.print(ardIDint);
  Serial.println("");
  Serial.print("Sensorin ID, jolta tieto pyydetaan: ");
  Serial.print(sensorIDint);
  Serial.println("");
  Serial.print("Dippikytkinten arvot: ");
  for(int i = 33; i > 29; i--) 
    {
      Serial.print(digitalRead(i)); //tulostetaan seriaalin dippien asennot
     }
  Serial.println("");
  Serial.print("Laitteen numero on: ");
  Serial.print(laite);
  Serial.println("");
  Serial.println("AD-muuntimen arvot");
  Serial.print("ADC 1: ");
  Serial.print(ads.readADC_SingleEnded(0));
  Serial.print(", muunnettuna ");
  Serial.print(ads.readADC_SingleEnded(0)*kerroin,3);
  Serial.print("V. ");
  Serial.print("Asteina: ");
  Serial.print(ads.readADC_SingleEnded(0)*kerroin*10,1);
  Serial.println("C.");
  Serial.print("ADC 2: ");
  Serial.print(ads.readADC_SingleEnded(1));
  Serial.print(", muunnettuna ");
  Serial.print(ads.readADC_SingleEnded(1)*kerroin,3);
  Serial.print("V. ");
  Serial.print("Kosteus % : ");
  Serial.print(((ads.readADC_SingleEnded(1)*kerroin)/5)*100,1);
  Serial.println("%.");
  Serial.print("ADC 3: ");
  Serial.print(ads.readADC_SingleEnded(2));
  Serial.print(", muunnettuna ");
  Serial.print(ads.readADC_SingleEnded(2)*kerroin,3);
  Serial.print("V. ");
  Serial.print("CO2: ");
  Serial.print(((ads.readADC_SingleEnded(2)*kerroin)/5)*2000,0);
  Serial.println("ppm.");
  Serial.println("-----------------------------------");
  Serial.println("");
  

 if (kehystunnus == 01)
  {
  if (laite==ardIDint)
    {
        //ADC kutsu
        luku=ads.readADC_SingleEnded(sensorIDint-1);
        char viesti[100];
        sprintf(viesti, "%d%d%d%d%d%d", tempp[0], tempp[1], tempp[2],tempp[3],tempp[4], luku );
        client.publish("vastaus",viesti);
     }
  }    
    
   
}
//Setup
void setup()
{
  //ADC:n alustus
   ads.begin();
   
  //Ethernetin alustus
  Ethernet.begin(mac);
   
  //Serialin alustus
  Serial.begin(57600);
 for(int i = 0; i < 4; i++) 
    {
      DIP = DIP | (digitalRead(i+30) << i); // tehdään bitin siirto, johon on luettuna dippikytkimen asento.
    }
  laite = DIP;
  DIP =0;
  //Serverin alustus
  for(int i = 0; i < 8; i++) 
    {    
      Dstate = Dstate | (digitalRead(2+i) << i); // tehdään bitin siirto, johon on luettuna dippikytkimen asento.
    }
  osoite = Dstate;  
  IPAddress server = Ethernet.localIP();
  server[3]=osoite;
  Serial.print("Asetettu serverin ip soite on ");
  Serial.println(server);
  Serial.print("Lokaali ip soite on ");
  Serial.println(Ethernet.localIP());
  client.setServer(server, 1883);
  //callbackin alustus  
  client.setCallback(callback);
  //ethernetyhteyden alustus
  if (Ethernet.begin(mac) == 0) 
    {
      Serial.println("Failed to configure Ethernet using DHCP");
      // DCHP virhe
      for(;;);
    }    
  delay(1500);
}

//Ohjelma
void loop()
{

  delay(10);
  if (!client.connected()) 
    {
      reconnect();
    }
   
  client.loop();
}


