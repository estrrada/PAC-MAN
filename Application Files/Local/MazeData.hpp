//
//  MazeData.hpp
//  PAC-MAN 2017
//
//  Created by Carlos Estrada on 11/16/17.
//  Copyright © 2017 Carlos Estrada. All rights reserved.
//
#pragma once
#include "Collision.hpp"

class MazeData {
	Texture mazeTexture, pelletTexture, powerpelletTexture;

public:
    Player mazeBG;
    
    MazeData();
    std::shared_ptr<Player> powerPellet;
    std::shared_ptr<Player> pellet;

    std::shared_ptr<Node> node;
    std::shared_ptr<Node> ghostNode;

	void placePellets();
	void placeNodes();
    void setValidNodeMovements();
    void teleportPlayer(Player &player);
    
    void draw(RenderWindow &window, Time &deltaTime);
};
