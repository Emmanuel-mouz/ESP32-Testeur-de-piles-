// Bibliothèque Wi-Fi pour ESP32
#include <WiFi.h>  

// Paramètres du réseau AP
const char* ssid     = "testeur de piles ESP32";       
const char* password = "";       

// Configuration IP statique (optionnelle)
IPAddress local_ip(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

//configuration des librairies de l'écrant oled
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);

//composants 
const int testeur_de_piles = 34;
const int buzzer = 25;

//fonctions
float convertisseur_tesion();
void affichage();
void oled_setup();
void wifi_setup();

void setup() {
  Serial.begin(115200);

  pinMode(buzzer,OUTPUT);

  oled_setup();
  wifi_setup();
}

void loop() {
  affichage();
  delay(500);  
}

void affichage(){
  display.clearDisplay();
  display.setTextSize(1);
  float tension = convertisseur_tesion();
  display.setCursor(0, 0);
  display.println("Testeur de piles");
  display.setCursor(0, 20);
  display.println("Tension : ");

  display.setTextSize(2);
  display.setCursor(0, 35);
  display.print(tension);
  display.print(" V");
  display.display();
} 

float convertisseur_tesion(){
  float tension = analogRead(testeur_de_piles) / 4095.0 * 3.3;

  return(tension);  
}

void oled_setup(){
  Wire.begin(21, 22);  

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true) {}
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.display();  
}

void wifi_setup(){
  // Mettre l'ESP32 en mode Access Point
  WiFi.mode(WIFI_AP);

  // Configuration IP statique
  if (!WiFi.softAPConfig(local_ip, gateway, subnet)) {
    Serial.println("Erreur configuration IP");
  }

  // Démarrage du point d'accès
  if (!WiFi.softAP(ssid, password)) {
    Serial.println("Erreur démarrage AP");
  } else {
    Serial.println("Point d'accès démarré");
    Serial.print("SSID : ");
    Serial.println(ssid);
    Serial.print("Mot de passe : ");
    Serial.println(password);
    Serial.print("IP : ");
    Serial.println(WiFi.softAPIP());
  }
}