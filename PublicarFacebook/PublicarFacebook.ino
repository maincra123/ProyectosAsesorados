#include <WiFiClientSecure.h>

const char* ssid     = "TURBONETT_ALSW"; //Nombre de la red
const char* password = "2526-4897"; //Contraseña

const char*  server = "facebook.com";//Servidor
String AccessToken = "EAACEdEose0cBAOgJJ7jbOM5OK9ZCXzkVMHuaL0vh9bB0TRCUsrv8cwmzbhvTLofoIshwqKmTX0GxaGjnrkPYErJQ4bNxJ0kOht9ZBBNN3y4idH6L2wpwP5qzGsdHqMVvPBRU7xWlZCIIb7MA5VERIuL57KtDZB8SlUvemtuHICiCEJg3r6DPPWjZBNz7ZAJBIZD";//Contraseña del greap API de facebook, cuidado caduze
WiFiClientSecure client;//Cliente para contartar a facebook

//Respuesta con Facebook
String Dato;
String Mensaje;

int PinBoton = 0;
int PinLed = 5;

void setup() {
  //Inicializa la comunicacion Serial
  Serial.begin(115200);
  delay(100);

  pinMode(PinBoton, INPUT);
  pinMode(PinLed, OUTPUT);
  Serial.print("Intentando Conectar a: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  //Intenta conectar al router, multiples veses:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    digitalWrite(PinLed, 1);
    delay(250);
    digitalWrite(PinLed, 0);
    delay(250);
  }

  Serial.print("Conctado a ");
  Serial.println(ssid);

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  //Preparar la consulta para facebook
  Dato = "POST https://graph.facebook.com/v2.9/";
  Dato.concat("me/feed?");
  Dato.concat("message=funciona3&access_token=");
  Dato.concat(AccessToken);
  Dato.concat(" HTTP/1.0");
}

void loop() {
  if (digitalRead(PinBoton) == 0) {
    Serial.println("Iniciando FB");
    ConsultaFB();
    delay(500);
  }
  else {
    Serial.print("*");
    delay(100);
  }
}

//Consulta a Facebook y verifica cual color es el ultimo
//En base a la posicion del texto
void ConsultaFB() {
  String Texto = ConsutaFacebook(Dato);

  //Muestra datos y el Texto que recibimos
  Serial.println(Texto);
  Serial.print("Cantidad de texto ");
  Serial.println(Texto.length());
}

//Funcion que hace la consulta a facebook a traves de la API Greap
//Es necesario enviar la URL a consultar ella devuelve la respuesta
String ConsutaFacebook(String URL) {
  String Texto = "\"error\"";
  //Valor Inicial
  Serial.println("\nEmpezando coneccion con el servidor...");
  if (!client.connect(server, 443))
    Serial.println("Conexion Fallida");
  else {
    Serial.println("Conexion Exitosa");

    //Hace la consulta HTTPS a facebook
    client.println(URL);
    client.println("Host: www.graph.facebook.com");
    client.println("Connection: close");
    client.println();
    delay(500);

    //Si nos mandaron datos lo guandamos en Texto
    while (client.available()) {
      Texto = client.readString();
    }

    //Nos Desconectamos del servidor
    if (!client.connected()) {
      Serial.println();
      Serial.println("Desconectar del servidor");
      client.stop();
    }
  }
  return Texto;
}
