#include <memory>
#include <fstream>
#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <array>
#include "Logger/Logger.h"

static bool thread_exit = false;
#define REFRESH_EVENT  (SDL_USEREVENT + 1)
int refresh_video(void *opaque){
	while (!thread_exit) {
		SDL_Event event;
		event.type = REFRESH_EVENT;
		SDL_PushEvent(&event);
		SDL_Delay(20);
	}
	return 0;
}
int main(int argc, char** argv){
	if(argc < 2){
		SIMLOG(SimLogger::Error, "please input a file path: ");
		exit(1);
	}
	std::ifstream file_stream;
	std::string in_file = std::string(getenv("HOME")) + "/" + std::string(argv[1]);
	SIMLOG(SimLogger::Debug, "file path: " << in_file );
	file_stream.open(in_file, std::ios::binary);
    const int width = 1920;
    const int height = 1080;
    const int bpp = 32;
    static unsigned char buffer[width*height*bpp/8];
    static unsigned char buffer_convert[width*height*4];
	if(SDL_Init(SDL_INIT_VIDEO)){
		SIMLOG(SimLogger::Error,"Failed to initialize sdl:" << SDL_GetError());
		return 1;
	}
	SDL_Window *window = SDL_CreateWindow("x11 play", 0, 0, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
	if (!window) {
		SIMLOG(SimLogger::Error,"Failed to create window:" << SDL_GetError());
		return 1;
	}
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		SIMLOG(SimLogger::Error,"Failed to create renderer:" << SDL_GetError());
		return 1;
	}
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, width, height);
	if (!texture) {
		SIMLOG(SimLogger::Error,"Failed to create texture:" << SDL_GetError());
		return 1;
	}
    SDL_Rect rect;
    SDL_Thread* thread = SDL_CreateThread(refresh_video, nullptr, nullptr);
    SDL_Event event;

    file_stream.read((char*)buffer, width * height * bpp / 8);
    while(1){
//    while(!file_stream.eof()){
    	SDL_WaitEvent(&event);
    	if(event.type == REFRESH_EVENT){

    		file_stream.read((char*)buffer, width * height * bpp / 8);
    		SDL_UpdateTexture(texture, nullptr, buffer, width*4);
    		rect.x = 0;
    		rect.y = 0;
    		rect.w = width;
    		rect.h = height;
    		SDL_RenderClear(renderer);
    		SDL_RenderCopy(renderer, texture, nullptr, &rect);
    		SDL_RenderPresent(renderer);
    		SDL_Delay(20);
    	}
    	else if(event.type == SDL_WINDOWEVENT){

    	}
    	else if(event.type == SDL_QUIT){
    		thread_exit = true;
    		break;
    	}
    }



	file_stream.close();


	exit(0);
}
