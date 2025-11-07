// EMS 2022: digital fitler implementation in ESP32

#define DOUT_PIN 0 
#define ADC_PIN A0 
#define M 22 //filter order
float x[M+1] = {0.0};
static double b[M+1] = {
  -0.00863448585734789,
  -0.013030452148334266,
  -0.01796815420481921,
  -0.018669616269555444,
  -0.011725361318956285,
  0.005315666039370919,
  0.0327986436242884,
  0.06829687143817895,
  0.1067441375907684,
  0.14141062316160696,
  0.1655651730489609,
  0.17422454975214038,
  0.1655651730489609,
  0.14141062316160696,
  0.1067441375907684,
  0.06829687143817895,
  0.0327986436242884,
  0.005315666039370919,
  -0.011725361318956285,
  -0.018669616269555444,
  -0.01796815420481921,
  -0.013030452148334266,
  -0.00863448585734789
};

void setup() {
  Serial.begin(115200);
  pinMode(ADC_PIN, INPUT);
  pinMode(DOUT_PIN, OUTPUT);
  analogWriteFreq(4000);      
  analogWriteRange(1023);     
}

void loop() {
  // 1. Acquire ADC sample (convert to volts)
  x[0] = analogRead(ADC_PIN) * 3.3 / 4095.0;  // ESP32 ADC is 12-bit (0–4095)

  // 2. Create FIR output signal
  float y0 = 0.0;
  for (int i = 0; i <= M; i++) {
    y0 += b[i] * x[i];
  }

  int pwmValue = (int)(constrain(y0, 0.0, 3.3) * 1023.0 / 3.3);
  analogWrite(DOUT_PIN, pwmValue);

  // 4. Print input/output for monitoring
  Serial.print(x[0]);
  Serial.print(" ");
  Serial.println(y0);

  // 5. Save current input value and shift every old input by 1
  for (int i = M; i > 0; i--) {
    x[i] = x[i - 1];
  }

  delayMicroseconds(200);
}