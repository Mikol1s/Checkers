#include "game_ui_controller.h"
#include <iostream>

sf::Sprite* load_spirite_and_texture(std::string path, float scale = 0.125f)
{
    sf::Texture* texture = new sf::Texture();
    sf::Sprite* sprite = new sf::Sprite();

    if (texture->loadFromFile("./source/" + path))
    {
        texture->setSmooth(true);

        sprite->setTexture(*texture);
        sprite->setScale(scale, scale); //skalowanie 8 razy do 64px
        sprite->setPosition(0, 0);
    }
    else
    {
        std::cerr << "Blad podczas ladowania obrazkow na tekstury" << std::endl;
    }

    return sprite;
}

sf::Text* GameUiController::create_text(std::string name, std::string content, int pos_x, int pos_y, int character_size)// ustawia wszystkie zmienne potrzebne
{
    sf::Text* text = new sf::Text();
    text->setPosition(pos_x, pos_y);
    text->setCharacterSize(character_size);
    text->setFont(m_font);
    text->setFillColor(sf::Color::Black);
    text->setString(content);

    m_ui_texts.push_back(std::make_pair(name, text));
    return text;
}

void GameUiController::load_pieces()// ladowanie pionkow
{
    std::cout << "Plansza" << std::endl;
    auto b = m_gameplay_controller_ref->gc_get_board();
    

    for (int i = 0; i < b.size(); i++)
    {
        std::cout << i << ": ";
        for (int j = 0; j < b[0].size(); j++)
        {
            std::cout << b[i][j]->pi_get_piece_color() << " ";
            b[i][j]->set_sprite_position(i * 64, j * 64);
            if (b[i][j]->pi_get_piece_color() != PieceColor::dummy)
                m_renderer_ref->gr_add_sprite_to_rendering(b[i][j]->pi_get_sprite(), 1);
        }
        std::cout << std::endl;
    }
    std::cout << "Koniec planszy" << std::endl;
}

void GameUiController::load_board_tiles_and_add_to_render()// dodawanie pol do renderowania
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (i % 2 == 0 && j % 2 == 0 || i % 2 == 1 && j % 2 == 1)
            {
                sf::Sprite* sp = load_spirite_and_texture("tile_0.png");
                sp->setPosition(j * 64, i * 64);
                m_renderer_ref->gr_add_sprite_to_rendering(sp, 0);
            }
            else
            {
                sf::Sprite* sp = load_spirite_and_texture("tile_1.png");
                sp->setPosition(j * 64, i * 64);
                m_renderer_ref->gr_add_sprite_to_rendering(sp, 0);
            }
        }
    }
}

void GameUiController::highlight_tile(std::pair<int, int> tile)// dodawanie nowego pola (podswietlonego) na warstwe wyzej 
{
    sf::Sprite* sp = load_spirite_and_texture("tile_2.png");
    sp->setPosition(tile.first * 64, tile.second * 64);
    m_renderer_ref->gr_add_sprite_to_rendering(sp, 0);
    m_highlighted_tile = sp;
}

void GameUiController::un_highlight_tile()// usuwanie sprite
{
    m_renderer_ref->gr_remove_sprite_from_rendering(m_highlighted_tile, 0);
}

void GameUiController::update_mouse_coordinates() // koordynaty myszki
{
    m_mouse_coords = sf::Mouse::getPosition((*m_window));
}

std::pair<int, int> GameUiController::coords_to_row_col(sf::Vector2f coords)// zamienianie koordynat X i Y na pola plansz
{
    int tile_width = 64;
    int tile_height = 64;
    int board_offset_horizontal = 0;// przesuniecia
    int board_offset_vertical = 0;

    int row = (coords.x - board_offset_horizontal) / tile_width;
    int col = (coords.y - board_offset_vertical) / tile_height;

    std::cerr << "Zamiana coordynat: " << coords.x << " " << coords.y << " na kolor - wiersza: " << row << " " << col << std::endl;

    if (row < 10 && col < 10)
        return std::make_pair(row, col);

    return std::make_pair(-1, -1);
}

void GameUiController::check_game_over()
{
    //Sprawdzanie czy gra powinna sie zakonczyc
    if (m_gameplay_controller_ref->gc_is_game_done() != 0)
    {
        int result = m_gameplay_controller_ref->gc_is_game_done();
        if (result == 1)
        {
            std::cerr << "Biale wygraly!!!!!!!" << std::endl;
        }
        else
        {
            std::cerr << "Czarne wygraly!!!!!" << std::endl;
        }
    }
}

void GameUiController::resolve_frame_events()
{
    sf::Event event;
    m_window->pollEvent(event);
    update_mouse_coordinates();

    //aktualizowanie sprite koordynat jesli myszka jest trzymana na czyms
    if (m_is_piece_grabbed == true)
    {
        sf::Vector2i c = m_mouse_coords;
        c.x -= 32;
        c.y -= 32;
        m_grabbed_piece->pi_get_sprite()->setPosition(m_window->mapPixelToCoords(c));
    }

    if (event.type == sf::Event::Closed) m_window->close();

    if (event.type == sf::Event::MouseButtonPressed)
    {
        std::cerr << "Kursor myszki zostal klikniety: " << m_mouse_coords.x << ", " << m_mouse_coords.y << std::endl;

        //Przyciski do chwycenia
        //----------------------------------------
        for (auto btn : m_ui_buttons)
            if (btn.second->getGlobalBounds().contains(m_window->mapPixelToCoords(m_mouse_coords)))
            {
                if (btn.first == "Restartuj gre")
                    reset_game();
                if (btn.first == "Wyjscie")
                    exit_game();

                std::cerr << "Wcisniety przycisk: " << btn.first << std::endl;
            }
        //----------------------------------------

        //Proba przesuniecia pionka
        //----------------------------------------
        if (m_is_piece_grabbed == false)
        {
            auto grabbed_piece = m_gameplay_controller_ref->gc_try_to_get_piece_at_coords(m_window->mapPixelToCoords(m_mouse_coords));
            if (grabbed_piece != nullptr) // czyli jakis pionek 
            {
                if (grabbed_piece->pi_get_move_direction() == m_whose_turn)
                {
                    std::cerr << "Pionek wlasnie zostal zlapany" << std::endl;
                    m_is_piece_grabbed = true;
                    m_grabbed_piece = grabbed_piece;
                    m_origin_tile = coords_to_row_col(m_window->mapPixelToCoords(m_mouse_coords));//kopiuje miejsce orginalnego chwyconego pionka
                }
                else
                {
                    std::cerr << "Probojesz zlapac pionek niewlasciwego gracza. Nie oszukuj ;)" << std::endl;
                }
            }
        }
        else
        {
            //Proba postawienia pionka
            std::pair<int, int> rowcol = coords_to_row_col(m_window->mapPixelToCoords(m_mouse_coords));
            std::cerr << "Proba przesuniecia pionka z " << m_origin_tile.first << "," << m_origin_tile.second << " do "
                << rowcol.first << "," << rowcol.second << std::endl;

            if (rowcol.first >= 0)
            {
                bool successfull_move;

                if (m_has_to_attack)
                    successfull_move = m_gameplay_controller_ref->gc_make_mandatory_attack_move(m_origin_tile, rowcol, m_has_to_attack_tile);
                else
                    successfull_move = m_gameplay_controller_ref->gc_make_move(m_origin_tile, rowcol);


                if (successfull_move)
                {

                    std::cerr << "Pionek zostal postawiony na " << rowcol.first << "," << rowcol.second << std::endl;
                    m_grabbed_piece->pi_get_sprite()->setPosition(rowcol.first * 64, rowcol.second * 64);
                    bool killed_any = false;
                    if (m_gameplay_controller_ref->gc_get_dead_piece_pointer() != nullptr)
                    {
                        std::cerr << "Pion zostal zbity" << std::endl;
                        killed_any = true;
                        piece_killed_ui_broadcast(m_gameplay_controller_ref->gc_get_dead_piece_pointer()->pi_get_piece_color());
                        std::cerr << "Pion zostal zbity" << std::endl;
                        m_renderer_ref->gr_remove_sprite_from_rendering(
                            m_gameplay_controller_ref->
                            gc_get_dead_piece_pointer()->
                            pi_get_sprite(),
                            1
                        );
                        std::cerr << "Pion zostal zbity" << std::endl;
                        m_gameplay_controller_ref->gc_reset_dead_piece_pointer();
                        un_highlight_tile();
                        check_game_over();
                        std::cerr << "Pion zostal zbity" << std::endl;
                    }

                    check_queen_conversion(m_grabbed_piece, rowcol);// sprawdzanie czy pionek nie przeszedl na pole przemiany w krolowa

                    if (killed_any)
                    {
                        //Sprawdzanie czy mozna wykonac wiecej atakow
                        m_has_to_attack_tile = m_gameplay_controller_ref->gc_check_if_has_to_attack(m_whose_turn);
                        m_has_to_attack = (m_has_to_attack_tile.first == -1) ? false : true;
                    }

                    if (!m_has_to_attack)
                    {
                        m_whose_turn *= -1;
                    }

                    //Sprawdzanie czy w rundzie gracz musi zaatakowac
                    m_has_to_attack_tile = m_gameplay_controller_ref->gc_check_if_has_to_attack(m_whose_turn);
                    m_has_to_attack = (m_has_to_attack_tile.first == -1) ? false : true;

                    if (m_has_to_attack)
                    {
                        //Zrobienie tak zeby zaatakowal
                        //Jesli nastepny gracz ma obowiazek zaatakowac zezwol tylko na ten ruch i podswietl pole
                        if (m_has_to_attack)
                        {
                            std::cerr << "Musisz zaatakowac plytke: " << m_has_to_attack_tile.first << " " << m_has_to_attack_tile.second << std::endl;
                            highlight_tile(m_has_to_attack_tile);
                        }
                    }

                }
                else
                {
                    std::cerr << "Niedozwolony ruch z " << m_origin_tile.first << "," << m_origin_tile.second << " do "
                        << rowcol.first << "," << rowcol.second << std::endl;
                    m_grabbed_piece->pi_get_sprite()->setPosition(m_origin_tile.first * 64, m_origin_tile.second * 64);
                }
                m_grabbed_piece = nullptr;
                m_is_piece_grabbed = false;
                m_origin_tile = std::make_pair(0, 0);

            }
          

        }
        //----------------------------------------
    }
}

void GameUiController::load_all_ui_background_visuals()
{
    //tlo
    //--------------------------------------
    sf::Sprite* sp = load_spirite_and_texture("background.png", 1.f);
    sp->setPosition(0, 0);
    m_renderer_ref->gr_add_sprite_to_rendering(sp, 0);
    //--------------------------------------

    //przyciski
    //--------------------------------------
    int buttons = 2;
    std::string button_names[2] = {
        "Restartuj gre",
        "Wyjscie"
    };

    int start_x = 768;// w prawo przycisk
    int start_y = 0;
    int break_between_buttons = 32;
    for (int i = 0; i < buttons; i++)
    {
        sf::Sprite* sp = load_spirite_and_texture("button.png", 0.5f);
        sp->setPosition(start_x, start_y + 64 * i);
        m_renderer_ref->gr_add_sprite_to_rendering(sp, 1);
        m_ui_buttons.push_back(std::make_pair(button_names[i], sp));

        //Przyciski tekst
        sf::Text* reset_text = create_text(button_names[i], button_names[i], start_x + 32, start_y + 10 + 64 * i, 32);
        m_renderer_ref->gr_add_text_to_rendering(reset_text, 1);
    }
    //--------------------------------------

    //Czcionka
    //--------------------------------------
    m_font.loadFromFile("./source/BebasNeue-Regular.ttf");
    //--------------------------------------

    int white_counter_y = 512;//pozycja gdzie ma byc licznik pionkow
    int black_counter_y = 576;
    //Licznik - tekst
    //--------------------------------------
    sf::Text* dead_white_pieces_cnt = create_text("dead_white", "0", start_x + 64, white_counter_y + 8, 32);// tworzenie tekstu
    m_renderer_ref->gr_add_text_to_rendering(dead_white_pieces_cnt, 1);

    sf::Text* dead_black_pieces_cnt = create_text("dead_black", "0", start_x + 64, black_counter_y + 8, 32);
    m_renderer_ref->gr_add_text_to_rendering(dead_black_pieces_cnt, 1);

    sf::Text* dead_pieces_text = create_text("dead_pieces", "Zbite Pionki:", start_x + 8, white_counter_y - 32, 32);
    m_renderer_ref->gr_add_text_to_rendering(dead_pieces_text, 1);

    sf::Text* turn_header_text = create_text("turn_header_text", "Aktualny gracz: ", start_x + 8, white_counter_y - 128, 32);
    

    sf::Text* turn_text = create_text("turn_text", "Czarny", start_x + 8, white_counter_y - 96, 32);
   


    //Grafika pionkow w liczniku
    //--------------------------------------
    sf::Sprite* white_pc = load_spirite_and_texture("white_piece.png");
    white_pc->setPosition(start_x, white_counter_y);
    m_renderer_ref->gr_add_sprite_to_rendering(white_pc, 1);


    sf::Sprite* black_pc = load_spirite_and_texture("black_piece.png");
    black_pc->setPosition(start_x, black_counter_y);
    m_renderer_ref->gr_add_sprite_to_rendering(black_pc, 1);


    //--------------------------------------


}

void GameUiController::piece_killed_ui_broadcast(PieceColor pclr)// podawanie koloru i wywolanie licznika 
{
    std::string color_string;
    if (pclr == PieceColor::white)
        color_string = "white";
    else if (pclr == PieceColor::black)
        color_string = "black";

    update_kill_counter(color_string);
}

void GameUiController::update_kill_counter(std::string color_string)// aktualizuje sie licznik zabitych danego koloru i wywolanie
{
    std::string full_text_string = "dead_" + color_string;
    for (int i = 0; i < m_ui_texts.size(); i++)
    {
        if (m_ui_texts[i].first == full_text_string)
        {
            std::string s = m_ui_texts[i].second->getString();
            int current = atoi(s.c_str());

            m_ui_texts[i].second->setString(std::to_string(++current));
            std::cerr << "Licznik " << full_text_string << " zostal zaktualizowany " << current << std::endl;
        }
    }
}

void GameUiController::exit_game()
{
    std::cerr << "Zamykanie gry" << std::endl;
    m_window->close();
    exit(0);
}

void GameUiController::reset_game()
{
    std::cerr << "Resetowanie gry!" << std::endl;

    //Usuniecie i ponowne zaladowanie wszystkich pionow
    auto b = m_gameplay_controller_ref->gc_get_board();
    for (int i = 0; i < b.size(); i++)
        for (int j = 0; j < b[0].size(); j++)
            if (b[i][j]->pi_get_piece_color() != PieceColor::dummy)
            {
                bool removed = m_renderer_ref->gr_remove_sprite_from_rendering(b[i][j]->pi_get_sprite(), 1);
                std::cerr << removed << " " << std::endl;
            }

    m_gameplay_controller_ref->gc_reset_game();
    m_renderer_ref->gr_remove_sprite_from_rendering(m_highlighted_tile, 0);
    load_pieces();

    //Restartowanie wszystkich wlasciowosci do domyslnych
    m_whose_turn = 1;
    m_has_to_attack = false;
    m_has_to_attack_tile = std::make_pair(-1, -1);

    for (auto i : m_ui_texts)// ustawianie licznika na 0 
    {
        if (i.first == "dead_black" || i.first == "dead_white")
            i.second->setString("0");

        if (i.first == "turn_text")
            i.second->setString("Black");
    }
    std::cerr << "Gra zostala zrestartowana!" << std::endl;
}

void GameUiController::check_queen_conversion(AbstractPiece* moving_piece, std::pair<int, int> to_tile)// zamiana w krolowa
{
    if ((to_tile.second == 0 && moving_piece->pi_get_piece_color() == PieceColor::white)
        || (to_tile.second == 9 && moving_piece->pi_get_piece_color() == PieceColor::black))
    {
        m_renderer_ref->gr_remove_sprite_from_rendering(
            moving_piece->
            pi_get_sprite(),
            1
        );

        moving_piece->change_to_queen(); // przypisanie wlasnosci ruchow krolowej
        m_renderer_ref->gr_add_sprite_to_rendering(moving_piece->pi_get_sprite(), 1);// dodawanie obrazka do krolowej
    }

}