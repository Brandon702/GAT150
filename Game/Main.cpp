// Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Graphics/Texture.h"
#include <iostream>
#include <SDL_image.h>
#include <SDL.h>

int main(int, char**)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

	SDL_Window* window = SDL_CreateWindow("GAT150", 100, 100, 800, 600, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		IMG_Quit();
		SDL_Quit();
		return 1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr)
	{
		std::cout << "ERROR: " << SDL_GetError() << std::endl;
		IMG_Quit();
		SDL_Quit();
		return 1;
	}
	int width = 128;
	int height = 128;
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, width, height);
	Uint32* pixels = new Uint32[width * height]; // Uint32 (r,g,b,a) (8,8,8,8)
	memset(pixels, 255, width * height * sizeof(Uint32)); //(255,255,255,255) (255,255,255,255)
	SDL_UpdateTexture(texture, NULL, pixels, width * sizeof(Uint32));

	nc::Texture texture2;
	texture2.Create("sf2.png", renderer);
	float angle{ 0 };

	SDL_Event event;
	bool quit = false;
	while (!quit)
	{
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		}

		SDL_SetRenderDrawColor(renderer, 55, 55, 55, 255);
		SDL_RenderClear(renderer);

		//draw

		for (int i = 0; i < width * height; i++)
		{
			Uint8 c1 = rand() % 256;
			Uint8 c2 = rand() % 256;
			Uint8 c3 = rand() % 256;
			pixels[i] = (c1 << 24 | c2 << 16 | c3 << 8);
		}
		SDL_UpdateTexture(texture, NULL, pixels, width * sizeof(Uint32));
		SDL_Rect rect = { 20,20,50,100 };
		SDL_RenderCopy(renderer, texture, nullptr, &rect);
		angle+= 1;
		texture2.Draw({ 500, 100 }, { 2,2 }, angle);

		SDL_RenderPresent(renderer);
	}
	IMG_Quit();
	SDL_Quit();

	return 0;
}
