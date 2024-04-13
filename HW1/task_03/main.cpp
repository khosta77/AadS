#include <iostream>
#include <string>
#include <fstream>

/*
// Задача 3.2
Во всех задачах из следующего списка следует написать структуру данных, обрабатывающую команды push* и pop*.
Формат входных данных.
В первой строке количество команд n. n ≤ 1000000.
Каждая команда задаётся как 2 целых числа: a b.
a = 1 - push front
a = 2 - pop front
a = 3 - push back
a = 4 - pop back
Команды добавления элемента 1 и 3 заданы с неотрицательным параметром b.
Для очереди используются команды 2 и 3. Для дека используются все четыре команды.
Если дана команда pop*, то число b - ожидаемое значение. Если команда pop вызвана для пустой
структуры данных, то ожидается “-1”. 
Формат выходных данных.
Требуется напечатать YES - если все ожидаемые значения совпали. Иначе, если хотя бы одно
ожидание не оправдалось, то напечатать NO.

Реализовать дек с динамическим зацикленным буфером (на основе динамического массива).
Требования: Дек должен быть реализован в виде класса.
*/

// На локальной машине, чтобы поверить
#define MAKETEST
#ifdef MAKETEST
#include <cassert>
#endif

template <typename T = int>
class MyDeque
{
private:
	size_t _size;
    size_t _image_size;
	size_t _head;
	size_t _tail;
	T* _arr;

    void reBuild()
    {
        _size *=  2;
	    T* buffer = new T[_size];
        for( size_t k = 0; k < _size; buffer[k++] = 0 );
	    size_t i = _head, j = 0;
	    while( i != _tail )
        {
		    buffer[j++] = _arr[i++];
            i %= _image_size;
	    }
	    buffer[j] = _arr[i];
        _head = 0;
	    _tail = ( _image_size - 1 );
	    delete[] _arr;
        _arr = buffer;
    }

    inline bool pushWhenImageSize0()
    {  //                               1 нужна, чтобы вернуло true
        return ( ( _image_size == 1 ) ? bool(1 + ( _tail = _head = 0 ) ) : false );
    }

    inline bool popWhenHeadEqualTail()
    {
        return ( ( _head == _tail ) ? ( _head = _tail = -1 ) : true );
    }


public:
	MyDeque() : _size(1), _image_size(0), _head(-1), _tail(-1)
	{
		_arr = new T[_size];
	}

    ~MyDeque()
    {
        delete[] _arr;
    }

	void pushFront( T item )
    {
    	if( _image_size == _size )
            reBuild();
	    ++_image_size;

	    if(!pushWhenImageSize0())
         	_head = ( ( --_head + _size ) % _size );

	    _arr[_head] = item;
    }
	
    T popFront()
    {
	    if( _image_size == 0 )
            return -1;  // Так, тут если использовать шаблоны, будет ошибка. Надо возращать throw
	    T buffer = _arr[_head];
	    --_image_size;

	    if(popWhenHeadEqualTail())
		 	_head = ( ( ++_head ) % _size );

	    return buffer;
    }

    void pushBack( T item )
    {
        if( _image_size == _size )
            reBuild();
	    ++_image_size;

	    if(!pushWhenImageSize0())
         	_tail = ( ( ++_tail ) % _size );

	    _arr[_tail] = item;
    }

    T popBack()
    {
	    if( _image_size == 0 )
            return -1;
	    T buffer = _arr[_tail];
        --_image_size;

	    if(popWhenHeadEqualTail())
		 	_tail = ( ( --_tail + _size ) % _size );

        return buffer;
    }

    void printA()
    {
        for( size_t i = 0; i < _size; ++i)
            std::cout << _arr[i] << " ";
        std::cout << std::endl;
    }
};

std::string dequeOperations( int ( *cmd )[2], const size_t& size )
{
    int buffer = 0;
    MyDeque md;
    for( size_t i = 0; i < size; ++i )
    {
        switch(cmd[i][0])
        {
        case 1:
            md.pushFront( cmd[i][1] );
            break;
        case 2:
            buffer = md.popFront();
            if( buffer != cmd[i][1] )
            {
                //std::cout << "popFront() i = " << i << " wait = " << cmd[i][1] << std::endl;
                //md.printA();
                return "NO";
            }
            break;
        case 3:
            md.pushBack( cmd[i][1] );
            break;
        case 4:
            buffer = md.popBack();
            if( buffer != cmd[i][1] )
            {
                //std::cout << "popBack() i = " << i << " wait = " << cmd[i][1] << std::endl;
                //md.printA();
                return "NO";
            }
            break;
        default:
            return "NO";
        }
    }
    return "YES";
}

//// Тест
#ifdef MAKETEST
class FileNotOpen : public std::exception
{
public:
    explicit FileNotOpen(const std::string &msg) : m_msg(msg) {}
    const char *what() const noexcept override { return m_msg.c_str(); }
private:
    std::string m_msg;
};

void testFromFile( const std::string& ft, const size_t& size, const std::string& result )
{
    int df[size][2];
    std::ifstream in(ft);
    if(!in)
        throw FileNotOpen(ft.c_str());
    for( size_t i = 0; i < size; ++i )
        in >> df[i][0] >> df[i][1];
    assert( ( dequeOperations( df, size ) == result ) );
}

void test1()
{
    const int tst_size = 3;
    int tst[tst_size][2] = {
                                { 1, 44 },
                                { 3, 50 },
                                { 2, 44 }
                           };
    assert( ( dequeOperations( tst, tst_size ) == "YES" ) );
}

void test2()
{
    const int tst_size = 2;
    int tst[tst_size][2] = {
                                { 2, -1 },
                                { 1, 10 }
                           };
    assert( ( dequeOperations( tst, tst_size ) == "YES" ) );
}

void test3()
{
    const int tst_size = 2;
    int tst[tst_size][2] = {
                                { 3, 44 },
                                { 4, 60 }
                           };
    assert( ( dequeOperations( tst, tst_size ) == "NO" ) );
}

void test4()
{
    const int tst_size = 5;
    int tst[tst_size][2] = {
                                { 1, 10 },
                                { 4, 10 },
                                { 2, -1 },
                                { 4, -1 },
                                { 3, 60 }
                           };
    assert( ( dequeOperations( tst, tst_size ) == "YES" ) );
}

void test5()
{
    MyDeque md;
    for ( int i = 0; i < 100; ++i )
        md.pushBack(i);
    for ( int i = 99; i >= 0; --i )
        assert( ( md.popBack() == i ) );
}

void test6()
{
    MyDeque md;
    for ( int i = 0; i < 100; ++i )
        md.pushBack(i);
    for ( int i = 0; i < 100; ++i )
        assert( ( md.popFront() == i ) );
}

void test7()  // Вот где собака зарыта
{
    MyDeque md;
    for ( int i = 0; i < 100; ++i )
        md.pushFront(i);
    for ( int i = 0; i < 100; ++i )
        assert( ( md.popBack() == i ) );
}

void test8()
{
    MyDeque md;
    for ( int i = 0; i < 100; ++i )
        md.pushFront(i);
    for ( int i = 99; i >= 0; --i )
        assert( ( md.popFront() == i ) );
}

void test9()
{
    const int tst_size = 4;
    int tst[tst_size][2] = {
                                { 1, 9 },
                                { 4, 9 },
                                { 1, 8 },
                                { 4, 8 }
                           };
    assert( ( dequeOperations( tst, tst_size ) == "YES" ) );
}

void test10()
{
    const int tst_size = 4;
    int tst[tst_size][2] = {
                                { 1, 10 },
                                { 3, 11 },
                                { 2, 10 },
                                { 4, 11 }
                           };
    assert( ( dequeOperations( tst, tst_size ) == "YES" ) );
}

void test11()
{
    const int tst_size = 4;
    int tst[tst_size][2] = {
                                { 1, 10 },
                                { 2, 10 },
                                { 2, -1 },
                                { 4, -1 }
                           };
    assert( ( dequeOperations( tst, tst_size ) == "YES" ) );
}

void test12()
{
    const int tst_size = 9;
    int tst[tst_size][2] = {
                                { 1, 1 },
                                { 1, 2 },
                                { 1, 3 },
                                { 1, 4 },
                                { 1, 5 },
                                { 1, 6 },
                                { 1, 7 },
                                { 2, 7 },
                                { 4, 1 }
                           };
    assert( ( dequeOperations( tst, tst_size ) == "YES" ) );
}

void test13()
{
    const int tst_size = 9;
    int tst[tst_size][2] = {
                                { 3, 1 },
                                { 3, 2 },
                                { 3, 3 },
                                { 3, 4 },
                                { 3, 5 },
                                { 3, 6 },
                                { 3, 7 },
                                { 2, 1 },
                                { 4, 7 }
                           };
    assert( ( dequeOperations( tst, tst_size ) == "YES" ) );
}

void test14()
{
    const int tst_size = 9;
    int tst[tst_size][2] = {
                                { 3, 1 },
                                { 2, 1 },
                                { 1, 2 },
                                { 4, 2 },
                                { 2, -1 },
                                { 4, -1 },
                                { 1, 3 },
                                { 3, 4 },
                                { 4, 4 }
                           };
    assert( ( dequeOperations( tst, tst_size ) == "YES" ) );
}

void test15()
{
    const int tst_size = 13;
    int tst[tst_size][2] = {
                                { 3, 119 },
                                { 4, 119 },
                                { 1, 24 },
                                { 1, 122 },
                                { 4, 24 },
                                { 1, 87 },
                                { 4, 122 },
                                { 3, 122 },
                                { 2, 87 },
                                { 1, 58 },
                                { 2, 58 },
                                { 2, 122 },
                                { 1, 28 }
                           };
    assert( ( dequeOperations( tst, tst_size ) == "YES" ) );
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
    test6();
    test7();
    test8();
    test9();
    test10();
    test11();
    test12();
    test13();
    test14();
    test15();
    testFromFile( "input18.txt", 100, "YES" );
    testFromFile( "input24.txt", 10000, "YES" );
#else
    int N = 0;
    std::cin >> N;
    int tst[N][2];
    for( int i = 0; i < N; ++i )
        std::cin >> tst[i][0] >> tst[i][1];
    std::cout << dequeOperations( tst, N ) << std::endl;
#endif
    return 0;
}


