// === IIR Filter Example for Raspberry Pi Pico 2 W ===
// Works in Arduino IDE using Earle Philhower's "Raspberry Pi Pico/RP2350" core
// Reads analog input (GPIO26 / ADC0), applies IIR filter, and prints filtered output via Serial

#define NUM_B 4
#define NUM_A 4

// === IIR filter coefficients (example low-pass filter) ===
float b[NUM_B] = { 1.0765251389e-02f, 3.2295754166e-02f, 3.2295754166e-02f, 1.0765251389e-02f };
float a[NUM_A] = { 1.0000000000e+00f, -1.8337559949e+00f, 1.1859656881e+00f, -2.6608761631e-01f };

// Delay buffers
float x_hist[NUM_B] = {0};
float y_hist[NUM_A] = {0};

// === Direct Form I IIR filter ===
float iir_process(float x) {
  // Shift history
  for (int i = NUM_B - 1; i > 0; i--) x_hist[i] = x_hist[i - 1];
  for (int i = NUM_A - 1; i > 0; i--) y_hist[i] = y_hist[i - 1];

  x_hist[0] = x;

  float y = 0;
  for (int i = 0; i < NUM_B; i++) y += b[i] * x_hist[i];
  for (int i = 1; i < NUM_A; i++) y -= a[i] * y_hist[i];

  y_hist[0] = y;
  return y;
}

void setup() {
  Serial.begin(115200);
  delay(2000);  // Wait for Serial monitor to connect

  analogReadResolution(10);
  Serial.println("Pico 2 W IIR Filter started...");
}

void loop() {
  const uint32_t sampleIntervalMicros = 250;
  static uint32_t lastMicros = 0;

  // Wait until next sample time
  if (micros() - lastMicros >= sampleIntervalMicros) {
    lastMicros += sampleIntervalMicros;

    // === Read input ===
    int raw = analogRead(A0);   // GPIO26 = ADC0
    float input = (float)raw * 3.3f / 4095.0f; // Convert to voltage (0–3.3V)

    // === Apply IIR filter ===
    float output = iir_process(input);

    // === Output filtered sample ===
    Serial.println(output);
    Serial.println(input);
  }
}