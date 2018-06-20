#include <SDL/SDL.h>
 
int main(int argc, char** argv)
{
        SDL_Init(SDL_INIT_EVERYTHING);
        SDL_Surface *screen, *image;    // every surface except for screen needs to be freed
        screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
//      screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE|SDL_FULLSCREEN);
        bool running = true;
        const int FPS = 30;
        Uint32 start;
        bool b[4] = {0,0,0,0};
        SDL_Rect rect;
        rect.x = 10;
        rect.y = 10;
        rect.w = 20;
        rect.h = 20;
        Uint32 color = SDL_MapRGB(screen->format, 0xff,0xff,0xff);
        Uint32 color2 = SDL_MapRGB(screen->format, 0,0,0);
        image = SDL_LoadBMP("tree.bmp");        // better to check later if image is NULL
        while(running) {
                start = SDL_GetTicks();
                SDL_Event event;
                while(SDL_PollEvent(&event)) {
                        switch(event.type) {
                                case SDL_QUIT:
                                        running = false;
                                        break;
                                case SDL_KEYDOWN:
                                        switch(event.key.keysym.sym) {
                                                case SDLK_UP:
                                                        b[0] = 1;
                                                        break;
                                                case SDLK_LEFT:
                                                        b[1] = 1;
                                                        break;
                                                case SDLK_DOWN:
                                                        b[2] = 1;
                                                        break;
                                                case SDLK_RIGHT:
                                                        b[3] = 1;
                                                        break;
                                       
                                        }
                                        break;
                                case SDL_KEYUP:
                                        switch(event.key.keysym.sym) {
                                                case SDLK_UP:
                                                        b[0] = 0;
                                                        break;
                                                case SDLK_LEFT:
                                                        b[1] = 0;
                                                        break;
                                                case SDLK_DOWN:
                                                        b[2] = 0;
                                                        break;
                                                case SDLK_RIGHT:
                                                        b[3] = 0;
                                                        break;
                                       
                                        }
                                        break;
                        }
                }
 
                //logic
                if(b[0])
                        rect.y--;
                if(b[1])
                        rect.x--;
                if(b[2])
                        rect.y++;
                if(b[3])
                        rect.x++;
 
                //render
                SDL_FillRect(screen, &screen->clip_rect, color);        // fill the screen white (maybe better outside while loop)
//              SDL_FillRect(screen, &rect, color2);
//              SDL_Rect rect;
                rect.x = 200;
                rect.y = 100;
                SDL_BlitSurface(image, NULL, screen, NULL);     // copy image to screen to specified coordinates
                //SDL_BlitSurface(image, NULL, screen, &rect);
                SDL_Flip(screen);
 
                if(1000/FPS > SDL_GetTicks()-start) {
                        SDL_Delay(1000/FPS-(SDL_GetTicks()-start));
                }
        }
        SDL_FreeSurface(image);
        SDL_Quit();
        return 0;
}
