#include <iostream>
#include <array>
#include <cassert>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <cstring>
#include <valarray>

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

const int SIZE = 16;
const std::array<int, SIZE> finishField = {1, 2, 3, 4,
                                                 5, 6, 7, 8,
                                                 9, 10, 11, 12,
                                                 13, 14, 15, 0};

class TagBoard
{
    std::array<int, SIZE> field;
    int emptyPos;
    int heuristics;

    friend struct GameStateHasher;

    size_t getInvCount() const
    {
        size_t inv_count = 0;
        for( int i = 0; i < SIZE - 1; ++i )
            for( int j = i + 1; j < SIZE; ++j )
                if ( ( field[i] > field[j] ) && ( field[i] && field[j] ) )
                    ++inv_count;
        return inv_count;
    }

    int CalcHeuristics()
    {
        int res = 0;
        for( int i = 0; i < SIZE; ++i )
            res += std::sqrt( ( std::pow( ( ( i % 4 ) - ( ( field[i] - 1 ) % 4 ) ), 2 ) +
                                std::pow( ( ( i / 4 ) - ( ( field[i] - 1 ) / 4 ) ), 2 ) ) );
        heuristics = res;
        return heuristics;
    }

public:
    TagBoard(const std::array<int, SIZE> &field) : field(field), emptyPos(-1), heuristics(-1)
    {
        for (int i = 0; i < SIZE; i++)
            if (field[i] == 0)
                emptyPos = i;
        assert(emptyPos != -1);
        CalcHeuristics();
    }

    bool IsComplete() const { return field == finishField; }
    bool IsSolvable() const { return (getInvCount() + ( (emptyPos / 4) + 1) ) % 2 == 0; }
    bool CanMoveLeft() const { return emptyPos % 4 != 3; }
    bool CanMoveRight() const { return emptyPos % 4 != 0; }
    bool CanMoveUp() const { return emptyPos < 12; }
    bool CanMoveDown() const { return emptyPos > 3; }

    TagBoard MoveLeft()
    {
        assert(CanMoveLeft());
        TagBoard newState(*this);
        std::swap( newState.field[emptyPos], newState.field[( emptyPos + 1 )] );
        ++newState.emptyPos;
        CalcHeuristics();
        return newState;
    }

    TagBoard MoveRight()
    {
        assert(CanMoveRight());
        TagBoard newState(*this);
        std::swap( newState.field[emptyPos], newState.field[( emptyPos - 1 )] );
        --newState.emptyPos;
        CalcHeuristics();
        return newState;
    }

    TagBoard MoveUp()
    {
        assert(CanMoveUp());
        TagBoard newState(*this);
        std::swap( newState.field[emptyPos], newState.field[( emptyPos + 4 )] );
        newState.emptyPos += 4;
        CalcHeuristics();
        return newState;
    }

    TagBoard MoveDown()
    {
        assert(CanMoveDown());
        TagBoard newState(*this);
        std::swap( newState.field[emptyPos], newState.field[( emptyPos - 4 )] );
        newState.emptyPos -= 4;
        CalcHeuristics();
        return newState;
    }

    bool operator==(const TagBoard &other) const { return field == other.field; }
    bool operator>( const TagBoard &rhs ) const { return ( heuristics > rhs.heuristics ); }

    const int getHeuristics() const { return heuristics; }

};

struct GameStateHasher
{
    size_t operator()(const TagBoard &state) const
    {
        size_t hash = 0;
        std::memcpy(&hash, state.field.data(), sizeof(hash));
        return hash;
    }
};

struct StateComp
{
    constexpr bool operator()( TagBoard const &a, TagBoard const &b )
    const noexcept { return a > b; }
};

class Game
{

public:
    Game( const std::array<int, SIZE> &field )
};

std::pair<int, std::string> GetSolution( const std::array<int, SIZE> &field )
{
    TagBoard startState(field);
    if (!startState.IsSolvable())
        return std::make_pair(-1, "");

    std::unordered_map<TagBoard, int, GameStateHasher> visited;
    visited[startState] = 'S';

    std::priority_queue<TagBoard, std::vector<TagBoard>, StateComp> queue;

    queue.push(startState);

    while( !queue.empty() )
    {
        TagBoard state = queue.top();
        queue.pop();

        if (state.IsComplete())
            break;

        if( state.CanMoveLeft() )
        {
            TagBoard newState = state.MoveLeft();
            if( visited.find(newState) == visited.end() )
            {
                visited[newState] = 'L';
                queue.push(newState);
            }
        }

        if( state.CanMoveRight() )
        {
            TagBoard newState = state.MoveRight();
            if (visited.find(newState) == visited.end())
            {
                visited[newState] = 'R';
                queue.push(newState);
            }
        }

        if( state.CanMoveUp() )
        {
            TagBoard newState = state.MoveUp();
            if (visited.find(newState) == visited.end())
            {
                visited[newState] = 'U';
                queue.push(newState);
            }
        }
        if( state.CanMoveDown() )
        {
            TagBoard newState = state.MoveDown();
            if( visited.find(newState) == visited.end() )
            {
                visited[newState] = 'D';
                queue.push(newState);
            }
        }
    }

    std::string path;
    GameState state(finishField);
    int counter = 0;
    while( visited[state] != 'S' )
    {
        char move = visited[state];
        counter++;
        switch( move )
        {
            case 'L':
            {
                state = state.MoveRight();
                path += 'L';
                break;
            }
            case 'R':
            {
                state = state.MoveLeft();
                path += 'R';
                break;
            }
            case 'D':
            {
                state = state.MoveUp();
                path += 'D';
                break;
            }
            case 'U':
            {
                state = state.MoveDown();
                path += 'U';
                break;
            }
        }
    }
    std::reverse(path.begin(), path.end());
    return std::pair<int, std::string>( counter, path );
}

std::pair<int, std::string> run(std::istream &in, std::ostream &out)
{
    std::array<int, SIZE> field;
    for (int i = 0; i < SIZE; ++i) {
        int buf = 0;
        in >> buf;
        field[i] = buf;
    }
    return GetSolution(field);
}

//// Тест
#ifdef MAKETEST
const std::array<int, SIZE> TEST_01 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0, 13, 14, 15, 12 };

void makeTest( const std::array<int, SIZE> firstState, const std::pair<int, std::string> resultTrue )
{
    auto result = GetSolution(firstState);
    assert( result.first == resultTrue.first );
    assert( result.second == resultTrue.second );
}
#endif

int main()
{
#ifdef MAKETEST
    makeTest( TEST_01, std::pair<int, std::string>( 1, "U" ) );
#else
    run(std::cin, std::cout);
#endif
    return 0;
}


