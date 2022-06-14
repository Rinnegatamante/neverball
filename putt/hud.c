/*
 * Copyright (C) 2003 Robert Kooima
 *
 * NEVERPUTT is  free software; you can redistribute  it and/or modify
 * it under the  terms of the GNU General  Public License as published
 * by the Free  Software Foundation; either version 2  of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT  ANY  WARRANTY;  without   even  the  implied  warranty  of
 * MERCHANTABILITY or  FITNESS FOR A PARTICULAR PURPOSE.   See the GNU
 * General Public License for more details.
 */

#include <math.h>

#include "gui.h"
#include "hud.h"
#include "hole.h"
#include "config.h"
#include "video.h"

/*---------------------------------------------------------------------------*/

static int Lhud_id;
static int Rhud_id;
static int fps_id;

/*---------------------------------------------------------------------------*/

void hud_init(void)
{
    static const GLubyte *color[5] = {
        gui_wht,
        gui_red,
        gui_grn,
        gui_blu,
        gui_yel
    };
    int i = curr_player();

    if ((Lhud_id = gui_hstack(0)))
    {
        gui_label(Lhud_id, curr_scr(), GUI_MED, color[i], gui_wht);
        gui_label(Lhud_id, _("Shot"), GUI_SML,  gui_wht,  gui_wht);
        gui_set_rect(Lhud_id, GUI_NE);
        gui_layout(Lhud_id, -1, -1);
    }
    if ((Rhud_id = gui_hstack(0)))
    {
        gui_label(Rhud_id, curr_par(), GUI_MED,  color[i], gui_wht);
        gui_label(Rhud_id, _("Par"),   GUI_SML, gui_wht,  gui_wht);
        gui_set_rect(Rhud_id, GUI_NW);
        gui_layout(Rhud_id, +1, -1);
    }
    if ((fps_id = gui_count(0, 1000, GUI_SML)))
    {
        gui_set_rect(fps_id, GUI_SE);
        gui_layout(fps_id, -1, +1);
    }
}

void hud_free(void)
{
    gui_delete(Lhud_id);
    gui_delete(Rhud_id);
    gui_delete(fps_id);
}

/*---------------------------------------------------------------------------*/

void hud_paint(void)
{
    if (config_get_d(CONFIG_FPS))
    {
        gui_set_count(fps_id, video_perf());
        gui_paint(fps_id);
    }

    gui_paint(Rhud_id);
    gui_paint(Lhud_id);
}

/*---------------------------------------------------------------------------*/

#ifdef __MOBILE__

static int swing_id;
static int pause_id;

/*---------------------------------------------------------------------------*/

void hud_mobile_init(void)
{
    if ((swing_id = gui_vstack(0)))
    {
        gui_state(swing_id, "   S   ", GUI_SML, -1, 0);
        gui_state(swing_id, "   W   ", GUI_SML, -1, 0);
        gui_state(swing_id, "   I   ", GUI_SML, -1, 0);
        gui_state(swing_id, "   N   ", GUI_SML, -1, 0);
        gui_state(swing_id, "   G   ", GUI_SML, -1, 0);
        gui_set_rect(swing_id, GUI_LFT);
        gui_layout(swing_id, 1, 0);
    }

    if ((pause_id = gui_vstack(0)))
    {
        gui_state(pause_id, "  Pause   ", GUI_SML, -2, 0);
        gui_set_rect(pause_id, GUI_SE);
        gui_layout(pause_id, -1, +1);
    }
}

void hud_mobile_free(void)
{
    gui_delete(swing_id);
    gui_delete(pause_id);
}

/*---------------------------------------------------------------------------*/

void hud_mobile_paint(void)
{
    gui_paint(swing_id);
    gui_paint(pause_id);
}

/*---------------------------------------------------------------------------*/

void hud_mobile_point(int x, int y)
{
    gui_point(swing_id, x, y);
    gui_point(pause_id, x, y);
}

/*---------------------------------------------------------------------------*/

int hud_mobile_click(void)
{
    int id = gui_token(gui_active());
    gui_focus(0);

    if (id == -1)
        return SDL_BUTTON_LEFT;
    else if (id == -2)
        return SDL_BUTTON_RIGHT;
    else
        return 0;
}

/*---------------------------------------------------------------------------*/

#endif /* __MOBILE__ */
