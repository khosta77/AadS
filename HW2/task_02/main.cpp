#include <iostream>
#include <fstream>
#include <stack>
#include <vector>

/*
// Задача 2.2
Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N. Требуется построить бинарное
дерево, заданное наивным порядком вставки. Т.е., при добавлении очередного числа K в дерево с корнем 
root, если root->Key ≤ K, то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
Требования: Рекурсия запрещена. Решение должно поддерживать передачу функции сравнения снаружи.
2_2. Выведите элементы в порядке pre-order (сверху вниз).
*/

// На локальной машине, чтобы поверить
#define MAKETEST
#ifdef MAKETEST
#include <cassert>
#endif

//// Код
 
template <typename T = int>  // Взял с семинара
bool defaultLess( const T &l, const T &r )
{
    return l > r;
}

template<typename T = int>
class BinaryTree
{
private:
    struct Node
    {
        T _item;
        Node* _p;
        Node* _l;
        Node* _r;

        Node() : _item(0), _p(nullptr), _l(nullptr), _r(nullptr) {}
        Node( const T& item, Node *parent, Node *left, Node *right ) : _item(item), _p(parent),
                                                                       _l(left), _r(right) {}
        ~Node()
        {
            if( this->_l != nullptr )
                delete this->_l;
            if( this->_r != nullptr )
                delete this->_r;
        }
    };

    Node* _root;
    bool (*_cmp)( const T&, const T& );

public:
    BinaryTree( bool ( *cmp )( const T& l, const T& r ) = defaultLess ) : _root(nullptr), _cmp(cmp) {}
    ~BinaryTree()
    {
        if( _root != nullptr )
            delete _root;
    }

    void push( const T& item )
    {
        if( _root == nullptr )
            return ( (void)( _root = new Node( item, nullptr, nullptr, nullptr ) ) );

        Node *it = _root;
        bool check = true;
        while( it != nullptr )
        {
            check = _cmp( item, it->_item );
            if( ( (check) ? it->_r : it->_l ) == nullptr )
                return ( (void)( ( (check) ? it->_r : it->_l ) = new Node( item, it, nullptr, nullptr ) ) );
            it = ( (check) ? it->_r : it->_l );
        }
    }
    
    std::vector<T> getPreOrder()
    {
        if( _root == nullptr )
            return std::vector<T>(0);
    
        std::stack<Node*> buffer;
        std::vector<T> preOrder;
        buffer.push(_root); 
        while( buffer.empty() != 1)
        {
            Node* it = buffer.top();
            buffer.pop();
            preOrder.push_back( it->_item );
        
            if ( it->_r )
                buffer.push( it->_r );
            if ( it->_l )
                buffer.push( it->_l );
        }

        return preOrder;
    }
};

template<typename T = int>
void getBinaryTreeFromStream( BinaryTree<T>& bt, std::istream &input )
{
    size_t N = 0;
    input >> N;
    T item = 0;
    for( size_t i = 0; i < N; ++i )
    {
        input >> item;
        bt.push(item);
    }
}

void printVectorInt( const std::vector<int>& vec )
{
    for( size_t i = 0; i < vec.size(); ++i )
        std::cout << vec[i] << ' ';
    std::cout << std::endl;
}

//// Тест
#ifdef MAKETEST
template<typename T = int>
std::vector<T> getVectorFromStream( std::istream &input )
{
    size_t N = 0;
    input >> N;
    T item = 0;
    std::vector<T> vec(N);
    for( size_t i = 0; i < N; ++i )
        input >> vec[i];
    return vec;
}

bool cmpVec( const std::vector<int>& l, const std::vector<int>& r )
{
    if( l.size() != r.size() )
        return false;
    for( size_t i = 0; i < l.size(); ++i )
        if( l[i] != r[i] )
            return false;
    return true;
}

class FileNotOpen : public std::exception
{
public:
    explicit FileNotOpen(const std::string &msg) : m_msg(msg) {}
    const char *what() const noexcept override { return m_msg.c_str(); }
private:
    std::string m_msg;
};

void makeTest( const std::string& input, const std::string& output )
{
    std::ifstream in( input ), out( output );
    if( !in )
        throw FileNotOpen( input );
    if( !out )
        throw FileNotOpen( output );
    
    BinaryTree bt;
    getBinaryTreeFromStream( bt, in );
    auto l = bt.getPreOrder();
    auto r = getVectorFromStream( out );
    assert( cmpVec( l, r ) );
}
#endif


int main()
{
#ifdef MAKETEST
    makeTest( "input/in01.txt", "output/out01.txt" );
    makeTest( "input/in02.txt", "output/out02.txt" );
    makeTest( "input/in03.txt", "output/out03.txt" );
    makeTest( "input/in04.txt", "output/out04.txt" );
    makeTest( "input/in05.txt", "output/out05.txt" );
    makeTest( "input/in06.txt", "output/out06.txt" );
    makeTest( "input/in07.txt", "output/out07.txt" );
#else
    BinaryTree bt;
    getBinaryTreeFromStream( bt, std::cin );
    auto preorder = bt.getPreOrder();
    printVectorInt( preorder );
#endif
    return 0;
}


