#include "gameplay_controller.h"
#include "abstract_piece.h"
#include "graphics_renderer.h"
#include <iostream>


GameplayController::GameplayController()
{
    //pamiêc poczatkowa dla wektora (alokacja)
    for (int i =0; i<10; i++)
        m_board.push_back(std::vector<AbstractPiece*>(10));
    
    gc_load_start_setup();
}

void GameplayController::gc_eliminate_piece(std::pair<int, int> tile)// usuwanie z konretnego miejsca pionka
{
    if(get_reference_to_piece_at_board(tile.first, tile.second)->pi_get_piece_color() == PieceColor::white)
        m_eliminated_white_pieces += 1;
    else 
        m_eliminated_black_pieces += 1;

    set_reference_at_board( new PieceDummy(), tile.first, tile.second);
}

void GameplayController::gc_clear_board()// usuwa wszystkie pionki (czyszczenie)
{
    for(int i =0; i<get_board_size(); i++)
        for (int j=0; j<get_board_size(); j++)
            set_reference_at_board(new PieceDummy(), i, j);
}

void GameplayController::gc_load_start_setup()
{
    //ustawienie czarnych pionkow
    for(int i =0; i<4; i++)// 4 gorne wiersze to czarne pionki
        for(int col =0; col < 10; col++)
        {
            if(i % 2 == 0 && col %2 == 1)
                set_reference_at_board(new PieceBlack(), i, col);
            else if(i % 2 == 1 && col %2 == 0)
                set_reference_at_board(new PieceBlack(), i, col);
            else
                set_reference_at_board(new PieceDummy(), i, col);
        }

    //ustawienie dummy
    for(int i =4; i<6; i++)
        for(int col =0; col < 10; col++)
            set_reference_at_board(new PieceDummy(), i, col);
    
    //ustawienie bialych pionkow
    for(int i =6; i<10; i++) // 4 dolne wiersze to biale pionki 
        for(int col =0; col < 10; col++)
        {
            if(i % 2 == 0 && col %2 == 1)
                set_reference_at_board(new PieceWhite(), i, col);
            else if(i % 2 == 1 && col %2 == 0)
                set_reference_at_board(new PieceWhite(), i, col);
            else
                set_reference_at_board(new PieceDummy(), i, col);
        }
}

void GameplayController::gc_reset_game()// ustawianie do wartosci domyslnych 
{
    gc_clear_board();
    m_eliminated_black_pieces = 0;
    m_eliminated_white_pieces = 0;
    gc_load_start_setup();
}

std::vector< std::vector<AbstractPiece*> > GameplayController::gc_get_board()// zwraca prywatna tablice wszystkich pionkow
{
    return m_board;
}

bool GameplayController::check_move_legal(std::pair<int, int> from_tile, std::pair<int, int> to_tile, bool is_attacking, int move_direction)// funkcja do sprawdzenia poprawnosci ruchu
{
    //Sprawdzanie czy ruch jest dozwolony; jesli nie zwraca ze to jest nie legalne
    // 1. zly ruch
    if( (to_tile.second > from_tile.second) && move_direction != 1 && !is_attacking)
    {
        std::cerr<<"Zly kierunek"<<std::endl;
        return false;
    }
    if((to_tile.second < from_tile.second) && move_direction != -1 && !is_attacking)
    {
        std::cerr<<"Zly kierunek"<<std::endl;
        return false;
    }
    // 2. zajetosc pola 
    if(is_tile_taken(to_tile))
    {
        std::cerr<<"Pole jest zajete"<<std::endl;
        return false;
    }
    // 3. nie atakuje , a normalny ruch jest za dlugi
    if(!is_attacking && ( abs(to_tile.second - from_tile.second) > 1 || abs(to_tile.first - from_tile.first) > 1) )
    {
        std::cerr<<"Zbyt dlugi ruch"<<std::endl;
        return false;
    }
    // 4. poza plansza
    if( to_tile.second >= 10 || to_tile.first >= 10 )
    {
        std::cerr<<"Ruch poza plansze"<<std::endl;
        return false;
    }
    // 5. umieszczony w zlym kolorze
    if(to_tile.second % 2 == 0 && to_tile.first % 2 ==0
        || to_tile.second % 2 == 1 && to_tile.first%2 == 1)
    {
        std::cerr<<"Umieszczono na zlym polu"<<std::endl;
        return false;
    }

    return true;
}

void GameplayController::print_board()// drukowanie planszy do konsoli
{
    std::cout<<"Plansza"<<std::endl;
     
    for (int i = 0; i<m_board.size(); i++)
    {
        std::cout<<i<<": ";
        for (int j = 0; j<m_board[0].size(); j++)
        {
            std::cout<<m_board[i][j]->pi_get_piece_color()<<" ";
        }
        std::cout<<std::endl;
    }   
    std::cout<<"Koniec Planszy"<<std::endl;
}

bool GameplayController::move_queen(std::pair<int, int> from_tile, std::pair<int, int> to_tile)// ruchy krolowej
{
    AbstractPiece* piece_moving = get_reference_to_piece_at_board(from_tile.first, from_tile.second);
    //Sprawdza czy dozwolony ruch
    //nie zajety
    if (is_tile_taken(to_tile))
        return false;
    //Na przekatnej
    if(abs(to_tile.first - from_tile.first) != abs(to_tile.second - from_tile.second)
            && from_tile != to_tile)
        return false;

    //Sprawdzanie czy przeskatujesz tylko nad jednym pionem
    int x_dir = from_tile.first - to_tile.first > 0 ? 1 : -1;
    int y_dir = from_tile.second - to_tile.second> 0 ? 1 : -1;
    std::pair<int, int> current_tile = to_tile;
    current_tile.first += x_dir;
    current_tile.second += y_dir;

    int found_pieces =0;
    AbstractPiece* piece_to_kill;
    std::pair<int, int> attack_tile;
    std::cerr<<"Krolowa na: "<<from_tile.first<<" "<<from_tile.second<<std::endl;
    std::cerr<<"Krolowa idzie na: "<<to_tile.first<<" "<<to_tile.second<<std::endl;
    while(current_tile.first != from_tile.first && current_tile.second != from_tile.second)
    {
        std::cerr<<"Sprawdzanie plytki: "<<current_tile.first<<" "<<current_tile.second<<std::endl;
        AbstractPiece* p = get_reference_to_piece_at_board(current_tile.first, current_tile.second);
        if(p->pi_get_piece_color() != PieceColor::dummy)
        {
            found_pieces++;
            piece_to_kill = p;
            attack_tile = current_tile;
        }

        if(p->pi_get_piece_color() == piece_moving->pi_get_piece_color())
            return false;

        current_tile.first += x_dir;
        current_tile.second += y_dir;
    }

    if (found_pieces > 1)
        return false;
    
    //Ruch
    set_reference_at_board(new PieceDummy(), from_tile.second, from_tile.first);
    set_reference_at_board(piece_moving, to_tile.second, to_tile.first);

    //Atakowanie
    if (found_pieces == 1 && &piece_to_kill != nullptr)
    { 
        std::cerr<<" Atakowanie na wiesz "<<attack_tile.second<<" koloru "<< attack_tile.first<<std::endl;
        m_ref_to_dead_piece = piece_to_kill;
        set_reference_at_board(new PieceDummy(), attack_tile.second, attack_tile.first);

    }

    return true;
}

bool GameplayController::gc_make_move(std::pair<int, int> from_tile, std::pair<int, int> to_tile)// ruszanie z pola
{
    print_board();

    AbstractPiece* piece_moving = get_reference_to_piece_at_board(from_tile.first, from_tile.second);
    int move_direction = piece_moving->pi_get_move_direction();

    if (piece_moving->is_queen())
    {
        std::cerr<<"Sprobuj ruszyc krolowa"<<std::endl;
        bool moved = move_queen(from_tile, to_tile);
        return moved;
    }

    bool is_attacking = false;
    int attack_row, attack_col; 
    if(abs(to_tile.second - from_tile.second) == 2 && abs(to_tile.first - from_tile.first) == 2)
    {
        attack_row = (from_tile.second + to_tile.second ) /2;
        attack_col = (from_tile.first + to_tile.first) /2;

        if(get_reference_to_piece_at_board(attack_col, attack_row)->pi_get_piece_color() 
            == piece_moving->pi_get_piece_color())
        {
            std::cerr<<"Nie mozesz zaatakowac swojego pionka"<<std::endl;
            return false;
        }

        is_attacking = true;
        //Sprawdzanie czy mozesz dalej zaatakowac
    }
    
    //Sprawdzanie czy ruch jest dozwolony
    bool legal = check_move_legal(from_tile, to_tile, is_attacking, move_direction);
    if (legal == false)
        return false;
    
    //Jesli jest dozwolony przesun pion
    set_reference_at_board(new PieceDummy(), from_tile.second, from_tile.first);
    set_reference_at_board(piece_moving, to_tile.second, to_tile.first);

    //Jesli jest to zbicie, usun pionek z planszy
    if(is_attacking)
    {
        std::cerr<<"Atakowanie na wiersz"<<attack_row<<" koloru"<<attack_col<<std::endl;
        m_ref_to_dead_piece = get_reference_to_piece_at_board(attack_col, attack_row);
        set_reference_at_board(new PieceDummy(), attack_row, attack_col);
    }
    
    return true;
}

void GameplayController::check_make_queen(AbstractPiece* moving_piece, std::pair<int, int> to_tile)
{
    //jest na krawedzi czy jestes na dole badz gorze plnaszy , jesli tak zmienia w krolowa
    if ((to_tile.second == 0 && moving_piece->pi_get_piece_color() == PieceColor::white )
        || (to_tile.second == 9 && moving_piece->pi_get_piece_color() == PieceColor::black))
    {
        moving_piece->change_to_queen();
    }   
}

bool GameplayController::gc_make_mandatory_attack_move(std::pair<int, int> from_tile, std::pair<int, int> to_tile, std::pair<int, int> attack_tile)// obowiazkowy ruch ataku
{
    std::cerr<<"zaatakuj!!!!"<<attack_tile.second<<" "<<from_tile.first<<" "<<to_tile.first<<std::endl;
    //Sprawdzanie czy gracz zdecydowal sie zaatakowac obowiazkowy cel
    if (!(attack_tile.first == to_tile.first && attack_tile.second == to_tile.second))
    {
        std::cerr<<"Gracz zaatakowal na podswietlonym polu"<<attack_tile.second<<" "<<attack_tile.first<<std::endl;
        return false;
    }

    AbstractPiece* piece_moving = get_reference_to_piece_at_board(from_tile.first, from_tile.second);
    int attack_row = (from_tile.second + to_tile.second ) /2;
    int attack_col = (from_tile.first + to_tile.first) /2;

    //Jesli ruch jest dozwolony przesun pion
    set_reference_at_board(new PieceDummy(), from_tile.second, from_tile.first);
    set_reference_at_board(piece_moving, to_tile.second, to_tile.first);

    std::cerr<<"Atakowanie na wiersz"<<attack_row<<" koloru "<<attack_col<<std::endl;
    m_ref_to_dead_piece = get_reference_to_piece_at_board(attack_col, attack_row);
    set_reference_at_board(new PieceDummy(), attack_row, attack_col);

    return true;
}

AbstractPiece* GameplayController::gc_try_to_get_piece_at_coords(sf::Vector2f coords)// pobieranie kordynat pionka 
{
    for (int i=0; i<10; i++) 
        for(int j=0; j<10; j++)
        {
            auto p = get_reference_to_piece_at_board(i,j);
            if (p->pi_get_piece_color() == PieceColor::dummy)// jesli uda sie pobrac coordynaty zwraca dany pionek, jesli nie nie zwraca nic
                continue;
            
            auto sp = p->pi_get_sprite();
            bool overlaps = sp->getGlobalBounds().contains(coords);

            if (overlaps)
                return p;
        }
    return nullptr;
}

AbstractPiece* GameplayController::gc_get_dead_piece_pointer()// jesli cos w nim jest, zwraca |||wskaznik na zbity pionek
{
   AbstractPiece* to_return = m_ref_to_dead_piece;
    return to_return;
}

void GameplayController::gc_reset_dead_piece_pointer()
{
   m_ref_to_dead_piece = nullptr;
}

std::pair<int, int> GameplayController::gc_check_if_has_to_attack(int player_turn_flag)// sprawdzanie czy musisz zaatakowac
{
    for (int i =0; i<10; i++)
        for(int j = 0; j<10; j++)
        {
            //Jesli pionek nalezy do obecnego gracza
            if(get_reference_to_piece_at_board(i, j)->pi_get_move_direction() == player_turn_flag)
            {
                std::vector<std::pair<int, int>> pairs = {
                    std::make_pair(2,2), 
                    std::make_pair(-2,2), 
                    std::make_pair(2,-2), 
                    std::make_pair(-2,-2)
                };

                //Sprawdzanie czy jest opcja ataku wokol obszaru figury
                for(auto pair : pairs)
                {
                    if ( i+ pair.first < 10 && i + pair.first >= 0 && j+ pair.second < 10 && j + pair.second >= 0)
                        if(get_reference_to_piece_at_board(i + pair.first, j+pair.second)->pi_get_piece_color() == PieceColor::dummy)
                            if(get_reference_to_piece_at_board(i + (pair.first / 2), j+ (pair.second/2) )->pi_get_move_direction() == player_turn_flag*-1)
                            {
                                std::cerr<<"Pole "<<i + (pair.first / 2)<<" "<<j+ (pair.second/2)<<" moze zostac zaatakowane"<<std::endl;
                                return std::make_pair(i + pair.first, j+ pair.second);
                            }
                }

            }
        }
    
    return std::make_pair(-1, -1);
}

int GameplayController::gc_is_game_done()// czy gra powinna sie zakonczyc
{
    int left_pieces[3] = {0, 0, 0};
    for (int i =0; i<10; i++)
        for(int j = 0; j<10; j++)
            left_pieces[get_reference_to_piece_at_board(i, j)->pi_get_piece_color()]++;
    
    if(left_pieces[1] == 0)
        return 2;
        
    if(left_pieces[2] == 0)
        return 1;
    
    return 0;
}
