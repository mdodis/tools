#include "../colors.h"

#ifndef COLOR_S_HH
typedef struct{
    float r, g, b;
}NRGB;
#endif

#define global static

#if !defined(CAIRO_HAS_PNG_FUNCTIONS)
#error "CAIRO_HAS_PNG_FUNCTIONS"
#endif

/* Window width and height in pixels */
const long WINDOW_DIM[2] = {175, 175};
/* Progress bar height in pixels */
const long BAR_HEIGHT = WINDOW_DIM[1];
/* Progress bar offset
Set BAR_HEIGHT to WINDOW_DIM[1] to have it
take up the full part of the screen */
const long BAR_YOFFSET = (WINDOW_DIM[1] - BAR_HEIGHT);
const double BAR_ALPHA = 0.75;

/* Image offset from top left corner of the encompassing window */
const double IMAGE_OFFSET[2] = {50, 50};
/* RGBA color (normalized) of the background */
#define BACKDROP_COLOR .2, .2, .2, .95
