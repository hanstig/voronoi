#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <cstdint>

struct point {
	uint16_t x;
	uint16_t y;
};

class graphics {
private:
	SDL_Renderer *renderer;
	SDL_Window *window;
	
	std::vector<point> points;

	void drawCircle(int16_t centreX, int32_t centreY, uint16_t radius);
	void drawPoints();

	void drawLines();
public:
	void init(uint16_t width, uint16_t height, const char* title);

	void doInput(void);

	void prepareScene(void);

	void presentScene(void);
};

#endif // GRAPHICS_H
