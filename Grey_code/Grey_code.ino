#include <ESP8266WiFi.h>

#include <WiFiClientSecure.h>

#include <UniversalTelegramBot.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

 

#define triggerPin D7

#define echoPin D8
#define led D0
#define ledd D3
#define leddd D6
TinyGPSPlus gps;
SoftwareSerial SerialGPS(4, 5); 

#define WIFI_SSID "Untrusted Network"

#define WIFI_PASSWORD "nmTRBFDmax4VSX9+zY2VDQ=="

#define BOT_TOKEN "5996527019:AAHs6hlxvCXCVR8_Qi0fYPFMe_E4J2OWNT4"

#define CHAT_ID "483580826"

const unsigned long BOT_MTBS = 1000;
X509List cert(TELEGRAM_CERTIFICATE_ROOT);

WiFiClientSecure secured_client;

UniversalTelegramBot bot(BOT_TOKEN, secured_client);

unsigned long bot_lasttime;

float Latitude , Longitude;
int year , month , date, hour , minute , second;
String DateString , TimeString , LatitudeString , LongitudeString; 

int distance;
int level;
long duration;

String distance_1;
String grey_1;
 
WiFiServer server(80);

void setup() {

  Serial.begin(115200);
  SerialGPS.begin(9600);
  configTime(0, 0, "pool.ntp.org");

  secured_client.setTrustAnchors(&cert);

  Serial.println();

 

  pinMode(triggerPin, OUTPUT);
  pinMode(led, OUTPUT);
   pinMode(ledd, OUTPUT);
    pinMode(leddd, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(D5, OUTPUT);

 

  Serial.print("Connecting to Wifi SSID "); 

  Serial.print(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); 

  while (WiFi.status() != WL_CONNECTED) 

  {

    Serial.print(".");

    delay(500);

  }

  Serial.print("\nWiFi connected. IP address: ");
  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());

 

  bot.sendMessage(CHAT_ID, "Bot started up", "");

}

 

void loop(){
   while (SerialGPS.available() > 0)
    if (gps.encode(SerialGPS.read()))
    {
      if (gps.location.isValid())
      {
        Latitude = gps.location.lat();
        LatitudeString = String(Latitude , 6);
        Longitude = gps.location.lng();
        LongitudeString = String(Longitude , 6);
      }

      if (gps.date.isValid())
      {
        DateString = "";
        date = gps.date.day();
        month = gps.date.month();
        year = gps.date.year();

        if (date < 10)
        DateString = '0';
        DateString += String(date);

        DateString += " / ";

        if (month < 10)
        DateString += '0';
        DateString += String(month);
        DateString += " / ";

        if (year < 10)
        DateString += '0';
        DateString += String(year);
      }

      if (gps.time.isValid())
      {
        TimeString = "";
        hour = gps.time.hour()+ 5; //adjust UTC
        minute = gps.time.minute();
        second = gps.time.second();
    
        if (hour < 10)
        TimeString = '0';
        TimeString += String(hour);
        TimeString += " : ";

        if (minute < 10)
        TimeString += '0';
        TimeString += String(minute);
        TimeString += " : ";

        if (second < 10)
        TimeString += '0';
        TimeString += String(second);
      }

    }
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }

  //Response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n <!DOCTYPE html> <html> <head> <meta http-equiv='refresh' content='5'><title>Garbage and Location Readings</title> <style>";
  s += "table, th, td {border: 1px solid blue;} </style> </head> <body> <h1  style=";
  s += "font-size:300%;";
  s += " ALIGN=CENTER>Garbage and Location Readings</h1>";
  s += "<p ALIGN=CENTER style=""font-size:150%;""";
  s += "> <b>Amount of Garbage Level and Location Co-ordinates</b></p> <table ALIGN=CENTER style=";
  s += "width:50%";
  s += "> <tr> <th>Garbage level in  % </th>";
  s += "<td ALIGN=CENTER >";

  s += level;
  s += "</td></tr><tr> <th>Latitude</th>";
  s += "<td ALIGN=CENTER >";
  s += LatitudeString;
  s += "</td> </tr> <tr> <th>Longitude</th> <td ALIGN=CENTER >";
  s += LongitudeString;
  s += "</td> </tr> <tr>  <th>Date</th> <td ALIGN=CENTER >";
  s += DateString;
  s += "</td></tr> <tr> <th>Time</th> <td ALIGN=CENTER >";
  s += TimeString;
  s += "</td>  </tr> </table> ";
 
  
  if (gps.location.isValid())
  {
    s += "<p align=center><a style=""color:RED;font-size:125%;"" href=""http://maps.google.com/maps?&z=15&mrt=yp&t=k&q=";
    s += LatitudeString;
    s += "+";
    s += LongitudeString;
    s += """ target=""_top"">Click here</a> to open the location in Google Maps.</p>";
  }

  s += "</body> </html> \n";

  client.print(s);
  delay(100);
  digitalWrite(triggerPin, HIGH);

  delayMicroseconds(10);

  digitalWrite(triggerPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = duration * 0.0340 / 2;
  level =((14-distance)/14.0)*100;
  distance_1 = String (distance);
  grey_1 = String(level);
  if(distance >=0 && distance <=10){
     
    

    Serial.print(distance);

    Serial.println(" cm OBSTACLES AHEAD");
    digitalWrite(led, HIGH);
    tone(D5,250);

    delay(50);

    

    String text = "Waste bin" + grey_1 + "%";

    bot.sendMessage(CHAT_ID, text, "Markdown");

    }

 

    else {
    Serial.print(distance);

    Serial.println("  cm");
 digitalWrite(led, LOW);
    noTone(D5);

    delay(100);

   bot.sendMessage(CHAT_ID, "");

 

    }

    
  if(distance >=6 && distance <=10){
     
    
    digitalWrite(ledd, HIGH);
  

    delay(50);

   

    }

 

    else {
 
 digitalWrite(ledd, LOW);
  
 

    }
    if(distance >=11 && distance <=14){
     
    
    digitalWrite(leddd, HIGH);
  

    delay(50);

   

    }

 

    else {
 
 digitalWrite(leddd, LOW);
  
 

    }
    if ( millis ( ) - bot_lasttime > BOT_MTBS )

    {    

      bot_lasttime = millis();      

      }    
 
  

} 
