#include "Enemy.h"

void ReadEnemies(std::vector<Enemy> &vec) {
	rapidxml::xml_document<> doc;
	std::ifstream file("../../res/files/configXML.xml");
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	std::string	content(buffer.str());
	doc.parse<0>(&content[0]);
	rapidxml::xml_node<> *node = doc.first_node()->first_node();
	while (node != nullptr) {
		Enemy enemy = Enemy();
		enemy.SetName(node->first_attribute()->value());
		enemy.SetLifePoints(std::stoi(node->first_node("lifePoints")->value()));
		enemy.SetAttackDamage(std::stoi(node->first_node("attackDamage")->value()));
		enemy.SetDefense(std::stoi(node->first_node("defense")->value()));
		enemy.SetSpeed(std::stoi(node->first_node("attackSpeed")->value()));
		rapidxml::xml_node<> *weapons = node->first_node("weapons")->first_node();
		while (weapons != nullptr) {
			Weapon weapon = Weapon();
			std::string weaponName = weapons->first_attribute()->value();
			std::string weaponType = weapons->first_node("type")->value();
			if (weaponName == "Sword")
				weapon.weaponName = WeaponName::SWORD;
			else if (weaponName == "Bow")
				weapon.weaponName = WeaponName::BOW;
			else if (weaponName == "Dagger")
				weapon.weaponName = WeaponName::DAGGER;
			else if (weaponName == "Staff")
				weapon.weaponName = WeaponName::STAFF;
			else if (weaponName == "Shield")
				weapon.weaponName = WeaponName::SHIELD;
			else if (weaponName == "Axe")
				weapon.weaponName = WeaponName::AXE;
			if (weaponType == "Melee")
				weapon.weaponType = WeaponType::MELEE;
			if (weaponType == "Ranged")
				weapon.weaponType = WeaponType::RANGED;
			weapon.range = std::stoi(weapons->first_node("range")->value());
			enemy.SetWeapon(weapon);
			weapons = weapons->next_sibling();
		}
		vec.push_back(enemy);
		node = node->next_sibling();
	}
}

int main() {
	std::vector<Enemy> enemies;
	ReadEnemies(enemies);
	return 0;
}