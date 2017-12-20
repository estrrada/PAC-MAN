//
//  Player.hpp
//  PAC-MAN 2017
//
//  Created by Carlos Estrada on 11/16/17.
//  Copyright © 2017 Carlos Estrada. All rights reserved.
//
#pragma once
#include "Nodes.hpp"
#include <memory>

/* Directions */
#define NONE       -1
#define LEFT        0
#define RIGHT       1
#define UP          2
#define DOWN        3

/* PAC-MAN States */
#define ALIVE       0
#define DEAD        1

/* Ghost States */
#define FACELEFT    0
#define FACERIGHT   1
#define FACEUP      2
#define FACEDOWN    3
#define SCATTER     4
#define BLINK       5

/* Game States */
#define STARTING    0    // WHEN GAME IS AT "READY" SCREEN
#define PLAYING     1    // WHEN PLAYER IS UNPAUSED
#define DYING       2    // WHEN PACMAN IS DYING
#define PAUSED      3    // WHEN GAME IS PAUSED
#define WINNER      4    // WHEN PLAYER COLLECTS ALL PELLETS
#define LOSER       5    // WHEN LIVES DROP TO ZERO

class Player : public AnimatedSprite {
    std::shared_ptr<Animation> state;
    Texture objtexture;
    IntRect TextureRect;
    int pstate = 0;
    
public:
    int direction = NONE, queueDirection = NONE, tryDirection = NONE;
    bool WASDkeys = true, isEdible = false;
    Vector2f movement;
    float speed_;
    
    Player(float speed = 10);
    Player(std::string spritesheet, size_t spritesize, size_t numberofStates, size_t framesperState, float speed = 10);
    void initialize(std::string spritesheet, size_t spritesize, size_t numberofStates, size_t framesperState, float speed = 10);
    
    void switchState(size_t playerstate);
    int getState() { return pstate; }
    void setPlayerSpeed(float speed);
    float getPlayerSpeed() { return speed_ * 120; }
    void setAnimationSpeed(float speed);
    
    IntRect getTextureRect();

    void nextDirection();
    void setDirectionAtNode(Node node);
    void setDirectionOpposite();
    void movePlayer(Time deltaTime, int gstate);
    void moveGhost(Time deltaTime, int gstate, float edibleTime, float edibleLimit);

    void ghostAI();

private:
    bool isCurrentDirectionValid(Node node, int choosedirection);
    int findOpposite(int dir);
};


