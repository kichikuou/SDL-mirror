/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2013 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/
#include "SDL_config.h"

#if SDL_VIDEO_DRIVER_NACL

#include "../SDL_sysvideo.h"
extern "C" {
#include "../../events/SDL_keyboard_c.h"
#include "../../events/SDL_mouse_c.h"
}

#include "SDL_naclvideo.h"
#include "SDL_naclwindow.h"

int
NACL_CreateWindow(_THIS, SDL_Window * window)
{
    fprintf(stderr, "NACL_CreateWindow\n");

    if (NACL_Window) {
        return SDL_SetError("NaCl only supports one window"); //FIXME: isn't it Pepper, actually?
    }

    /* Adjust the window data to match the screen */
    window->x = 0;
    window->y = 0;
    window->w = g_nacl_video_width;
    window->h = g_nacl_video_height;

    //FIXME: is it all true?
    window->flags &= ~SDL_WINDOW_RESIZABLE;     /* window is NEVER resizeable */
    window->flags |= SDL_WINDOW_FULLSCREEN;     /* window is always fullscreen */
    window->flags &= ~SDL_WINDOW_HIDDEN;
    window->flags |= SDL_WINDOW_SHOWN;          /* only one window on NaCl */
    window->flags |= SDL_WINDOW_INPUT_FOCUS;    /* always has input focus */

    /* One window, it always has focus */
    SDL_SetMouseFocus(window);
    SDL_SetKeyboardFocus(window);

    printf("CREATED WINDOW %dx%d\n", g_nacl_video_width, g_nacl_video_height);fflush(stdout);

    SDL_PrivateVideoData *dd = (SDL_PrivateVideoData*) _this->driverdata;
    if (window->w > dd->ow || dd->h > dd->oh) {
        return NULL;
    }
    dd->bpp = 32; // Let SDL handle pixel format conversion.
    dd->w = window->w;
    dd->h = window->h;

    NACL_Window = window;

    return 0;
}

void
NACL_SetWindowTitle(_THIS, SDL_Window * window)
{
    fprintf(stderr, "NACL_SetWindowTitle\n");
    //TODO: do nothing for now
}

//FIXME: move here code to deallocate context from naclvideo
void
NACL_DestroyWindow(_THIS, SDL_Window * window)
{
    fprintf(stderr, "NACL_DestroyWindow\n");

    if (window == NACL_Window) {
        NACL_Window = NULL;

        if(window->driverdata) {
            SDL_free(window->driverdata);
            window->driverdata = NULL;
        }
    }
}

#endif /* SDL_VIDEO_DRIVER_NACL */

/* vi: set ts=4 sw=4 expandtab: */
