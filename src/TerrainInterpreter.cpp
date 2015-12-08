#include <include/TerrainInterpreter.h>

const sf::Color TerrainInterpreter::mGrassColor = sf::Color::Green;
const sf::Color TerrainInterpreter::mPathColor = sf::Color::White;

const unsigned char TerrainInterpreter::GRASS = 0x01;
const unsigned char TerrainInterpreter::PATH = 0x10;
const unsigned char TerrainInterpreter::NONE = 0x00;