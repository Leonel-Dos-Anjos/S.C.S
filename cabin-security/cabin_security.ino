#include <Keypad.h>
#include <Adafruit_Fingerprint.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

// ——— Definições de pinos ———
#define BUZZER_PIN    12
#define DHT_PIN       7
#define FLAME_PIN     A0
#define MOTOR_PIN1    9    // L293D IN1
#define MOTOR_PIN2    10   // L293D IN2

// ——— Keypad setup ———
const byte ROWS = 4, COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}   // 'D' = ENTER
};
byte rowPins[ROWS] = {22,23,24,25};
byte colPins[COLS] = {26,27,28,29};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ——— Fingerprint sensor using Serial1 (Mega) ———
Adafruit_Fingerprint finger(&Serial1);

// ——— DHT11 sensor ———
#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);

// ——— LCD 16×2 ———
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ——— Passwords ———
const char* unlockPass = "1234";
const char* lockPass   = "4321";
const char* enrollCode = "0000";
char inputPass[5];
uint8_t passIndex = 0;
uint8_t wrongAttempts = 0;

// ——— Enrollment buffer ———
char enrollBuffer[4];
uint8_t enrollIdx = 0;

// ——— Flame alert control ———
const int FLAME_THRESHOLD = 500;
const unsigned long ALERT_INTERVAL = 5000;
unsigned long lastFlameAlert = 0;

// ——— Setup ———
void setup() {
  Serial.begin(9600);
  Serial1.begin(57600);             // initialize Serial1 for fingerprint
  finger.begin(57600);
  dht.begin();

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(MOTOR_PIN1, OUTPUT);
  pinMode(MOTOR_PIN2, OUTPUT);
  pinMode(FLAME_PIN, INPUT);

  lcd.init();
  lcd.backlight();
  showPrompt();
}

// ——— Main loop ———
void loop() {
  checkSensors();
  char key = keypad.getKey();
  if (key) handleKey(key);
}

// ——— Show default prompt ———
void showPrompt() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Digite a senha:");
  passIndex = 0;
  memset(inputPass, 0, sizeof(inputPass));
}

// ——— Sensor reading ———
void checkSensors() {
  float temp = dht.readTemperature();
  float humid = dht.readHumidity();
  Serial.print("TEMP:"); Serial.print(temp);
  Serial.print(",HUMID:"); Serial.print(humid);

  int flameVal = analogRead(FLAME_PIN);
  Serial.print(",FLAME:");
  if (flameVal < FLAME_THRESHOLD) {
    Serial.println("ALERT");
    unsigned long now = millis();
    if (now - lastFlameAlert > ALERT_INTERVAL) {
      lastFlameAlert = now;
      lcd.clear(); lcd.setCursor(0,0); lcd.print("!! CHAMA !!");
      tone(BUZZER_PIN, 2000, 500);
      delay(1000);
      showPrompt();
    }
  } else {
    Serial.println("NORMAL");
  }
}

// ——— Handle keypad ———
void handleKey(char key) {
  if (key == 'D') {
    inputPass[passIndex] = '\0';
    if (strcmp(inputPass, unlockPass) == 0) { unlockDoor();   resetWrong(); Serial.println("EVENT:DOOR:UNLOCK"); }
    else if (strcmp(inputPass, lockPass  ) == 0) { lockDoor();     resetWrong(); Serial.println("EVENT:DOOR:LOCK"); }
    else if (strcmp(inputPass, enrollCode) == 0) { promptEnroll(); resetWrong(); }
    else { wrongPass(); Serial.println("EVENT:AUTH_FAIL"); }
    showPrompt();
    return;
  }
  if (key == '*') {
    resetWrong();
    if (checkFingerprint()) { unlockDoor(); resetWrong(); Serial.println("EVENT:AUTH_SUCCESS"); }
    else                     { wrongPass(); Serial.println("EVENT:AUTH_FAIL"); }
    showPrompt();
    return;
  }
  if (passIndex < 4 && isDigit(key)) {
    inputPass[passIndex++] = key;
    lcd.setCursor(passIndex-1,1);
    lcd.print('*');
  }
}

// ——— Reset wrong attempts ———
void resetWrong() {
  wrongAttempts = 0;
}

// ——— Door control ———
void unlockDoor() {
  lcd.clear(); lcd.setCursor(0,0); lcd.print("Porta Aberta");
  digitalWrite(MOTOR_PIN1, HIGH);
  digitalWrite(MOTOR_PIN2, LOW);
  delay(1000);
  digitalWrite(MOTOR_PIN1, LOW);
  digitalWrite(MOTOR_PIN2, LOW);
}
void lockDoor() {
  lcd.clear(); lcd.setCursor(0,0); lcd.print("Porta Trancada");
  digitalWrite(MOTOR_PIN1, LOW);
  digitalWrite(MOTOR_PIN2, HIGH);
  delay(1000);
  digitalWrite(MOTOR_PIN1, LOW);
  digitalWrite(MOTOR_PIN2, LOW);
}

// ——— Wrong password handler ———
void wrongPass() {
  lcd.clear(); lcd.setCursor(0,0); lcd.print("Senha Incorreta");
  tone(BUZZER_PIN, 1500, 500);
  if (++wrongAttempts >= 4) {
    lcd.clear(); lcd.setCursor(0,0); lcd.print("Contacte Adm!");
    while(true);
  }
}

// ——— Fingerprint auth ———
bool checkFingerprint() {
  lcd.clear(); lcd.setCursor(0,0); lcd.print("Scan Impressao");
  if (finger.getImage() != FINGERPRINT_OK) return false;
  if (finger.image2Tz(1) != FINGERPRINT_OK) return false;
  if (finger.fingerFastSearch() != FINGERPRINT_OK) return false;
  return true;
}

// ——— Enrollment prompt ———
void promptEnroll() {
  lcd.clear(); lcd.setCursor(0,0); lcd.print("Enroll ID:");
  lcd.setCursor(0,1);
  enrollIdx = 0;
  memset(enrollBuffer, 0, sizeof(enrollBuffer));
  while (true) {
    char k = keypad.getKey();
    if (!k) continue;
    if (k == 'D') {
      enrollBuffer[enrollIdx] = '\0';
      uint8_t id = atoi(enrollBuffer);
      if (id > 0) enrollFingerprint(id);
      break;
    }
    if (enrollIdx < 3 && isDigit(k)) {
      enrollBuffer[enrollIdx++] = k;
      lcd.print(k);
    }
  }
}

// ——— Enrollment ———
void enrollFingerprint(uint8_t id) {
  lcd.clear(); lcd.setCursor(0,0); lcd.print("Coloque o dedo");
  delay(2000);
  if (finger.getImage() != FINGERPRINT_OK || finger.image2Tz(1) != FINGERPRINT_OK) {
    lcd.clear(); lcd.print("Erro Leitura 1"); return;
  }
  lcd.clear(); lcd.setCursor(0,0); lcd.print("Retire e repita");
  delay(2000);
  if (finger.getImage() != FINGERPRINT_OK || finger.image2Tz(2) != FINGERPRINT_OK) {
    lcd.clear(); lcd.print("Erro Leitura 2"); return;
  }
  if (finger.createModel() != FINGERPRINT_OK) {
    lcd.clear(); lcd.print("Model Fail"); return;
  }
  if (finger.storeModel(id) == FINGERPRINT_OK) {
    lcd.clear(); lcd.setCursor(0,0); lcd.print("Enroll OK ID:");
    lcd.setCursor(0,1); lcd.print(id);
    Serial.print("EVENT:ENROLL,ID:"); Serial.println(id);
  } else {
    lcd.clear(); lcd.print("Erro Salvar");
  }
  delay(1500);
}
