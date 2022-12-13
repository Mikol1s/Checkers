#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>

//renderowanie grafiki na ekranie
class GraphicsRenderer
{
    private:
        std::map<int, std::vector<sf::Sprite*>> m_sprite_map;
        std::map<int, std::vector<sf::Text*>> m_text_map;
    public:
        //dodanie sprite do renderowania, piorytetem jest wartwa, im wyzsza, tym wiecej na wierzchu
        void gr_add_sprite_to_rendering(sf::Sprite* sprite, int priority);
        bool gr_remove_sprite_from_rendering(sf::Sprite*, int layer);

        //dodawanie textu do renderowania
        void gr_add_text_to_rendering(sf::Text* text, int priority);

        //Renderowanie ramki z aktualnym stanem okna
        void gr_render_whole_scene(sf::RenderWindow* window);

        //konstruktor z inicjatorami
        GraphicsRenderer()
        {
            //mapa wartw 
            for(int i =0; i<10; i++)
            {
                m_sprite_map.insert(std::make_pair(i, std::vector<sf::Sprite*>()));
            }
        }// sprite- grafika 2d
};
