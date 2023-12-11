#pragma once

//C++ Libraries
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <queue>

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
// =========== Global Variables  ==========
// ========================================

int const ROWS = 28;
int const COLUMNS = 31;

float const TILE_SIZE = 15.5;
float const TILE_OFFSET = 16.5;

Color const WALL_COLOR = Color::Blue;

