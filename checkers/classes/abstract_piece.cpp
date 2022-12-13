#include "abstract_piece.h"
#include <iostream>

int AbstractPiece::pi_get_move_direction()// zwracanie zmiennych prywatnych
{
    return m_move_direction;
}

void AbstractPiece::pi_set_move_direction(int direction)
{
    if (direction == 1 || direction == -1)
        m_move_direction = direction;
    else
        std::cerr<<"Zly kierunek"<<std::endl;
}

bool AbstractPiece::pi_get_alive_status()
{
    return m_alive;
}

void AbstractPiece::pi_set_alive_status(bool new_status)
{
    m_alive = new_status;
}

sf::Sprite* AbstractPiece::pi_get_sprite()
{
    return m_sprite;
}

sf::Texture* AbstractPiece::pi_get_texture()
{
    return m_texture;
}

void AbstractPiece::pi_set_texture(sf::Texture* new_texture)
{
    m_texture = new_texture;
}

void AbstractPiece::pi_set_sprite(sf::Sprite* new_sprite)
{
    m_sprite = new_sprite;
}

void AbstractPiece::set_sprite_position(int row, int col)
{
    if(m_piece_color != PieceColor::dummy)
        m_sprite->setPosition(sf::Vector2f(col,row));
}

int AbstractPiece::pi_get_render_priority()
{
    return m_render_priority;
}

void AbstractPiece::pi_set_render_priority(int priority)
{
    m_render_priority = priority;
}

void AbstractPiece::pi_set_piece_color(PieceColor color)
{
    m_piece_color = color;
}

PieceColor AbstractPiece::pi_get_piece_color()
{
    return m_piece_color;
}

void AbstractPiece::change_to_queen()
{
    isQueen = true;
    std::string filename = m_piece_color == PieceColor::black ? "black_piece_queen.png" : "white_piece_queen.png"; // warunek 3 elementowy, jesli polor pionka jest czarny ustawia nazwe pliku do wczytania fillname

    sf::Texture* texture = new sf::Texture();
    if ( texture->loadFromFile("./source/" + filename)  )
    {
        texture->setSmooth(true);
        pi_set_texture(texture);

        sf::Sprite* sprite = new sf::Sprite();
        sprite->setTexture(*texture);
        sprite->setScale(0.125, 0.125); //skala = pomniejszenie 8 razy do 64 px bo pliki 512X512
        sprite->setPosition(m_sprite->getPosition());
        pi_set_sprite(sprite);

        std::cerr<<"Zamiana w krolowa"<<std::endl;
    }
    else
    {
        std::cerr<<"Blad podczas ladowania obrazu jako tekstury"<<std::endl;
    }
}

bool AbstractPiece::is_queen()
{
    return isQueen;
}