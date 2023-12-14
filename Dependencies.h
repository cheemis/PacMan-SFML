#pragma once

//C++ Libraries
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <queue>
#include <stack>

//SFML
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include <SFML/Graphics/Shape.hpp>


using namespace std;
using namespace sf;

namespace fs = std::filesystem;

// ========================================
// ====== Constant Global Variables  ======
// ========================================

//window variables
int const WINDOW_SIZE = 512;

//board varaibles
int const ROWS = 28;
int const COLUMNS = 31;

//tile variables
float const TILE_SIZE = 15.5;
float const TILE_OFFSET = 16.5;

//board variables
float const STARTING_SPEED = 500;
float const SPEED_INCREASE = 10;

//size variables
float const ENTITY_RADIUS = TILE_SIZE / 2;

//color variables
Color const WALL_COLOR1 = Color::Blue;
Color const WALL_COLOR2 = Color::Red;
Color const WALL_COLOR3 = Color::Green;
Color const PELLET_COLOR = Color::White;

//collision variables (depricated)
int const COLLISION_REGION_SIZE = 4;

//timing variables
float const DEATH_TIME = 2;
float const LOADING_TIME = 1;




