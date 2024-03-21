#include <iostream>

/*
// Задача 2.2
Дан массив целых чисел А[0..n-1]. Известно, что на интервале [0, m] значения массива строго возрастают,
а на интервале [m, n-1] строго убывают. Найти m за O(log m).
Требования:  Время работы O(log m). Внимание! В этой задаче сначала нужно определить диапазон
для бинарного поиска размером порядка m с помощью экспоненциального поиска, а потом уже в нем
делать бинарный поиск.
*/

// На локальной машине, чтобы поверить
#define MAKETEST
#ifdef MAKETEST
#include <cassert>
#endif

// Взял с лекции 1, только немного изменил условие и возврат
// Бинарный поиск без рекурсии. Ищем первое вхождение.
int BinarySearchMax( const double* arr, int left, int right )  // Границы включительно, внутри массива
{
    int first = left;
    int last = right;
    int mid = 0;
    while( first < last )
    {
        mid = ( ( first + last ) / 2 );
        if( arr[mid] < arr[( mid + 1 )] ) // Надо сравнивать не крайние элементы, а центральные
            first = ( mid + 1 );
        else
            last = mid;
    }
    return first;
}

int exponentSearchMax( const double* arr, const int& size )
{
    // Если массив всего в два элемента, то сразу проверяем
    if( ( size == 2 ) ||  ( arr[0] > arr[1] ) )  // Возможно такое, что самый большой элемент будет 1-ым
        return ( ( arr[0] > arr[1] ) ? 0 : 1 );  // Приходится делаить двойную проверку

    int i = 2;  // Экспонециальный поиск
    while( ( i < size ) && ( arr[( i / 2 )] < arr[i] ) )
        i *= 2;

	// Может быть заскок за левую границу, надо конкретно ее проверить
    if ( arr[( ( i / 2 ) - 1 )] >= arr[( i / 2 )])  // Проверка левой корректности
        return BinarySearchMax( arr, ( i / 4 ), ( i / 2 ) );

    if( i >= size )  // Проведем проверку, на выход за границы, скорее всего мы вышли за диапазон
        return BinarySearchMax( arr, ( i / 2 ), ( size - 1 ) );
    return BinarySearchMax( arr, ( i / 2 ), i );  // Бинарный поиск в массиве
}

int bin_search(int left,int right, int *a, int flag)
{
    int mid = 0;
    while(left<right)
    {
        if(right-left == 1)
          return a[right-1]>a[left-1] ? (right-1) : (left-1);
        mid = (left+right)/2;
        if(a[mid-1]>a[mid-2] && a[mid-1] > a[mid])
            return mid-1;
        else
            if(a[mid-2] > a[mid])
                right = mid-1;
            else
                left = mid;
    }
    if(left == right)
        return right-1;
    return -1;
}

#ifdef MAKETEST  // Сделал тест, чтобы локально проверить
template<typename T>
T myMaxI( const T* a, const int& s ) // stupid method
{
    int max_i = 0;
    for( int i = 0; i < s; ++i )
    {
        if( a[i] > a[max_i] )
        {
            max_i = i;

        }
    }
    return max_i;
}

void test1()  // Проверяем 2 элемента, большой первый.
{
    const int size = 2;
    double array[size] = { 2, 1 };
    assert( ( exponentSearchMax( &array[0], size ) == myMaxI( &array[0], size ) ) );
}

void test2()  // Проверяем 2 элемента большой второй
{
    const int size = 2;
    double array[size] = { 1, 2 };
    assert( ( exponentSearchMax( &array[0], size ) == myMaxI( &array[0], size ) ) );
}

void test3() {  // Проверяем массив больше 2, m под 0 номером
    const int size = 5;
    double array[size] = { 5, 4, 3, 2, 1 };
    assert( ( exponentSearchMax( &array[0], size ) == myMaxI( &array[0], size ) ) );
}

void test4()  // m под 1
{
    const int size = 5;
    double array[size] = { 4, 5, 3, 2, 1 };
    assert( ( exponentSearchMax( &array[0], size ) == myMaxI( &array[0], size ) ) );
}

void test5()  // m под 2, стык границы
{
    const int size = 5;
    double array[size] = { 3, 4, 5, 2, 1 };
    assert( ( exponentSearchMax( &array[0], size ) == myMaxI( &array[0], size ) ) );
}

void test6()  // m последний элемент
{
    const int size = 5;
    double array[size] = { 1, 2, 3, 4, 5 };
    assert( ( exponentSearchMax( &array[0], size ) == myMaxI( &array[0], size ) ) );
}

void test7()  // Пример
{
    const int size = 10;
    double array[size] = { 1, 2, 3, 4, 5, 6, 7, 6, 5, 4 };
    assert( ( exponentSearchMax( &array[0], size ) == myMaxI( &array[0], size ) ) );
}

void test8()
{
    const int size = 3;
    double array[size] = { 1, 2, 1 };
    assert( ( exponentSearchMax( &array[0], size ) == myMaxI( &array[0], size ) ) );
}

void test9()  // m глубоко в середние
{
    const int size = 16;
    double array[size] = { 1303, 1492, 2470, 3074, 3493, 4202, 4231, 4296, 4483, 4520, 5450, 6201, 
                           7027, 7772, 8054, 8986 };
    assert( ( exponentSearchMax( &array[0], size ) == myMaxI( &array[0], size ) ) );
}

void test10()
{
    const int size = 10;
    double array[size] = { 1, 2, 3, 4, 3, 2, 1, 0, -1, -2 };
    //std::cout << "max_i = " << myMaxI( &array[0], size ) << std::endl;
    assert( ( exponentSearchMax( &array[0], size ) == myMaxI( &array[0], size ) ) );
}

void test11()
{
    const int size = 6;
    double array[size] = { 3, 4, 3, 2, 1, 0 };
    assert( ( exponentSearchMax( &array[0], size ) == myMaxI( &array[0], size ) ) );
}

void test12()
{
    const int size = 12;
    double array[size] = { 0, 50, 100, 90, 0, -3, -4, -5, -6, -7, -8, -9 };
    assert( ( exponentSearchMax( &array[0], size ) == myMaxI( &array[0], size ) ) );
}

//// Проверка краевых условий
void test13()
{
    const int size = 17; // 0 |[1:2]|[2:4]|[4:8]|[8:16]|
    double array[size] = {  0, 1, 2, 3, -1, -2, -3, -4, -5, -6, -7, -8 };
    assert( ( exponentSearchMax( &array[0], size ) == myMaxI( &array[0], size ) ) );
}

void test14()
{
    const int size = 17; // 0 |[1:2]|[2:4]|[4:8]|[8:16]|
    double array[size] = {  -1, 0, 1, 2, 1, 0, -2, -3, -4, -5, -6, -7 };
    assert( ( exponentSearchMax( &array[0], size ) == myMaxI( &array[0], size ) ) );
}

void test15()
{
    const int size = 17; // 0 |[1:2]|[2:4]|[4:8]|[8:16]|
    double array[size] = {  0, 1, 2, 3, 4, 3, 2, 1, 0, -1, -2, -3 };
    assert( ( exponentSearchMax( &array[0], size ) == myMaxI( &array[0], size ) ) );
}

void test16()
{
    const int size = 17; // 0 |[1:2]|[2:4]|[4:8]|[8:16]|
    double array[size] = { 0, 1, 2, 3, 4, 5, 4, 3, 2, 1, 0, -1 };
    assert( ( exponentSearchMax( &array[0], size ) == myMaxI( &array[0], size ) ) );
}

void test17()
{
    const int size = 100;
    double array[size];
    for ( int i = 0; i <= 51; ++i )
        array[i] = i;
    for ( int i = 52, j = 50; i < 100; ++i, --j )
        array[i] = j;
    assert( ( exponentSearchMax( &array[0], size ) == myMaxI( &array[0], size ) ) );
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
    test16();
    test17();
#else
    int size;
	std::cin >> size;
	double array[size];
	for (int i = 0; i < size; ++i)
    	std::cin >> array[i];
    std::cout << exponentSearchMax(&array[0], size) << std::endl;
#endif
    return 0;
}


