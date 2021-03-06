#include "../colors.h"
/* See LICENSE file for copyright and license details. */
/* appearance */
static unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 16;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "DejaVu Sans Book:size=8" };
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_accent[]      = COLOR_ACCENT;
static const char col_cyan[]        = "#005577";
static const char col_black[]       = "#000000";
static const char col_red[]         = "#ff0000";
static const char col_yellow[]      = "#ffff00";
static const char col_white[]       = "#ffffff";
static const unsigned int baralpha = 0xff;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, COLOR_BG, col_gray2 },
	[SchemeSel]  = { col_gray4, COLOR_BGL, COLOR_ACCENT },
	[SchemeWarn] =	 { col_black, col_yellow, col_red },
	[SchemeUrgent]=	 { col_white, col_red,    col_red },
    [SchemeCol1] = { col_gray1, COLOR_1, COLOR_7},
    [SchemeCol2] = { col_gray1, COLOR_2, COLOR_7},
    [SchemeCol3] = { col_gray1, COLOR_3, COLOR_7},
    [SchemeCol4] = { col_gray1, COLOR_4, COLOR_7},
    [SchemeCol5] = { col_gray1, COLOR_5, COLOR_7},
    [SchemeCol6] = { col_gray1, COLOR_6, COLOR_7},
    [SchemeCol7] = { col_gray1, COLOR_7, COLOR_7},
};
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
	[SchemeWarn] =	{ OPAQUE, baralpha, borderalpha },
	[SchemeUrgent]=	{ OPAQUE, baralpha, borderalpha },
    [SchemeCol1] = { OPAQUE, baralpha, borderalpha},
    [SchemeCol2] = { OPAQUE, baralpha, borderalpha},
    [SchemeCol3] = { OPAQUE, baralpha, borderalpha},
    [SchemeCol4] = { OPAQUE, baralpha, borderalpha},
    [SchemeCol5] = { OPAQUE, baralpha, borderalpha},
    [SchemeCol6] = { OPAQUE, baralpha, borderalpha},
    [SchemeCol7] = { OPAQUE, baralpha, borderalpha},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const Rule rules[] = {
	/* xprop(1):
  *	WM_CLASS(STRING) = instance, class
  *	WM_NAME(STRING) = title
     *
     *WM_CLASS(STRING) = "qbittorrent", "qBittorrent"
WM_CLASS(STRING) = "google-chrome", "Google-chrome"
  */

	/* class            instance                title                   tags mask       isfloating   monitor */
	{"mpv",             NULL,                   NULL,                   0,              1,           -1 },
	{"Gimp",            NULL,                   NULL,                   0,              1,           -1 },
	{"Google-chrome",   NULL,                   NULL,                   1 << 8,         0,           -1 },
	{"firefox",         NULL,                   NULL,                   1 << 8,         0,           -1 },
	{"discord",         NULL,                   NULL,                   1 << 4,         0,            1 },
	{NULL,              "skype",                NULL,                   1 << 6,         0,            1 },
	{NULL,              NULL,                  "ranger",                0,              1,           -1 },
	{NULL,              NULL,                   "calc",                 0,              1,           -1 },
    {"Thunderbird",     "Mail",                 NULL,                   1 << 2,         0,            1 },
	{"Spotify",         "spotify",              "Spotify Free",         1 << 6,         0,           -1 },
	{"GNU Octave",      "octave-gui",           "Octave",               1 << 5,         0,           -1 },
	{"Steam",           "Steam",                NULL,                	1 << 3,         0,           -1 },
	{NULL,              "qbittorrent",          NULL,                   1 << 7,         0,           -1 },
	{NULL,              "pcmanfm",              NULL,                   0,              1,           -1 },
	{NULL,              "thunar",               NULL,                   0,              1,           -1 },
	{NULL,              "zathura",              NULL,                   0,              1,           -1 },
	{NULL,              "imv",                  NULL,                   0,              1,           -1 },
	{NULL,              "simplescreenrecorder", NULL,                   1 << 3,         1,           -1 },
	{NULL,              NULL,                   "imv",                  0,              1,           -1 },
	{NULL,              "xarchiver",            NULL,                   0,              1,           -1 },
	{NULL,              "engrampa",             NULL,                   0,              1,           -1 },
	{NULL,              "popup",                NULL,                   0,              1,           -1 },
	{"Code",            "code",                 NULL,                   0,              1,           -1 },
	{"popup",           NULL,                   NULL,                   0,              1,           -1 },
	{"Mojosetup",       NULL,                   NULL,                   0,              1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[T]",      tile },    /* first entry is default */
	{ "[F]",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "[B]",      bstack },
	{ "[H]",      bstackhoriz },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]   = { "j4-dmenu-desktop","--dmenu=dmenu -i -p ▶","--term=st", NULL };
static const char *termcmd[]    = { "st", NULL };
static const char *lockcmd[]    = { "/home/miked/scripts/dlock", ">" , "/home/miked/fuck.txt",NULL };
static const char *muttcmd[]    = { "thunderbird", NULL };
static const char *cmuscmd[]    = { "st","-e", "cmus", NULL };
static const char *rangercmd[]  = { "pcmanfm", NULL };
static const char *homepagecmd[]= { "firefox", NULL };

/* volume control */
#include <X11/XF86keysym.h>

#ifndef DWM_USER_SCRIPT_DIRECTORY
#error "DWM_USER_SCRIPT_DIRECTORY"
#endif

#define _STR(x) #x
#define STR(x) _STR(x)

static const char *upvol[]   = { STR(DWM_USER_SCRIPT_DIRECTORY) "audio-control", "u", NULL };
static const char *downvol[] = { STR(DWM_USER_SCRIPT_DIRECTORY) "audio-control", "d", NULL };
static const char *mutevol[] = { STR(DWM_USER_SCRIPT_DIRECTORY) "audio-control", "m", NULL };
static const char *backinc[] = { STR(DWM_USER_SCRIPT_DIRECTORY) "brightness-control", "u", NULL};
static const char *backdec[] = { STR(DWM_USER_SCRIPT_DIRECTORY) "brightness-control", "d", NULL};
static const char *printsc[] = { STR(DWM_USER_SCRIPT_DIRECTORY) "dscreen", NULL};

static const char *cmuplay[] = { "cmus-remote", "-u", NULL };
static const char *cmunext[] = { "cmus-remote", "-n", NULL };
static const char *cmuprev[] = { "cmus-remote", "-r", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ 0,                            XF86XK_MonBrightnessUp,     spawn,              {.v = backinc} },
	{ 0,                            XF86XK_MonBrightnessDown,   spawn,              {.v = backdec} },
	{ 0,                            XF86XK_Mail,                spawn,              {.v = muttcmd} },
	{ 0,                            XF86XK_Explorer,            spawn,              {.v = rangercmd} },
	{ 0,                            XF86XK_HomePage,            spawn,              {.v = homepagecmd} },
	{ 0,                            XF86XK_AudioPlay,           spawn,              {.v = cmuplay } },
	{ 0,                            XF86XK_AudioNext,           spawn,              {.v = cmunext } },
	{ 0,                            XF86XK_AudioPrev,           spawn,              {.v = cmuprev } },
	{ 0,                            XF86XK_AudioLowerVolume,    spawn,              {.v = downvol } },
	{ 0,                            XF86XK_AudioMute,           spawn,              {.v = mutevol } },
	{ 0,                            XF86XK_AudioRaiseVolume,    spawn,              {.v = upvol   } },
	{ ShiftMask,                    0xff61,                     spawn,              {.v = printsc   } },

	{ MODKEY,                       XK_e,                       spawn,              {.v= rangercmd} },

	{ MODKEY,                       XK_F4,                      spawn,              {.v= cmuscmd} },
	{ MODKEY|ShiftMask,             XK_Escape,                  spawn,              {.v= lockcmd} },
	{ Mod1Mask,                     XK_space,                   spawn,              {.v = dmenucmd } },
	{ MODKEY,                       XK_Return,                  spawn,              {.v = termcmd } },
	{ MODKEY,                       XK_b,                       togglebar,          {0} },
	{ Mod1Mask,                     XK_Tab,                     focusstack,         {.i = +1 } },
	{ MODKEY,                       XK_j,                       focusstack,         {.i = +1 } },
	{ MODKEY,                       XK_k,                       focusstack,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,                       rotatestack,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,                       rotatestack,        {.i = -1 } },
	{ MODKEY,                       XK_i,                       incnmaster,         {.i = +1 } },
	{ MODKEY,                       XK_d,                       incnmaster,         {.i = -1 } },
	{ MODKEY,                       XK_h,                       setmfact,           {.f = -0.05} },
	{ MODKEY,                       XK_l,                       setmfact,           {.f = +0.05} },
	{ MODKEY,                       XK_Return,                  zoom,               {0} },
	{ MODKEY,                       XK_Tab,                     view,               {0} },
	{ MODKEY,                       XK_q,                       killclient,         {0} },
	{ MODKEY,                       XK_t,                       setlayout,          {.v = &layouts[0]} }, // tile
	{ MODKEY,                       XK_f,                       setlayout,          {.v = &layouts[1]} }, // floating
	{ MODKEY,                       XK_m,                       setlayout,          {.v = &layouts[2]} }, // monocle
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_o,      setlayout,      {.v = &layouts[4]} },
	/*{ MODKEY,                       XK_u,                       setlayout,          {.v = &layouts[3]} }, // centered master*/
	/*{ MODKEY,                       XK_o,                       setlayout,          {.v = &layouts[4]} }, // floating centered master*/
	//{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_f,                       togglefloating,     {0} },
	{ MODKEY,                       XK_0,                       view,               {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,                       tag,                {.ui = ~0 } },
	{ MODKEY,                       XK_comma,                   focusmon,           {.i = -1 } },
	{ MODKEY,                       XK_period,                  focusmon,           {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,                   tagmon,             {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,                  tagmon,             {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
        TAGKEYS(                        XK_2,                      1)
        TAGKEYS(                        XK_3,                      2)
        TAGKEYS(                        XK_4,                      3)
        TAGKEYS(                        XK_5,                      4)
        TAGKEYS(                        XK_6,                      5)
        TAGKEYS(                        XK_7,                      6)
        TAGKEYS(                        XK_8,                      7)
        TAGKEYS(                        XK_9,                      8)
	{ MODKEY,                       XK_Escape,                  quit,               {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

