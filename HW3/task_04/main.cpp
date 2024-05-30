#include <iostream>
#include <array>
#include <cassert>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <cstring>
#include <valarray>
#include <exception>
#include <string>
#include <cxxabi.h>
#include <typeinfo>

/*
// Задача 4.1 Пятнашки
Написать алгоритм для решения игры в “пятнашки”. Решением задачи является приведение к виду:
    [ 1  2  3  4 ]
    [ 5  6  7  8 ]
    [ 9  10 11 12]
    [ 13 14 15 0 ]
, где 0 задает пустую ячейку. Достаточно найти хотя бы какое-то решение. Число перемещений костяшек не
обязано быть минимальным.
//// Формат ввода
Начальная расстановка.
//// Формат вывода
Если вам удалось найти решение, то в первой строке файла выведите число перемещений, которое требуется
сделать в вашем решении. А во второй строке выведите соответствующую последовательность ходов: L означает,
что в результате перемещения костяшка сдвинулась влево, R – вправо, U – вверх, D – вниз. Если же выигрышная
конфигурация недостижима, то выведите в выходной файл одно число −1.
*/

// На локальной машине, чтобы поверить
#define MAKETEST
#ifdef MAKETEST
#include <cassert>
#endif

//// Код

#define SIZE 16

const std::array<int, SIZE> masterBoard = {
    1,  2,  3,  4,
    5,  6,  7,  8,
    9,  10, 11, 12,
    13, 14, 15, 0
};

class TagException : public std::exception
{
private:
    std::string m_msg;

public:
    explicit TagException(const std::string &msg) : m_msg(msg) {}
    const char *what() const noexcept override
    {
		return m_msg.c_str();
	}
};

class PositionEquallyMinusOne : public TagException
{
public:
    explicit PositionEquallyMinusOne() : TagException(
            "Данная пятнашка не решаемая"
    ) {}
};

class MoveThrow : public TagException
{
public:
    explicit MoveThrow( const std::string &move ) : TagException(
            "В ходе перемещения " + move + " что-то пошло не так"
    ) {}
};

class Game
{
    struct TagBoard
    {
        std::array<int, SIZE> _board;
        int _emptyPos;
        int _heuristics;

        size_t getInvCount() const
        {
            size_t inv_count = 0;
            for( int i = 0; i < SIZE - 1; ++i )
                for( int j = i + 1; j < SIZE; ++j )
                   if ( ( _board[i] > _board[j] ) && ( _board[i] && _board[j] ) )
                        ++inv_count;
            return inv_count;
        }

        int CalcHeuristics()
        {
            _heuristics = 0;
            for( int i = 0; i < SIZE; ++i )
                _heuristics += std::sqrt( ( std::pow( ( ( i % 4 ) - ( ( _board[i] - 1 ) % 4 ) ), 2 ) +
                                            std::pow( ( ( i / 4 ) - ( ( _board[i] - 1 ) / 4 ) ), 2 ) ) );
            return _heuristics;
        }

        TagBoard(const std::array<int, SIZE> &board) : _board(board), _emptyPos(-1), _heuristics(-1)
        {
            for (int i = 0; i < SIZE; i++)
                if (_board[i] == 0)
                    _emptyPos = i;
            if( _emptyPos == -1 )
                throw PositionEquallyMinusOne();
            CalcHeuristics();
        }

        bool IsComplete( std::array<int, SIZE> &check ) const { return ( _board == check ); }
        bool IsSolvable() const { return ( ( ( getInvCount() + ( (_emptyPos / 4 ) + 1 ) ) % 2 ) == 0 ); }
        bool CanMoveLeft() const { return ( ( _emptyPos % 4 ) != 3 ); }
        bool CanMoveRight() const { return ( ( _emptyPos % 4 ) != 0 ); }
        bool CanMoveUp() const { return ( _emptyPos < 12 ); }
        bool CanMoveDown() const { return ( _emptyPos > 3 ); }

        TagBoard MoveLeft()
        {
            if( !CanMoveLeft() )
                throw MoveThrow("Left");
            TagBoard newState(*this);
            std::swap( newState._board[_emptyPos], newState._board[( _emptyPos + 1 )] );
            ++newState._emptyPos;
            CalcHeuristics();
            return newState;
        }

        TagBoard MoveRight()
        {
            if( !CanMoveRight() )
                throw MoveThrow("Right");
            TagBoard newState(*this);
            std::swap( newState._board[_emptyPos], newState._board[( _emptyPos - 1 )] );
            --newState._emptyPos;
            CalcHeuristics();
            return newState;
        }

        TagBoard MoveUp()
        {
            if( !CanMoveUp() )
                throw MoveThrow("Up");
            TagBoard newState(*this);
            std::swap( newState._board[_emptyPos], newState._board[( _emptyPos + 4 )] );
            newState._emptyPos += 4;
            CalcHeuristics();
            return newState;
        }

        TagBoard MoveDown()
        {
            if( !CanMoveDown() )
                throw MoveThrow("Down");
            TagBoard newState(*this);
            std::swap( newState._board[_emptyPos], newState._board[( _emptyPos - 4 )] );
            newState._emptyPos -= 4;
            CalcHeuristics();
            return newState;
        }

        bool operator==( const TagBoard &other ) const { return ( _board == other._board ); }
        bool operator>( const TagBoard &rhs ) const { return ( _heuristics > rhs._heuristics ); }
    };

    struct GameStateHasher
    {
        size_t operator()( const TagBoard &state ) const
        {
            size_t hash = 0;
            std::memcpy( &hash, state._board.data(), sizeof(hash) );
            return hash;
        }
    };

    struct StateComp
    {
        constexpr bool operator()( const TagBoard &lhs, const TagBoard &rhs )
        const noexcept { return lhs > rhs; }
    };

    std::array<int, SIZE> _controlBoard;
    std::array<int, SIZE> _board;
    TagBoard _startState;
    std::unordered_map<TagBoard, int, GameStateHasher> _visited;
    std::priority_queue<TagBoard, std::vector<TagBoard>, StateComp> _queue;
    std::string _path;
    int _counter;

    void twistLeft( TagBoard &state )
    {
        TagBoard newState = state.MoveLeft();
        if( _visited.find(newState) == _visited.end() )
        {
            _visited[newState] = 'L';
            _queue.push(newState);
        }    
    }
    
    void twistRight( TagBoard &state )
    {
        TagBoard newState = state.MoveRight();
        if (_visited.find(newState) == _visited.end())
        {
            _visited[newState] = 'R';
            _queue.push(newState);
        }    
    }
    
    void twistUp( TagBoard &state )
    {
        TagBoard newState = state.MoveUp();
        if (_visited.find(newState) == _visited.end())
        {
            _visited[newState] = 'U';
            _queue.push(newState);
        }      
    }
    
    void twistDown( TagBoard &state )
    {
        TagBoard newState = state.MoveDown();
        if( _visited.find(newState) == _visited.end() )
        {
            _visited[newState] = 'D';
            _queue.push(newState);
        }     
    }

    void twist()
    {
        while( !_queue.empty() )
        {
            TagBoard state = _queue.top();
            _queue.pop();

            if (state.IsComplete(_controlBoard))
                break;

            if( state.CanMoveLeft() )
                twistLeft(state);

            if( state.CanMoveRight() )
                twistRight(state);
 
            if( state.CanMoveUp() )
                twistUp(state);
            
            if( state.CanMoveDown() )
                twistDown(state);
        }
    }

    void switchPath( TagBoard &state, const int &move )
    {
        switch( move )
        {
            case 'L':
            {
                state = state.MoveRight();
                _path += 'L';
                break;
            }
            case 'R':
            {
                state = state.MoveLeft();
                _path += 'R';
                break;
            }
            case 'D':
            {
                state = state.MoveUp();
                _path += 'D';
                break;
            }
            case 'U':
            {
                state = state.MoveDown();
                _path += 'U';
                break;
            }
        }
    }

    void findPath()
    {
        TagBoard state(_controlBoard);
        while( _visited[state] != 'S' )
        {
            ++_counter;
            switchPath( state, _visited[state] );
        }
        std::reverse( _path.begin(), _path.end() );
    }

public:
    Game( const std::array<int, SIZE> &field, const std::array<int, SIZE> &controlBoard = masterBoard ) : 
        _controlBoard(controlBoard), _board(field), _startState(_board), _counter(0) {}
    
    std::pair<int, std::string> play()
    {
        if (!_startState.IsSolvable())
            return std::make_pair(-1, "");
        _visited[_startState] = 'S';
        _queue.push(_startState);
        twist();
        findPath();
        return std::pair<int, std::string>( _counter, _path );
    }
};

std::array<int, SIZE> readArray( std::istream &in )
{
    std::array<int, SIZE> array;
    for (int i = 0, elem = 0; ( ( i < SIZE ) && ( in >> elem ) ); ++i)
        array[i] = elem;
    return array;
}

void printPair( const std::pair<int, std::string> &pair, std::ostream &out )
{
    out << pair.first << '\n' << pair.second << std::endl;   
}

void run( std::istream &in, std::ostream &out )
{
    Game game( readArray( in ) );
    printPair( game.play(), out );
}

//// Тест
#ifdef MAKETEST
const std::array<int, SIZE> TEST_01 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0, 13, 14, 15, 12 };

void makeTest( const std::array<int, SIZE> firstState, const std::pair<int, std::string> resultTrue )
{
    Game game(firstState);
    auto result = game.play();
    assert( result.first == resultTrue.first );
    assert( result.second == resultTrue.second );
}
#endif

int main()
{
#ifdef MAKETEST
    makeTest( TEST_01, std::pair<int, std::string>( 1, "U" ) );
#else
    run( std::cin, std::cout );
#endif
    return 0;
}


