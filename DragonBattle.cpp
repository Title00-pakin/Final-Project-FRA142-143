#include "DragonBattle.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

// Character class implementation
Character::Character(string n, int health) {
    name = n;
    hp = health;
    maxHp = health;
}

void Character::showStats() {
    cout << name << " HP: " << hp << "/" << maxHp << endl;
}

void Character::takeDamage(int damage) {
    hp = hp - damage;
    if (hp < 0) hp = 0;
}

void Character::heal(int amount) {
    hp = hp + amount;
    if (hp > maxHp) hp = maxHp;
}

bool Character::isAlive() {
     return hp > 0;
}

// Paladin class implementation
Paladin::Paladin(string n) : Character(n, 100) {
    immortalMode = false;
    totalDamage = 0;
}

void Paladin::takeDamage(int damage) {
    if (immortalMode) {
        cout << name << " is immortal! No damage taken!" << endl;
        immortalMode = false;
        return;
    }
    // reduce damage by 20%
    int reducedDmg = damage * 0.8;
    
    hp = hp - reducedDmg;
    if (hp < 0) hp = 0;
    cout << name << " takes " << reducedDmg << " damage (reduced from " << damage << ")" << endl;
}

int Paladin::useSkill(int skill, int dice2) {
    if (skill == 1) {
        // attack skill
        int dmg = 15;
        if (dice2 == 6) dmg = dmg * 2;
        totalDamage = totalDamage + dmg;
        cout << name << " attacks for " << dmg << " damage!" << endl;
        return dmg;
    } else {
        // immortal skill
        immortalMode = true;
        cout << name << " activates immortal mode!" << endl;
        return 0;
    }
}

// Warrior class implementation
Warrior::Warrior(string n) : Character(n, 100) {
    totalDamage = 0;
}

int Warrior::useSkill(int skill, int dice2) {
    if (skill == 1) {
        int dmg = 25;
        if (dice2 >= 5) dmg = dmg * 2; // crit on 5-6
        totalDamage = totalDamage + dmg;
        cout << name << " attacks for " << dmg << " damage!" << endl;
        return dmg;
    } else {
        // heavy attack
        int dmg = 40;
        if (dice2 >= 5) dmg = dmg * 2;
        totalDamage = totalDamage + dmg;
        cout << name << " heavy attack for " << dmg << " damage!" << endl;
        return dmg;
    }
}

// Mage class implementation
Mage::Mage(string n) : Character(n, 100) {
    totalDamage = 0;
}

void Mage::passive() {
    heal(7);
    cout << name << " heals 7 HP from passive!" << endl;
}

int Mage::useSkill(int skill, int dice2) {
    if (skill == 1) {
        int dmg = 15;
        if (dice2 == 6) dmg = dmg * 2;
        totalDamage = totalDamage + dmg;
        cout << name << " magic attack for " << dmg << " damage!" << endl;
        return dmg;
    } else {
        // team heal skill
        int healAmt = 30;
        if (dice2 == 6) healAmt = healAmt * 2;
        cout << name << " team heal for " << healAmt << " HP!" << endl;
        return -healAmt;
    }
}

// Dragon class implementation
Dragon::Dragon() : Character("Dragon", 500) {}

int Dragon::attack() {
    if (hp >= 100) {
        return 22;
    } else {
        return 39;
    }
}

void Dragon::showStats() {
    cout << "Dragon HP: " << hp << "/500";
    if (hp >= 100) {
        cout << " (Phase 1)" << endl;
    } else {
        cout << " (Phase 2)" << endl;
    }
}

// Game class implementation
Game::Game() {
    dragon = new Dragon();
    turnCount = 0;
    srand(time(0));
}

Game::~Game() {
    delete dragon;
    for (int i = 0; i < 2; i++) {  // แก้ไข: ใช้ 2 แทน heroes.size()
        delete heroes[i];
    }
}

int Game::rollDice() {
    return (rand() % 6) + 1;
}

bool Game::canUseSkill(int heroType, int skill, int dice) {
    if (heroType == 1) {
        if (skill == 1) return dice > 1;
        if (skill == 2) return dice >= 5;
    } else if (heroType == 2) {
        if (skill == 1) return dice > 1;
        if (skill == 2) return dice >= 5;
    } else if (heroType == 3) {
        if (skill == 1) return dice > 1;
        if (skill == 2) return dice > 5;
    }
    return false;
}

void Game::selectHeroes() {
    cout << "=== Hero Selection ===" << endl;
    cout << "Choose your heroes:" << endl;
    cout << "1. Paladin - has damage reduction and immortal skill" << endl;
    cout << "2. Warrior - high damage with critical hits" << endl;
    cout << "3. Mage - can heal and has passive regeneration" << endl;
    
    bool used[4] = {false, false, false, false};
    
    for (int i = 1; i <= 2; i++) {
        cout << "\nPlayer " << i << " pick class (1-3): ";
        int choice;
        cin >> choice;
        
        // validation loop
        while (choice < 1 || choice > 3 || used[choice]) {
            if (used[choice]) {
                cout << "Already picked! Choose different: ";
            } else {
                cout << "Invalid! Choose 1-3: ";
            }
            cin >> choice;
        }
        
        used[choice] = true;
        string playerName = "Player " + to_string(i);
        
        if (choice == 1) {
            heroes.push_back(new Paladin(playerName));
            cout << playerName << " is now Paladin!" << endl;
        } else if (choice == 2) {
            heroes.push_back(new Warrior(playerName));
            cout << playerName << " is now Warrior!" << endl;
        } else if (choice == 3) {
            heroes.push_back(new Mage(playerName));
            cout << playerName << " is now Mage!" << endl;
        }
    }
}

void Game::playerTurn(int playerIdx, int heroType) {
    cout << "\n" << heroes[playerIdx]->name << "'s turn:" << endl;
    
    // mage has passive healing
    if (heroType == 3) {
        Mage* mage = static_cast<Mage*>(heroes[playerIdx]);  // แก้ไข: ใช้ static_cast
        mage->passive();
    }
    
    // roll first dice
    int dice1 = rollDice();
    cout << "Dice roll: " << dice1 << endl;
    
    // check what skills are available
    vector<int> availableSkills;
    for (int skill = 1; skill <= 2; skill++) {
        if (canUseSkill(heroType, skill, dice1)) {
            availableSkills.push_back(skill);
        }
    }
    
    if (availableSkills.size() == 0) {
        cout << "No skills available!" << endl;
        return;
    }
    
    // show available skills
    cout << "Available skills: ";
    for (int i = 0; i < availableSkills.size(); i++) {
        cout << availableSkills[i];
        if (i != availableSkills.size() - 1) cout << ", ";
    }
    cout << endl;
    
    // Always prompt player to choose skill (removed auto-selection)
    int skillChoice;
    cout << "Pick skill: ";
    cin >> skillChoice;
    
    // check if valid choice
    bool validChoice = false;
    for (int i = 0; i < availableSkills.size(); i++) {
        if (availableSkills[i] == skillChoice) {
            validChoice = true;
            break;
        }
    }
    while (!validChoice) {
        cout << "Wrong choice! Pick valid skill: ";
        cin >> skillChoice;
        for (int i = 0; i < availableSkills.size(); i++) {
            if (availableSkills[i] == skillChoice) {
                validChoice = true;
                break;
            }
        }
    }
    
    // roll second dice for skill effect
    int dice2 = rollDice();
    cout << "Second dice: " << dice2 << endl;
    
    int result = 0;
    if (heroType == 1) {
        Paladin* paladin = static_cast<Paladin*>(heroes[playerIdx]);  // แก้ไข: ใช้ static_cast
        result = paladin->useSkill(skillChoice, dice2);
    } else if (heroType == 2) {
        Warrior* warrior = static_cast<Warrior*>(heroes[playerIdx]);  // แก้ไข: ใช้ static_cast
        result = warrior->useSkill(skillChoice, dice2);
    } else if (heroType == 3) {
        Mage* mage = static_cast<Mage*>(heroes[playerIdx]);  // แก้ไข: ใช้ static_cast
        result = mage->useSkill(skillChoice, dice2);
    }
    
    if (result > 0) {
        // damage dragon
        dragon->takeDamage(result);
    } else if (result < 0) {
        // heal team
        int healAmount = -result;
        heroes[0]->heal(healAmount);
        heroes[1]->heal(healAmount);
    }
}

void Game::dragonTurn() {
    cout << "\nDragon attacks!" << endl;
    
    // find alive heroes
    vector<int> targets;
    for (int i = 0; i < 2; i++) {
        if (heroes[i]->isAlive()) {
            targets.push_back(i);
        }
    }
    
    if (targets.size() == 0) return;
    
    // pick random target
    int targetIdx = targets[rand() % targets.size()];
    int dmg = dragon->attack();
    cout << "Dragon hits " << heroes[targetIdx]->name << " for " << dmg << " damage!" << endl;
    
    heroes[targetIdx]->takeDamage(dmg);
}

void Game::gameResults() {
    cout << "\n=== GAME OVER ===" << endl;
    cout << "Battle lasted " << turnCount << " turns" << endl;
    
    // แก้ไข: ใช้ heroTypes array ที่เก็บไว้แล้วใน run() function
    // เนื่องจากไม่สามารถเข้าถึง heroTypes ได้จากที่นี่ ให้ใช้วิธีง่ายๆ
    
    // show damage dealt by each player  
    for (int i = 0; i < 2; i++) {
        int dmg = 0;
        
        // ตรวจสอบว่าเป็น class ไหนโดยใช้ dynamic_cast แบบปลอดภัย
        if (Paladin* p = dynamic_cast<Paladin*>(heroes[i])) {
            dmg = p->totalDamage;
        } else if (Warrior* w = dynamic_cast<Warrior*>(heroes[i])) {
            dmg = w->totalDamage;
        } else if (Mage* m = dynamic_cast<Mage*>(heroes[i])) {
            dmg = m->totalDamage;
        }
        
        cout << heroes[i]->name << " dealt " << dmg << " total damage" << endl;
    }
    
    if (dragon->isAlive()) {
        cout << "Dragon wins! Heroes defeated!" << endl;
    } else {
        cout << "Heroes win! Dragon defeated!" << endl;
    }
}

void Game::run() {
    cout << "=== Dragon Battle RPG ===" << endl;
    cout << "Welcome to the dragon battle!" << endl;
    
    selectHeroes();
    cout << "\n=== BATTLE START ===" << endl;
    
    // keep track of hero types for easier handling
    int heroTypes[2];
    for (int i = 0; i < 2; i++) {
        if (dynamic_cast<Paladin*>(heroes[i])) {
            heroTypes[i] = 1;
        } else if (dynamic_cast<Warrior*>(heroes[i])) {
            heroTypes[i] = 2;
        } else {
            heroTypes[i] = 3;
        }
    }
    
    // main game loop
    while (dragon->isAlive() && (heroes[0]->isAlive() || heroes[1]->isAlive())) {
        turnCount++;
        cout << "\n--- Turn " << turnCount << " ---" << endl;
        
        // players take turns
        for (int i = 0; i < 2; i++) {
            if (heroes[i]->isAlive()) {
                playerTurn(i, heroTypes[i]);
            }
            if (!dragon->isAlive()) break;
        }
        
        // dragon attacks if still alive
        if (dragon->isAlive() && (heroes[0]->isAlive() || heroes[1]->isAlive())) {
            dragonTurn();
        }
        
        // show current status
        cout << "\nCurrent Status:" << endl;
        for (int i = 0; i < 2; i++) {  // แก้ไข: ใช้ 2 แทน heroes.size()
            heroes[i]->showStats();
        }
        dragon->showStats();
    }
    
    gameResults();
}

int main() {
        Game game;
        game.run();
        return 0;
    }