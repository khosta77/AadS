#include <iostream>
#include <string>
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

//// Код

class MyDeque
{
private:
    int _size;
    int _image_size;
    int _head;
    int _tail;
    int *_arr;

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

    void reBuildNew()
    {
        const int mid = _head;
        const int _size_buffer = _size;  // Создаем новый массив
        _size *= 2;
        int* buffer = fillNewArray( _size );
        int _new_head = 0;
        int _new_tail = 0;
        int i = 0, j = 0;
        for( i = 0; i <= mid; ++i )
            buffer[i] = _arr[i];
        _new_head = --i;

        for( i = (_size - 1), j = ( _size_buffer - 1 ); j >= _tail; --j, --i )
            buffer[i] = _arr[j];
        _new_tail = ++i;

        _head = _new_head;
        _tail = _new_tail;
        delete[] _arr;
        _arr = buffer;
    }

public:
    explicit MyDeque(const int& resize = 1) : _size(resize), _image_size(0), _head(0), _tail(( _size - 1 ))
    {
        _arr = fillNewArray( _size );
    }

    ~MyDeque()
    {
        delete[] _arr;
    }

    void pushFront( int item )
    {        
        if( ( ( ++_image_size ) == _size ) || ( _head == _tail ) )
            reBuildNew();
        _head = ( ( _arr[0] != -1 ) ? ( ( (++_head) ? _head : ( _size - 1 ) ) % _size ) : 0 );
        _arr[_head] = item;
        std::cout << _tail << std::endl;
#if 0
        std::cout << "pushFront(" << item << ")" << std::endl;
        if ( _arr[_head] == -1 )  // Случай когда кладем первый элемент
        {
            std::cout << "1" << std::endl;
            _arr[_head] = item;
            return;
        }

        if( ( _head - 1 ) == _tail )  // Случай когда хвост и голова пересеклись
        {
            std::cout << "2" << std::endl;
            reBuild();
        }

        if( ( _head - 1 ) == -1 )  // Случай когда мы вышли за пределы 0
        {
            std::cout << "3" << std::endl;
            _head = ( _size - 1 );
            if( _head  == _tail )  // Проверка заполнения массива
            {
                reBuild();
                --_head;
            } 
            else if( ( _size == 1 ) || ( _size == 2 ) )
            {
                reBuild();
            }
            _arr[_head] = item;
            return;
        }

        _arr[--_head] = item;
#endif
    }

    int popFront()
    {
        _head = ( ( _arr[_head] != -1 ) ? _head : ( _size + _head - 1 ) % _size );
        int buffer_item = _arr[_head];
        _arr[_head] = -1;
        _image_size--;
        return buffer_item;
#if 0
        if( _arr[_head] == -1 )
            return -1;
        int buffer_item = _arr[_head];
        _arr[_head++] = -1;
        if( _head == _tail )
        {
            _tail = ( ( _tail + 1 ) == _size ) ? 0 : ( _tail + 1 );
            return buffer_item;
        }

        if( _head == _size )  // Можно сократить
        {
            if( _tail == 0 )
                ++_tail;
            _head = 0;
        }
        return buffer_item;
#endif
    }

    void pushBack( int item )
    {
        if( ( ( ++_image_size ) == _size ) || ( _head == _tail ) )
            reBuildNew();
        _tail = ( ( _arr[( _size - 1 )] != -1 ) ? ( --_tail ) : ( _size - 1 ) );
        _arr[_tail] = item;
#if 0
        if(_arr[_head] == -1)  // Для начала поверим есть ли элементы в деке
        {
            _arr[_head] = item;
            return;
        }

        if( ( _tail + 1 ) == _head )  // Проверяем пересечение с головой
        {
            reBuild();
        }

        if( ( _tail + 1 ) >= _size )  // Проверяем пересеение с максимумом
        {
            if( _head == 0 )
            {
                reBuild();
            }
            if( _size == 2 )
            {
                reBuild();
                _arr[( _size - 1 )] = item;
                _tail = 0;
                return;
            }
            _arr[_tail] = item;
            _tail = 0;
            return;
        }

        _arr[_tail] = item;
        ++_tail;
#endif
    }

    int popBack()
    {
        _tail = ( ( _arr[_tail] == -1 ) ? ( _size + _tail + 1 ) : ( _size + _tail ) ) % _size;
        int buffer_item = _arr[_tail];
        _arr[_tail] = -1;
        _image_size--;
        return buffer_item;
#if 0
        if(_arr[_head] == -1)  // Проверим есть ли вообще элементы.
            return -1;

        int buffer_item = 0;

        if( ( _tail - 1 ) == -1 )
        {
            _tail = ( _size - 1 );
            buffer_item = _arr[_tail];
            _arr[_tail] = -1;
            return buffer_item;
        }

        buffer_item = _arr[--_tail];
        _arr[_tail] = -1;
        if( _tail == _head )
            ++_tail;

        return buffer_item;
#endif
    }

//#ifdef MAKETEST
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
//#endif
};

std::string dequeOperations( int ( *cmd )[2], const int& size )
{
    int buffer = 0;
    MyDeque md;
    for( int i = 0; i < size; ++i )
    {
        switch(cmd[i][0])
        {
        case 1:
            md.pushFront( cmd[i][1] );
            break;
        case 2:
            buffer = md.popFront();
            if( buffer != cmd[i][1] )
                return "NO";
            break;
        case 3:
            md.pushBack( cmd[i][1] );
            break;
        case 4:
            buffer = md.popBack();
            if( buffer != cmd[i][1] )
                return "NO";
            break;
        default:
            return "NO";
        }
    }
    return "YES";
}

//// Тест
#ifdef MAKETEST
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
    md.printA();
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
#else
#if 0
    int N = 0;
    std::cin >> N;
    int tst[N][2];
    for( int i = 0; i < N; ++i )
        std::cin >> tst[i][0] >> tst[i][1];
    std::cout << dequeOperations( tst, N ) << std::endl;
#else  // Была скачущая ошибка, решил эту гадость оставить, пока тесты не пройдет.
    MyDeque md;
    
    //md.printA();
    for ( int i = 0; i < 8; ++i )
    {
        //md.pushBack(i);
        (i % 2 == 1) ? md.pushFront(i) : md.pushBack(i);
    }
    md.printA();
    //std::cout << md.getSize() << std::endl;
    for ( int i = 8; i > 0; --i )
        std::cout << md.popFront() << std::endl;
    //    //std::cout << ( md.popBack() == i ) << " ";
    //std::cout << std::endl;
    //md.pushBack(10);
    //int b = md.popBack();
    //std::cout << b << std::endl;
#endif
#endif
    return 0;
}


