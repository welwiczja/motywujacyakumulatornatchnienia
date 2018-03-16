#include <SDL/SDL_main.h>
#include <SDL/begin_code.h>
#include <SDL/close_code.h>
#include <SDL/SDL_active.h>
#include <SDL/SDL_audio.h>
#include <SDL/SDL_byteorder.h>
#include <SDL/SDL_cdrom.h>
#include <SDL/SDL_config.h>
#include <SDL/SDL_cpuinfo.h>
#include <SDL/SDL_endian.h>
#include <SDL/SDL_error.h>
#include <SDL/SDL_events.h>
#include <SDL/SDL_getenv.h>
#include <SDL/SDL.h>
#include <SDL/SDL_joystick.h>
#include <SDL/SDL_keyboard.h>
#include <SDL/SDL_keysym.h>
#include <SDL/SDL_loadso.h>
#include <SDL/SDL_main.h>
#include <SDL/SDL_mouse.h>
#include <SDL/SDL_mutex.h>
#include <SDL/SDL_name.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_platform.h>
#include <SDL/SDL_quit.h>
#include <SDL/SDL_rwops.h>
#include <SDL/SDL_stdinc.h>
#include <SDL/SDL_syswm.h>
#include <SDL/SDL_thread.h>
#include <SDL/SDL_timer.h>
#include <SDL/SDL_types.h>
#include <SDL/SDL_version.h>
#include <SDL/SDL_video.h>
#include <iostream>
#include <cstdlib>

#define DOTSIZE 1
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600

using namespace std; // hoping Roger doesn't see

SDL_Surface *screen = NULL;

void PUT_pixel (int x, int y, int color) {
	SDL_Rect r;
	r.x = x;
	r.y = y;
	r.w = DOTSIZE;
	r.h = DOTSIZE;

	SDL_FillRect(screen, &r, color);
}

// sdl.beuc.net
Uint32 GET_pixel(int x, int y)
{
	SDL_Surface *surface = screen;

	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	if (x < 0 || x > SCREEN_WIDTH || y < 0 || y > SCREEN_HEIGHT) {
		printf ("Access violation. %d %d\n", x, y);
		return 0;
	}

	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch(bpp) {
		case 1:
			return *p;
			break;

		case 2:
			return *(Uint16 *)p;
			break;

		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;
			break;

		case 4:
			return *(Uint32 *)p;
			break;

		default:
			return 0;       /* shouldn't happen, but avoids warnings */
	}
}

char GET_red (int x, int y) {
	Uint32 res = GET_pixel (x, y);
	return (res & 0xFF0000) >> 16;
}

int main ( int argc, char ** argv ) {
	SDL_Surface *hello = NULL;

	SDL_Init (SDL_INIT_EVERYTHING);
	screen = SDL_SetVideoMode (SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE);

	if (screen == NULL) {
		puts ("Screen null.");
	}

	while (1) {
		// Fun stuff
		int x = rand () % SCREEN_WIDTH;
		int y = rand () % SCREEN_HEIGHT;
		int color = (rand() & 0xFF0000);

		for (int i = 0; i < SCREEN_WIDTH; i++) {
			PUT_pixel (i, SCREEN_HEIGHT-DOTSIZE, color);
		}

		int delta = DOTSIZE; // delta = DOTSIZE
		for (int i = 1; i < SCREEN_HEIGHT; i += delta) {
			for (int j = 1; j < SCREEN_WIDTH; j+= delta) {
				//puts ("loop");
				if (GET_red (j, i) > 0xFF) {
					puts ("Red larger than 0xff");
				}
				int newcolor = ((
					  GET_red (j, i) 
					+ GET_red (j, i + delta)
					+ GET_red (j + delta, i + delta)
					+ GET_red (j - delta, i + delta)) / 4) << 16;
				//printf ("newcolor: %d\n", newcolor);
				PUT_pixel (j, i, newcolor);
			}
		}

		//PUT_pixel  (x, y, color);
		SDL_Delay (200);

		// End of Fun stuff
		SDL_Event event;
		while (SDL_PollEvent (&event)) {
			switch (event.type) {
				case SDL_KEYDOWN:
					exit (0);
			}
		}

		SDL_Flip (screen);
	}
	SDL_Quit();
	return 0;
}

