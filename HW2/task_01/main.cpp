#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
#include <map>

/*
// Задача 1.2 Хеш-таблица (6 баллов)
Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
Хранимые строки непустые и состоят из строчных латинских букв. 

Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.

Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов
в случае, когда коэффициент заполнения таблицы достигает 3/4.

Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества
и проверки принадлежности данной строки множеству. 

Для разрешения коллизий используйте двойное хеширование.

Требования: В таблице запрещено хранение указателей на описатель элемента.

//// Формат входных данных:
Каждая строка входных данных задает одну операцию над множеством. Запись операции состоит из типа операции
и следующей за ним через пробел строки, над которой проводится операция. Тип операции один из трех символов:
+ означает добавление данной строки в множество;
- означает удаление строки из множества;
? означает проверку принадлежности данной строки множеству.
При добавлении элемента в множество НЕ ГАРАНТИРУЕТСЯ, что он отсутствует в этом множестве. При удалении
элемента из множества НЕ ГАРАНТИРУЕТСЯ, что он присутствует в этом множестве.

//// Формат выходных данных:
Программа должна вывести для каждой операции одну из двух строк OK или FAIL, в зависимости от того,
встречается ли данное слово в нашем множестве.
*/

// На локальной машине, чтобы поверить
//#define MAKETEST
#ifdef MAKETEST
#include <cassert>
#endif

//// Код

template <typename T = std::string>
size_t hashStr( const T& key, const size_t& random_state, const size_t& maximum = 1e9 )
{
    size_t hash = 0, pow = 1;
    for ( auto element : key ) 
    {
        hash = ( hash + element * pow ) % maximum;
        pow = ( pow * random_state ) % maximum;
    }
    return hash;
}

template <typename T = std::string>
size_t getDoubleHash(const std::string& key )
{
    return ( ( hashStr( key, 41 ) << 32 ) | hashStr( key, 42 ) );
}

#define CELL_EMPTY  0x00
#define CELL_DELETE 0x01
#define CELL_KEY    0x02

template <typename T = std::string>
struct Cell
{
    T _item;
    size_t _hash;
    uint8_t _state;

    Cell() : _hash( 0 ), _state( CELL_EMPTY ) {}
    Cell( T item, const size_t& hash ) : _item( std::move( item ) ), _hash( hash ),
                                         _state( CELL_KEY ) {}
};


template <typename T = std::string>
class HashTable
{
    size_t _size;
    size_t ( *_hsh )( const T& );
    std::vector<Cell<T>> _table;

    void reBuild()
    {
        std::vector<Cell<T>> buffer( ( _table.size() * 2 ) ); 
        size_t hash = 0, cnt = 0;
        for( size_t i  = 0; i < _table.size(); ++i )
        {
            if( _table[i]._state == CELL_KEY )
            {
                hash = ( _table[i]._hash % buffer.size() );
                while( cnt = 0, buffer[hash]._state != CELL_EMPTY )
                    hash = ( ( hash + ++cnt ) % buffer.size() );
                buffer[hash] = _table[i];
            }
        }
        _table = std::move( buffer );
        buffer.clear();
    }

    /** @brief universalHashTableMethod - операции добавления, удаления и поиска элемента +- одинаковые 
     *                                    по реализации. Решил что лучше будет объединить в один метод.
     *                                    Чтобы не было большого повторяющегося кода. появлется небольшое
     *                                    ветвление, но оно выполняется за О(1), так что им можно принебречь.
     *  @param item - вводимое значение
     *  @param isAdded - если мы хотим добавить элемент true
     *  @param isDeleted - если мы хотим удалить true
     *  +-------+---------+-----------+
     *  |isAdded|isDeleted| Результат |
     *  +-------+---------+-----------+
     *  | fasle | false   | Поиск     |
     *  +-------+---------+-----------+
     *  | true  | fasle   | Добавление|
     *  +-------+---------+-----------+
     *  | fasle | true    | Удаление  |
     *  +-------+---------+-----------+
     *  | true  | true    | Ошибка!!! |
     *  +-------+---------+-----------+
     * */
    bool universalHashTableMethod( const T& item, const bool& isAdded, const bool& isDeleted )
    {
        if ( isAdded && ( ( float )_size >= ( 0.75 * ( float )_table.size() ) ) )
            reBuild();

        const size_t hashValue = _hsh( item );
        size_t hash = ( hashValue % _table.size() ), cnt = 0;
        // удаленный элемент из ряда идущих элементов
        std::pair<bool, size_t> deletedItem = std::pair<bool, size_t>( true, 0 );

        while( ( _table[hash]._state != CELL_EMPTY ) && ( cnt < _table.size() ) )
        {
            if( ( _table[hash]._item == item ) && ( _table[hash]._state != CELL_DELETE) )
            {
                if( isDeleted )
                    _table[hash]._state = CELL_DELETE;
                return ( ( !isAdded ) ? true : false);
            }

            if( isAdded && ( ( _table[hash]._state == CELL_DELETE ) && ( deletedItem.first ) ) )
            {
                deletedItem = std::pair<bool, size_t>( false, hash );
                break;  // Если бы попали на удаленную ячейку, сразу можно выйти
            }

            hash = ( ( hash + ++cnt ) % _table.size() );
        }

        if( !isAdded )  // Если мы не добавлеям новый элемент, то возвращаем false
            return false;

        hash = ( ( deletedItem.first ) ? hash : deletedItem.second );
        _table[hash] = Cell( std::move( item ), hashValue );
        ++_size;
        return true;
    }

public:
    HashTable( size_t ( *_hasher )( const T& ) = getDoubleHash ) : _hsh(_hasher), _size(0)
        { _table.resize(8); }

    ~HashTable() { _table.clear(); }

    bool find( const T& item ) { return universalHashTableMethod( item, false, false ); }

    bool add( const T& item ) { return universalHashTableMethod( item, true, false ); }    
    
    bool remove( const T& item ) { return universalHashTableMethod( item, false, true ); }

    void print()
    {
        std::cout << std::endl;
        for( auto cell : _table )
            std::cout << "h " << cell._hash << " e " << cell._item << " s " << ((int)cell._state) << std::endl;
    }

};

template <typename T = std::string>
std::string check( HashTable<T>& ht, const char& cmd, const std::string& value )
{
    switch( cmd )
    {
        case '+':
            return ( ( ht.add( value ) ? "OK\n" : "FAIL\n" ) );
        case '-':
            return ( ( ht.remove( value ) ? "OK\n" : "FAIL\n" ) );
        case '?':
            return ( ( ht.find( value ) ? "OK\n" : "FAIL\n" ) );
        default:
            std::cout << "Неизвестная команда: " << cmd << " uint8_t = " << ( ( uint8_t )cmd ) << std::endl; 
            throw;
    }
    return "FAIL\n";
}

//// Тест
#ifdef MAKETEST
void test1()
{
    std::string df[7][3] = {
        { "+", "hello", "OK\n" },
        { "+", "bye", "OK\n" },
        { "?", "bye", "OK\n" },
        { "+", "bye", "FAIL\n" },
        { "-", "bye", "OK\n" },
        { "?", "bye", "FAIL\n" },
        { "?", "hello", "OK\n" }
    };
    HashTable hashTable;
    for( size_t i = 0; i < 7; ++i )
    {
        if( df[i][2] != check( hashTable, df[i][0][0], df[i][1] ) )
            assert(0);
    }
    //hashTable.print();
}

void test2()
{
    std::string df[17][3] = {
        { "+", "1", "OK\n" },
        { "?", "1", "OK\n" },
        { "+", "2", "OK\n" },
        { "+", "3", "OK\n" },
        { "+", "4", "OK\n" },
        { "+", "4", "FAIL\n" },
        { "?", "5", "FAIL\n" },
        { "-", "1", "OK\n" },
        { "?", "1", "FAIL\n" },
        { "+", "1", "OK\n" },
        { "?", "1", "OK\n" },
        { "-", "2", "OK\n" },
        { "-", "1", "OK\n" },
        { "?", "1", "FAIL\n" },
        { "?", "9", "FAIL\n" },
        { "?", "2", "FAIL\n" },
        { "+", "2", "OK\n" }
    };
    HashTable hashTable;
    for( size_t i = 0; i < 17; ++i )
    {
        if( df[i][2] != check( hashTable, df[i][0][0], df[i][1] ) )
        {
            hashTable.print();
            std::cout << i << ") " << df[i][0] << " " << df[i][1] << " " << df[i][2] << std::endl;
            assert(0);
        }
    }
}

void test3()
{
    const size_t SIZE = 24;
    std::string df[SIZE][3] = {
        { "+", "1", "OK\n" },
        { "+", "2", "OK\n" },
        { "+", "3", "OK\n" },
        { "+", "4", "OK\n" },
        { "+", "5", "OK\n" },
        { "+", "6", "OK\n" },
        { "?", "1", "OK\n" },
        { "?", "2", "OK\n" },
        { "?", "3", "OK\n" },
        { "?", "4", "OK\n" },
        { "?", "5", "OK\n" },
        { "?", "6", "OK\n" },
        { "-", "1", "OK\n" },
        { "-", "2", "OK\n" },
        { "-", "3", "OK\n" },
        { "-", "4", "OK\n" },
        { "-", "5", "OK\n" },
        { "-", "6", "OK\n" },
        { "?", "1", "FAIL\n" },
        { "?", "2", "FAIL\n" },
        { "?", "3", "FAIL\n" },
        { "?", "4", "FAIL\n" },
        { "?", "5", "FAIL\n" },
        { "?", "6", "FAIL\n" }
    };
    HashTable hashTable;
    for( size_t i = 0; i < SIZE; ++i )
    {
        if( df[i][2] != check( hashTable, df[i][0][0], df[i][1] ) )
        {
            hashTable.print();
            std::cout << i << ") " << df[i][0] << " " << df[i][1] << " " << df[i][2] << std::endl;
            assert(0);
        }
    }
    //hashTable.print();
}

void test4()
{
    const size_t SIZE = 20;
    std::string df[SIZE][3] = {
        { "+", "1", "OK\n" },
        { "+", "2", "OK\n" },
        { "+", "3", "OK\n" },
        { "+", "4", "OK\n" },
        { "+", "5", "OK\n" },
        { "+", "6", "OK\n" },
        { "?", "1", "OK\n" },
        { "?", "2", "OK\n" },
        { "?", "3", "OK\n" },
        { "?", "4", "OK\n" },
        { "?", "5", "OK\n" },
        { "?", "6", "OK\n" },
        { "+", "7", "OK\n" },
        { "?", "7", "OK\n" },
        { "?", "1", "OK\n" },
        { "?", "2", "OK\n" },
        { "?", "3", "OK\n" },
        { "?", "4", "OK\n" },
        { "?", "5", "OK\n" },
        { "?", "6", "OK\n" }
    };
    HashTable hashTable;
    for( size_t i = 0; i < SIZE; ++i )
    {
        if( df[i][2] != check( hashTable, df[i][0][0], df[i][1] ) )
        {
            hashTable.print();
            std::cout << i << ") " << df[i][0] << " " << df[i][1] << " " << df[i][2] << std::endl;
            assert(0);
        }
    }
    //hashTable.print();
}

void test5()
{
    std::string df[17][3] = {
        { "+", "1", "OK\n" },
        { "?", "1", "OK\n" },
        { "+", "2", "OK\n" },
        { "+", "3", "OK\n" },
        { "+", "4", "OK\n" },
        { "+", "5", "OK\n" },
        { "+", "6", "OK\n" },
        { "+", "7", "OK\n" },
        { "+", "8", "OK\n" },
        { "+", "9", "OK\n" },
        { "+", "10", "OK\n" },
        { "?", "1", "OK\n" },
        { "-", "1", "OK\n" },
        { "?", "1", "FAIL\n" },
        { "?", "9", "OK\n" },
        { "-", "9", "OK\n" },
        { "?", "9", "FAIL\n" }
    };
    HashTable hashTable;
    for( size_t i = 0; i < 17; ++i )
    {
        if( df[i][2] != check( hashTable, df[i][0][0], df[i][1] ) )
        {
            hashTable.print();
            std::cout << i << ") " << df[i][0] << " " << df[i][1] << " " << df[i][2] << std::endl;
            assert(0);
        }
    }
}

#endif

int main()
{
#ifdef MAKETEST
    test1();
    test2();
    test3();
    test4();
    test5();
#else
    std::string cmd, item;
    HashTable hashTable;
    while( std::cin >> cmd >> item )
    {
        std::cout << check( hashTable, cmd[0], item );
        cmd.clear(); item.clear();
    }
#endif
    return 0;
}


