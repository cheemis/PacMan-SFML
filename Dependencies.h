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
int const WINDOW_HEIGHT = 512;
int const WINDOW_WIDTH = 612;

//board varaibles
int const ROWS = 31;
int const COLUMNS = 28;

//tile variables
float const TILE_SIZE = 15.5;
float const TILE_OFFSET = 16.5;

//Pacman variables
float const PACMAN_SPEED = 500;
int const MAX_LIVES = 3;

//Ghost varaibles
int const NUM_GHOSTS = 4;
float const GHOST_SPEED = 550;
float const FLEEING_TIME = 10;
float const FLASHING_TIME = 3;
Color const FLEEING_COLOR = Color::Blue;
Color const DEAD_COLOR = Color(85, 85, 85);
int const CHANCE_TO_RANDOM = 5;

//size variables
float const ENTITY_RADIUS = TILE_SIZE / 2;

//color variables
Color const WALL_COLOR1 = Color::Blue;
Color const WALL_COLOR2 = Color(120, 81, 169); //purple
Color const WALL_COLOR3 = Color::Green;
Color const PELLET_COLOR = Color::White;
Color const POWER_COLOR = Color::Yellow;

//timing variables
float const DEATH_TIME = 2;
float const LOADING_TIME = 4;
float const READY_TIME = 2;

//text variables
string const FONT_NAME = "Retronoid.otf";
float const FONT_TITLE_SIZE = 64;
float const FONT_REGULAR_SIZE = 28;
float const TEXT_SPACER = 55;
float const LIVES_SPACER = 4 * FONT_REGULAR_SIZE + TEXT_SPACER;

//points variables
int const PELLET_POINTS = 1;
int const POWER_POINTS = 10;
int const GHOST_POINTS = 50;
int const COMPLETE_POINTS = 100;


