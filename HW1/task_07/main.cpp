#include <iostream>
#include <string>

/*
// Задача 7.1
    Дан массив строк. Количество строк не больше 100000. Отсортировать массив методом поразрядной
    сортировки MSD по символам. Размер алфавита - 256 символов. Последний символ строки = ‘\0’.
*/

// На локальной машине, чтобы поверить
//#define MAKETEST
#ifdef MAKETEST
#include <cassert>
#endif

//// Код

class MSD
{
private:
    size_t _size;
    std::string* _arr;
    size_t maxSize;

    const size_t maxSymbolSize = 256;

    size_t maxSizeInStrings()
    {
        maxSize = 0;
        for (size_t i = 0; i < _size; ++i)
            if (_arr[i].size() > maxSize)
                maxSize = _arr[i].size();
        return maxSize;
    }

    void readStrArray()
    {
        size_t real_size = 1;
        _arr = new std::string[real_size];
        std::string item;

        while( std::cin >> item )
        {
            if( !item.empty() )
            {
                if( _size == real_size )
                {
                    real_size *= 2;
                    std::string* buffer = new std::string[real_size];
                    std::copy( _arr, ( _arr + _size ), buffer );
                    delete[] _arr;
                    _arr = buffer;
                }
                _arr[_size++] = item;
            }
            item.clear();
        }

        maxSizeInStrings();
    }

    void clearStrings( std::string* strs, const size_t& strs_size )
    {
        for( size_t i = 0; i < strs_size; ++i )
            strs[i].clear();
        delete[] strs;
    }

    inline int getSymbolId( const size_t& i, const size_t& d ) const
    {
        return ( ( _arr[i].size() <= d ) ? 0 : ( _arr[i][d] + 1 ) ); 
    }

    int* getIndexes( const int& l, const int& r, const size_t& size )
    {
        int* indexes = new int[( maxSymbolSize + 2 )];  // не заполняет по умолчанию 0-ями
        for( size_t i = 0; i < (maxSymbolSize + 2); indexes[i++] = 0 );
        for( size_t i = l; i <= r; ++i )
            ++indexes[( getSymbolId( i, size ) )];
        for( size_t k = 1, K = ( maxSymbolSize + 2 ); k < K; ++k )
            indexes[k] += indexes[( k - 1 )];
        return indexes;
    }

    void updateArray( int* indexes, const int& l, const int& r, const size_t& size )
    {
        const size_t buffer_size = ( r - l + 1 );
        std::string* buffer = new std::string[buffer_size];
        for( int i = r; i >= l; --i )
            buffer[--indexes[getSymbolId( i, size )]] = _arr[i];

        for ( size_t i = l; i <= r; ++i )
            _arr[i] = buffer[( i - l )];
        clearStrings( buffer, buffer_size );
    }

    void sort_msd( const int& l, const int& r, const size_t& size )
    {
        if( ( size >= maxSize ) || ( l >= r ) )
            return;
        int* indexes = getIndexes( l, r, size );
        updateArray( indexes, l, r, size );
        for( int k = 0; k < maxSymbolSize; ++k )
            sort_msd( ( l + indexes[k] ), ( l + indexes[( k + 1 )] - 1 ), ( size + 1 ) );
        delete[] indexes;
    }

public:
    MSD() : _size(0)
    {
        readStrArray();
        sort_msd( 0, ( _size - 1 ), 0 );
    }

#ifdef MAKETEST
    MSD( std::string* arr, const size_t& size ) : _size(size), _arr(arr)
    {
        maxSizeInStrings();
        sort_msd( 0, ( _size - 1 ), 0 );
    }

    std::string operator[]( const size_t& i )
    {
        return _arr[i];
    }
#endif

    ~MSD()
    {
#ifndef MAKETEST
        clearStrings( _arr, _size );
#endif
    }

    friend std::ostream& operator<<( std::ostream&, const MSD& );
};

std::ostream& operator<<( std::ostream &os, const MSD& msd )
{
    for(size_t i = 0; i < msd._size; ++i )
        os << msd._arr[i] << std::endl;
    return os;
}

//// Тест
#ifdef MAKETEST
void test1()
{
    std::string in[4] = {
        "ab",
        "aa",
        "aaa",
        "a"
    };
    std::string out[4] = {
        "a",
        "aa",
        "aaa",
        "ab"
    };
    MSD msd( &in[0], 4 );
    for( size_t i = 0; i < 4; ++i )
        assert( ( msd[i] == out[i] ) );
}

void test2()
{
    std::string in[6] = {
        "ab",
        "abb",
        "aaaaaa",
        "aaa",
        "z",
        "az"
    };
    std::string out[6] = {
        "aaa",
        "aaaaaa",
        "ab",
        "abb",
        "az",
        "z"
    };
    MSD msd( &in[0], 6 );
    for( size_t i = 0; i < 6; ++i )
        assert( ( msd[i] == out[i] ) );
}

#endif

int main()
{
#ifdef MAKETEST
    test1();
    test2();
#else
    MSD msd;
    std::cout << msd;
#endif
    return 0;
}


