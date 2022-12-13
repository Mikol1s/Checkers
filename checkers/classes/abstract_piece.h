#pragma once
#include <SFML/Graphics.hpp>

enum PieceColor // typ wyliczeniowy - dziala jak int 
{
    dummy,
    white,
    black
    
};

class AbstractPiece // klasa abstrakcyjna - niedopracowany pionek
{
    private:
        // 1 to dol -1 to gora
        int m_move_direction; 
        //true - zyje , false - nie zyje
        bool m_alive = true;
        // pole tekstury
        sf::Texture* m_texture;
        //sprite texture
        sf::Sprite* m_sprite;
        // warstwa na ktorej ma byc renderowany
        int m_render_priority;
        // kolor 
        PieceColor m_piece_color;
        // krolowa
        bool isQueen = false;
    protected:
        void pi_set_piece_color(PieceColor color); // protected aby klasy mialy dziedziczenie
        void pi_set_move_direction(int direction);
    public:
        // zwraca 1 jesli dol i -1 jesli gora
        int pi_get_move_direction();
        bool pi_get_alive_status();
        void pi_set_alive_status(bool new_status);
        // laduje i powiazuje  sprite z jego tekstura
        virtual void pi_init_texture_and_sprite() = 0; // klasa abstrakcyjna
        sf::Sprite* pi_get_sprite();
        sf::Texture* pi_get_texture();
        void pi_set_sprite(sf::Sprite*);
        void pi_set_texture(sf::Texture*);
        int pi_get_render_priority();
        void pi_set_render_priority(int priority);
        // nadaje kolor pionka
        PieceColor pi_get_piece_color();
        // uwstawienie sprite na pozycje renderowania
        void set_sprite_position(int row, int col);
        // zamiana w krolowa
        void change_to_queen();
        bool is_queen();
};

