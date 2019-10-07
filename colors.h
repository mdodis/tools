#ifndef COLOR_S_HH
#define COLOR_S_HH

#define NEON_BLUE "#4D4DFF"
#define SPACE_CADET "#1E2952"
#define CADET_GREAY "#91A3B0"
#define MIKED_ORANGE "#DE621B"
#define DWM_CYAN "#005577"
#define BURGUNDY_RED "#5e000e"

#define SOFT_RED "#80283d"
#define SOFT_RED_INTENSE "#ae3753"

#define COLOR_ACCENT SOFT_RED
#define COLOR_ACCENT_INTENSE SOFT_RED_INTENSE

typedef struct
{
    float r, g, b;
} NRGB;


static NRGB from_hex(const char* hx)
{
    hx++;
    unsigned int l = strtol(hx, 0, 16);
    //assert(sizeof(l) == 4);
    unsigned char* bytes = (unsigned char*)&l;
    
    NRGB result;
    result.r = (float)bytes[2] / 255.f;
    result.g = (float)bytes[1] / 255.f;
    result.b = (float)bytes[0] / 255.f;
    return result;
}

#endif
