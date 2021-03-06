#include <X11/XF86keysym.h>
/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int gappx     = 5;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const Bool viewontag         = True;     /* Switch view on tag switch */
static const char *fonts[]          = { "Source Code Pro:size=15" };
static const char dmenufont[]       = "Source Code Pro:size=15";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#ffffff";
static const char col_gray4[]       = "#ffffff";
static const char col_cyan[]        = "#37474F";
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class         instance    title       tags mask     isfloating   monitor */
	{ "listen",      NULL,       NULL,       0,            1,           -1 },
	{ "Qq",          NULL,       NULL,       0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[T]",     tile },    /* first entry is default */
	{ "[W]",     NULL },    /* no layout function means floating behavior */
	{ "[M]",     monocle },
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
static const char *dmenucmd[]      = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]       = { "st", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "80x25", NULL };
static const char *browsercmd[]    =  { "/home/zsu/.config/scripts/browser.sh", NULL };
static const char *upvolcmd[]      =  {"/home/zsu/.config/scripts/alsa-up.sh", NULL };
static const char *downvolcmd[]    =  {"/home/zsu/.config/scripts/alsa-down.sh", NULL };
static const char *togglevolcmd[]  =  {"/home/zsu/.config/scripts/alsa-toggle.sh", NULL };
static const char *screenshotcmd[] =  { "flameshot", "gui", NULL };
static const char *calccmd[]       =  { "mate-calc", NULL };
static const char *musiccmd[]      =  { "/home/zsu/.config/scripts/listen1.sh", NULL };
static const char *trayercmd[]     =  { "/home/zsu/.config/scripts/trayer.sh" };
static const char *keyboard[]      =  { "/home/zsu/.config/scripts/colemak.sh", NULL };

static Key keys[] = {
	/* modifier                     key             function        argument */
	{ MODKEY,                       XK_o,           spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return,      spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_apostrophe,  togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,                       XK_b,           spawn,          {.v = calccmd } },
	{ MODKEY,                       XK_c,           spawn,          {.v = browsercmd } },
	{ MODKEY,                       XK_j,           spawn,          {.v = keyboard } },
	{ MODKEY,                       XK_l,           spawn,          {.v = musiccmd } },
	{ MODKEY,                       XK_v,           spawn,          {.v = trayercmd } },
	{ 0,                            XK_Print,       spawn,          {.v = screenshotcmd } },
	{ MODKEY|ControlMask,           XK_comma,       spawn,          {.v = downvolcmd } },
  { MODKEY|ControlMask,           XK_period,      spawn,          {.v = upvolcmd } },
	{ MODKEY|ControlMask,           XK_m,           spawn,          {.v = togglevolcmd } },
	{ MODKEY|ControlMask,           XK_n,           rotatestack,    {.i = +1 } },
	{ MODKEY|ControlMask,           XK_i,           rotatestack,    {.i = -1 } },
	{ MODKEY,                       XK_i,           focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_n,           focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_comma,       incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_period,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_k,           setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_m,           setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_space,       zoom,           {0} },
	{ MODKEY,                       XK_Tab,         view,           {0} },
	{ MODKEY|ControlMask,           XK_z,           killclient,     {0} },
	{ MODKEY,                       XK_t,           setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_r,           setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_s,           setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_h,           fullscreen,     {0} },
	{ MODKEY,                       XK_d,           setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,       togglefloating, {0} },
	{ MODKEY,                       XK_y,           view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_y,           tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_F5,          focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_F5,          focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_F5,          tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_F5,          tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_bracketleft, setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_bracketright,setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_bracketleft, setgaps,        {.i = 0  } },
	TAGKEYS(                        XK_1,                            0)
	TAGKEYS(                        XK_2,                            1)
	TAGKEYS(                        XK_3,                            2)
	TAGKEYS(                        XK_4,                            3)
	TAGKEYS(                        XK_5,                            4)
	TAGKEYS(                        XK_6,                            5)
	TAGKEYS(                        XK_7,                            6)
	TAGKEYS(                        XK_8,                            7)
	TAGKEYS(                        XK_9,                            8)
	{ MODKEY,                       XK_F12,         quit,           {0} },
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

