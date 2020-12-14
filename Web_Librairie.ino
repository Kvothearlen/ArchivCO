#include <WiFi.h>
#include <WebServer.h>

const char *ssid = "Livebox-Adrien Cailly";
const char *password = "57806400EF283953E82E118669";
WebServer server(80);

const int led1 = 19;
const int led2 = 18;
const int led3 = 2;
const int led4 = 4;

void handleRoot()
{
    String page = "<!DOCTYPE html>";

    page += "<html lang='fr'>";

    page += "<head>";
    page += "    <title>Serveur ESP32</title>";
    page += "    <meta http-equiv='refresh' content='60' name='viewport' content='width=device-width, initial-scale=1' charset='UTF-8' />";
    page += "    <link rel='stylesheet' href='https://www.w3schools.com/w3css/4/w3.css'>";
    page += "</head>";

    page += "<body>";
    page += "    <div class='w3-card w3-blue w3-padding-small w3-jumbo w3-center'>";
    page += "        <p>Liste des archives présentes</p>";
    page += "    </div>";

    page += "    <div class='w3-bar'>";
    page += "        <a class='w3-bar-item w3-border w3-jumbo' style='width:25%;'>Titre</a>";
    page += "        <a class='w3-bar-item w3-border w3-jumbo' style='width:25%;'>Auteur</a>";
    page += "        <a class='w3-bar-item w3-border w3-jumbo' style='width:25%;'>Disponibilitée</a>";
    page += "        <a class='w3-bar-item w3-border w3-jumbo' style='width:25%;'>Sélection</a>";
    page += "    </div>";
    
    page += "    <div class='w3-bar'>";
    page += "        <a class='w3-bar-item w3-border w3-jumbo' style='width:25%;'>Livre1</a>";
    page += "        <a class='w3-bar-item w3-border w3-jumbo' style='width:25%;'>Auteur1</a>";
    page += "        <a class='w3-bar-item w3-border w3-jumbo' style='width:25%;'>Disponible</a>";
    page += "        <a href='/1' class='w3-bar-item w3-button w3-border w3-jumbo' style='width:25%;'>Sélectionner</a>";
    page += "    </div>";
    
    page += "    <div class='w3-bar'>";
    page += "        <a class='w3-bar-item w3-border w3-jumbo' style='width:25%;'>Livre2</a>";
    page += "        <a class='w3-bar-item w3-border w3-jumbo' style='width:25%;'>Auteur2</a>";
    page += "        <a class='w3-bar-item w3-border w3-jumbo' style='width:25%;'>Disponible</a>";
    page += "        <a href='/2' class='w3-bar-item w3-button w3-border w3-jumbo' style='width:25%;'>Sélectionner</a>";
    page += "    </div>";

    page += "    <div class='w3-bar'>";
    page += "        <a class='w3-bar-item w3-border w3-jumbo' style='width:25%;'>Livre3</a>";
    page += "        <a class='w3-bar-item w3-border w3-jumbo' style='width:25%;'>Auteur2</a>";
    page += "        <a class='w3-bar-item w3-border w3-jumbo' style='width:25%;'>Indisponible</a>";
    page += "        <a href='/0' class='w3-bar-item w3-button w3-border w3-jumbo' style='width:25%;'>Sélectionner</a>";
    page += "    </div>";

    page += "    <div class='w3-bar'>";
    page += "        <a class='w3-bar-item w3-border w3-jumbo' style='width:25%;'>Livre4</a>";
    page += "        <a class='w3-bar-item w3-border w3-jumbo' style='width:25%;'>Auteur3</a>";
    page += "        <a class='w3-bar-item w3-border w3-jumbo' style='width:25%;'>Disponible</a>";
    page += "        <a href='/3' class='w3-bar-item w3-button w3-border w3-jumbo' style='width:25%;'>Sélectionner</a>";
    page += "    </div>";

    page += "    <div class='w3-bar'>";
    page += "        <a class='w3-bar-item w3-border w3-jumbo' style='width:25%;'>Livre5</a>";
    page += "        <a class='w3-bar-item w3-border w3-jumbo' style='width:25%;'>Auteur4</a>";
    page += "        <a class='w3-bar-item w3-border w3-jumbo' style='width:25%;'>Disponible</a>";
    page += "        <a href='/4' class='w3-bar-item w3-button w3-border w3-jumbo' style='width:25%;'>Sélectionner</a>";
    page += "    </div>";

    page += "</body>";

    page += "</html>";

    server.setContentLength(page.length());
    server.send(200, "text/html", page);
}

void handle1()
{
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    server.sendHeader("Location","/");
    server.send(303);
}
void handle2()
{
    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    server.sendHeader("Location","/");
    server.send(303);
}
void handle3()
{
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, LOW);
    server.sendHeader("Location","/");
    server.send(303);
}
void handle4()
{
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, HIGH);
    server.sendHeader("Location","/");
    server.send(303);
}
void handle0(){
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    server.sendHeader("Location","/");
    server.send(303);
}

void handleNotFound()
{
    server.send(404, "text/plain", "404: Not found");
}

void setup()
{
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n");

    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
    pinMode(led4, OUTPUT);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);

    WiFi.persistent(false);
    WiFi.begin(ssid, password);
    Serial.print("Tentative de connexion...");

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(100);
    }

    Serial.println("\n");
    Serial.println("Connexion etablie!");
    Serial.print("Adresse IP: ");
    Serial.println(WiFi.localIP());

    server.on("/", handleRoot);
    server.on("/0", handle0);
    server.on("/1", handle1);
    server.on("/2", handle2);
    server.on("/3", handle3);
    server.on("/4", handle4);
    server.onNotFound(handleNotFound);
    server.begin();

    Serial.println("Serveur web actif!");
}

void loop()
{
    server.handleClient();
}
