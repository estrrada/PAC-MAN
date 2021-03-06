//
//  Game.cpp
//  PAC-MAN 2017
//
//  Created by Carlos Estrada on 11/16/17.
//  Copyright © 2017 Carlos Estrada. All rights reserved.
//

#include "Game.hpp"

/***** GHOST LOGIC *****/
void ghostCollisions(Player &pacman, Ghost &ghost, Audio &eatghost, int &ghostCount, int &score,
                     int &gamestate, Audio &death, bool &edible, Clock &startClock) {
    Vector2f position(512, 501);
    if (checkCollision(pacman, ghost) && ghost.isEdible) {
        ghostCount++;
        ghost.setPosition(position);
        ghost.isEdible = false;
        eatghost.play();
        
        switch(ghostCount) {
            case 1: score += 200;
                break;
            case 2: score += 400;
                break;
            case 3: score += 800;
                break;
            case 4: score += 1600;
                break;
        }
    }
    else if (checkCollision(pacman, ghost) && !ghost.isEdible) {
        gamestate = DYING;
        pacman.setState(DEAD);
        pacman.manageState();
        death.play();
        edible = false;
        startClock.restart().asSeconds();
    }
}

bool checkCollision(Player& object1, Player& object2) {
    return Collision::BoundingBoxTest(object1, object2);
}

void findNode(Player& player, Node& node) {
    if ((Vector2i)node.getPosition() == (Vector2i)player.getPosition()) {
        player.setDirectionAtNode(node);
    }
}

/***** SCORE AND LEVELS *****/
void updatePoints(std::ostringstream &ss, int &score, Text &playerScore) {
    ss.str("");
    ss << score;
    playerScore.setString(ss.str());
}

int getHighScore() {
    std::ifstream readscore(".score");
    if(readscore.is_open()) {
        int score;
        readscore >> score;
        readscore.close();
        return score;
    }
    return 0;
}

void saveHighScore(int score) {
    std::ofstream writescore(".score");
    writescore << score;
    writescore.close();
}

void oneUp(int &score, int &lifeScore, int &lifeCount, Audio &life) {
    if (score >= lifeScore) { // GAIN A LIFE EVERY 10,000 POINTS
        if (lifeCount < 7) {
            lifeCount++;
            lifeScore += 10000;
            life.play();
        }
    }
}

void levelUp(int &pelletCount, int &level, float &looppitch, Audio &siren, Player &fruit) {
    level++;
    if (level < 7) {
        fruit.setState(level - 1);
    }
    else {
        fruit.setState(5);
    }
    fruit.setPosition(Vector2f(512, 576));
    pelletCount = 0;
    looppitch = 1.0;
    siren.setPitch(looppitch);
}

void setDifficulty(Player &blinky, Player &inky, Player &pinky, Player &clyde, float &edibleLimit) {
    if (edibleLimit > 0.0) {
        edibleLimit -= 1.0;
    }
    if (blinky.getPlayerSpeed() < 60) {
        float newspeed = blinky.getPlayerSpeed() + 2;
        blinky.setPlayerSpeed(newspeed);
        inky.setPlayerSpeed(newspeed);
        pinky.setPlayerSpeed(newspeed);
        clyde.setPlayerSpeed(newspeed);
    }
}

/***** SOUNDS *****/
void soundSwitcher(bool &isEdible, int &gamestate, Audio &siren, Audio &scatter) {
    if (gamestate == PLAYING) {
        if (isEdible) {
            siren.stop();
            if (Audio::Paused == scatter.getStatus() || Audio::Stopped == scatter.getStatus()) {
                scatter.play();
            }
        }
        else {
            scatter.stop();
            if (Audio::Paused == siren.getStatus() || Audio::Stopped == siren.getStatus()) {
                siren.play();
            }
        }
    }
    else {
        siren.stop();
        scatter.stop();
    }
}

void toggleMute(bool &isMuted, Player &sound, Audio &chomp1, Audio &chomp2, Audio &scatter, Audio &theme,
                Audio &siren, Audio &eatfruit, Audio &life, Audio &death, Audio &eatghost) {
    float volume;
    if (isMuted) {
        volume = 100; isMuted = false;
        sound.setState(isMuted);
    }
    else {
        volume = 0; isMuted = true;
        sound.setState(isMuted);
    }
    chomp1.setVolume(volume); chomp2.setVolume(volume);
    scatter.setVolume(volume); siren.setVolume(volume);
    eatfruit.setVolume(volume); eatghost.setVolume(volume);
    life.setVolume(volume); death.setVolume(volume);
    theme.setVolume(volume);
}

/***** RESETS *****/
void resetStats(int &lifeCount, int &pelletCount, int &score, int &lifeScore, int &level, float &looppitch,
                Player &fruit, std::ostringstream &ss, Text &playerScore, Clock &startClock) {
    level = 1;
    fruit.setState(0);
    lifeCount = 4;
    pelletCount = 0;
    score = 0;
    lifeScore = 10000;
    looppitch = 1.0;
    updatePoints(ss, score, playerScore);
    startClock.restart().asSeconds();
}

void resetGame(Player &pacman, Ghost &blinky, Ghost &inky, Ghost &pinky, Ghost &clyde, MazeData &maze, int &gamestate) {
    Vector2f pacmanPos(512, 746), blinkyPos(512, 405), inkyPos(463, 501), pinkyPos(512, 501), clydePos(562, 501);
    
    pacman.setPosition(pacmanPos);
    pacman.direction = NONE;
    pacman.queueDirection = NONE;
    pacman.setRotation(0);
    pacman.setFrame(0);
    
    blinky.setPosition(blinkyPos); inky.setPosition(inkyPos); pinky.setPosition(pinkyPos); clyde.setPosition(clydePos);
    blinky.setState(FACERIGHT); inky.setState(FACEUP); pinky.setState(FACEDOWN); clyde.setState(FACEUP);
    blinky.queueDirection = inky.queueDirection = pinky.queueDirection = clyde.queueDirection = NONE;
    blinky.direction = inky.direction = pinky.direction = clyde.direction = NONE;
    blinky.isEdible = inky.isEdible = pinky.isEdible = clyde.isEdible = false;
    
    if (gamestate == WINNER || gamestate == STARTING || gamestate == PAUSED) {
        maze.placePellets();
    }
}
