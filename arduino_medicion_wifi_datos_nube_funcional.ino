#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

#define PIN_TRIG D5
#define PIN_ECHO D6
#define LED_ROJO D2
#define LED_VERDE D7
#define LED_AMARILLO D3

const char* ssid = "moto g04";          // Reemplaza con el nombre de tu red WiFi
const char* password = "Elyiyo87.";     // Reemplaza con la contraseña de tu red
const char* server = "api.thingspeak.com";

unsigned long myChannelNumber = 2783033; // Reemplaza con el número de tu canal
const char* myWriteAPIKey = "3NY4VEP1MKQHOE2E"; // Reemplaza con tu clave API de escritura

WiFiClient client;

float tiempo;
float distancia;

void setup() {
   Serial.begin(9600);
   WiFi.begin(ssid, password);
   
   // Conectar a WiFi
   Serial.print("Conectando a WiFi");
   while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
   }
   Serial.println("\nConexión establecida!");
   
   // Conectar a ThingSpeak
   ThingSpeak.begin(client);

   pinMode(PIN_TRIG, OUTPUT);
   pinMode(PIN_ECHO, INPUT);
   pinMode(LED_VERDE, OUTPUT);
   pinMode(LED_ROJO, OUTPUT);
   pinMode(LED_AMARILLO, OUTPUT);
}

void loop() {
   // Medir distancia
   digitalWrite(PIN_TRIG, HIGH);  
   delayMicroseconds(1);
   digitalWrite(PIN_TRIG, LOW);  
   
   tiempo = pulseIn(PIN_ECHO, HIGH);
   distancia = tiempo / 58.2;

   Serial.print("Distancia: ");
   Serial.println(distancia);

   // Control de LEDs según la distancia (lógica corregida)
   if (distancia <= 5) { // Nivel cercano al sensor (estanque lleno)
      digitalWrite(LED_VERDE, HIGH);   // Enciende LED verde
      digitalWrite(LED_AMARILLO, LOW); // Apaga LED amarillo
      digitalWrite(LED_ROJO, LOW);     // Apaga LED rojo
   } 
   else if (distancia > 5 && distancia <= 15) { // Nivel intermedio
      digitalWrite(LED_VERDE, LOW);    // Apaga LED verde
      digitalWrite(LED_AMARILLO, HIGH); // Enciende LED amarillo
      digitalWrite(LED_ROJO, LOW);     // Apaga LED rojo
   } 
   else { // Nivel bajo (estanque casi vacío)
      digitalWrite(LED_VERDE, LOW);    // Apaga LED verde
      digitalWrite(LED_AMARILLO, LOW); // Apaga LED amarillo
      digitalWrite(LED_ROJO, HIGH);    // Enciende LED rojo
   }

   // Enviar datos a ThingSpeak
   int response = ThingSpeak.writeField(myChannelNumber, 1, distancia, myWriteAPIKey);
   if (response == 200) {
      Serial.println("Datos enviados a ThingSpeak correctamente.");
   } else {
      Serial.print("Error al enviar datos. Código: ");
      Serial.println(response);
   }

   delay(3000); // 

}
