#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ir_Coolix.h>
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>

//Introducimos los datos de la red a la que se conectara
//We enter the data of the network to which you will connect

const char* ssid = "xxxx";
const char* password = "xxxx";
int contconexion = 0;

#define equipo "xxx"  // ponemos un nombre al equipo de cada oficina. Put a name of machine in each office

//Configuramos la conexión con telegram
//We set up the telegram connection
const char BotToken[] = "xxxxxxxxxxxxxxxxx";

WiFiClientSecure client;
UniversalTelegramBot bot (BotToken, client);

int Bot_mtbs = 1000; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done
bool Start = false;

//Configurar el servidor web
//Configure the web server
ESP8266WebServer server(80);

//Configuramos la librería Coolix que controla los fan coils de mi oficina. Conectado al GPIO04
// We set up the Coolix library that controls the fan coils in my office. Connection to GPIO04

const uint16_t kIrLed = 4;
IRCoolixAC ac(kIrLed);

//----------------------------------------------------------------------------------
//Preparamos la web para contrololarlo --- We prepare the web page

String principal = "<!DOCTYPE html>"
"<html>"
"<head>"
"<title>Remote Control AC</title>"
"<meta charset='UTF-8'>"
"<style type='text/css'>"
"*{margin: 5px; padding: 5px;}"
"span {float: left; width: 150px;}"
".boton {width: 800px; height:250px; font-family: sans-serif; font-size: 50px;}"
"</style>"
"</head>"
"<body style='background-color:rgb(221,160,221)'>"
"<a href='oficina01'><button class='boton'>OFICINA_01</button></a><br>"
"<a href='oficina02'><button class='boton'>OFICINA_02</button></a><br>"
"<a href='oficina03'><button class='boton'>OFICINA_03</button></a><br>"
"<a href='oficina04'><button class='boton'>OFICINA_04</button></a><br>"
"</body>"
"</html>";

String oficina02 = "<!DOCTYPE html>"
"<html>"
"<head>"
"<title>Remote Control AC</title>"
"<meta charset='UTF-8'>"
"<style type='text/css'>"
"*{margin: 5px; padding: 5px;}"
"span {float: left; width: 150px;}"
".boton {width: 250px; height:250px; font-family: sans-serif; font-size: 50px;}"
"</style>"
"</head>"
"<body>"
"<table>"
"<tr>"
"<td><a href='power1'><button class='boton'>ON</button></td>"
"<td></td>"
"<td><a href='off1'><button class='boton'>OFF</button></td>"
"</tr>"
"<tr>"
"<td></td>"
"<td><a href='/'><button class='boton'>HOME</td>"
"<td></td>"
"</tr>"
"</table>"
"</body>"
"</html>";

String oficina01 = "<!DOCTYPE html>"
"<html>"
"<head>"
"<title>REMOTE CONTROL A/C</title>"
"<meta charset='UTF-8'>"
"<style type='text/css'>"
"*{margin: 5px; padding: 5px;}"
"span {float: left; width: 150px;}"
".boton {width: 250px; height:250px; font-family: sans-serif; font-size: 50px;}"
"</style>"
"</head>"
"<body>"
"<table>"
"<tr>"
"<td><a href='power'><button class='boton'>ON</button></td>"
"<td></td>"
"<td><a href='off'><button class='boton'>OFF</button></td>"
"</tr>"
"<tr>"
"<td></td>"
"<td><a href='/'><button class='boton'>HOME</td>"
"<td></td>"
"</tr>"
"</table>"
"</body>"
"</html>";

String oficina03 = "<!DOCTYPE html>"
"<html>"
"<head>"
"<title>REMOTE CONTROL A/C</title>"
"<meta charset='UTF-8'>"
"<style type='text/css'>"
"*{margin: 5px; padding: 5px;}"
"span {float: left; width: 150px;}"
".boton {width: 250px; height:250px; font-family: sans-serif; font-size: 50px;}"
"</style>"
"</head>"
"<body>"
"<table>"
"<tr>"
"<td><a href='power2'><button class='boton'>ON</button></td>"
"<td></td>"
"<td><a href='off2'><button class='boton'>OFF</button></td>"
"</tr>"
"<tr>"
"<td></td>"
"<td><a href='/'><button class='boton'>HOME</td>"
"<td></td>"
"</tr>"
"</table>"
"</body>"
"</html>";

String oficina04 = "<!DOCTYPE html>"
"<html>"
"<head>"
"<title>REMOTE CONTROL A/C</title>"
"<meta charset='UTF-8'>"
"<style type='text/css'>"
"*{margin: 5px; padding: 5px;}"
"span {float: left; width: 150px;}"
".boton {width: 250px; height:250px; font-family: sans-serif; font-size: 50px;}"
"</style>"
"</head>"
"<body>"
"<table>"
"<tr>"
"<td><a href='power3'><button class='boton'>ON</button></td>"
"<td></td>"
"<td><a href='off3'><button class='boton'>OFF</button></td>"
"</tr>"
"<tr>"
"<td></td>"
"<td><a href='/'><button class='boton'>HOME</td>"
"<td></td>"
"</tr>"
"</table>"
"</body>"
"</html>";

//----------------------------------------------------------------------------------
void paginaPrincipal() {
  server.send(200, "text/html", principal); 
}
void paginaOficina01() {
  server.send(200, "text/html", oficina01); 
}
void paginaOficina02() {
  server.send(200, "text/html", oficina02); 
}
void paginaOficina03() {
  server.send(200, "text/html", oficina03); 
}
void paginaOficina04() {
  server.send(200, "text/html", oficina04); 
}

//----------------------------------------------------------------------------------
//Funciones de encendido y apagado -- on/off functions

void teclapower() {
  if (equipo == "xxx") {
  Serial.println("ON");
  ac.setPower(true);
  ac.setMode(kCoolixHeat);
  ac.setFan(kCoolixFanMax);
  ac.setTemp(kCoolixTempMax);
  ac.send();
  server.send(200, "text/html", oficina01); 
  }
  else {
    
  }
}
void teclaoff() {
  if (equipo == "xxx") {
  Serial.println("OFF");
  ac.setPower(false);
  ac.send();
  server.send(200, "text/html", oficina01);
  }
  else {
    
  } 
}
void teclapower1() {
  if (equipo == "xxx") {
  Serial.println("ON");
  ac.setPower(true);
  ac.setMode(kCoolixHeat);
  ac.setFan(kCoolixFanMax);
  ac.setTemp(kCoolixTempMax);
  ac.send();
  server.send(200, "text/html", oficina02); 
  }
  else {
    
  }
}
void teclaoff1() {
  if (equipo == "xxx") {
  Serial.println("OFF");
  ac.setPower(false);
  ac.send();
  server.send(200, "text/html", oficina02); 
  }
  else {
    
  }
}
void teclapower2() {
  if (equipo == "xxx") {
  Serial.println("ON");
  ac.setPower(true);
  ac.setMode(kCoolixHeat);
  ac.setFan(kCoolixFanMax);
  ac.setTemp(kCoolixTempMax);
  ac.send();
  server.send(200, "text/html", oficina03); 
  }
  else {
    
  }
}
void teclaoff2() {
  if (equipo == "xxx") {
  Serial.println("OFF");
  ac.setPower(false);
  ac.send();
  server.send(200, "text/html", oficina03); 
  }
  else {
    
  }
}
void teclapower3() {
  if (equipo == "xxx") {
  Serial.println("ON");
  ac.setPower(true);
  ac.setMode(kCoolixHeat);
  ac.setFan(kCoolixFanMax);
  ac.setTemp(kCoolixTempMax);
  ac.send();
  server.send(200, "text/html", oficina04); 
  }
  else {
    
  }
}
void teclaoff3() {
  if (equipo == "xxx") {
  Serial.println("OFF");
  ac.setPower(false);
  ac.send();
  server.send(200, "text/html", oficina04); 
  }
  else {
    
  }
}

//----------------------------------------------------------------------------------
//Configuramos el bot de telegram --- We set up the telegram bot 


void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";

    if (text == "/onOficina01") {
      teclapower();
      bot.sendMessage(chat_id, "Calefaccion encendida");  
    }
    if (text == "/onOficina02") {
      teclapower1();
      bot.sendMessage(chat_id, "Calefaccion encendida");   
    }
    if (text == "/onOficina03") {
      teclapower2();
      bot.sendMessage(chat_id, "Calefaccion encendida");  
    }
    if (text == "/onOficina04") {
      teclapower3();
      bot.sendMessage(chat_id, "Calefaccion encendida");  
    }
    if (text == "/offOficina01") {
      teclaoff();
      bot.sendMessage(chat_id, "Calefaccion apagada");  
    }
    if (text == "/offOficina02") {
      teclaoff1();
      bot.sendMessage(chat_id, "Calefaccion apagada");    
    }
    if (text == "/offOficina03") {
      teclaoff2();
      bot.sendMessage(chat_id, "Calefaccion apagada");  
    }
    if (text == "/offOficina04") {
      teclaoff3();
      bot.sendMessage(chat_id, "Calefaccion apagada");  
    }
    if (text == "/options") {
      String keyboardJson = "[[\"/onOficina01\", \"/onOficina02\"],[\"/onOficina03\",\"/onOficina04\"],[\"/offOficina01\", \"/offOficina02\"],[\"/offOficina03\",\"/offOficina04\"]]";
      bot.sendMessageWithReplyKeyboard(chat_id, "Elige una de estas opciones", "", keyboardJson, true);
    }
  }
}  
//-------------------------------------------------------
void setup() {
  ac.begin();
  #if ESP8266
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  #else  // ESP8266
  Serial.begin(115200, SERIAL_8N1);
  #endif 
  Serial.println("");
  Serial.println("");
 
  WiFi.mode(WIFI_STA); //para que inicie solamente en modo estación
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED and contconexion <50) { //Cuenta hasta 50 si no se puede conectar lo cancela
    ++contconexion;
    Serial.print(".");
    delay(500);
  }
  if (contconexion <50) {
      
      IPAddress Ip(192,168,100,46); //Ponemos la IP que le asignamos a la NodeMCU -- We put the IP we assign to the NodeMCU
      IPAddress Gateway(192,168,100,1);  //Ponemos la IP del gateway al que se conecta -- We put the IP of the gateway
      IPAddress Subnet(255,255,255,0); 
      WiFi.config(Ip, Gateway, Subnet); 
      
      Serial.println("");
      Serial.println("WiFi conectado");
      Serial.println(WiFi.localIP());
          
      Serial.println("WebServer iniciado...");

      server.on("/", paginaPrincipal);
      server.on("/oficina01", paginaOficina01);
      server.on("/oficina02", paginaOficina02); 
      server.on("/oficina03", paginaOficina03); 
      server.on("/oficina04", paginaOficina04); 
      
      server.on("/power", teclapower);
      server.on("/off", teclaoff); 
      server.on("/power1", teclapower1); 
      server.on("/off1", teclaoff1); 
      server.on("/power2", teclapower2); 
      server.on("/off2", teclaoff2);
      server.on("/power3", teclapower3); 
      server.on("/off3", teclaoff3);
      
      server.begin(); //incia el webserver   
  }
  else { 
      Serial.println("");
      Serial.println("Error de conexion");
  }

 bot._debug = true;
 client.setFingerprint("BB DC 45 2A 07 E3 4A 71 33 40 32 DA BE 81 F7 72 6F 4A 2B 6B");
}

//----------------------------------------------------------------------------------
void loop() {
  server.handleClient();  
  
  if (millis() > Bot_lasttime + Bot_mtbs)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    Bot_lasttime = millis();
  }
}
