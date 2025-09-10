// === OLED + LM35 (SH1106 + Adafruit) ===============================
// Muestra temperatura del LM35 en Serial y en pantalla OLED

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// ---- Config OLED (SH1106 I2C) ----
#define I2C_ADDRESS   0x3C        // cámbialo a 0x3D si tu módulo lo requiere
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ---- Config LM35 ----
const int PIN_LM35 = A0;                 // Salida del LM35 a A0
const unsigned long INTERVALO_MS = 1000; // Actualización cada 1 segundo
unsigned long tAnterior = 0;

// Si tu Arduino trabaja a 3.3 V, cambia VREF a 3.3
const float VREF = 5.0;  // Referencia ADC (5.0 en la mayoría de Arduinos a 5 V)

void setup() {
  Serial.begin(9600);
  delay(100);

  // ---- Inicializar OLED ----
  display.begin(I2C_ADDRESS, true);
  display.setContrast(255);   // Brillo (0–255)
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);

  // Mensaje de arranque
  display.setCursor(0, 0);
  display.println(F("LM35 + OLED SH1106"));
  display.println(F("Inicializando..."));
  display.display();
  delay(800);

  // Cabecera en Serial
  Serial.println(F("LM35 - Lectura simple (Temp en C)"));
}

void loop() {
  unsigned long ahora = millis();
  if (ahora - tAnterior >= INTERVALO_MS) {
    tAnterior = ahora;

    // ---- Lectura LM35 ----
    int lectura = analogRead(PIN_LM35);            // 0..1023
    float voltaje = lectura * (VREF / 1023.0);     // en Volts
    float tempC   = voltaje * 100.0;               // LM35: 10 mV/°C -> *100

    // ---- Salida por Serial ----
    Serial.print(F("Temperatura: "));
    Serial.print(tempC, 2);
    Serial.println(F(" °C"));

    // (Opcional) imprime también el ADC y el voltaje
    // Serial.print(F("ADC: ")); Serial.print(lectura);
    // Serial.print(F("  | V: ")); Serial.println(voltaje, 3);

    // ---- Mostrar en OLED ----
    display.clearDisplay();        // Limpia antes de dibujar para evitar ghosting
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println(F("LM35 - Lectura"));

    display.setTextSize(2);        // Grande para la temperatura
    display.setCursor(0, 42);
    display.print(tempC, 1);
    display.print((char)247);      // simbolo °
    display.print(F("C"));

    display.display();             // Refresca la pantalla
  }

  // Aquí puedes hacer otras tareas no bloqueantes
}