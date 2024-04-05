#include <iostream>
#include <cassert>

/*
// Задача 4.1
Напишите программу, которая использует кучу для слияния K отсортированных массивов суммарной длиной N.
//// Требования:
    Время работы O(N * logK)
    Куча должна быть реализована в виде шаблонного класса.
    Решение должно поддерживать передачу функции сравнения снаружи.
    Куча должна быть динамической.
//// Формат ввода:
    Сначала вводится количество массивов K. Затем по очереди размер каждого массива и элементы массива.
    Каждый массив упорядочен по возрастанию.
////Формат вывода:
    Итоговый отсортированный массив.
*/

// На локальной машине, чтобы поверить
#define MAKETEST
#ifdef MAKETEST
#include <random>
static std::random_device engine;
#endif
template <typename T = int>  // Взял с семинара
bool defaultLess( const T &l, const T &r )
{
    return l < r;
}

class ArrayException : public std::exception
{
public:
    explicit ArrayException( const std::string &msg ) : m_msg( msg ) {}
    const char *what() const noexcept override
    {
		return m_msg.c_str();
	}

private:
    std::string m_msg;
};

class InvalidArrayStream : public ArrayException
{
public:
    InvalidArrayStream() : ArrayException("Произошла ошибка при чтении из потока") {}
};

template <typename T>
class OutOfRange : public ArrayException
{
public:
    OutOfRange( const size_t& i, const T& myarray ) : ArrayException(
        "Индекс (" + std::to_string( i )  + ") выход за границы Массива. Размер массива [" +
        std::to_string( myarray.size() ) + "]") {}
};

template <typename T = int>
class MyArray  // При множественном вложении явно укзаать тип данных!!!!!
{
private:
    size_t _size;
    T* _arr;

public:
    MyArray( const size_t& resize = 0 ) noexcept : _size(resize)  // Тут что то explicit лишнее
    {
        _arr = new T[_size];
    }

    MyArray( const MyArray<T>& rhs ) noexcept
    {
        _size = rhs.size();
        delete[] _arr;
        _arr = new T[_size];
        for( size_t i = 0; i < _size; ++i )
            _arr[i] = rhs._arr[i];
    }

    MyArray( std::istream& is )
    {
        _size = 0;
        is >> _size;
        _arr = new T[_size];
        for( size_t i = 0; i < _size; ++i )
            if ( !( is >> _arr[i] ) )
            {
                delete[] _arr;
                throw InvalidArrayStream();
            }
    }

    MyArray<T>& operator=( const MyArray<T> &rhs ) noexcept
    {
	    if ( &rhs != this ) {
            delete[] _arr;
            _size = rhs.size();
            _arr = new T[_size];
		    for (size_t i = 0; i < _size; ++i)
                this->_arr[i] = rhs._arr[i];
	    }
        return *this;
    }

    ~MyArray()
    {
        delete[] _arr;
    }

    T& operator[]( const size_t& i ) const noexcept
    {
        return _arr[i];
    }

    T& operator[]( const size_t& i ) noexcept
    {
        return _arr[i];
    }

    T& at( const size_t& i ) {
        if ( i > _size )
            throw OutOfRange( i, this );
        return _arr[i];
    }

    T& at( const size_t& i ) const {
        if ( i > _size )
            throw OutOfRange( i, this );
        return _arr[i];
    }

    bool operator!=( const MyArray<T>& rhs ) const
    {
        if( this->_size != rhs._size )
            return true;
        for( size_t i = 0; i < _size; ++i )
            if( this->_arr[i] != rhs._arr[i] )
                return true;
        return false;
    }

    bool empty() const noexcept
    {
        return ( _size == 0 );
    }

    size_t size() const noexcept
    { 
        return this->_size;
    }

    void clear() noexcept
    {
        delete[] _arr;
    }

};

template < typename T = int >
std::istream& operator>>( std::istream& is, MyArray<T>& obj )
{
    MyArray<T> buf(is);  // ужасно сделално.
    obj = buf;
    return is;
}

// friend
template < typename T = int >
std::ostream& operator<<( std::ostream& os, const MyArray<T>& arr ) noexcept
{
    //os << arr.size() << '\n';
    for ( size_t i = 0; i < arr.size(); ++i )
        os << arr[i] << ' ';
    os << '\n';
    return os;
}

//// Код
template <typename T = int>
class MyHeap
{
private:
    size_t _size;
    size_t _image_size;
    bool (*_cmp)( const T&, const T& );
    T* _arr;

    inline void mySwap( T& l, T& r )
    {
        T tmp = l;
        l = r;
        r = tmp;        
    }

    size_t localSwap( T& l, T& r, const size_t li )
    {
        mySwap( l, r );
        return li;
    }

    void reBuild()
    {
        _size *= 2;
        T* buffer = new T[_size];
        for( size_t i = 0; i < _image_size; ++i)
            buffer[i] = _arr[i];
        delete[] _arr;
        _arr = buffer;
    }

    inline size_t root( const size_t& I ) const { return ( ( I != 0 ) ? ( ( I - 1 ) / 2 ) : 0 ); }
    
    inline size_t left( const size_t& I ) const { return ( ( 2 * I ) + 1 ); }
    
    inline size_t right( const size_t& I ) const { return ( ( 2 * I ) + 2 ); }

    void sieveUp( const size_t& I )
    {
        size_t i = I;
        size_t j = root(i);

        while( ( i != 0 ) && _cmp( _arr[i], _arr[j] ) )
        {
            i = localSwap( _arr[j], _arr[i], j );
            j = root(i);
        }
    }

    void sieveDown( const size_t& I )
    {
        size_t i = I;
        size_t l = left(i);
        size_t r = right(i);

        while( ( i < ( _image_size / 2 ) ) && ( _cmp( _arr[l], _arr[i] ) ||
                     ( _cmp(_arr[r], _arr[i] ) && ( r < _image_size ) ) ) )
        {
            i = ( _cmp( _arr[l], _arr[r] ) || ( r >= _image_size ) ) ? localSwap( _arr[l], _arr[i], l ) : 
                                                                    localSwap( _arr[r], _arr[i], r );
            l = left(i);
            r = right(i);
        }
    }

public:
    MyHeap( bool ( *cmp )( const T& l, const T& r ) = defaultLess ) : _size(1), _image_size(0), _cmp(cmp)
    {
        _arr = new T[_size];
    }

    ~MyHeap()
    {
        delete[] this->_arr;
    }

    void push( T element )
    {
        if ( _image_size == _size )
            reBuild();
        _arr[_image_size++] = element;
        sieveUp( ( _image_size - 1 ) );
    }

    T pop()
    {
        if ( _image_size == 0 )
            assert(0);
        mySwap( _arr[0], _arr[( _image_size - 1 )] );
        T result = _arr[--_image_size];
        sieveDown(0);
        return result;
    }

};

template < typename L, typename R >
struct MyPair
{
public:
    L _l;
    R _r;

    MyPair() {}
    MyPair( const L& l, const R& r ) : _l(l), _r(r) {}
    ~MyPair() {}

    bool operator<( const MyPair< L, R >& rhs ) const
    {
        return ( _r < rhs._r );
    }

    bool operator>( const MyPair< L, R >& rhs ) const
    {
        return ( _r > rhs._r );
    }
};

// friend
std::ostream& operator<<( std::ostream& os, const MyPair< size_t, int >& mypair )
{
    os << mypair._l << ' ' << mypair._r << '\n';
    return os;
}


// friend
std::ostream& operator<<( std::ostream& os, const MyPair< size_t, MyArray<int> >& mypair )
{
    os << "Группа: " << mypair._l << '\n';
    os << mypair._r;
    return os;
}

MyPair< size_t, MyArray<int> > readPair( const size_t& num )
{
    MyPair< size_t, MyArray<int> > mypair;
    mypair._l = num;
    std::cin >> mypair._r;
    return mypair;
}

MyArray< MyPair< size_t, MyArray<int> > > readPairs()
{
    size_t N = 0;
    std::cin >> N;
    MyArray< MyPair< size_t, MyArray<int> > > mypairs(N);
    for( size_t i = 0; i < N; ++i )
        mypairs[i] = readPair( i );
    return mypairs;
}

size_t getSizeFromPairs( const MyArray< MyPair< size_t, MyArray<int> > >& df )
{
    size_t size = 0;
    for( size_t i = 0; i < df.size(); ++i)
        size += df[i]._r.size();
    return size;
}

MyArray<int> revolver( const MyArray< MyPair< size_t, MyArray<int> > >& df )
{
    MyHeap< MyPair< size_t, int > > heap;
    MyArray<size_t> cnt(df.size());
    for( size_t i = 0; i < cnt.size(); ++i)
    {
        heap.push( MyPair< size_t, int >( df[i]._l, df[i]._r[0] ) );
        ++cnt[i];
    }

    const size_t SIZE = getSizeFromPairs(df);
    MyArray<int> sortedArray(SIZE);
    MyPair< size_t, int > mypair;
    for( size_t i = 0, index = 0; i < SIZE; ++i )
    {
        mypair = heap.pop();  // O(K), Прокручиваем значния
        index = mypair._l;
        sortedArray[i] = mypair._r;
        if( cnt[index] < df[index]._r.size() )
        {
            heap.push( MyPair< size_t, int >( index, df[index]._r[cnt[index]] ) );
            ++cnt[index];
        }
    }
    return sortedArray;
}

//// Тест
#ifdef MAKETEST
template <typename T = int>
void mySort( T *arr, int l, int r, bool ( *cmp )( const T &l, const T &r ) = defaultLess )
{
    for ( int i = l; i < r; i++ )
    {
        for ( int j = l; j < r; j++ )
        {
            if ( cmp( arr[j + 1], arr[j] ) )
            {
                std::swap( arr[j + 1], arr[j] );
            }
        }
    }
}

template <typename T = int>
bool arrComparator( T* lm, T* rm, const size_t& N, bool ( *cmp )( const T &l, const T &r ) = defaultLess )
{
    for( size_t i = 0; i < N; ++i )
        if( cmp( lm[i], rm[i] ) )
            return false;
    return true;
}

template <typename T = int>
T genT(T from, T to) {
    return std::uniform_real_distribution<T>(from, to)(engine);
}

template <typename T = int>
void randomGenerator( T* arr, const size_t& N, T from, T to )
{
    for( size_t i = 0; i < N; ++i)
        arr[i] = genT(from, to);
}

template <typename T = int>
void fillHeapFromArr( MyHeap<T>& mh, T* arr, const size_t& N )
{
    for( size_t i = 0; i < N; ++i )
        mh.push(arr[i]);
}

template <typename T = int>
void fillArrFromHeap( MyHeap<T>& mh, T* arr, const size_t& N )
{
    for( size_t i = 0; i < N; ++i )
        arr[i] = mh.pop();
}

template <typename T = int>
bool tester( MyHeap<T>& mh, const size_t& size, bool ( *cmp )( const T &l, const T &r ) = defaultLess )
{
    T *arr_test = new T[size];
    T *arr_heap = new T[size];
    randomGenerator<T>( &arr_test[0], size, -10, 10);
    fillHeapFromArr<T>( mh, &arr_test[0], size );
    fillArrFromHeap<T>( mh, &arr_heap[0], size );
    mySort<T>( &arr_test[0], 0, ( size - 1 ) );
    return arrComparator<T>( &arr_test[0], &arr_heap[0], size, cmp );
}

void test1()
{
    MyHeap mh;
    assert( tester( mh, 1 ) );
}

void test2()
{
    MyHeap mh;
    assert( tester( mh, 2 ) );
}

void test3()
{
    MyHeap mh;
    assert( tester( mh, 3 ) );
}

void test4()
{
    MyHeap mh;
    assert( tester( mh, 5 ) );
}

void test5()
{
    MyHeap mh;
    assert( tester( mh, 16 ) );
}

void test6()
{
    MyHeap mh;
    assert( tester( mh, 100 ) );
}

void test7()
{
    MyHeap mh;
    assert( tester( mh, 1000 ) );
}

void test8()
{
    MyHeap<float> mh;
    assert( tester<float>( mh, 1000 ) );
}

void test9()
{
    MyHeap<double> mh;
    assert( tester<double>( mh, 1000 ) );
}

template <typename T = int>  // Взял с семинара
bool defaultMore( const T &l, const T &r )
{
    return l > r;
}

void test10()
{
    MyHeap mh( defaultMore );
    assert( tester( mh, 1000, defaultMore ) );
}

void test11()
{
    MyArray<MyPair<size_t, MyArray<int>>> df = {
        MyPair<size_t, MyArray<int>>(0, MyArray<int>(1)),
        MyPair<size_t, MyArray<int>>(1, MyArray<int>(2)),
        MyPair<size_t, MyArray<int>>(2, MyArray<int>(3))
    };  // Это ужасно, надо бы переписать
    for( size_t i = 0; i < df.size(); ++i )
    {
        for( size_t j = 0; j < df[i]._r.size(); ++j)
        {
            df[i]._r[j] = genT<int>(-100, 100);
        }
    }
/*
    MyArray<int> a1(1);
    a1[0] = 6;
    MyPair< size_t, MyArray<int> > p1(0, a1);
    MyArray<int> a2(2);
    a2[0] = 50;
    a2[1] = 90;
    MyPair< size_t, MyArray<int> > p2(1, a2);
    MyArray<int> a3(3);
    a3[0] = 1;
    a3[1] = 10;
    a3[2] = 70;
    MyPair< size_t, MyArray<int> > p3(2, a3);
    df[0] = p1;
    df[1] = p2;
    df[3] = p3;
    MyArray<int> otvet(6);
    otvet[0] = 1;
    otvet[1] = 6;
    otvet[2] = 10;
    otvet[3] = 50;
    otvet[4] = 70;
    otvet[5] = 90;
*/
    auto res = revolver(df);
    //assert( ( res.size() == otvet.size() ) );
    for( size_t i = 1; i < res.size(); ++i )
        assert( ( res[( i - 1 )] < res[i] ) );
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
#else
    auto df = readPairs();
    auto array = revolver( df );
    std::cout << array;
/*
    size_t N = 0, SIZE = 0;
    MyHeap mh;
    int buffer;
    std::cin >> N;
    for( size_t i = 0, j = 0, k = 0; i < N; ++i)
    {
        std::cin >> j;
        for( k = 0; k < j; ++k )
        {
            std::cin >> buffer;
            mh.push(buffer);
        }
        SIZE += j;
    }
    for( size_t i = 0; i < SIZE; ++i )
        std::cout << mh.pop() << " ";
    std::cout << std::endl;
*/
#endif
    return 0;
}


