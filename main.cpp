#include "CommonFunc.h"
#include "BaseObject.h"
//#include "Game_map.h"
BaseObject gBackground;
SDL_Texture* PlayerStanding;
BaseObject Itachi;

// Animation
const int FRAME_WIDTH = 32;   // Each frame width
const int FRAME_HEIGHT = 32;  // Frame height
const int NUM_FRAMES = 4;     // Total number of frames

SDL_Rect spriteClips[NUM_FRAMES];  // Array to store frame data

const int FRAME_COUNT = 3;  // Số frame trong animation
const int FRAME_DELAY = 100;

// Animation control

SDL_Event event;

int x = SCREEN_WIDTH/2-30;  // Player position
int y = 550;
int frame = 0;
float scaleFactor = 2.0f;

Uint32 lastFrameTime = 0;
const int frameDelay = 200;   // Time (ms) per frame
const int speed = 2;

bool Init()
{
	bool success = true;
	int ret = IMG_Init(SDL_INIT_VIDEO);
	if (ret < 0)
	{
		success = false;
	}
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

	gWindow = SDL_CreateWindow("C++ SDL2 game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL) {
		success = false;
	}
	else
	{
		gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
		if (gRenderer == NULL) {
			success = false;
		}
		else {
			SDL_SetRenderDrawColor(gRenderer, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) && imgFlags)) {
				success = false;
			}
		}
	}
	return success;
}

void close()
{
	gBackground.Free();

	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	IMG_Quit();
	SDL_Quit();
}

bool loadBackground()
{
	bool ret = gBackground.LoadImg("res/Grass-map1.png", gRenderer);
	if (ret == false) return false;
	return true;
}



// Animation player
SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer) {
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (!loadedSurface) {
		std::cout << "Failed to load image: " << IMG_GetError() << std::endl;
		return nullptr;
	}
	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 192, 192, 192));

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
	SDL_FreeSurface(loadedSurface);
	return texture;
}


// Render animation Player
void renderAnimation(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, int frame, float scale) {
	if (!texture) return;

	SDL_Rect spriteClips[4] = {
		{0, 0, 32, 32},  // First frame
		{32, 0, 32, 32}, // Second frame
		{64, 0, 32, 32},
		{96, 0, 32, 32},
	};

	// Scale the sprite
	SDL_Rect renderQuad = {
		x, y,
		static_cast<int>(32 * scale),
		static_cast<int>(32 * scale)
	};



	SDL_RenderCopy(renderer, texture, &spriteClips[frame], &renderQuad);
}

//Render animation boss
std::vector<SDL_Texture*> frames;

void LoadFrames(SDL_Renderer* renderer) {
	for (int i = 1; i < FRAME_COUNT+1; i++) {
		char filename[50];
		sprintf_s(filename, sizeof(filename), "res/Amaterasu/%d.png", i);

		SDL_Texture* texture = loadTexture(filename,renderer);
		if (texture) frames.push_back(texture);
	}
}

void RenderItachiAnimation(SDL_Renderer* renderer, int x, int y) {
	static int currentFrame = 0;
	static Uint32 lastTime = 0;

	Uint32 now = SDL_GetTicks();
	if (now - lastTime >= FRAME_DELAY) {  // Đổi frame sau FRAME_DELAY ms
		if (!frames.empty()) {  // Kiểm tra nếu frames có dữ liệu
			currentFrame = (currentFrame + 1) % frames.size();
		}

		lastTime = now;
	}

	SDL_Rect dstRect = { x, y, 100, 100 };  // Vị trí hiển thị nhân vật
	SDL_RenderCopy(renderer, frames[currentFrame], NULL, &dstRect);
}

void FreeFrames() {
	for (SDL_Texture* tex : frames) {
		SDL_DestroyTexture(tex);
	}
	frames.clear();
}


// Main
int main(int argv, char* argc[]) {
	if (Init() == false) return -1;
	if (loadBackground() == false) return -1;

	SDL_Texture* playerSprite = loadTexture("res/Player-sheet.png", gRenderer);
	if (!playerSprite) {
		std::cout << "Player texture failed to load!" << std::endl;
	}

	LoadFrames(gRenderer);
	bool gameStop = false;
	while (!gameStop) {
		while (SDL_PollEvent(&gEvent) != 0) {
			if (gEvent.type == SDL_QUIT) {
				gameStop = true;
			}
			
		}
		// Player movement
		// Player movement
		const Uint8* keys = SDL_GetKeyboardState(NULL);
		if (keys[SDL_SCANCODE_W] && y - speed >= 0) y -= speed;
		if (keys[SDL_SCANCODE_A] && x - speed >= 0) x -= speed;
		if (keys[SDL_SCANCODE_S] && y + speed + FRAME_HEIGHT * scaleFactor <= SCREEN_HEIGHT) y += speed;
		if (keys[SDL_SCANCODE_D] && x + speed + FRAME_WIDTH * scaleFactor <= SCREEN_WIDTH) x += speed;

		
	
		
		

		// Update frame every `frameDelay` milliseconds
		if (SDL_GetTicks() - lastFrameTime > frameDelay) {
			frame = (frame + 1) % 4;  // Loop between 0, 1, 2
			lastFrameTime = SDL_GetTicks();
		}
		SDL_SetRenderDrawColor(gRenderer, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);

		SDL_RenderClear(gRenderer);
		SDL_Delay(16);
		gBackground.Render(gRenderer, NULL);
		
		renderAnimation(gRenderer, playerSprite, x, y, frame,scaleFactor);
		RenderItachiAnimation(gRenderer, 200, 200);
		
		
		SDL_RenderPresent(gRenderer);
		
		
	}
	FreeFrames();
	close();
	return 0;
}