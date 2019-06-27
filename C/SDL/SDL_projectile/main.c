// Demonstrates the concepts behind basic projectiles.
// Public domain, do what you like with it.
// When executing the program, press the mouse button down anywhere within the window;
// an object will be created that's thrown at a random angle when the mouse button
// is released.
// play around with the variables 'gravity' and 'velocity' to change its behaviour.
// The random numbers generated in the mouse handler code control the angle the object
// is thrown at.
// contact: seth.ballantyne@gmail.com
//
// have fun :)

#include <SDL.h>
#include <math.h>
#include <time.h>

#define OBJECT_SIZE		 32
#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600

// cap the demo at 30 FPS
#define FRAMES_PER_SECOND 30

// specifies how often in milliseconds the frame should be updated.
const int TICK_INTERVAL = 1000 / FRAMES_PER_SECOND;

typedef struct
{
	float x;
	float y;
	float velocity_x;
	float velocity_y;
	float angle;
	SDL_Surface *surface;
} object_t;

SDL_Surface *screen = NULL;
object_t object;

Uint32 next_time;

void clean_up(void);
void handle_edge_collisions(void);
int random(int lower, int upper);
int toggle_fullscreen(void);
Uint32 time_left(void);

int main(int argc, char **argv)
{
	const float PI_RADIANS = 3.1459 / 180.0f;

	srand(time(NULL));

	int result  = SDL_Init(SDL_INIT_VIDEO);
	if(result != 0)
	{
		fprintf(stderr, "Error initialising SDL: %s", SDL_GetError());
		return 0;
	}

	atexit(clean_up);

	screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if(!screen)
	{
		fprintf(stderr, "Error setting video mode: %s\n", SDL_GetError());
		SDL_Quit();

		return 0;
	}

	object.surface = SDL_LoadBMP("object.bmp");
	if(!object.surface)
	{
		fprintf(stderr, "Error loading bitmap: %s\n", SDL_GetError());
		return 0;
	}

	int moving = 0;
	int quit = 0;

	SDL_Event event;

	// the rate at which the object will fall to the "ground".
	// the greater the value, the heavier it is.
	float gravity = 0.2f;

	// the objects velocity; the greater the value, the higher it'll travel before falling.
	float velocity = 13.0f;

	SDL_Rect dest = { .w = object.surface->w, .h = object.surface->h };
	next_time = SDL_GetTicks() + TICK_INTERVAL;
	while(quit != 1)
	{
		//fprintf(stdout, "%d\n", random(-30, 30));
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_MOUSEBUTTONUP:
				object.x = event.motion.x;
				object.y = event.motion.y;

				// 360 = flat on the X axis, pointing right.
				// we're aiming for a V-shaped range.
				// well..a bit wider than a V. :P
				object.angle = random(220, 340) * PI_RADIANS;
				object.velocity_x = velocity * cos(object.angle);
				object.velocity_y = velocity * sin(object.angle);
				moving = 1;
				break;
			case SDL_KEYUP:
				switch(event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						quit = 1;
						break;
					case SDLK_F1:
						result = toggle_fullscreen();
						if(!result)
						{
							fprintf(stderr, "toggle_fullscreen() failed: %s\n", SDL_GetError());
						}
						break;
					default:
						break;
				}
				break;
			case SDL_QUIT:
				quit = 1;
				break;
			default:
				break;
			}
		}

		if(moving)
		{
			object.x += object.velocity_x;
			object.y += object.velocity_y;

			object.velocity_y += gravity;
		}

		handle_edge_collisions();

		dest.x = object.x;
		dest.y = object.y;

		SDL_FillRect(screen, NULL, 0);
		SDL_BlitSurface(object.surface, NULL, screen, &dest);
		SDL_Flip(screen);

		SDL_Delay(time_left());
		next_time += TICK_INTERVAL;
	
	}

	return 0;
}

// frees up memory allocated to the various surfaces and shutsdown SDL.
void clean_up(void)
{
	if(screen != NULL)
	{
		SDL_FreeSurface(screen);
	}

	if(object.surface != NULL)
	{
		SDL_FreeSurface(object.surface);
	}

	SDL_Quit();
}

//
void handle_edge_collisions(void)
{
	// collided with the left or right edges of the screen
	if(object.x <= 0) 
	{
		object.x = 0;
		object.velocity_x = -object.velocity_x;
	}
	else if(object.x + OBJECT_SIZE >= WINDOW_WIDTH)
	{
		object.x = WINDOW_WIDTH - OBJECT_SIZE;
		object.velocity_x = -object.velocity_x;
	}

	if(object.y <= 0)
	{
		object.y = 0;
		object.velocity_y = -object.velocity_y;
	}
	else if(object.y + OBJECT_SIZE >= WINDOW_HEIGHT)
	{
		object.y = WINDOW_HEIGHT - OBJECT_SIZE;
		object.velocity_y = -object.velocity_y;
	}
}

// generates a value within the range of lower to upper, inclusive. 
// Example: random(0, 3) will generate a value from 0 to 3. 
int random(int lower, int upper)
{
	return ((rand() % (upper - lower + 1)) + lower);
}

// returns 1 if the fullscreen mode was succesfully toggled, otherwise 0.
// In the case of failure, the function will abort the program if it can't return
// to the original resolution and window mode that was in use prior to toggle_fullscreen()
// being called. 
int toggle_fullscreen(void)
{
	Uint32 current_flags = screen->flags;

	// 0 = use the current resolution
	screen = SDL_SetVideoMode(0, 0, 0, screen->flags ^ SDL_FULLSCREEN);
	if(!screen)
	{
		// failed to toggle the window mode; attempt to switch back
		screen = SDL_SetVideoMode(0, 0, 0, current_flags);
		if(!screen)
		{
			// failed to switch back; we're really fucked now. 
			fprintf(stderr, "Failed to restore window mode after fullscreen toggle failed. Exiting.\n");
			exit(1);
		}

		// switching back succeeded; report that the toggle failed.
		return 0;
	}

	// successfully toggled fullscreen mode.
	return 1;
}

// returns the amount of milliseconds the game should sleep for before updating the next frame.
Uint32 time_left(void)
{
	Uint32 now = SDL_GetTicks();
	if(next_time <= now)
	{
		// no need to delay! update *now*!
		return 0;
	}
	else
	{
		return next_time - now;
	}
}

