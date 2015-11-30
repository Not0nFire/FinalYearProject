#ifndef _TERRAIN_INTERPRETER_H
#define _TERRAIN_INTERPRETER_H

#include <SFML/Graphics.hpp>

class TerrainInterpreter {
private:
	sf::Image mImage;
	static const sf::Color mGrassColor, mPathColor;
public:
	static const unsigned char GRASS	=	0x01;
	static const unsigned char PATH		=	0x10;

	TerrainInterpreter(std::string imagePath){
		mImage.loadFromFile(imagePath);
	}

	unsigned char interpretPixel(unsigned int x, unsigned int y) {
		unsigned char terrainFlags = 0x00;
		const sf::Color pixel = mImage.getPixel(x, y);

		if (pixel == mGrassColor) {
			terrainFlags = GRASS;
		} else if (pixel == mPathColor) {
			terrainFlags = PATH;
		}

		return terrainFlags;
	}

	unsigned char interpretArea(unsigned int left, unsigned int top, unsigned int width, unsigned int height) {
		unsigned char terrainFlags = 0x00;
		const unsigned int right = left + width;
		const unsigned int bottom = top + height;

		//for each column in the area...
		for (unsigned int y = top; y < bottom; ++y) {

			//for each row in the column...
			for (unsigned int x = left; x < right; ++x) {

				//interpret the data of the pixel at x,y
				const unsigned char pixelFlag = interpretPixel(x, y);

				//if the flag of the pixel is not already in the terrainFlags field...
				if (!(terrainFlags & pixelFlag)) {
					terrainFlags = terrainFlags | pixelFlag;
				}
				else if ((terrainFlags & PATH) && (terrainFlags & GRASS)) {
					//break if terrainFlags already holds all possible flags
					break;
				}

			}//end for(x)

			if ((terrainFlags & PATH) && (terrainFlags & GRASS)) {
				//break if terrainFlags already holds all possible flags
				break;
			}

		}//end for(y)

		return terrainFlags;
	}
};

const sf::Color TerrainInterpreter::mGrassColor = sf::Color::Green;
const sf::Color TerrainInterpreter::mPathColor = sf::Color::White;
#endif