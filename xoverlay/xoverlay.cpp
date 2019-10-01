#include <assert.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>

#include <cairo.h>
#include <unistd.h>
#include <cairo-xlib.h>

#include <chrono>
#include <future>
#include <thread>

#include "../colors.h"

#if !defined(CAIRO_HAS_PNG_FUNCTIONS)
#error "CAIRO_HAS_PNG_FUNCTIONS"
#endif

const char* STAT_CMD = 0;
const char* STAT_ICON_FULL = 0;
const char* STAT_ICON_NONE = 0;

float STAT_MIN = 0.f;
float STAT_MAX = 100.f;

#define MIKED_OW 200
#define MIKED_OH 200

constexpr int WIND_X = (1366 / 2) - (MIKED_OW / 2);
constexpr int WIND_Y = 768 / 2 - MIKED_OH;
constexpr int WIND_W = MIKED_OW;
constexpr int WIND_H = MIKED_OH;
const float TIMEOUT = 950.f;

struct NRGB
{
    float r, g, b;
};

NRGB BAR_COLOR = {};
static cairo_surface_t* g_photo_full;
static cairo_surface_t* g_photo_none;

timespec timespec_diff(timespec start, timespec end)
{
    timespec temp;
    if ((end.tv_nsec - start.tv_nsec) < 0) {
        temp.tv_sec = end.tv_sec - start.tv_sec - 1;
        temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec - start.tv_sec;
        temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return temp;
}

struct Timer
{
    struct timespec last;
    
    double get_elapsed_ms()
    {
        struct timespec now;
        struct timespec delta_timespec;
        clock_gettime(CLOCK_MONOTONIC, &now);
        delta_timespec = timespec_diff(this->last, now);
        double delta = (double)delta_timespec.tv_nsec /(double) 1000000.0;
        return delta;
    }
    
    double get_elapsed_secs()
    {
        struct timespec now;
        struct timespec delta_timespec;
        clock_gettime(CLOCK_MONOTONIC, &now);
        delta_timespec = timespec_diff(this->last, now);
        return delta_timespec.tv_sec;
    }
    
    void reset()
    {
        clock_gettime(CLOCK_MONOTONIC, &last);
    }
};


void parse_options(int argc, char** argv)
{
    int c;
    while ((c = getopt(argc, argv, "l:m:c:f:n:")) != -1)
    {
        switch (c)
        {
            case 'l':
            {
                char* least = optarg;
                float l = atof(least);
                STAT_MIN = l;
            }break;
            
            case 'm':
            {
                char* most = optarg;
                float m = atof(most);
                STAT_MAX = m;
            }break;
            
            case 'c':
            {
                char* cmd = optarg;
                STAT_CMD = cmd;
                printf("%s\n", cmd);
            }break;
            
            case 'f':
            {
                char* path = optarg;
                STAT_ICON_FULL = path;
                printf("%s\n", path);
            } break;
            
            case 'n':
            {
                char* path = optarg;
                STAT_ICON_NONE = path;
                printf("%s\n", path);
            } break;
            
            default:
            {
                puts("nnoooo");
                exit(0);
            } break;
        }
    }
    
    if (STAT_CMD == 0) exit(245);
}

inline float fmap(
float in_a, float in_b,
float out_a, float out_b,
float in)
{
    float slope = (out_b - out_a) / (in_b - in_a);
    return out_a + (slope * (in - in_a));
}

float get_percentage(const char* command, float min, float max)
{
    FILE* fp;
    fp = popen(command, "r");
    if (!fp) exit(-1);
    
    char buffer[1024];
    if (!fgets(buffer, 1024 - 1, fp))
    {
        puts("fgets");
        exit(-1);
    }
    
    fclose(fp);
    
    float result = atof(buffer);
    
    
    return fmap((float)STAT_MIN, (float)STAT_MAX, 0.f, 1.f, result);
}

void init(cairo_t* cr)
{
    if (STAT_ICON_FULL)
        g_photo_full = cairo_image_surface_create_from_png(STAT_ICON_FULL);
    if (STAT_ICON_NONE)
        g_photo_none = cairo_image_surface_create_from_png(STAT_ICON_NONE);
}

struct FitRect
{
    double scale;
    double x, y;
};

FitRect fit_surface_to_rectangle(cairo_surface_t* surf, double width, double height)
{
    FitRect result = {};
    
    double xscale, yscale;
    xscale = cairo_image_surface_get_width(surf);
    yscale = cairo_image_surface_get_height(surf);
    
#define MAX(x,y) x > y ? x : y
    
    double fitx, fity;
    double fitx_p, fity_p;
    fitx = fitx_p = xscale;
    fity = fity_p = yscale;
    
    if (width < xscale)
    {
        fitx_p = (xscale - (xscale - width));
        fitx = fitx_p / xscale;
    }
    else
    {
        fitx_p = (width);
        fitx = fitx_p / xscale;
    }
    if (height < yscale)
    {
        fity_p = (yscale - (yscale - height));
        fity = fity_p / yscale;
    }
    else
    {
        fity_p = (height);
        fity = fity_p / yscale;
    }
    double max_p = MAX(fitx_p, fity_p);
    result.scale = MAX(fitx, fity);
    return result;
}

void draw(cairo_t *cr, float t) {
    
    cairo_identity_matrix(cr);
    cairo_set_source_rgba(cr, 0, 0, 0, 0);
    cairo_rectangle(cr, 0, 0, WIND_W, WIND_H);
    cairo_fill(cr);
    
    assert(t <= 1.f);
    
    cairo_set_source_rgba(cr, .2, .2, .2, .95);
    cairo_rectangle(cr, 0, 0 , WIND_W, WIND_H);
    cairo_fill(cr);
    
    cairo_set_source_rgba(cr, BAR_COLOR.r, BAR_COLOR.g, BAR_COLOR.b, 0.75);
    cairo_rectangle(cr, 0, 0, WIND_W * t, WIND_H);
    cairo_fill(cr);
    
    cairo_surface_t* img = g_photo_full;
    if (t <= 0.f && (g_photo_none))
    {
        img = g_photo_none;
    }
    
    if (img)
    {
        
        const double offset_pixels = 32;
        FitRect fr = fit_surface_to_rectangle(img, WIND_W - offset_pixels, WIND_H - offset_pixels);
        cairo_translate(cr,offset_pixels / 2, offset_pixels / 2);
        cairo_scale(cr, fr.scale, fr.scale);
        
        cairo_set_source_surface(cr, img, 0, 0);
        cairo_paint(cr);
    }
}

#define PIDFILE "/tmp/xoverlay.lock"
#define RESETFILE (char*)"/tmp/xoverlay.reset"
#include <sys/file.h>
#include <sys/stat.h>
#include <errno.h>

int file_exists(char *filename)
{
    struct stat   buffer;   
    return (stat (filename, &buffer) == 0);
}

int main(int argc, char** argv) 
{
    // open lock file exclusively to prevent other instances
    // from opening it
    int fd = open(PIDFILE,  O_CREAT | O_RDWR, 0666);
    int rc = flock(fd,  LOCK_EX | LOCK_NB);
    if (rc)
    {
        // if it's already opened, message running process
        // to reset its countdown timer
        if (errno == EWOULDBLOCK)
        {
            // bad michael
            system("touch /tmp/xoverlay.reset");
            exit(2);
        }
    }
    
    parse_options(argc, argv);
    
    BAR_COLOR = {0.682, 0.215, 0.325};
    
    Display *d = XOpenDisplay(NULL);
    Window root = DefaultRootWindow(d);
    int default_screen = XDefaultScreen(d);
    
    // these two lines are really all you need
    XSetWindowAttributes attrs;
    attrs.override_redirect = true;
    
    XVisualInfo vinfo;
    if (!XMatchVisualInfo(d, DefaultScreen(d), 32, TrueColor, &vinfo)) {
        printf("No visual found supporting 32 bit color, terminating\n");
        exit(EXIT_FAILURE);
    }
    attrs.colormap = XCreateColormap(d, root, vinfo.visual, AllocNone);
    attrs.background_pixel = 0;
    attrs.border_pixel = 0;
    
    Window overlay = XCreateWindow(
        d, root,
        WIND_X, WIND_Y, WIND_W, WIND_H, 0,
        vinfo.depth, InputOutput,
        vinfo.visual,
        CWOverrideRedirect | CWColormap | CWBackPixel | CWBorderPixel, &attrs
        );
    
    XMapWindow(d, overlay);
    
    cairo_surface_t* surf = cairo_xlib_surface_create(d, overlay,
                                                      vinfo.visual,
                                                      WIND_W, WIND_H);
    cairo_t* cr = cairo_create(surf);
    
    init(cr);
    
    float t = 0.0f;
    draw(cr, t);
    XFlush(d);
    
    
    Timer tm;
    tm.reset();
    
    using namespace std::chrono_literals;
    auto future = std::async(std::launch::async, get_percentage,STAT_CMD, STAT_MIN, STAT_MAX);
    while (1)
    {
        XClearWindow(d, overlay);
        
        auto status = future.wait_for(0ms);
        if (status == std::future_status::ready)
        {
            float result = future.get();
            t = result;
            future = std::async(std::launch::async, get_percentage,STAT_CMD, STAT_MIN, STAT_MAX);
        }
        
        draw(cr, t);
        XFlush(d);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        
        if (file_exists(RESETFILE))
        {
            unlink(RESETFILE);
            tm.reset();
        }
        
        if (tm.get_elapsed_ms() >= TIMEOUT) break;
        printf("%f\n", tm.get_elapsed_ms());
    }
    
    cairo_destroy(cr);
    cairo_surface_destroy(surf);
    
    XUnmapWindow(d, overlay);
    XCloseDisplay(d);
    flock(fd, LOCK_UN);
    return 0;
}
