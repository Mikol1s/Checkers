#pragma once
#include <SFML/Graphics.hpp>
#include "abstract_piece.h"

class PieceBlack : public AbstractPiece
{
private:
	// nazwa obrazu w katalogu z rozszerzeniem
	std::string m_image_pathname = "black_piece.png";
public:
	//zaladowanie tekstury i ustawienie sprite
	void pi_init_texture_and_sprite();
	//konstruktor
	PieceBlack()
	{
		//zdefiniowanie koloru
		pi_set_piece_color(PieceColor::black);
		//zdefiniowanie kierunku ruchu
		pi_set_move_direction(1);
		//wczytuje wizualizacje
		pi_init_texture_and_sprite();
	}
};

class PieceWhite : public AbstractPiece
{
    private:
        // nazwa obrazu w katalogu z rozszerzeniem
        std::string m_image_pathname = "white_piece.png";
    public:
        //zaladowanie tekstury i ustawienie sprite
        void pi_init_texture_and_sprite();
        //konstruktor
        PieceWhite()
        {
            //zdefiniowanie koloru
            pi_set_piece_color(PieceColor::white);
            //zdefiniowanie kierunku ruchu
            pi_set_move_direction(-1);
            //wczytuje wizualizacje
            pi_init_texture_and_sprite();
        }
};



class PieceDummy : public AbstractPiece //dummy - zapychacz, aby na kazdym polu byl pionek ale na tym ktorych nie znajduja sie biale badz czarne nie widac w grafice
{
    private:
        //nazwa obrazu w katalogu z rozszerzeniem
        std::string m_image_pathname = "";
    public:
        //zaladowanie tekstury i ustawienie sprite
        void pi_init_texture_and_sprite()
        {
            //nic nie robi
        }
        //konstruktor
        PieceDummy()
        {
            //zdefiniowanie koloru
            pi_set_piece_color(PieceColor::dummy);
        }
};
