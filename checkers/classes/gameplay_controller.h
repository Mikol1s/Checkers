#pragma once
#include <vector>
#include <iostream>
#include "abstract_piece.h"
#include "pieces.h"


class GameplayController
{
    private:
        // plansza do gry zawierajaca odniesienia do pionow
        std::vector< std::vector<AbstractPiece*> > m_board;// 2 wymiarowa tablica wektorow 
        int m_eliminated_white_pieces = 0, m_eliminated_black_pieces = 0;
        AbstractPiece* m_ref_to_dead_piece = nullptr; // wskaznik do pionka ktory ma byc usuniety
        void set_reference_at_board(AbstractPiece* new_reference, int row, int col)// gdzie postawic pionek
        {
            std::cerr<<" ustawienie "<<new_reference->pi_get_piece_color()<<" na: (wiersz, kolumna) "<<row<<" "<<col<<std::endl;
            m_board[row][col] = new_reference;
        }
        AbstractPiece* get_reference_to_piece_at_board(int row, int col)// pobieranie z konkretnego miejsca wskaznik do pionka
        {
           
           
            return m_board[col][row];
        }
        int get_board_size()
        {
            return m_board.size();
        }
        bool is_tile_taken(std::pair<int, int> tile) // czy miejsce jest zajete
        { 
            
            if (m_board[tile.second][tile.first]->pi_get_piece_color() == PieceColor::dummy)
                return false;
            std::cerr<<"Pole "<<tile.second<<" "<<tile.first<<" jest zajete"<<std::endl;
            return true;
        }
    protected:
        // usuwanie wszystkich referencji z tablicy
        void gc_clear_board();
        // zniszcz pole i zapelnij wyeliminowany pion
        void gc_eliminate_piece(std::pair<int, int> tile);
        // ladowanie konfiguracji
        void gc_load_start_setup();
        // sprawdzanie ruchu
        bool check_move_legal(std::pair<int, int> from_tile, std::pair<int, int> to_tile, bool is_attacking, int move_direction);
        // drukowanie planszy
        void print_board();
    public:
        //czyszczenie wszystkich zmiennych i referencje
        void gc_reset_game();
        //wykonanie ruchu, zwraca czy ruch powiodl sie
        bool gc_make_move(std::pair<int, int> from_tile, std::pair<int, int> to_tile);
        //dostanie tablicy
        std::vector< std::vector<AbstractPiece*> > gc_get_board();
        //probwanie zdobycia koordynat pionka
        AbstractPiece* gc_try_to_get_piece_at_coords(sf::Vector2f coords); 
        AbstractPiece* gc_get_dead_piece_pointer();
        void gc_reset_dead_piece_pointer();
        std::pair<int, int> gc_check_if_has_to_attack(int player_turn_flag);
        bool gc_make_mandatory_attack_move(std::pair<int, int> from_tile, std::pair<int, int> to_tile, std::pair<int, int> attack_tile);
        //zwracaj 0 jesli nie, 1 jesli biale pionki wygraja, 2 jesli czarne wygraja
        int gc_is_game_done();
        //sprawdzanie czy stworzona krolowa, jesli tak to zrob to
        void check_make_queen(AbstractPiece* moving_piece, std::pair<int, int> to_tile);
        bool move_queen(std::pair<int, int> from_tile, std::pair<int, int> to_tile);
        GameplayController();
};