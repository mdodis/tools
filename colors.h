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

union NRGB
{
    float colors[3];
    struct
    {
        float r, g, b;
    };
    
    static NRGB from_hexstr(const char* hexstr)
    {
        const char* c = hexstr + 1;
        int cnt = 0;
        
        unsigned int l = strtol(c, 0, 16);
        printf("%8x\n", l);
        assert(sizeof(l) == 4);
        unsigned char* bytes = (unsigned char*)&l;
        
        float r = (float)bytes[2] / 255.f;
        float g = (float)bytes[1] / 255.f;
        float b = (float)bytes[0] / 255.f;
        
        return {r, g, b};
    }
};


#endif
