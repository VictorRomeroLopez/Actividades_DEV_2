#include "Enemy.h"

Enemy::Enemy(){

}

Enemy::~Enemy(){

}

void Enemy::SetName(std::string value) {
	name = value;
}
void Enemy::SetAttackDamage(int value) {
	attackDamage = value;
}
void Enemy::SetLifePoints( int value) {
	lifePoints = value;
}
void Enemy::SetSpeed(int value) {
	attackSpeed = value;
}
void Enemy::SetDefense(int value) {
	defense = value;
}
void Enemy::SetWeapon(Weapon value) {
	weapon.push_back(value);
}
