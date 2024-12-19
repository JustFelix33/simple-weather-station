#define BLYNK_TEMPLATE_ID "TMPL6c7eT2RB5"
#define BLYNK_TEMPLATE_NAME "Environment Monitor"
#define BLYNK_AUTH_TOKEN "Qa-ABSHLr4kpF9n3iDa3lB2ZhQORLeFE"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <BlynkSimpleEsp8266.h>

// Blynk credentials
char auth[] = BLYNK_AUTH_TOKEN;   // Replace with your Blynk Auth Token
char ssid[] = "Hotpots";    // Replace with your WiFi SSID
char pass[] = "2444666668888888"; // Replace with your WiFi Password

// Pin Definitions
#define DHT_PIN D3         // DHT sensor data pin
#define LED_PIN D2         // LED pin
#define RAIN_ANALOG_PIN A0 // Raindrop sensor analog pin

// DHT Sensor Type
#define DHT_TYPE DHT22     // Change to DHT11 if using DHT11

// Initialize DHT sensor
DHT dht(DHT_PIN, DHT_TYPE);

BlynkTimer timer; // Timer for sending data to Blynk

void setup() {
  // Start Serial Monitor
  Serial.begin(115200);

  // Initialize pins
  pinMode(LED_PIN, OUTPUT); // LED output
  dht.begin();              // Start DHT sensor

  // Connect to Blynk
  Blynk.begin(auth, ssid, pass);

  // Set up a timer to send data every 2 seconds
  timer.setInterval(2000L, sendToBlynk);

  Serial.println("Setup Complete! Monitoring Rain and Temperature...");
}

// Function to send data to Blynk
void sendToBlynk() {
  // Read rain sensor (analog)
  int rainAnalog = analogRead(RAIN_ANALOG_PIN);

  // Determine rain status and control LED
  if (rainAnalog < 800) { // Threshold for rain detection
    digitalWrite(LED_PIN, HIGH); // Turn ON LED
    Blynk.virtualWrite(V0, "Rain Detected");
  } else {
    digitalWrite(LED_PIN, LOW);  // Turn OFF LED
    Blynk.virtualWrite(V0, "No Rain");
  }

  // Read DHT sensor
  float temperature = dht.readTemperature(); // Read temperature (Celsius)
  float humidity = dht.readHumidity();       // Read humidity (%)

  // Check if readings are valid
  if (!isnan(temperature) && !isnan(humidity)) {
    // Send temperature and humidity to Blynk
    Blynk.virtualWrite(V1, temperature); // Virtual Pin V1 for Temperature
    Blynk.virtualWrite(V2, humidity);    // Virtual Pin V2 for Humidity

    // Print readings to Serial Monitor
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  } else {
    Serial.println("Failed to read from DHT sensor!");
  }

  // Print rain intensity to Serial Monitor
  Serial.print("Rain Analog Value: ");
  Serial.println(rainAnalog);
}

void loop() {
  Blynk.run(); // Run Blynk
  timer.run(); // Run timer
}