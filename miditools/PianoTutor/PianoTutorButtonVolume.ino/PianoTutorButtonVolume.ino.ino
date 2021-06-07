/* Piano Tutor - A piano tutoring/learning system based on MuseScore and LEDs.
 *
 * Copyright (C) 2021  Pedro Parrilla
 *
 * This program, is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License
 * as published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307, USA
 */

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include <ButtonSMP.h>

#include <HID-Project.h>

// Pin de control de la tira led
#define PIN            7

// Numero de Pines enchufados a la tira led
#define NUMPIXELS      144

// Creamos el objeto de la clase NeoPixel

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

uint32_t col_white, col_black, col_off;


// Metronomo y control de reproduccion

const int volumePin = A0;
int volumeValue = 10;
int t = 0;
Button buttonPlay = Button(2, PULLUP);
Button buttonInit = Button(3, PULLUP);
Button buttonSetLoopIntervals = Button(4, PULLUP);
Button buttonLoop = Button(5, PULLUP);

bool posLoop = true;


void setup() {
  col_white = pixels.Color(50, 50, 50);
  col_black = pixels.Color(10, 10, 50);
  col_off = pixels.Color(0, 0, 0);
  pixels.begin();
  pixels.show();
  pinMode(volumePin, INPUT);
  // For control volume
  Consumer.begin();
  Keyboard.begin();


  Serial.begin(115200);
  while (!Serial);
  Serial.println("PianoTutor v0.2 is ready!");
}

char buf[9];  // Codificado en HEX, utilizando el formato key+rgb+sep: kkrrggbbs

int char2hex(char c) {
  if (c >= '0' && c <= '9')
    return c - '0';
  else if (c >= 'a' && c <= 'f')
    return 10 + c - 'a';
  else if (c >= 'A' && c <= 'F')
    return 10 + c - 'A';
}

int readVolume() {
  int value = analogRead(volumePin);
  value = map(value, 0, 1023, 0, 20);
  if (volumeValue < value) {
    Consumer.write(MEDIA_VOL_UP);
    volumeValue = value;
  } else if (volumeValue > value) {
    volumeValue = value;
    Consumer.write(MEDIA_VOL_DOWN);
  }
}

void buttonAction(char action) {
  if (action == 'P')
    Keyboard.press(' ');
  else if (action == 'R'){
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('/');
  } else if (action == 'L')
    Keyboard.press('l');
  else if (action == '[') {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('[');
    posLoop = false;
  } else if (action == ']') {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(']');
    posLoop = true;
  }
  delay(100);
  Keyboard.releaseAll();
}

void loop() {
  if (millis() + 5 > t){
    readVolume();
  }
  if (buttonPlay.uniquePress())
    buttonAction('P');
  if (buttonInit.uniquePress())
    buttonAction('R');
  if (buttonLoop.uniquePress())
    buttonAction('L');
  if (buttonSetLoopIntervals.uniquePress())
    if(posLoop)
      buttonAction('[');
    else
      buttonAction(']');

  int k, r, g, b;
  int avail = Serial.available();
  if (avail == 0)
    return;
  char ch = Serial.peek();

  if (ch != 'k' && ch != 'K' && ch != 'h' && ch != 'H' && ch != 'c' && ch != 'C' && ch != 'F' && ch != 'P') {

    Serial.read(); // discard non-recognized command
    return;
  }
  if ((ch == 'k' || ch == 'K') && avail >= 16) {
    Serial.read(); // skip 'k'
    k = Serial.parseInt();
    Serial.read(); // skip 'r'
    r = Serial.parseInt();
    Serial.read(); // skip 'g'
    g = Serial.parseInt();
    Serial.read(); // skip 'b'
    b = Serial.parseInt();
    pixels.setPixelColor(k, r, g, b);
    if (ch == 'k') {
      pixels.show();
    }
  } else if ((ch == 'h' || ch == 'H') && avail >= 9) {
    Serial.read(); // skip 'h'
    Serial.readBytes(buf, 8);

    k = (char2hex(buf[0]) << 4) | char2hex(buf[1]);
    r = (char2hex(buf[2]) << 4) | char2hex(buf[3]);
    g = (char2hex(buf[4]) << 4) | char2hex(buf[5]);
    b = (char2hex(buf[6]) << 4) | char2hex(buf[7]);

    pixels.setPixelColor(k, r, g, b);
    if (ch == 'h') {
      pixels.show();
    }
  } else if (ch == 'F') {
    Serial.read(); // skip 'F';
    pixels.show();
  } else if (ch == 'c' || ch == 'C') {
    Serial.read(); // skip 'c';
    for (int k = 0; k < NUMPIXELS; ++k)
      pixels.setPixelColor(k, 0, 0, 0);
    if (ch == 'c')
      pixels.show();
  } else if (ch == 'P') {
    Serial.read(); // skip 'P';
    Serial.write('P');
    Serial.flush();
  }
}
