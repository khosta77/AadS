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

/*
h - head
t - tail
| 0| 1| 2| 3| 4| 5| 6| 7| 9|
+--+--+--+--+--+--+--+--+--+
|-1|-1|-1|-1|10|02|32|-1|-1|
+--+--+--+--+--+--+--+--+--+
|  |  |  |  |h |  |  | t|  |
*/
class MyDeque
{
private:
    int _size;
    int *_arr;
    int _head;
    int _tail;

    int* fillNewArray( const int& size, const int& item = -1 )
    {
        int* buffer = new int[size];
        for( int i = 0; i < size; ++i )
            buffer[i] = item;
        return buffer;
    }

    void reBuild()
    {
        const int _size_buffer = _size;  // Создаем новый массив
        _size *= 2;
        int* buffer = fillNewArray( _size );
    
        int _new_head = ( _size / 2 ), _new_tail = _new_head;  // Задаем новые границы
        int i = 0, j = 0;

        if( _arr[( _size_buffer - 1 )] == -1 )
        {
            for( i = ( _head ); i < _tail; ++i, ++j)
                buffer[( _new_tail + j )] = _arr[i];
            _new_tail = ( _new_tail + j + 1 );
        }
        else
        {
            for( i = ( _head ); i < _size_buffer; ++i, ++j)
                buffer[( _new_head + j )] = _arr[i];
            for( i = 0; i < _tail; ++i, ++j )
                buffer[( _new_head + j )] = _arr[i];
            _new_tail = ( ( _new_head + j ) == _size ) ? 0 : ( _new_head + j );
        }
        if( _head == _tail )
        {
            for(i = 0, j = ( _size_buffer - 1 ); i < j; ++i)
                buffer[( _new_head + i )] = _arr[i];
            _new_tail = ( _size - 1);
        }

        _head = _new_head;
        _tail = _new_tail;
        delete[] _arr;
        _arr = buffer;
    }

public:
    explicit MyDeque(const int& resize = 8) : _size(resize)
    {
        _arr = fillNewArray( _size );
        _head = ( _size / 2 );
        _tail = ( _head + 1 );
    }

    ~MyDeque()
    {
        delete[] _arr;
    }

    void pushFront( int item )
    {
        //printA();
        //std::cout << "in pushFront it = " << item << " _t = " << _tail << " _hD = " << _head << " ar[_h] = " << _arr[_head];
        if ( _arr[_head] == -1 )  // Случай когда кладем первый элемент
        {
            _arr[_head] = item;
            if(_head == _tail)
                _tail = ( _tail + 1) % _size;
            //std::cout << std::endl;
            return;
        }

        if( ( _head - 1 ) == _tail )  // Случай когда хвост и голова пересеклись
        {
            reBuild();
        }

        if( ( _head - 1 ) == -1 )  // Случай когда мы вышли за пределы 0
        {
            //std::cout << " (_he=-1)";
            _head = ( _size - 1 );
            if( _head  == _tail )  // Проверка заполнения массива
            {
                reBuild();
                --_head;
            }
            _arr[_head] = item;
            //std::cout << std::endl;
            return;
        }

        _arr[--_head] = item;
        //std::cout << " _hNew = " << _head;
        //std::cout << std::endl;
    }

    int popFront()
    {
        if( _arr[_head] == -1 )
            return -1;
        int buffer_item = _arr[_head];
        _arr[_head++] = -1;
        if( _head == _tail )
        {
            _tail = ( ( _tail + 1 ) == _size ) ? 0 : ( _tail + 1 );
            return buffer_item;
        }

        if( _head == _size )
        {
            if( _tail == 0 )
                ++_tail;
            _head = 0;
        }
        return buffer_item;
    }

    void pushBack( int item )
    {
        //printA();
        //std::cout << "in pushBack it = " << item;
        if(_arr[_head] == -1)  // Для начала поверим есть ли элементы в деке
        {
            //std::cout << " (_arr[_h]=-1) _head = " << _head << std::endl;
            _arr[_head] = item;
            if(_head == _tail)
                _tail = ( _tail + 1) % _size;
            return;
        }

        if( ( _tail + 1 ) == _head )  // Проверяем пересечение с головой
        {
            //std::cout << " (T+1=H) rBuild";
            reBuild();
        }


        if( ( _tail + 1 ) >= _size )  // Проверяем пересеение с максимумом
        {
            //std::cout << " (tail>size) ";
            if( _head == 0 )
            {
                //std::cout << " rBuild";
                reBuild();
            }

            _arr[_tail] = item;
            _tail = 0;
            //std::cout << " _tailN = " << _tail << std::endl;
            //printA();
            return;
        }

        _arr[_tail] = item;
        ++_tail;
        //std::cout << " _tailN = " << _tail << std::endl;
    }

    int popBack()
    {
        //printA();
        //std::cout << "in popBack(" << _size << ") _taild = " << _tail << " _headd = " << _arr[_head];
        if(_arr[_head] == -1)  // Проверим есть ли вообще элементы.
            return -1;
        int buffer_item = 0;

        if( ( _tail - 1 ) == -1 )
        {
            //std::cout << " (_tail<0)";
            _tail = ( _size - 1 );
            buffer_item = _arr[_tail];
            _arr[_tail] = -1;
            //std::cout << " buf_it = " << buffer_item << " _tailN = " << _tail << std::endl; 
            //printA();
            return buffer_item;
        }

        if( _tail == _head )
        {
            buffer_item = _arr[_tail];
            //std::cout << " _arr[t] = " << _arr[_tail];
            _arr[_tail] = -1;
            _tail = ( _tail + 1 ) % _size;
            //std::cout << " buf_it = " << buffer_item;
            //std::cout << " (_head=_tail) _tailN = " << _tail << std::endl;
            //printA();
            return buffer_item;
        }

        if( _arr[_tail] == -1 )  // Сократить когда все заработет
        {
            buffer_item = _arr[--_tail];
            _arr[_tail] = -1;
        }
        else
        {
            buffer_item = _arr[_tail];
            _arr[_tail] = -1;
            --_tail;
        }
        //buffer_item = _arr[( ( _arr[_tail] == -1 ) ? --_tail : _tail )];  //_arr[--_tail];
        //std::cout << " buf_it = " << buffer_item;
        //_arr[_tail] = -1;

        if( _tail == _head )
        {
            ++_tail;
            //std::cout << " (_tail=_head)";
        }
        //std::cout << " _tailN = " << _tail << std::endl;
        return buffer_item;
    }

    inline int getSize() const
    {
        return _size;
    }

    void printA()
    {
        for( int i = 0; i < _size; ++i)
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


