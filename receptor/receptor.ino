#include <VirtualWire.h>
#include <LiquidCrystal.h>

const int dataPin = 9;
const int pinBuzzer = 10;
const int ledPin = 13;
int keepAlive = 0;
int enter = 0;
int dataInt = 1024;
int antDataInt = 0;
int llueve = 0;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
String mode = "normal";
void setup()
{
  Serial.begin(9600);
  vw_setup(2000);
  vw_set_rx_pin(dataPin);
  vw_rx_start();
  tone(pinBuzzer, 440);
  delay(700);
  tone(pinBuzzer, 670);
  delay(700);
  tone(pinBuzzer, 880);
  delay(700);

  //detener tono durante 500ms
  noTone(pinBuzzer);
  delay(500);
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Hola");
  delay(5000);
  lcd.clear();
  lcd.print("Menos de 400");
  lcd.setCursor(0, 1);
  lcd.print("significa lluvia");
  delay(5000);
  lcd.clear();
  lcd.print("Men.400 = lluvia");
}

void loop()
{
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  Serial.println(mode);

  enter = vw_wait_rx_max(800);
  if (enter) {
    keepAlive = 0;
    vw_get_message((uint8_t *)buf, &buflen);
    String dataString;
    if ((char)buf[0] == 'k') {
      for (int i = 1; i < buflen; i++) {
        dataString.concat((char)buf[i]);
      }
      dataInt = dataString.toInt();  // Convertir a int
      lcd.setCursor(0, 1);
      lcd.print("                ");
      if (llueve == 0) {
        lcd.setCursor(0, 1);
        lcd.print(dataInt);
      } else {
        lcd.setCursor(0, 1);
        lcd.print("Llueve! :O");
      }
      if ((antDataInt / 2 > dataInt) || dataInt < 400) {
        tone(pinBuzzer, 440);
        delay(200);
        tone(pinBuzzer, 880);
        delay(200);
        tone(pinBuzzer, 880);
        delay(200);
        //detener tono durante 500ms
        noTone(pinBuzzer);
        delay(500);
        llueve = 1;
      } else {
        llueve = 0;
        antDataInt = dataInt;
      }

    } else if ((char)buf[0] == 's') {
      mode = "short";
    } else if ((char)buf[0] == 'l') {
      mode = "long";
    }
  } else {
    keepAlive++;
    if ((keepAlive > 20) && mode == "normal") {
      lcd.setCursor(0, 1);
      lcd.print("No hay conexion");
    }
    if ((keepAlive > 40) && mode == "short") {
      lcd.setCursor(0, 1);
      lcd.print("No hay conexion");
    }
    if ((keepAlive > 80) && mode == "long") {
      lcd.setCursor(0, 1);
      lcd.print("No hay conexion");
    }
  }
}
