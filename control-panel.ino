#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

// RFID Reader Pins
// IRQ              UNCONNECTED
// SCK              13
// MISO             12
// MOSI             11
#define SDA_PIN 10
#define RST_PIN 9

// LCD I2C Address
#define LCD_ADDRESS 0x27

MFRC522 mfrc522(SDA_PIN, RST_PIN);
LiquidCrystal_I2C lcd(LCD_ADDRESS, 16, 2);

const String IDS[10][2] = {{"Lawrence", "8E DD E7 C6"},
                  {"Ryan", "DE 23 93 C0"},
                  {"Vhenise", "EE D2 92 C0"},
                  {"Jerome", "3E 38 E1 C6"},
                  {"Sophia", "CE 83 90 C0"},
                  {"David", "FE 23 AE C0"},
                  {"Aouien", "CE FB F4 C5"},
                  {"Nicole", "DE AE B0 C0"},
                  {"Card 1", "82 49 39 51"},
                  {"Card 2", "F9 A5 56 B2"}};

int rfid_scanned = 0;

void rfid_display() {
  lcd.clear();
  lcd.print("Scan Access Card");
}

void rfid_check() {
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) return;

  //Show UID on serial monitor
  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  content = content.substring(1, content.length());

  String _active = "";

  for (byte e = 0; e < 10; e++) {
    if (content == IDS[e][1]) {
      _active = IDS[e][0];
      rfid_scanned++;
      break;
    }
    else {
      _active = "Unknown Card";
    }
  }
  rfid_display();
  lcd.setCursor(0, 1);
  lcd.print(_active);
  Serial.print(rfid_scanned);
  delay(1000);
}

void setup() {
  Serial.begin(9600);
  Wire.begin();

  lcd.init();
  lcd.backlight();
  SPI.begin();
  mfrc522.PCD_Init();

  rfid_display();
}

void loop() {
  // put your main code here, to run repeatedly:
  rfid_check();

  if (rfid_scanned >= 2) {
    lcd.clear();
    lcd.print("Turn on Switches");
    delay(1500);
  }
}
