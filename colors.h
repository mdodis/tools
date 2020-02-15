#ifndef COLOR_S_HH
#define COLOR_S_HH

#define NEON_BLUE "#4D4DFF"
#define SPACE_CADET "#1E2952"
#define CADET_GREAY "#91A3B0"
#define DWM_CYAN "#005577"
#define BURGUNDY_RED "#5e000e"

#define SOFT_RED "#80283d"
#define SOFT_RED_INTENSE "#ae3753"

#define BLUEISH_GREEN "#009e73"
#define KEPPEL "#3AB09E"
#define VIRIDIAN "#40826D"

#define SOLARIZED_ACCENT    "#b58900"
#define SOLARIZED_BG        "#002b36"
#define SOLARIZED_BGL       "#005266"

#define COLOR_0 "#002b36" /* black   */
#define COLOR_1 "#dc322f" /* red     */
#define COLOR_2 "#859900" /* green   */
#define COLOR_3 "#b58900" /* yellow  */
#define COLOR_4 "#268bd2" /* blue    */
#define COLOR_5 "#6c71c4" /* magenta */
#define COLOR_6 "#2aa198" /* cyan    */
#define COLOR_7 "#93a1a1" /* white   */
#define COLOR_8 "#657b83" /* black   */
#define COLOR_9 "#dc322f" /* red     */
#define COLOR_A "#859900" /* green   */
#define COLOR_B "#b58900" /* yellow  */
#define COLOR_C "#268bd2" /* blue    */
#define COLOR_D "#6c71c4" /* magenta */
#define COLOR_E "#2aa198" /* cyan    */
#define COLOR_F "#fdf6e3" /* white   */

#define COLOR_ACCENT    SOLARIZED_ACCENT
#define COLOR_BG        SOLARIZED_BG
#define COLOR_BGL       SOLARIZED_BGL
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
