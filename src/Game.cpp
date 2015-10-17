#include <include/Game.h>

Game::Game()
: mRenderer(sf::VideoMode(800U, 600U), "C00165681 - Final Year Project [WIP]", sf::Style::Default, sf::ContextSettings()) {
	
}

Game::~Game() {
	
}

int Game::run() {

	mRenderer.startRenderLoop(60U);
	
	return EXIT_SUCCESS;
}