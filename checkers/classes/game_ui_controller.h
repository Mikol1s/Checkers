#pragma once
#include <vector>
#include "abstract_piece.h"
#include "pieces.h"
#include <SFML/Graphics.hpp>
#include "graphics_renderer.h"
#include "gameplay_controller.h"

class GameUiController
{
    private:
        GraphicsRenderer* m_renderer_ref;// wskaznik do klasy renderujacej
        GameplayController* m_gameplay_controller_ref; //wskaznik do klasy kontrolujacej
        sf::Vector2i m_mouse_coords;
        sf::RenderWindow* m_window;
        std::pair<int, int> m_origin_tile;
        AbstractPiece* m_grabbed_piece;
        bool m_is_piece_grabbed = false;
        // czyja kolej 1-black, -1white
        int m_whose_turn = 1;
        bool m_has_to_attack = false;
        std::pair<int, int> m_has_to_attack_tile;// pole pionka ktore trzeba zbic
        sf::Sprite* m_highlighted_tile;// wybrane pole
        // vektor przyciskow z string etykieta i sprite
        std::vector< std::pair<std::string,sf::Sprite* > > m_ui_buttons;
        std::vector< std::pair<std::string,sf::Text* > > m_ui_texts;
        sf::Font m_font;

    protected:
        void update_mouse_coordinates();
        sf::Text* create_text(std::string name, std::string content, int pos_x, int pos_y, int character_size);
        // zamienianie koordynat na wiersze i kolory na planszy
        std::pair<int, int> coords_to_row_col(sf::Vector2f);
        void highlight_tile(std::pair<int, int> tile);
        void un_highlight_tile();
        void check_game_over();
        // Oglasza do wszystkich predefiniowanych elementow interfejsu uzytkownika że ​​dany element zostal zabity
        void piece_killed_ui_broadcast(PieceColor pclr);
        // aktualizacja licznika
        void update_kill_counter(std::string color_string);
        // przyciski
        void exit_game();
        void reset_game();
    public:
        void load_board_tiles_and_add_to_render();
        void load_all_ui_background_visuals();
        void load_pieces();
        void resolve_frame_events();
        void check_queen_conversion(AbstractPiece* moving_piece, std::pair<int, int> to_tile);

        GameUiController(GraphicsRenderer* renderer_ref, 
                            GameplayController *gameplay_controller_ref,
                            sf::RenderWindow* window) // konstruktor ktory przyjmuje wskazniki na render, controller, okno
        {
            m_gameplay_controller_ref = gameplay_controller_ref;
            m_renderer_ref = renderer_ref;
            m_window = window;
            m_is_piece_grabbed = false;
            load_pieces();
            load_all_ui_background_visuals();
            load_board_tiles_and_add_to_render();
             

        }
};

