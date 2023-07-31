#include <iostream>
#define OLIVEC_IMPLEMENTATION
#include "olive.c"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define PI 3.14159265359

#define WIDTH 960
#define HEIGHT 720
#define BACKGROUND_COLOR 0xFF181818
#define GRID_COUNT 10
#define GRID_PAD 0.5 / GRID_COUNT
#define GRID_SIZE ((GRID_COUNT - 1) * GRID_PAD)
#define CIRCLE_RADIUS 5
#define Z_START 0.25
#define ABOBA_PADDING 50

uint32_t pixels[WIDTH * HEIGHT];

static float angle = 0;

Olivec_Canvas vc_render(float dt) {
  angle += 0.25 * PI * dt;

  Olivec_Canvas oc = olivec_canvas(pixels, WIDTH, HEIGHT, WIDTH);

  olivec_fill(oc, BACKGROUND_COLOR);
  for (int ix = 0; ix < GRID_COUNT; ++ix) {
    for (int iy = 0; iy < GRID_COUNT; ++iy) {
      for (int iz = 0; iz < GRID_COUNT; ++iz) {
        float x = ix * GRID_PAD - GRID_SIZE / 2;
        float y = iy * GRID_PAD - GRID_SIZE / 2;
        float z = Z_START + iz * GRID_PAD;

        float cx = 0.0;
        float cz = Z_START + GRID_SIZE / 2;

        float dx = x - cx;
        float dz = z - cz;

        float a = atan2f(dz, dx);
        float m = sqrtf(dx * dx + dz * dz);

        dx = cosf(a + angle) * m;
        dz = sinf(a + angle) * m;

        x = dx + cx;
        z = dz + cz;

        x /= z;
        y /= z;

        uint32_t r = ix * 255 / GRID_COUNT;
        uint32_t g = iy * 255 / GRID_COUNT;
        uint32_t b = iz * 255 / GRID_COUNT;
        uint32_t color =
            0xFF000000 | (r << (0 * 8)) | (g << (1 * 8)) | (b << (2 * 8));
        olivec_circle(oc, (x + 1) / 2 * WIDTH, (y + 1) / 2 * HEIGHT,
                      CIRCLE_RADIUS, color);
      }
    }
  }

  return oc;
}

int main() {
  vc_render(0.3f);

  const char *file_path = "flag_jp.png";
  if (!stbi_write_png(file_path, WIDTH, HEIGHT, 4, pixels,
                      sizeof(uint32_t) * WIDTH)) {
    fprintf(stderr, "ERROR: could not write %s\n", file_path);
    return 1;
  }

  return 0;
}