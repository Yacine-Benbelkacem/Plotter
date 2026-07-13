#include "Text.h"
#include <SDL2/SDL_ttf.h>

static TTF_Font* g_font = NULL;

int Text_Init(const char* font_path, int pt_size){
    if(g_font != NULL){
        return 0; // already initialised
    }
    if(TTF_WasInit() == 0 && TTF_Init() != 0){
        return -1;
    }
    g_font = TTF_OpenFont(font_path, pt_size);
    if(g_font == NULL){
        return -1;
    }
    return 0;
}

void Text_Quit(void){
    if(g_font != NULL){
        TTF_CloseFont(g_font);
        g_font = NULL;
    }
    if(TTF_WasInit()){
        TTF_Quit();
    }
}

void Text_Measure(const char* text, int* w, int* h){
    if(g_font == NULL || text == NULL){
        if(w) *w = 0;
        if(h) *h = 0;
        return;
    }
    TTF_SizeUTF8(g_font, text, w, h);
}

/* Render `text` to a fresh texture. Caller owns the returned texture and its
   size is written to the w and h out-parameters. Returns NULL on failure. */
static SDL_Texture* make_texture(SDL_Renderer* renderer, const char* text,
                                 SDL_Color color, int* w, int* h){
    if(g_font == NULL || text == NULL || text[0] == '\0'){
        return NULL;
    }
    SDL_Surface* surf = TTF_RenderUTF8_Blended(g_font, text, color);
    if(surf == NULL){
        return NULL;
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    if(w) *w = surf->w;
    if(h) *h = surf->h;
    SDL_FreeSurface(surf);
    return tex;
}

void Text_Draw(SDL_Renderer* renderer, const char* text, int x, int y,
               TextAlign halign, SDL_Color color){
    int w, h;
    SDL_Texture* tex = make_texture(renderer, text, color, &w, &h);
    if(tex == NULL) return;

    SDL_Rect dst = { .w = w, .h = h, .y = y - h / 2 };
    switch(halign){
        case TEXT_ALIGN_LEFT:   dst.x = x;         break;
        case TEXT_ALIGN_CENTER: dst.x = x - w / 2; break;
        case TEXT_ALIGN_RIGHT:  dst.x = x - w;     break;
    }

    SDL_RenderCopy(renderer, tex, NULL, &dst);
    SDL_DestroyTexture(tex);
}

void Text_DrawRotated(SDL_Renderer* renderer, const char* text, int x, int y,
                      double angle, SDL_Color color){
    int w, h;
    SDL_Texture* tex = make_texture(renderer, text, color, &w, &h);
    if(tex == NULL) return;

    SDL_Rect dst = { .x = x - w / 2, .y = y - h / 2, .w = w, .h = h };
    SDL_RenderCopyEx(renderer, tex, NULL, &dst, angle, NULL, SDL_FLIP_NONE);
    SDL_DestroyTexture(tex);
}
