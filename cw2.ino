#include <Time.h>
#include <TimeLib.h>
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include <EEPROM.h>

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// These #defines make it easy to set the backlight color
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

struct Pet {
  int dev;  //development
  int happ; //happiness
  int full; //fullness
  int age;
} pet;

unsigned int nextHappMillis = 12;
unsigned int nextFullMillis = 16;
unsigned long startTime = 0;

boolean pause = false;

void setup() {
  // Debugging output
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  EEPROM.get(0, pet);

  if (pet.age > 0) {
    while (true) {
      uint8_t buttons = lcd.readButtons();
      lcd.setCursor(0, 0);
      lcd.print("Load Pet -> UP");
      lcd.setCursor(0, 1);
      lcd.print("New Pet -> DOWN");
      if (buttons & BUTTON_UP) {
        lcd.clear();
        for (int i = pet.age; i > nextHappMillis; i -= 7) {
          nextHappMillis += 7;
        }
        for (int j = pet.age; j > nextFullMillis; j -= 11) {
          nextFullMillis += 11;
        }
      }
      if (buttons & BUTTON_DOWN) {
        pet.dev = 0;
        pet.happ = 2;
        pet.full = 3;
        pet.age = 0;
      }
      if (buttons) {
        break;
      }

    }
  }
  else {
    pet.dev = 0;
    pet.happ = 2;
    pet.full = 3;
    pet.age = 0;


  }

  lcd.setBacklight(WHITE);
}





uint8_t i = 0;
void loop() {

  if (pause) {

    lcd.setCursor(0, 0);
    lcd.print("Save ^");
    lcd.setCursor(8, 0);
    lcd.print("Delete >");
    lcd.setCursor(0, 1);
    lcd.print("New <");
    lcd.setCursor(8, 1);
    lcd.print("Resume V");

    uint8_t buttons = lcd.readButtons();

    if (buttons & BUTTON_UP) {
      EEPROM.put(0, pet);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Pet is saved");
      delay(1000);
      lcd.clear();


    }

    if (buttons & BUTTON_LEFT) {
      pause = false;
      lcd.clear();
      delay(500);
      pet.dev = 0;
      pet.happ = 2;
      pet.full = 3;
      pet.age = 0;
      nextHappMillis = 12;
      nextFullMillis = 16;


    }

    if (buttons & BUTTON_DOWN) {
      pause = false;
      lcd.clear();
      delay(500);
    }

    if (buttons & BUTTON_RIGHT) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Pet is cleared");
      for (int i = 0 ; i < EEPROM.length() ; i++) {
        EEPROM.write(i, 0);

      }
      lcd.clear();
    }



  }
  else {
    active();
  }


}

void active() {
  if (startTime + 1000 < millis()) {
    startTime = millis();
    pet.age++;
  }

  uint8_t buttons = lcd.readButtons();

  if (pet.dev < 1 && pet.age == 5) {
    pet.dev = 1;

  }
  if (pet.dev > 0) {
    if (pet.happ > 0 && pet.age >= nextHappMillis) {
      nextHappMillis += 7;
      pet.happ--;
    }

    if (pet.full > 0 && pet.age >= nextFullMillis) {
      nextFullMillis += 11;
      pet.full--;


    }    if (pet.full == 0) {
      pet.happ = 0;
    }
  }
  lcd.setCursor(0, 0);
  lcd.print("Age>");
  lcd.print(pet.age / 60);
  lcd.print(":");
  if (pet.age % 60 < 10) {
    lcd.print(0);
  }
  lcd.print(pet.age % 60);
  lcd.setCursor(0, 1);
  lcd.print("Stage>");
  lcd.print(pet.dev);
  lcd.setCursor(10, 0);
  lcd.print("Happ>");
  lcd.print(pet.happ);
  lcd.setCursor(10, 1);
  lcd.print("Full>");
  lcd.print(pet.full);


  if (buttons & BUTTON_UP && pet.dev > 0 ) {
    if (pet.full < 3) {
      pet.full++;
      nextFullMillis += 11;
    }
    else if (pet.full == 3) {
      pet.happ = 0;
    }
    delay(500);
  }

  if (buttons & BUTTON_LEFT && pet.happ < 2 && pet.full >= 2) {
    pet.happ++;
    nextHappMillis += 7;
    delay(500);
  }

  if (buttons & BUTTON_RIGHT && pet.age >= 35 && pet.happ >= 1 && pet.full >= 3) {
    pet.dev = 2;
    delay(500);
  }

  if (buttons & BUTTON_DOWN) {
    pause = true;
    delay(500);
    lcd.clear();
  }

}








