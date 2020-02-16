# Control_FanCoils
Control remoto vía web y telegram de los fancoils Midea

Mediante este programa he generado un control remoto para 
los equipos de frío/calor (fan coils) que tengo instalados
en la oficina.

Dichos equipos solo tienen un control mediante mando a distancia
por infrarrojos. 

Conectanda una nodeMCU V.1 (ESP8266) a la red wifi con un modulo
infrarrojo emisor, puedo controlarlos via web o con un bot en 
telegram.

Solo he implementado las opciones de encendido y apagado, pero
se podría controlar todos los parametros. El encendido lo realizo
a 30ºC, y el ventilador al máximo.

Para obtener los codigos IR, utilizo inicialmente el ejemplo
de la librería IRremoteESP8266 llamado IRrecDumpV2 con un receptor
infrarrojo conectado a un arduino, usando el mando a distancia de
los equipos. Esta libreria me devuelve el protocolo usado por el
equipo, en este caso Coolix, y tambien los datos Raw por si no lo 
reconoce.

La conexión a la nodeMCU del modulo emisor infrarojo es muy sencilla
solamente lleva una conexión a GND, otra a VIN, y otra a un pin datos,
en este caso el GPIO04.

