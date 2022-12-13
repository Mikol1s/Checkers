#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include "classes/abstract_piece.h"
#include "classes/pieces.h"
#include "classes/graphics_renderer.h"
#include "classes/gameplay_controller.h"
#include "classes/game_ui_controller.h"

int main() //uruchamianie gry
{
    sf::RenderWindow window(sf::VideoMode(1024, 640), "Warcaby");
    GraphicsRenderer graphicsRenderer;
    GameplayController gameplayController;
    GameUiController gameUiController(&graphicsRenderer, &gameplayController, &window);

    

    while (window.isOpen())
    {
        gameUiController.resolve_frame_events();
        graphicsRenderer.gr_render_whole_scene(&window);
    }
    return 0;
}

