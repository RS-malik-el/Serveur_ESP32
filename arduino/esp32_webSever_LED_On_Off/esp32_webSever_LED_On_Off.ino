/**
 * e-mail : openprogramming23@gmail.com
 * gitHub : https://github.com/RS-malik-el
 * Donation : paypal.me/RachelSysteme
 *
 * @AUTEUR : Exaucé KIMBEMBE / @OpenProgramming
 * DATE    : 09/05/2023
 *
 * @Bibliothèques nécessaires pour réaliser ce projet:
 * *** https://github.com/me-no-dev/ESPAsyncWebServer
 * *** https://github.com/me-no-dev/AsyncTCP
 * 
 *@Plugin : extension a installer sur l'IDE arduino pour le transfert des fichiers dans la
 * mémoire flash.
 * *** https://github.com/me-no-dev/arduino-esp32fs-plugin/releases/  
 * 
 * @Board : ESP32
 * 
 * Ce code permet de contrôler une LED en utilisant une page web.
 * Si l'ESP32 est connecté à un réseau WiFi, la LED intégrée à l'ESP32 clignote régulièrement 
 * pour indiquer que la connexion est active. Si la connexion est perdue, l'ESP32 tente de se 
 * reconnecter automatiquement. 
 * 
 * Le code est composé de plusieurs fichiers, 
 * *** Un fichier HTML qui contient la page web, 
 * *** Un fichier CSS pour la mise en forme, 
 * *** Un fichier JavaScript pour la gestion des événements
 * *** Deux photos qui sont utilisées comme images de fond.
 * 
*/

// Interruption de la compilation si la carte ESP32 n'est pas sélectionné
#ifndef ESP32
  #error "Veillez selectionner une carte ESP32"
#endif

#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <SPIFFS.h> // SPIFFS (SPI Flash File System)

// Remplacez ici par votre SSID et votre mot de passe WiFi
const char* ssid = "MonSSID";
const char* password = "MonMotDePasseWiFi";

AsyncWebServer server(80);// Crée un objet de serveur web sur le port 80

// pin utilisé
#define PIN_LED  4

// Prototype
void blink(void); // Indique que l'esp32 est en mode AP_STA

void setup() {
  Serial.begin(115200);          // Initialisation de la communication série
  pinMode(PIN_LED, OUTPUT);      // Définit la broche LED comme une sortie
  pinMode(LED_BUILTIN, OUTPUT);  // Définit la broche LED comme une sortie
  digitalWrite(PIN_LED, LOW);    // Éteint la LED au démarrage
  digitalWrite(LED_BUILTIN, LOW);// Éteint la LED au démarrage
  
  // Initialisation du SPIFFS
  if (!SPIFFS.begin(true)){
    Serial.println("Une erreur s'est produite lors du montage de SPIFFS.");
    return;
  }

  // Ouverture du répertoire racine de SPIFFS
  File root = SPIFFS.open("/");
  // Ouverture du premier fichier contenu dans le répertoire racine de SPIFFS
  File file = root.openNextFile();

  Serial.println("\n\n");
  // Affichage des titres des fichiers contenu dans le SPIFFS
  while (file){
    Serial.print("Fichier stocker: ");
    Serial.println(file.name());
    file.close();
    file = root.openNextFile();
  }
Serial.println("\n\n");
  // Configuration du wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connexion en cours au WiFi...");
  }

  Serial.println("\nConnecté au WiFi avec succès !");
  Serial.print("Adresse IP : http://");
  Serial.println(WiFi.localIP());
 
  //------------ Initialisation du serveur
  server.begin();
  Serial.println("\nServeur web démarré !");

  //------------- Page d'acceuil (page Web)
  server.on("/", HTTP_GET,[](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/page_web.html", "text/html");// envoi du fichier html
  });

  server.on("/style.css", HTTP_GET,[](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");// envoi du fichier css
  });
  
  server.on("/script.js", HTTP_GET,[](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/script.js", "application/javascript");// envoi du fichier js
  });
                      
  server.on("/on.png", HTTP_GET,[](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/on.png", "image/png");// envoi du fichier png
  });
                      
  server.on("/off.png", HTTP_GET,[](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/off.png", "image/png");// envoi du fichier png
  });


  //----------- Requête allumage lampe
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
      digitalWrite(PIN_LED, HIGH);
      request->send(200);
  });
  //----------- Requête extinction lampe
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
      digitalWrite(PIN_LED, LOW);
      request->send(200);
  });
}//fin setup()


void loop(){
  // Clignotement du voyant si WIFI actif
  if(WiFi.isConnected()==true)
    blink();
  // Reconnexion automatique en cas de perte de connexion WIFI
  else{
    WiFi.reconnect(); 
    digitalWrite(LED_BUILTIN, LOW);
  }  
}

// Clignotement du voyant
void blink(void){
  static unsigned long int init_t = millis();
  static bool etat = false;

  if ((millis()-init_t) >= 250){
    etat = not etat;
    digitalWrite(LED_BUILTIN, etat);
    init_t = millis();
  }
}