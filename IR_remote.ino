//Inkluderar bibliotek
#include <IRremote.h>
#include "U8glib.h"

//Förbereder PINs för att skicka och ta emot signal, osv.
const int RECV_PIN = 11;
const int PIN_SEND = 3;
IRrecv irrecv(RECV_PIN);

//Variabel för resultat från IR recievern
decode_results results;

//Initierar variabler för sparandet av HEX-koder osv.
uint32_t copied_code = 0xE0E0D02F;
uint32_t hexValue1 = 0xE0E0D02F;
uint32_t hexValue2 = 0xEECF44F4;
uint32_t hexValue3 = 0xD051C300;
String type_code;

//Initiera oled
U8GLIB_SSD1306_128X64 oled(U8G_I2C_OPT_NONE);

void setup() {
  Serial.begin(9600);

  //Setup för IR-reviever och IR-lampa
  irrecv.enableIRIn();
  irrecv.blink13(true);
  IrSender.begin(PIN_SEND);

  //Bestämmer PINMODE
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);

  //Oled skärm textfont
  oled.setFont(u8g_font_helvB08);
}

void loop() {

  //Kallar på funktionen updateoled
  updateoled(type_code, copied_code, hexValue1, hexValue2, hexValue3);

  //Lagrar senaste värde från recievern som har känd krypteringstyp
  if ((results.decode_type != UNKNOWN) && (results.value != 0xFFFFFFFF)) {
    copied_code = results.value;
  }

  //Kollar vilken krypteringstyp senaste värde från recievern har och skriver ut i serial monitorn samt till variabeln type_code
  if (irrecv.decode(&results)) {
    switch (results.decode_type) {
      case NEC:
        Serial.print("NEC: ");
        type_code = "NEC: ";
        break;
      case SONY:
        Serial.print("SONY: ");
        type_code = "SONY: ";
        break;
      case RC5:
        Serial.print("RC5: ");
        type_code = "RC5: ";
        break;
      case RC6:
        Serial.print("RC6: ");
        type_code = "RC6: ";
        break;
      case SAMSUNG:
        Serial.print("SAMSUNG: ");
        type_code = "SAMSUNG: ";
        break;
      case UNKNOWN:
        Serial.print("UNKNOWN: ");
        //type_code = "UNKNOWN: ";
        break;
      default:
        Serial.print(results.decode_type);
        //type_code = "UNK: ";
        break;
    }
    // ... (Lägg till fler avkodnings-typer vid behov)

    Serial.println(results.value, HEX);
    irrecv.resume();
  }

  //Då kopieringsknappen är nedtryckt lagras copied_code till knapp
  if (digitalRead (8) == 1) {

    if (digitalRead (5) == 1) {
      hexValue1 = uint32_t (copied_code);
      Serial.print("New code: ");
      Serial.println(hexValue1, HEX);
      delay(2000);
    }
    else if (digitalRead (6) == 1) {
      hexValue2 = uint32_t (copied_code);
      Serial.print("New code: ");
      Serial.println(hexValue2, HEX);
      delay(2000);
    }
    else if (digitalRead (7) == 1) {
      hexValue3 = uint32_t (copied_code);
      Serial.print("New code: ");
      Serial.println(hexValue3, HEX);
      delay(2000);
    }
  }

  //Skickar IR signal då kopieringsknappen inte är nedtryckt
  //send_code(hexValue1, hexValue2, hexValue3);
  if (digitalRead (8) != 1) {
    if (digitalRead (5) == 1) {
      IrSender.sendNEC(hexValue1, 32);
      delay (25);
    }

    if (digitalRead (6) == 1) {
      IrSender.sendNEC(hexValue2, 32);
      delay (25);
    }

    if (digitalRead (7) == 1) {
      IrSender.sendNEC(hexValue3, 32);
      delay (25);
    }
  }

}

/*
  Skriver ut lagrade HEX-värden på oled-skärmen
  Parametrar:
    - String code_type
    - uint32_t copied_code
    - uint32_t value_1
    - uint32_t value_2
    - uint32_t value_3
  returnerar: void
*/
void updateoled(String code_type, uint32_t copied_code, uint32_t value_1, uint32_t value_2, uint32_t value_3) {
  oled.firstPage();

  //Bestämmer vart på skärmen varje stycke text ska stå.
  do {
    oled.drawStr(0, 10, (code_type + String(copied_code, HEX)).c_str());
    oled.drawStr(0, 25, ("Knapp 1: " + String(value_1, HEX)).c_str());
    oled.drawStr(0, 40, ("Knapp 2: " + String(value_2, HEX)).c_str());
    oled.drawStr(0, 55, ("Knapp 3: " + String(value_3, HEX)).c_str());

  } while (oled.nextPage());
}

//String find_type(uint32_t org_data) {

//}

//void send_code(uint32_t hexValue1, uint32_t hexValue2, uint32_t hexValue3) {

//}
