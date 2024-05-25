#include <stdio.h>;
#include <SDL.h>;
#include <SDL_ttf.h>
#include <cstdio>
#include <vector>;
#include <string>;
#include <algorithm>;
#include "Constant.h";
#include "Circle.h";

circle cir;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;
SDL_Surface* surface;
TTF_Font* font;

int game_is_running = FALSE;
int last_frame_time = 0;
int collision_count = 0;
const float gravity = 9.81f;
char keep_count[100];


struct Particle {
	// position
	int x;
	int y;
	int r;
	// velocity
	float vx;
	float vy;
	// acceleration
	float a;
}particle;

int initializeWindow(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "ERROR\n");
		return 0;
	}

	window = SDL_CreateWindow(
		"SIMULATOR",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		640, 640,
		SDL_WINDOW_BORDERLESS
	);

	if (!window) {
		fprintf(stderr, "ERROR INITALIZING WINDOW \n");
		return 0;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer) {
		fprintf(stderr, "ERROR INITALIZING RENDERER \n");
		return 0;
	}

	if (TTF_Init() == -1)
	{
		return false;
	}

	font = TTF_OpenFont("PublicPixel-0W5Kv.ttf", 40);
	
	if (!font) {
		fprintf(stderr, "ERROR LOADING FONT \n");
	}
}
void processInput() {
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_QUIT:
		game_is_running = FALSE;
		break;

	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE) {
			game_is_running = FALSE;
			printf("exit");
			break;
		}
	}
}

void start() {
	particle.x = 640 / 2;
	particle.y = 640 / 2;
	particle.r = 20;
	particle.vx = 100;
	particle.vy = 200;
	particle.a = 0;

	collision_count = 0;
}

void textRend() {
	surface = TTF_RenderText_Solid(font, keep_count, (SDL_Color{ 255,255,255,255 }));
	if (!surface) {
		fprintf(stderr, "SURFACE CREATION FAILED \n");
	}

	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture) {
		fprintf(stderr, "TEXTURE CREATION FAILED \n");
	}
}

void update() {
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);
	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
		SDL_Delay(time_to_wait);
	}

	//get a delta time factor, convert it to seconds to be used to update my game objects;
	float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;
	//store the ms of the current frame to be used in the next one;
	last_frame_time = SDL_GetTicks();

	//dynamics
	particle.vy += gravity * delta_time;

	particle.vx += particle.a * delta_time;
	particle.x += particle.vx * delta_time;
	
	particle.vy += particle.a * delta_time;
	particle.y += particle.vy * delta_time;
	int s;
	s = sprintf_s(keep_count, "%d", collision_count);

	//collision detection
	if (particle.x <= 0 || particle.x >= 640) {
		particle.vx = -particle.vx;
		collision_count++;
		//particle.a += 10;
		printf("Collision count: %d\n", collision_count);
		printf("Acceleration: %f\n", particle.a);
		printf("VX: %f\n", particle.vx);
		printf("\n");
	}
	if (particle.y <= 0 || particle.y >= 640) {
		particle.vy = -particle.vy; 
		collision_count++;
		//particle.a += 10;
		printf("Collision count: %d\n", collision_count);
		printf("Acceleration: %f\n", particle.a);
		printf("VX: %f\n", particle.vx);
		printf("\n");
	}

	textRend();
}


void render() {
	SDL_SetRenderDrawColor(renderer, 25, 25, 25, 55);
	SDL_RenderClear(renderer);


	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	cir.SDL_RenderFillCircle(renderer, particle.x, particle.y, particle.r);

	//SDL_Rect text_rect{
	//	text_rect.x = 5,
	//	text_rect.y = 5,
	//	text_rect.w = 140,
	//	text_rect.h = 10
	//};
	//SDL_RenderCopy(renderer, texture, NULL, &text_rect);

	SDL_Rect count_text_rect{
	count_text_rect.x = 640 / 2,
	count_text_rect.y = 5,
	count_text_rect.w = 10,
	count_text_rect.h = 10
	};
	SDL_RenderCopy(renderer, texture, NULL, &count_text_rect);
	//SDL_UpperBlit(c_surface, NULL, header_surface, &count_text_rect);

	SDL_RenderPresent(renderer);
}


void destroyWindow() {
	SDL_FreeSurface(surface);
	TTF_CloseFont(font);
	TTF_Quit;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_QUIT;
}

int main(int argc, char* argv[]) {
	game_is_running = initializeWindow();
	start();

	while (game_is_running) {
		processInput();
		update();
		render();
	}

	destroyWindow();

	return 0;
}

///
/// TODO:
/// [x] create a sphere
/// [x] particle dynamics
///		[x]pos, v, a
/// [x] collision detection
/// [x] collision response
/// [] ui elements
/// [] multiple balls
///		[] sphere collisions
///		[] sphere collision responses
/// [] continuous collision detection to avoid tunneling at high speed
/// 