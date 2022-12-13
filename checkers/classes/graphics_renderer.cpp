#include "graphics_renderer.h"
#include "abstract_piece.h"
#include <iostream>


void GraphicsRenderer::gr_render_whole_scene(sf::RenderWindow* window)// renderowanie okna "warcaby"
{
    sf::Event event;
    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window->close();
    }

    window->clear();
    //renderowanie wszystkich sprite na kolejnych warstwach
    for(auto record : m_sprite_map)
    {
        for(auto spr : record.second)
        {
            window->draw(*spr);
        }
    }

    //renderowanie wszystkich tekstur na kolejnych warstwach
    for(auto record : m_text_map)
    {
        for(auto txt : record.second)
        {
            window->draw(*txt);
        }
    }
    window->display();
}

//piorytet 0-plansza, 1- na planszy
void GraphicsRenderer::gr_add_sprite_to_rendering(sf::Sprite* sprite, int priority)
{
    m_sprite_map[priority].push_back(sprite);
    std::cerr<<"Dodano nowego sprite do renderowania na warstwie: "<<priority<<std::endl;
}

void GraphicsRenderer::gr_add_text_to_rendering(sf::Text* text, int priority)
{
    std::cerr<<"Adres do zrenderowania "<<text<<std::endl;
    m_text_map[priority].push_back(text);
    std::cerr<<"Dodano nowy rekst do renderowania na warstwie: "<<priority<<std::endl;
}

bool GraphicsRenderer::gr_remove_sprite_from_rendering(sf::Sprite* sprite, int layer)
{
    std::cerr<<"Koniec"<<std::endl;
    for(int i =0 ; i < m_sprite_map[layer].size(); i++)
    {
        if(m_sprite_map[layer][i] == sprite)
            {
                m_sprite_map[layer].erase(m_sprite_map[layer].begin() + i);
                std::cerr<<"Usunieto sprite z renderowania"<<std::endl;
                return true;
            }
    }
    return false;
}



