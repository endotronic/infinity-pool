#include <Adafruit_NeoPixel.h>
#define NUM_LEDS 324
#define THIRD NUM_LEDS/3
#define TWO_THIRDS 2*THIRD

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


Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, 6, NEO_GRB + NEO_KHZ800);
CRGB c1_b, c2_b, c3_b, strip1[NUM_LEDS], strip2[NUM_LEDS];
uint8_t offset = 0;

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  c1_b = CRGB(255, 0, 0);
  c2_b = CRGB(0, 255, 0);
  c3_b = CRGB(0, 0, 255);
}

void loop() {
  CRGB c1_e = CRGB(random(0, 160), random(0, 120), random(0, 120));
  CRGB c2_e = CRGB(random(0, 120), random(0, 160), random(0, 120));
  CRGB c3_e = CRGB(random(0, 120), random(0, 120), random(0, 160));
  fillBuffer(strip1, c1_b, c2_b, c3_b);
  fillBuffer(strip2, c1_e, c2_e, c3_e);

  for (uint8_t fade = 0; fade < 255; fade++) {
  for (uint8_t betweenFades = 0; betweenFades < 5; betweenFades++) {
    for (uint8_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor((i + offset) % NUM_LEDS,
          blend(strip1[i].r, strip2[i].r, fade),
          blend(strip1[i].g, strip2[i].g, fade),
          blend(strip1[i].b, strip2[i].b, fade));
    }

    strip.show();
    delay(10);

    if (++offset > NUM_LEDS) {
      offset = 0;
    }
  }
  }

  c1_b = c1_e;
  c2_b = c2_e;
  c3_b = c3_e;
}

void fillBuffer(struct CRGB *buffer, struct CRGB c1, struct CRGB c2, struct CRGB c3) {
  for (uint16_t led = 0; led < NUM_LEDS; led++) {
    if (led < THIRD) {
      buffer->r = (((THIRD - 1 - led) * c1.r) / THIRD) + ((led * c2.r) / THIRD);
      buffer->g = (((THIRD - 1 - led) * c1.g) / THIRD) + ((led * c2.g) / THIRD);
      buffer->b = (((THIRD - 1 - led) * c1.b) / THIRD) + ((led * c2.b) / THIRD);
      buffer++;
    } else if (led < TWO_THIRDS) {
      buffer->r = (((TWO_THIRDS - 1 - led) * c2.r) / THIRD) + (((led - THIRD) * c3.r) / THIRD);
      buffer->g = (((TWO_THIRDS - 1 - led) * c2.g) / THIRD) + (((led - THIRD) * c3.g) / THIRD);
      buffer->b = (((TWO_THIRDS - 1 - led) * c2.b) / THIRD) + (((led - THIRD) * c3.b) / THIRD);
      buffer++;
    } else {
      buffer->r = (((NUM_LEDS - 1 - led) * c3.r) / THIRD) + (((led - TWO_THIRDS) * c1.r) / THIRD);
      buffer->g = (((NUM_LEDS - 1 - led) * c3.g) / THIRD) + (((led - TWO_THIRDS) * c1.g) / THIRD);
      buffer->b = (((NUM_LEDS - 1 - led) * c3.b) / THIRD) + (((led - TWO_THIRDS) * c1.b) / THIRD);
      buffer++;
    }
  }
}

uint8_t blend(uint8_t c1, uint8_t c2, uint8_t fade) {
  return ((c1 * (255 - fade)) / 255) + ((c2 * fade) / 255);
}
