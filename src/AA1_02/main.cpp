#include <SDL.h>		// Always needs to be included for an SDL app
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <exception>
#include <iostream>
#include <string>
#include "Colisions.h"
#include <time.h>

//Game general information
#define SCREEN_WIDTH 848
#define SCREEN_HEIGHT 480

#define FPS 60

#define BACKGROUND_MENU "../../res/img/bg.jpg"
#define BACKGROUND_PLAY "../../res/img/bgCastle.jpg"

#define FONT_SAYAN "../../res/ttf/saiyan.ttf"
#define FONT_MARIO "../../res/ttf/supermario.ttf"

#define SPRITE_PNJ "../../res/img/spCastle.png"

enum class GameState { PLAYING, PAUSE, MENU };

int main(int, char*[])
{
	// --- INIT SDL ---
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		throw "No es pot inicialitzar SDL subsystems";

	// --- WINDOW ---
	SDL_Window *m_window{ SDL_CreateWindow("SDL...", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN) };
	if (m_window == nullptr)
		throw "No es pot inicialitzar SDL_Window";

	// --- RENDERER ---
	SDL_Renderer *m_renderer{ SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) };
	if (m_renderer == nullptr)
		throw "No es pot inicialitzar SDL_Renderer";

	//-->SDL_Image 
	const Uint8 imgFlags{ IMG_INIT_PNG | IMG_INIT_JPG };
	if (!(IMG_Init(imgFlags) & imgFlags)) throw "Error: SDL_image init";

	//-->SDL_TTF
	if (TTF_Init() != 0) throw "No es pot inicialitzar SDL_ttf";

	//-->SDL_Mix
	#pragma region Audio
	Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
	Mix_Music *music = NULL;
	music = Mix_LoadMUS("../../res/au/mainTheme.mp3");
	Mix_VolumeMusic(2);
	if (music == NULL)return false;
	Mix_PlayMusic(music, -1);
	#pragma endregion

	// --- SPRITES ---
		//Background
	#pragma region Background
	SDL_Texture* bgTexture{ IMG_LoadTexture(m_renderer, BACKGROUND_MENU) };
	if (bgTexture == nullptr) throw "Error: bgTexture init";
	SDL_Rect bgRect{ 0,0,SCREEN_WIDTH, SCREEN_HEIGHT };
	#pragma endregion

	SDL_Texture *playerTexture{ IMG_LoadTexture(m_renderer, "../../res/img/dragonball.png") };
	if (playerTexture == nullptr) throw "No s'han pogut crear les";
	SDL_Rect playerRect{ 0, 0, 50, 50 };
	SDL_Rect playerTarget{ 0, 0, 100, 100 };

	//-->Animated Sprite ---
	SDL_Texture *playerTexture2{ IMG_LoadTexture(m_renderer, SPRITE_PNJ) };
	SDL_Rect playerRect2, playerPosition2;
	int textWidth, textHeight, frameWidth, frameHeight;
	SDL_QueryTexture(playerTexture2, NULL, NULL, &textWidth, &textHeight);
	frameWidth = textWidth / 12;
	frameHeight = textHeight / 8;
	playerPosition2.x = playerPosition2.y = 0;
	playerRect2.x = playerRect2.y = 0;
	playerPosition2.h = playerRect2.h = frameHeight;
	playerPosition2.w = playerRect2.w = frameWidth;
	int frameTime = 0;

	// --- TEXT ---
		//fuente que usaremos
	TTF_Font *font{ TTF_OpenFont( FONT_SAYAN, 50) };
	TTF_Font *scoreFont{ TTF_OpenFont( FONT_MARIO, 50) };

	if (font == nullptr) throw "No es pot inicialitzar la font saiyan";
	if (scoreFont == nullptr) throw "No es pot inicialitzar la font super mario";
		//texto con el color
	SDL_Surface *tmpSurf{ TTF_RenderText_Blended(font, "Dragon Ball Z" , SDL_Color{255,255,0,0}) };
	if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
		//Creamos una textura con el surface que queremos
	SDL_Texture *tittleTextNorm{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		//Cambiamos la surface para la siguiente Textura
	tmpSurf = { TTF_RenderText_Blended(font, "Dragon Ball Z" , SDL_Color{ 0,255,0,0 }) };
		//Creamos una nueva textura con el texto cambiado
	SDL_Texture *tittleTextHover{ SDL_CreateTextureFromSurface(m_renderer,tmpSurf) };
		//Creamos una textura auxiliar que contendrá el texture que tengamos que usar
	SDL_Texture *tittleTextAux{ SDL_CreateTextureFromSurface(m_renderer,tmpSurf) };
		//Creamos el espacio para el texto
	SDL_Rect textTittle{ (SCREEN_WIDTH - tmpSurf->w) / 2,50, tmpSurf->w, tmpSurf->h };

	#pragma region PlayButton
	tmpSurf = { TTF_RenderText_Blended(font, "Play", SDL_Color{ 255,0,0,0 }) };
	SDL_Texture *playTextNormal{ SDL_CreateTextureFromSurface(m_renderer,tmpSurf) };
	tmpSurf = { TTF_RenderText_Blended(font, "Play", SDL_Color{ 150,0,0,0 }) };
	SDL_Texture *playTextHover{ SDL_CreateTextureFromSurface(m_renderer,tmpSurf) };
	SDL_Texture *playTextAux{ SDL_CreateTextureFromSurface(m_renderer,tmpSurf) };
	SDL_Rect textPlay{ (SCREEN_WIDTH - tmpSurf->w) / 2,300, tmpSurf->w, tmpSurf->h };
	#pragma endregion
	#pragma region SoundOffButton
	tmpSurf = { TTF_RenderText_Blended(font, "Sound off", SDL_Color{ 255,0,0,0 }) };
	SDL_Texture *soundOffTextNormal{ SDL_CreateTextureFromSurface(m_renderer,tmpSurf) };
	tmpSurf = { TTF_RenderText_Blended(font, "Sound off", SDL_Color{ 150,0,0,0 }) };
	SDL_Texture *soundOffTextHover{ SDL_CreateTextureFromSurface(m_renderer,tmpSurf) };
	SDL_Texture *soundTextAux{ SDL_CreateTextureFromSurface(m_renderer,tmpSurf) };
	SDL_Rect textSoundOff{ (SCREEN_WIDTH - tmpSurf->w) / 2,350, tmpSurf->w, tmpSurf->h };
	#pragma endregion
	#pragma region SoundOnButton
	tmpSurf = { TTF_RenderText_Blended(font, "Sound on", SDL_Color{ 255,0,0,0 }) };
	SDL_Texture *soundOnTextNormal{ SDL_CreateTextureFromSurface(m_renderer,tmpSurf) };
	tmpSurf = { TTF_RenderText_Blended(font, "Sound on", SDL_Color{ 150,0,0,0 }) };
	SDL_Texture *soundOnTextHover{ SDL_CreateTextureFromSurface(m_renderer,tmpSurf) };
	SDL_Rect textSoundOn{ (SCREEN_WIDTH - tmpSurf->w) / 2,350, tmpSurf->w, tmpSurf->h };
	#pragma endregion
	#pragma region Exit
	tmpSurf = { TTF_RenderText_Blended(font, "Exit", SDL_Color{ 255,0,0,0 }) };
	SDL_Texture *exitTextNormal{ SDL_CreateTextureFromSurface(m_renderer,tmpSurf) };
	tmpSurf = { TTF_RenderText_Blended(font, "Exit", SDL_Color{ 150,0,0,0 }) };
	SDL_Texture *exitTextHover{ SDL_CreateTextureFromSurface(m_renderer,tmpSurf) };
	SDL_Texture *exitTextAux{ SDL_CreateTextureFromSurface(m_renderer,tmpSurf) };
	SDL_Rect textExit{ (SCREEN_WIDTH - tmpSurf->w) / 2,400, tmpSurf->w, tmpSurf->h };
	#pragma endregion
	SDL_Texture *timeTexture;


	//tanquem la surface i font ja que no la tornem a utilitzar
	TTF_CloseFont(font);

	// --- AUDIO ---

	// --- GAME LOOP ---
	SDL_Event event;
	bool isRunning = true;
	bool leftMouseButton = false;
	bool sound = true;
	GameState gameState = GameState::MENU;
	clock_t lastTime = clock();
	char timeArray[5];
	float timeDown = 60;
	float deltaTime = 0;

	while (isRunning) {
		// HANDLE EVENTS
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				isRunning = false;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) isRunning = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT)
					leftMouseButton = true;
				break;
			case SDL_MOUSEMOTION:
				playerTarget.x = event.motion.x - 25;
				playerTarget.y = event.motion.y - 25;
				break;
			default:;
			}
		}
		// UPDATE

		#pragma region CheckTittle
		if (checkInside(textTittle, Vector2{ event.motion.x, event.motion.y }))
			tittleTextAux = tittleTextHover;
		else
			tittleTextAux = tittleTextNorm;

		#pragma endregion
		#pragma region CheckPlayButton
		if (checkInside(textPlay, Vector2{ event.motion.x, event.motion.y })) {
			playTextAux = playTextHover;
			if (leftMouseButton) {
				bgTexture = IMG_LoadTexture(m_renderer, BACKGROUND_PLAY);
				gameState = GameState::PLAYING;
				if (bgTexture == nullptr) throw "Error: bgTexture init";
				leftMouseButton = false;
			}
		}
		else
			playTextAux = playTextNormal;

		#pragma endregion
		#pragma region CheckSoundButton
		if (checkInside(textSoundOff, Vector2{ event.motion.x, event.motion.y })) {
			if (sound) {
				soundTextAux = soundOffTextHover;
				Mix_ResumeMusic();
			}
			else {
				soundTextAux = soundOnTextHover;
				Mix_PauseMusic();
			}
			if (leftMouseButton) {
				sound = !sound;
				leftMouseButton = false;
			}
		}
		else {
			if (sound)
				soundTextAux = soundOffTextNormal;
			else
				soundTextAux = soundOnTextNormal;
		}

		#pragma endregion
		#pragma region CheckExitButton
		if (checkInside(textExit, Vector2{ event.motion.x, event.motion.y })) {
			exitTextAux = exitTextHover;
			if (leftMouseButton) {
				isRunning = false;
				leftMouseButton = false;
			}
		}
		else
			exitTextAux = exitTextNormal;

		#pragma endregion


		tmpSurf = { TTF_RenderText_Blended(scoreFont, timeArray, SDL_Color{ 0, 255, 0, 0 }) };
		timeTexture = SDL_CreateTextureFromSurface(m_renderer, tmpSurf);
		SDL_Rect timeTittle{ SCREEN_WIDTH - tmpSurf->w - 30 , 30, tmpSurf->w, tmpSurf->h };

		frameTime++;
		
		if (FPS / frameTime <= 9)
		{
			frameTime = 0;
			playerRect2.x += frameWidth;
			if (playerRect2.x >= textWidth / 4)
				playerRect2.x = 0;
		}

		//Up
		playerRect2.y = frameHeight * 3;

		playerRect.x += (playerTarget.x - playerRect.x) / 10;
		playerRect.y += (playerTarget.y - playerRect.y) / 10;

		// DRAW
		SDL_RenderClear(m_renderer);

		//Background
		SDL_RenderCopy(m_renderer, bgTexture, nullptr, &bgRect);

		//Player

		if (gameState == GameState::MENU) {
			//Text
			SDL_RenderCopy(m_renderer, tittleTextAux, nullptr, &textTittle);
			//Buttons
			SDL_RenderCopy(m_renderer, playTextAux, nullptr, &textPlay);
			SDL_RenderCopy(m_renderer, soundTextAux, nullptr, &textSoundOff);
			SDL_RenderCopy(m_renderer, exitTextAux, nullptr, &textExit);
		} 
		else if(gameState == GameState::PLAYING) {
			SDL_RenderCopy(m_renderer, playerTexture, nullptr, &playerRect);
			SDL_RenderCopy(m_renderer, playerTexture2, &playerRect2, &playerPosition2);
			SDL_RenderCopy(m_renderer, timeTexture, nullptr, &timeTittle);

			if (timeDown < 0) {
				timeDown = 60;
				bgTexture = IMG_LoadTexture(m_renderer, "../../res/img/bg.jpg");
				gameState = GameState::MENU;
			}
			snprintf(timeArray, sizeof(timeArray), "%f", timeDown);
			//Control de segons que portem
			#pragma region DeltaTime
			deltaTime = (clock() - lastTime);
			lastTime = clock();
			deltaTime /= CLOCKS_PER_SEC;
			timeDown -= deltaTime;
			#pragma endregion
		}
		else if (gameState == GameState::PAUSE) {
			
		}

		leftMouseButton = false;
		//Update screen
		SDL_RenderPresent(m_renderer);
	}

	// --- DESTROY ---
	SDL_DestroyTexture(bgTexture);
	SDL_DestroyTexture(playerTexture);
	#pragma region CloseTittle
	SDL_DestroyTexture(tittleTextNorm);
	SDL_DestroyTexture(tittleTextHover);
	SDL_DestroyTexture(tittleTextAux);
	#pragma endregion
	#pragma region ClosePlayButton
	SDL_DestroyTexture(playTextNormal);
	SDL_DestroyTexture(playTextHover);
	SDL_DestroyTexture(playTextAux);
	#pragma endregion
	#pragma region CloseSoundOffButton
	SDL_DestroyTexture(soundOffTextNormal);
	SDL_DestroyTexture(soundOffTextHover);
	SDL_DestroyTexture(soundTextAux);
	#pragma endregion
	#pragma region CloseSoundOnButton
	SDL_DestroyTexture(soundOnTextNormal);
	SDL_DestroyTexture(soundOnTextHover);
	SDL_DestroyTexture(soundTextAux);
	#pragma endregion
	#pragma region CloseExitButton
	SDL_DestroyTexture(exitTextNormal);
	SDL_DestroyTexture(exitTextHover);
	SDL_DestroyTexture(exitTextAux);
	#pragma endregion
	Mix_CloseAudio();
	Mix_Quit();
	SDL_FreeSurface(tmpSurf);
	TTF_CloseFont(scoreFont);
	IMG_Quit();
	TTF_Quit();
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);

	// --- QUIT ---
	SDL_Quit();

	return 0;
}