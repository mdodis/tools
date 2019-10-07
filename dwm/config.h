#include "../colors.h"
/* See LICENSE file for copyright and license details. */
/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int gappx     = 10;        /* gaps between windows */
static const unsigned int snap      = 16;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Liberation Sans:size=9" };
static const char dmenufont[]       = "Liberation Sans:size=9";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_accent[]      = COLOR_ACCENT;
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_accent,col_accent },
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
	{"Gimp",            NULL,                   NULL,                   0,              1,           -1 },
	{"Google-chrome",   NULL,                   NULL,                   1 << 8,         0,           -1 },
	{"Firefox",         NULL,                   NULL,                   1 << 8,         0,           -1 },
	{"discord",         NULL,                   NULL,                   1 << 4,         0,            1 },
	{NULL,              "skype",                NULL,                   1 << 6,         0,            1 },
	{NULL,              NULL,                  "ranger",                0,              1,           -1 },
	{NULL,              NULL,                   "calc",                 0,              1,           -1 },
    {"Thunderbird",     "Mail",                 NULL,                   1 << 2,         0,            1 },
	{"Spotify",         "spotify",              "Spotify Free",         1 << 6,         0,           -1 },
	{"GNU Octave",      "octave-gui",           "Octave",               1 << 5,         0,           -1 },
	{"Steam",           "Steam",                NULL,                	1 << 3,         0,           -1 },
	{NULL,              "qbittorrent",          NULL,                   1 << 4,         0,           -1 },
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
	{ "|M|",      centeredmaster },
	{ "<F>",      centeredfloatingmaster },
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
static const char *dmenucmd[]   = { "j4-dmenu-desktop","--dmenu=dmenu -i -l 30","--term=st", NULL };
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
	{ MODKEY,                       XK_u,                       setlayout,          {.v = &layouts[3]} }, // centered master
	{ MODKEY,                       XK_o,                       setlayout,          {.v = &layouts[4]} }, // floating centered master
	//{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_f,                       togglefloating,     {0} },
	{ MODKEY,                       XK_0,                       view,               {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,                       tag,                {.ui = ~0 } },
	{ MODKEY,                       XK_comma,                   focusmon,           {.i = -1 } },
	{ MODKEY,                       XK_period,                  focusmon,           {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,                   tagmon,             {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,                  tagmon,             {.i = +1 } },
	{ MODKEY,                       XK_minus,                   setgaps,            {.i = -1 } },
	{ MODKEY,                       XK_equal,                   setgaps,            {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,                   setgaps,            {.i = 0  } },
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

