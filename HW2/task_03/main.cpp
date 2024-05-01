#include <iostream>
#include <deque>

/*
// Задача 3
Постройте B-дерево минимального порядка t и выведите его по слоям. В качестве ключа используются числа, 
лежащие в диапазоне [0..232-1]
//// Требования:
* B-дерево должно быть реализовано в виде шаблонного класса.
* Решение должно поддерживать передачу функции сравнения снаружи.

//// Формат ввода:
Сначала вводится минимальный порядок дерева t. Затем вводятся элементы дерева.

//// Формат вывода:
Программа должна вывести B-дерево по слоям. Каждый слой на новой строке, элементы должны выводится в 
том порядке, в котором они лежат в узлах.
*/

// На локальной машине, чтобы поверить
//#define MAKETEST
#ifdef MAKETEST
#include <cassert>
#endif

//// Код

template <typename T = size_t>  // Взял с семинара
bool defaultLess( const T &l, const T &r )
{
    return l < r;
}

template <typename T = size_t>
class BTree
{
    struct Node
    {
        size_t _order;
        bool _isLeaf;
    private:
        bool (*_cmp)( const T&, const T& );
    public:
        size_t _size;

        T *_keys;
        Node **_children;

        Node() = delete;
        Node( const size_t& order, const bool& isLeaf, bool ( *cmp )( const T& l, const T& r ) ) : 
            _order(order), _isLeaf(isLeaf), _cmp(cmp), _size(0)
        {
            _keys = new T[( ( 2 * _order ) - 1 )];
            _children = new Node*[( 2 * _order )];
        }
    
        ~Node()
        {
            for( size_t i = 0; i < _size; ++i )
                delete[] _children[i];
            delete[] _children;
            delete[] _keys;
        }

        void pushToNode( const T& key )
        {
            size_t i = ( _size - 1 );

            if( _isLeaf )
            {
                for( ; ( ( i >= 0 ) && _cmp( key, _keys[i] ) ); --i )
                    _keys[( i + 1 )] = _keys[i];
                _keys[( i + 1 )] = key;
                _size = ( _size + 1 );
                return;
            }

            while( ( ( i >= 0 ) && _cmp( key, _keys[i] ) ) )
                --i;

            if( _children[( i + 1 )]->_size == ( ( 2 * _order ) - 1 ) )
            {
                reBuild( (i + 1), _children[( i + 1 )] );
                if( _cmp( _keys[( i + 1 )], key ) )
                    ++i;
            }
            _children[( i + 1 )]->pushToNode(key);
        }

        void reBuild( const size_t& I, Node *node )
        {
            Node* buffer = new Node( node->_order, node->_isLeaf, _cmp );
            buffer->_size = ( _order - 1 );

            for( size_t j = 0; j < ( _order - 1 ); ++j )
                buffer->_keys[j] = node->_keys[j + _order];
            
            if( !node->_isLeaf )
                for( size_t j = 0; j < _order; ++j )
                    buffer->_children[j] = node->_children[( j + _order )];
            
            node->_size = ( _order - 1 );
            for( size_t j = _size; j >= ( I + 1 ); --j )
                _children[( j + 1 )] = _children[j];

            _children[( I + 1 )] = buffer;

            for( size_t j = _size; j > I; --j )
                _keys[( j + 1 )] = _keys[j];
            _keys[I] = node->_keys[( _order - 1 )];
            _size = ( _size + 1 );
        }
    };

    size_t _order;
    bool (*_cmp)( const T&, const T& );
    Node *_root;

    /** @brief printNode - выведет ноду и сохранит указатели на ее листья, если есть.
     * */
    void printNode( std::deque<Node*>& deq, Node* node )
    {
        for( size_t i = 0; i < node->_size; ++i)
        {
            std::cout << node->_keys[i] << ' ';
            if( !node->_isLeaf )
                deq.push_back(node->_children[i]);
        }
        if( !node->_isLeaf )
            deq.push_back(node->_children[(node->_size)]);
    }

public:
    BTree() = delete;
    BTree( const size_t& order, bool ( *cmp )( const T& l, const T& r ) = defaultLess ) : _order(order), 
        _cmp(cmp), _root(nullptr) {}

    ~BTree() = default;

    void printLevels()
    {
        if( _root == nullptr )
            return;

        std::deque<Node*> deq;
        printNode( deq, _root );
        std::cout << std::endl;

        size_t N = deq.size(), j = 1;

        while( !deq.empty() )
        {
            Node* node = deq.front();
            deq.pop_front();

            printNode( deq, node );
            if( (j++) == N )
            {
                std::cout << std::endl;
                N = deq.size();
                j = 0;
            }       
        }
        std::cout << std::endl;
    }

    void push( const T& key )
    {
        if ( _root == nullptr )
        {
            _root = new Node( _order, true, _cmp );
            _root->_keys[0] = key;
            _root->_size = 1;
            return;
        }

        if ( _root->_size != ( ( 2 * _order ) - 1 ) )
            return ( (void)( _root->pushToNode(key) ) );

        Node *node = new Node( _order, false, _cmp );
        node->_children[0] = _root;
        node->reBuild( 0, _root );
        node->_children[( ( _cmp( node->_keys[0], key ) ? 1 : 0 ) )]->pushToNode(key);
        _root = node;
    }
};

//// Тест
#ifdef MAKETEST
void test1()
{
    std::cout << "--------------------------" << std::endl;
    BTree tree(2);
    for( int i = 0; i < 10; ++i )
        tree.push(i);
    tree.printLevels();
}

void test2()
{
    std::cout << "--------------------------" << std::endl;
    BTree tree(4);
    for( int i = 0; i < 10; ++i )
        tree.push(i);
    tree.printLevels();
}
#endif

int main()
{
#ifdef MAKETEST
    test1();
    test2();
#else
    size_t order = 0;
    std::cin >> order;
    BTree tree(order);
    size_t key = 0;
    while( std::cin >> key )
    {
        tree.push(key);
    }
    tree.printLevels();
#endif
    return 0;
}


