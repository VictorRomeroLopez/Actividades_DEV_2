#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "../../dep/inc/XML/rapidxml.hpp"
#include "../../dep/inc/XML/rapidxml_iterators.hpp"
#include "../../dep/inc/XML/rapidxml_print.hpp"
#include "../../dep/inc/XML/rapidxml_utils.hpp"

enum class WeaponType { MELEE, RANGED };
enum class WeaponName { SWORD, BOW, STAFF, DAGGER, SHIELD, AXE };

struct Weapon {
	WeaponName weaponName;
	WeaponType weaponType;
	int range;
};

class Enemy
{
private:
	std::string name;
	int lifePoints;
	int attackDamage;
	int attackSpeed;
	int defense;
	std::vector<Weapon> weapon;

public:
	Enemy();
	~Enemy();
	void SetName(std::string);
	void SetLifePoints(int);
	void SetAttackDamage(int);
	void SetSpeed(int);
	void SetDefense(int);
	void SetWeapon(Weapon);
};

