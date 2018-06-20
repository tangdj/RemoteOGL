#include <SDL/SDL.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#define WIDTH 640
#define HEIGHT 480

static _Bool init_app(const char * name, SDL_Surface * icon, uint32_t flags)
{
    atexit(SDL_Quit);
    if(SDL_Init(flags) < 0)
        return 0;

    SDL_WM_SetCaption(name, name);
    SDL_WM_SetIcon(icon, NULL);

    return 1;
}

static uint8_t * init_data(uint8_t * data)
{
    for(size_t i = WIDTH * HEIGHT * 3; i--; )
        data[i] = (i % 3 == 0) ? (i / 3) % WIDTH :
            (i % 3 == 1) ? (i / 3) / WIDTH : 0;

    return data;
}

static _Bool process(uint8_t * data)
{
    for(SDL_Event event; SDL_PollEvent(&event);)
        if(event.type == SDL_QUIT) return 0;

    for(size_t i = 0; i < WIDTH * HEIGHT * 3; i += 1 + rand() % 3)
        data[i] -= rand() % 8;

    return 1;
}

static void render(SDL_Surface * sf)
{
    SDL_Surface * screen = SDL_GetVideoSurface();
    if(SDL_BlitSurface(sf, NULL, screen, NULL) == 0)
        SDL_UpdateRect(screen, 0, 0, 0, 0);
}

static int filter(const SDL_Event * event)
{ return event->type == SDL_QUIT; }

#define mask32(BYTE) (*(uint32_t *)(uint8_t [4]){ [BYTE] = 0xff })

int main(int argc, char * argv[])
{
    (void)argc, (void)argv;
    static uint8_t buffer[WIDTH * HEIGHT * 3];

    _Bool ok =
        init_app("SDL example", NULL, SDL_INIT_VIDEO) &&
        SDL_SetVideoMode(WIDTH, HEIGHT, 24, SDL_HWSURFACE);

    assert(ok);

    SDL_Surface * data_sf = SDL_CreateRGBSurfaceFrom(
        init_data(buffer), WIDTH, HEIGHT, 24, WIDTH * 3,
        mask32(0), mask32(1), mask32(2), 0);

    SDL_SetEventFilter(filter);

    for(; process(buffer); SDL_Delay(10))
        render(data_sf);

    return 0;
}
