#include <Adafruit_NeoPixel.h>

#define BIG_POOL

#ifdef BIG_POOL
#define NUM_STRIPS 2
#define NUM_LEDS_PER_STRIP 380
#else
#define NUM_STRIPS 1
#define NUM_LEDS_PER_STRIP 324
#endif

#define THIRD NUM_LEDS_PER_STRIP / 3
#define TWO_THIRDS 2 * THIRD

struct CRGB
{
  uint8_t r, g, b;

    // allow construction from R, G, B
    inline CRGB( uint8_t ir, uint8_t ig, uint8_t ib)  __attribute__((always_inline))
        : r(ir), g(ig), b(ib)
    {
    }

    inline CRGB()  __attribute__((always_inline))
      : r(0), g(0), b(0)
    {
    }
};


Adafruit_NeoPixel *strip[NUM_STRIPS];
CRGB c1_b, c2_b, c3_b;
uint16_t offset = 0;

void setup() {
  for (uint8_t i = 0; i < NUM_STRIPS; i++) {
    strip[i] = new Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 6, NEO_GRB + NEO_KHZ800);
    if (strip[i]) {
      strip[i]->begin();
      strip[i]->show(); // Initialize all pixels to 'off'
    }
  }

  c1_b = CRGB(255, 0, 0);
  c2_b = CRGB(0, 255, 0);
  c3_b = CRGB(0, 0, 255);
}

void loop() {
  CRGB c1_e = CRGB(random(0, 255), random(0, 160), random(0, 160));
  CRGB c2_e = CRGB(random(0, 160), random(0, 255), random(0, 160));
  CRGB c3_e = CRGB(random(0, 160), random(0, 160), random(0, 255));
  CRGB c1, c2;

  for (uint8_t fade = 0; fade < 255; fade++) {
    for (uint16_t i = 0; i < NUM_LEDS_PER_STRIP; i++) {
      c1 = getColorInGradient(i, c1_b, c2_b, c3_b);
      c2 = getColorInGradient(i, c1_e, c2_e, c3_e);

      for (uint8_t s = 0; s < NUM_STRIPS; s++) {
        uint16_t loc;
        if (s % 2 == 0) {
          loc = (i + offset) % NUM_LEDS_PER_STRIP;
        } else {
          loc = NUM_LEDS_PER_STRIP - ((i + offset) % NUM_LEDS_PER_STRIP);
        }

        if (strip[s]) {
          strip[s]->setPixelColor(loc,
            blend(c1.r, c2.r, fade),
            blend(c1.g, c2.g, fade),
            blend(c1.b, c2.b, fade));
        }
      }
    }

    for (uint8_t s = 0; s < NUM_STRIPS; s++) {
      if (strip[s]) {
        strip[s]->show();
      }
    }

    if (++offset > NUM_LEDS_PER_STRIP) {
      offset = 0;
    }
  }

  // After reaching the end of the fade, the end becomes the beginning
  c1_b = c1_e;
  c2_b = c2_e;
  c3_b = c3_e;
}

void fillBuffer(struct CRGB *buffer, struct CRGB c1, struct CRGB c2, struct CRGB c3) {
  for (uint16_t led = 0; led < NUM_LEDS_PER_STRIP; led++) {
    buffer[led] = getColorInGradient(led, c1, c2, c3);
  }
}

struct CRGB getColorInGradient(uint16_t led, struct CRGB c1, struct CRGB c2, struct CRGB c3) {
  CRGB color;

  if (led < THIRD) {
    color.r = (((THIRD - 1 - led) * c1.r) / THIRD) + ((led * c2.r) / THIRD);
    color.g = (((THIRD - 1 - led) * c1.g) / THIRD) + ((led * c2.g) / THIRD);
    color.b = (((THIRD - 1 - led) * c1.b) / THIRD) + ((led * c2.b) / THIRD);
  } else if (led < TWO_THIRDS) {
    color.r = (((TWO_THIRDS - 1 - led) * c2.r) / THIRD) + (((led - THIRD) * c3.r) / THIRD);
    color.g = (((TWO_THIRDS - 1 - led) * c2.g) / THIRD) + (((led - THIRD) * c3.g) / THIRD);
    color.b = (((TWO_THIRDS - 1 - led) * c2.b) / THIRD) + (((led - THIRD) * c3.b) / THIRD);
  } else {
    color.r = (((NUM_LEDS_PER_STRIP - 1 - led) * c3.r) / THIRD) + (((led - TWO_THIRDS) * c1.r) / THIRD);
    color.g = (((NUM_LEDS_PER_STRIP - 1 - led) * c3.g) / THIRD) + (((led - TWO_THIRDS) * c1.g) / THIRD);
    color.b = (((NUM_LEDS_PER_STRIP - 1 - led) * c3.b) / THIRD) + (((led - TWO_THIRDS) * c1.b) / THIRD);
  }

  return color;
}

uint8_t blend(uint8_t c1, uint8_t c2, uint8_t fade) {
  return ((c1 * (255 - fade)) / 255) + ((c2 * fade) / 255);
}
