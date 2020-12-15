#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <WiFi.h>
#include "ESPAsyncWebServer.h"

// Replace with your network credentials
const char* ssid = "Livebox-Adrien Cailly";
const char* password = "57806400EF283953E82E118669";

Adafruit_BME680 bme;

float temperature;
float humidity;
float pressure;
float gasResistance;

AsyncWebServer server(80);
AsyncEventSource events("/events");

unsigned long lastTime = 0;  
unsigned long timerDelay = 3000;

void getBME680Readings(){
  // Tell BME680 to begin measurement.
  unsigned long endTime = bme.beginReading();
  if (endTime == 0) {
    Serial.println(F("Failed to begin reading :("));
    return;
  }
  if (!bme.endReading()) {
    Serial.println(F("Failed to complete reading :("));
    return;
  }
  temperature = bme.temperature;
  pressure = bme.pressure / 100.0;
  humidity = bme.humidity;
  gasResistance = bme.gas_resistance / 1000.0;
}

String processor(const String& var){
  getBME680Readings();
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return String(temperature);
  }
  else if(var == "HUMIDITY"){
    return String(humidity);
  }
  else if(var == "PRESSURE"){
    return String(pressure);
  }
 else if(var == "GAS"){
    return String(gasResistance);
  }
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>BME680 Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    p {  font-size: 1.2rem;}
    body {  margin: 0;}
    .topnav { overflow: hidden; background-color: #4B1D3F; color: white; font-size: 1.7rem; }
    .content { padding: 20px; }
    .card { background-color: white; box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5); }
    .cards { max-width: 700px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr)); }
    .reading { font-size: 2.8rem; }
    .card.temperature { color: #0e7c7b; }
    .card.humidity { color: #17bebb; }
    .card.pressure { color: #3fca6b; }
    .card.gas { color: #d62246; }
    .card.fire { color: #fe1b00; }
  </style>
</head>
<body>
  <div class="topnav">
    <h3>Tableau de bord</h3>
  </div>
  <div class="content">
    <div class="cards">
    
      <div class="card temperature" id="chaud" style="display:none;">
        <h4><i class="fas fa-temperature-high"></i> TOO HOT</h4><p><span class="reading"><span id="temp1">%TEMPERATURE%</span> &deg;C</span></p>
      </div>
      
      <div class="card temperature" id="temp_ok" style="display:none;">
        <h4><i class="fas fa-thermometer-half"></i> CORRECT</h4><p><span class="reading"><span id="temp2">%TEMPERATURE%</span> &deg;C</span></p>
      </div>

      <div class="card temperature" id="froid" style="display:none;">
        <h4><i class="fas fa-temperature-low"></i> TOO COLD</h4><p><span class="reading"><span id="temp3">%TEMPERATURE%</span> &deg;C</span></p>
      </div>

      <div class="card humidity" id="dry" style="display:none;">
        <h4><i class="fas fa-tint"></i> TOO DRY</h4><p><span class="reading"><span id="hum1">%HUMIDITY%</span> &percnt;</span></p>
      </div>
      
      <div class="card humidity" id="humid_ok" style="display:none;">
        <h4><i class="fas fa-tint"></i> CORRECT</h4><p><span class="reading"><span id="hum2">%HUMIDITY%</span> &percnt;</span></p>
      </div>

      <div class="card humidity" id="humid" style="display:none;">
        <h4><i class="fas fa-tint"></i> TOO HUMID</h4><p><span class="reading"><span id="hum3">%HUMIDITY%</span> &percnt;</span></p>
      </div>
      
      <div class="card pressure">
        <h4><i class="fas fa-angle-double-down"></i> PRESSURE</h4><p><span class="reading"><span id="pres">%PRESSURE%</span> hPa</span></p>
      </div>
      
      <div class="card gas" id="gas_ok" style="display:none;">
        <h4><i class="fas fa-wind"></i> GAS</h4><p><span class="reading"><span id="gas1">%GAS%</span> K&ohm;</span></p>
      </div>

      <div class="card gas" id="vent" style="display:none;">
        <h4><i class="fas fa-exclamation-triangle"></i></i> VENTILATE</h4><p><span class="reading"><span id="gas2">%GAS%</span> K&ohm;</span></p>
      </div>

      <div class="card fire" id="onFire" style="display:none;">
        <h4><i class="fas fa-fire"></i> FIRE </h4><p><span class="reading"><span id="fire">%FIRE%</span></span></p>
      </div>
      
    </div>
  </div>
<script>
if (!!window.EventSource) {
 var source = new EventSource('/events');
 
 source.addEventListener('open', function(e) {
  console.log("Events Connected");
 }, false);
 source.addEventListener('error', function(e) {
  if (e.target.readyState != EventSource.OPEN) {
    console.log("Events Disconnected");
  }
 }, false);
 
 source.addEventListener('message', function(e) {
  console.log("message", e.data);
 }, false);
 
 source.addEventListener('temperature', function(e) {
  console.log("temperature", e.data);
  var temp = parseFloat(e.data);
  if(temp>19){ 
  document.getElementById("temp1").innerHTML = e.data; document.getElementById("chaud").style.display = 'block'; document.getElementById("temp_ok").style.display = 'none'; document.getElementById("froid").style.display = 'none'; }
  else if(temp<17){ 
  document.getElementById("temp3").innerHTML = e.data; document.getElementById("chaud").style.display = 'none'; document.getElementById("temp_ok").style.display = 'none'; document.getElementById("froid").style.display = 'block';}
  else{ 
  document.getElementById("temp2").innerHTML = e.data; document.getElementById("chaud").style.display = 'none'; document.getElementById("temp_ok").style.display = 'block'; document.getElementById("froid").style.display = 'none';}
 }, false);
 
 source.addEventListener('humidity', function(e) {
  console.log("humidity", e.data);
  var hum = parseFloat(e.data);
  if(hum>55){ document.getElementById("hum3").innerHTML = e.data; document.getElementById("humid").style.display = 'block'; document.getElementById("humid_ok").style.display = 'none'; document.getElementById("dry").style.display = 'none'; }
  else if(hum<45){ document.getElementById("hum1").innerHTML = e.data; document.getElementById("humid").style.display = 'none'; document.getElementById("humid_ok").style.display = 'none'; document.getElementById("dry").style.display = 'block';}
  else{ document.getElementById("hum2").innerHTML = e.data; document.getElementById("humid").style.display = 'none'; document.getElementById("humid_ok").style.display = 'block'; document.getElementById("dry").style.display = 'none';}
 }, false);
 
 source.addEventListener('pressure', function(e) {
  console.log("pressure", e.data);
  document.getElementById("pres").innerHTML = e.data;
 }, false);
 
 source.addEventListener('gas', function(e) {
  console.log("gas", e.data);
  var gaz = parseFloat(e.data);
  if(50<gaz){ 
  document.getElementById("gas1").innerHTML = e.data; document.getElementById("gas_ok").style.display = 'block'; document.getElementById("vent").style.display = 'none';}
  else{ 
  document.getElementById("gas2").innerHTML = e.data; document.getElementById("gas_ok").style.display = 'none'; document.getElementById("vent").style.display = 'block';}
 }, false);

 source.addEventListener('fire', function(e) {
  console.log("fire", e.data);
  var fi = parseFloat(e.data);
  if(fi==1){ document.getElementById("onFire").style.display = 'block';}
  else{ document.getElementById("gas_ok").style.display = 'none';}
 }, false);
}
</script>
</body>
</html>)rawliteral";

const int LedChaud = 15;
const int LedFroid=4;
const int LedSec = 13;
const int LedHumide = 2;
const int LedPollue = 18;
const int LedFeu = 19;


void setup() {
  Serial.begin(115200);

  // Set the device as a Station and Soft Access Point simultaneously
  WiFi.mode(WIFI_AP_STA);
  
  // Set device as a Wi-Fi Station
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Setting as a Wi-Fi Station..");
  }
  Serial.print("Station IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Init BME680 sensor
  if (!bme.begin()) {
    Serial.println(F("Could not find a valid BME680 sensor, check wiring!"));
    while (1);
  }
  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms

  // Handle Web Server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Handle Web Server Events
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);
  server.begin();

  pinMode(LedChaud, OUTPUT);
  pinMode(LedFroid, OUTPUT);
  pinMode(LedHumide, OUTPUT);
  pinMode(LedSec, OUTPUT);
  pinMode(LedPollue, OUTPUT);
  pinMode(LedFeu, OUTPUT);
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    getBME680Readings();
    Serial.printf("Temperature = %.2f ºC \n", temperature);
    Serial.printf("Humidity = %.2f % \n", humidity);
    Serial.printf("Pressure = %.2f hPa \n", pressure);
    Serial.printf("Gas Resistance = %.2f KOhm \n", gasResistance);
    Serial.println();

    // Send Events to the Web Server with the Sensor Readings
    events.send("ping",NULL,millis());
    events.send(String(temperature).c_str(),"temperature",millis());
    events.send(String(humidity).c_str(),"humidity",millis());
    events.send(String(pressure).c_str(),"pressure",millis());
    events.send(String(gasResistance).c_str(),"gas",millis());
    if(50>gasResistance&&temperature>60){
      events.send(String(1).c_str(),"fire",millis());
    }
    lastTime = millis();
  }
  if(temperature>19){
    digitalWrite(LedChaud, HIGH);
  }
  if(temperature<17){
    digitalWrite(LedFroid, HIGH);
  }
  if(humidity>55){
    digitalWrite(LedHumide, HIGH);
  }
  if(humidity<45){
    digitalWrite(LedSec, HIGH);
  }
  if(gasResistance<50){
    digitalWrite(LedPollue, HIGH);
  }
  if(gasResistance<50&&temperature>50){
    digitalWrite(LedFeu, HIGH);
  }
}
