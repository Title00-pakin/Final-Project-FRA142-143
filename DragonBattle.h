#ifndef DRAGON_BATTLE_H
#define DRAGON_BATTLE_H

#include <string>
#include <vector>
using namespace std;

class Character {
public:
    string name;
    int hp;
    int maxHp;
    
    Character(string n, int health);
    virtual ~Character() = default;
    virtual void showStats();
    virtual void takeDamage(int damage);
    void heal(int amount);
    bool isAlive();
};

class Paladin : public Character {
public:
    bool immortalMode;
    int totalDamage;
    
    Paladin(string n);
    void takeDamage(int damage) override;
    int useSkill(int skill, int dice2);
};

class Warrior : public Character {
public:
    int totalDamage;
    
    Warrior(string n);
    int useSkill(int skill, int dice2);
};

class Mage : public Character {
public:
    int totalDamage;
    
    Mage(string n);
    void passive();
    int useSkill(int skill, int dice2);
};

class Dragon : public Character {
public:
    Dragon();
    int attack();
    void showStats() override;
};

class Game {
private:
    vector<Character*> heroes;
    Dragon* dragon;
    int turnCount;
    
public:
    Game();
    ~Game();
    
    int rollDice();
    bool canUseSkill(int heroType, int skill, int dice);  
    void selectHeroes();
    void playerTurn(int playerIdx, int heroType);
    void dragonTurn();
    void gameResults();
    void run();
};

#endif