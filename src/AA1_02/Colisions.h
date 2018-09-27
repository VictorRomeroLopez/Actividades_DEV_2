#pragma once
#include <SDL.h>
#include "Tipos.h"

bool checkInside(SDL_Rect rect, Vector2 vec) {
	return rect.x < vec.x && rect.y < vec.y && (rect.x + rect.w)>vec.x && (rect.y + rect.h)>vec.y;
}