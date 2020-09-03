#ifndef HUD_H
#define HUD_H

/*---------------------------------------------------------------------------*/

void hud_init(void);
void hud_free(void);

void hud_paint(void);

#ifdef __MOBILE__
void hud_mobile_init(void);
void hud_mobile_free(void);

void hud_mobile_paint(void);

void hud_mobile_point(int x, int y);
int hud_mobile_click(void);
#endif

/*---------------------------------------------------------------------------*/

#endif
