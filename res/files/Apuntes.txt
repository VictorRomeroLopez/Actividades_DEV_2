int main(int argc, char* arg)

/*
	//Crear una app bàsica
	SDL_Renderer * m_renderer = nullptr;
	SDL_Window * m_window = nullptpr;

	//INIT SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
		throw "no se puede";

	m_window = SDL_CreateWindow("First project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (m_window == nullptr) 
		throw "no es pot iniciar";

	//RENDERER
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (m_renderer == nullptr)
		throw "no es pot iniciar";

	SDL_SetRendererDrawColor(m_renderer, 255,255,255,255);

	SDLWINDOW
	- SDL_CreateWindow(...)
	- DestroyWindow(win*)
	cantonada superior esquerra pos (0,0)

	SDLRENDER
	- SDL_CreateRender(...)
	- SDL_SetRenderDrawColor(...)
	//Fer al final
	- SDL_RenderPresent(r*)//tira tot el buffer per pantalla
	- SDL_RenderClean(r*)
	- SDL_DestroyRender(r*)

	SDL_Point: (x,y)
	SDL_Line:((x,y),(x,y))
	SDL_Rect: (x,y,w,h);

	SDL_Color(r,g,b,a) [0,255]

	Per pintar una imatge
	#include <SDL_image.h>
	init: 
		//operaciones binarias
		const Uint8 imgFlags{ IMG_INIT_PNG | IMG_INIT_JPG };
		if(!(IMG_Init(imgFlags) & imgFlags)) throw "Error";

		IMG_Quit();

	SDL_image
	Background:
	SDL_Texture *bgTexture{IMG_LoadTexture(render, "Path relatiu")};
	if(bgTexture == nullptr) throw "Error";
	SDL_Rect bgRect{0,0,SCREEN_WIDTH, SCREEN_HEIGHT};

	//GAMELOOP
	SDL_RenderCopy(m_renderer, bgTexture, nullptr, &bgRect);
	SDL_RenderPResent(m_renderer);
	SDL_RenderClean(m_renderer);

	SDL_DestroyTexture(bgTexture);


*/
