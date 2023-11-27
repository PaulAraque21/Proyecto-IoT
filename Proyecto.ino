#include <Wire.h>
#include <DFRobot_DHT11.h>
#include <LCD_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h> 
#include <ArduinoJson.h>  
// Definiciones para DHT11
const int DHT_PIN = 15;
DFRobot_DHT11 dht;

// Definiciones para LCD
LCD_I2C lcd(0x27, 20, 4);

// Para el wifi
const char* ssid = "red ph1"; 
const char* password = "valeale18";
// Nuestro API endpoint
const char* apiEndpoint =  "https://ideal-lamp-q77w6xwqwrrc4g97-5000.app.github.dev/sensor_data";

// Definición para el sensor de gas MQ135
const int MQ135_PIN = 4;

void setupWifi() 
{
  Serial.begin(9600);
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.print(" Connected: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  lcd.begin();
  lcd.backlight();
  Serial.begin(115200);
  setupWifi();
}

void sendData(float temperature, float humidity,float gas) 
{
  Serial.print("Sending data to API: ");

  // Set up HTTP connection with the API endpoint
  HTTPClient http;
  http.begin(apiEndpoint);
  http.addHeader("Content-Type", "application/json");

  // Create a JSON document using the ArduinoJson library
  StaticJsonDocument<200> doc;

  // Add the data to the JSON document
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;

  // Add the current date and time to the JSON document. This will change to a date from the proper sensor in the future
  doc["gas"] = gas;

  // Serialize the JSON document to a string
  String json;
  serializeJson(doc, json);

  // Send the POST request to the API endpoint
  int httpResponseCode = http.POST(json);
  if (httpResponseCode > 0) 
  {
    // Print the response from the server
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String responseString = http.getString();
    Serial.println("Received response: " + responseString);
  } else 
  {
    // Print the error code
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}

void loop() {
  
  dht.read(DHT_PIN);
  float temperatura = dht.temperature;
  float humedad = dht.humidity;
  int valorGas = analogRead(MQ135_PIN);
  float concentracionGas = map(valorGas, 0, 1023, 0, 100);
  
   if (temperatura >= 32) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PELIGRO");
    lcd.setCursor(0, 1);
    lcd.print("TEMPERATURA ELEVADA");
    delay(5000); 
  }
  if (temperatura <= 4) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PELIGRO");
    lcd.setCursor(0, 1);
    lcd.print("TEMPERATURA BAJA");
    delay(5000); 
  } 

  if (humedad >= 60) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PELIGRO: NIVEL");
    lcd.setCursor(0, 1);
    lcd.print("ALTO DE HUMEDAD");
    delay(5000); 
  }

  if (humedad <= 30) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PELIGRO: NIVEL");
    lcd.setCursor(0, 1);
    lcd.print("BAJO DE HUMEDAD");
    delay(5000); 
  }

  if (concentracionGas >= 50) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PELIGRO: NIVEL");
    lcd.setCursor(0, 1);
    lcd.print("ALTO DE GAS");
    delay(5000); 
  }
  
   if (temperatura >  4 && temperatura < 32 && humedad > 30 && humedad < 60 && concentracionGas < 50) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Niveles Estables");
    delay(2000); 
    mostrarTemperatura(temperatura); 
    delay(2000); 
    mostrarHumedad(humedad); 
    delay(2000); 
    mostrarGas(concentracionGas);
    delay(2000); 
  }

  sendData(temperatura, humedad, concentracionGas);
}

void mostrarTemperatura(float temperatura) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperatura);
  lcd.print(" C");
}

void mostrarHumedad(float humedad) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Humedad: ");
  lcd.print(humedad);
  lcd.print("%");
}

void mostrarGas(float concentracionGas) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Gas: ");
  lcd.print(concentracionGas);
  lcd.print("ppm");
  delay(1000);

}
