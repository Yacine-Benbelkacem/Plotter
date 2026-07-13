#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>

typedef enum {
    TEXT_ALIGN_LEFT,
    TEXT_ALIGN_CENTER,
    TEXT_ALIGN_RIGHT
} TextAlign;

/* Initialise SDL_ttf and open a shared font. Returns 0 on success, -1 on error.
   Safe to call multiple times; the font is opened only once. */
int  Text_Init(const char* font_path, int pt_size);
void Text_Quit(void);

/* Draw text anchored at (x, y). The x anchor is interpreted per `halign`;
   the text is always vertically centred on y. */
void Text_Draw(SDL_Renderer* renderer, const char* text, int x, int y,
               TextAlign halign, SDL_Color color);

/* Draw text rotated `angle` degrees clockwise, centred on (x, y). */
void Text_DrawRotated(SDL_Renderer* renderer, const char* text, int x, int y,
                      double angle, SDL_Color color);

/* Pixel dimensions the given string would occupy with the current font. */
void Text_Measure(const char* text, int* w, int* h);

#endif // TEXT_H
