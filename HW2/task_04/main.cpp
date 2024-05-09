#include <iostream>
#include <stack>
#include <vector>
#include <fstream>
#include <string>
#include <stdint.h>

/*
// Задача 4.1
Солдаты. В одной военной части решили построить в одну шеренгу по росту. Т.к. часть была далеко не
образцовая, то солдаты часто приходили не вовремя, а то их и вовсе приходилось выгонять из шеренги за
плохо начищенные сапоги. Однако солдаты в процессе прихода и ухода должны были всегда быть выстроены по
росту – сначала самые высокие, а в конце – самые низкие. За расстановку солдат отвечал прапорщик, который
заметил интересную особенность – все солдаты в части разного роста. Ваша задача состоит в том, чтобы помочь
прапорщику правильно расставлять солдат, а именно для каждого приходящего солдата указывать, перед каким
солдатом в строе он должен становится.

//// Требования: скорость выполнения команды - O(log n).

////Формат входных данных.
Первая строка содержит число N – количество команд (1 ≤ N ≤ 30 000). В каждой следующей строке содержится
описание команды: число 1 и X если солдат приходит в строй (X – рост солдата, натуральное число
до 100 000 включительно) и число 2 и Y если солдата, стоящим в строе на месте Y надо удалить из строя.
Солдаты в строе нумеруются с нуля.

//// Формат выходных данных.
На каждую команду 1 (добавление в строй) вы должны выводить число K – номер позиции, на которую должен
встать этот солдат (все стоящие за ним двигаются назад).
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
    return l < r;
}

template<typename T = int>
class AVLTree
{
    struct Node
    {
        T _item;
        int _sizeNodes;
        uint8_t _height;
        Node* _l;
        Node* _r;

        Node( const T& item ) : _item(item), _height(1), _sizeNodes(1), _l(nullptr) , _r(nullptr) {}
    };

    Node* _root = nullptr;
    bool (*_cmp)( const T&, const T& );

    inline uint8_t getHeight( Node* node ) const { return ( ( node != nullptr ) ? node->_height : 0 ); }
    inline int getBalanceFactor( Node* node ) { return ( getHeight(node->_r) - getHeight(node->_l) ); }
    inline int getSizeNode( Node* node ) { return ( ( node == nullptr ) ? 0 : node->_sizeNodes ); }

    void fixNodes( Node* node )
    {
        node->_sizeNodes = ( getSizeNode(node->_l) + getSizeNode(node->_r) + 1 );
    }

    void fixHeight( Node* node )
    {
        node->_height = ( std::max( getHeight(node->_r), getHeight(node->_l) ) + 1 );
    }

    Node* rotateRight( Node* node )
    {
        Node* bufferNode = node->_l;
        node->_l = bufferNode->_r;
        bufferNode->_r = node;

        fixNodes(node);
        fixHeight(node);
        fixNodes(bufferNode);
        fixHeight(bufferNode);
        return bufferNode;
    }

    Node* rotateLeft( Node* node )
    {
        Node* bufferNode = node->_r;
        node->_r = bufferNode->_l;
        bufferNode->_l = node;

        fixNodes(node);
        fixHeight(node);
        fixNodes(bufferNode);
        fixHeight(bufferNode);
        return bufferNode;
    }

    Node* balancing( Node* node )
    {
        fixHeight(node);
        fixNodes(node);
        if( getBalanceFactor(node) == 2 )
        {
            if( getBalanceFactor(node->_r) < 0 )
                node->_r = rotateRight(node->_r);
            return rotateLeft(node);
        }
        if( getBalanceFactor(node) == -2 )
        {
            if( getBalanceFactor(node->_l) > 0 )
                node->_l = rotateLeft(node->_l);
            return rotateRight(node);
        }

        return node;
    }

    Node* push( Node* node, const T& k, int& position )
    {
        if( node == nullptr ) 
            return new Node(k);

        ++(node->_sizeNodes);

        if( _cmp(k, node->_item) )
        {
            position += ( getSizeNode(node->_r) + 1 );
            node->_l = push(node->_l, k, position);
        }
        else
            node->_r = push(node->_r, k, position);
        return balancing(node);
    }

    Node* findMin( Node* node )
    {
        return ( node->_l ? findMin(node->_l) : node );
    }

    Node* popMin( Node* node )
    {
        if( node->_l == 0 )
            return node->_r;

        node->_l = popMin(node->_l);
        --node->_sizeNodes;

        return balancing(node);
    }

    Node* pop( Node* node, const int& position )
    {
        if( node == nullptr )
            return nullptr;
        if( position >= node->_sizeNodes )
            return node;

        int current = 0;
        std::stack<Node*> nodes;

        for( ;; )
        {
            int sizeNode = getSizeNode(node->_r), buffer = ( position - current );
            if( buffer > sizeNode )
            {
                nodes.push(node);
                node = node->_l;
                current += ( sizeNode + 1 );
            }
            else if( buffer < sizeNode )
            {
                if( node->_r == nullptr )
                    break;

                nodes.push(node);
                node = node->_r;
            }
            else
            {
                Node* left = node->_l;
                Node* right = node->_r;
                T _item = node->_item;

                delete node;

                if( right != nullptr )
                {
                    Node* min = findMin(right);
                    min->_r = popMin(right);
                    min->_l = left;
                    fixNodes(min);
                    node = balancing(min);
                    break;
                }

                if( left != nullptr )
                {
                    node = left;
                    break;
                }

                if( nodes.empty() )
                    return nullptr;

                node = nodes.top();
                nodes.pop();
                ( ( _cmp( _item, node->_item ) ) ? node->_l : node->_r ) = nullptr;
                --node->_sizeNodes;
                break;
            }
        }

        while( !nodes.empty() )
        {
            Node* bufferNode = nodes.top();
            --bufferNode->_sizeNodes;

            ( ( _cmp( bufferNode->_item, node->_item ) ) ? bufferNode->_l : bufferNode->_r ) = node;

            node = balancing(bufferNode);
            nodes.pop();
        }

        return node;
    }
public:
    AVLTree( bool ( *cmp )( const T& l, const T& r ) = defaultLess ) : _root(nullptr), _cmp(cmp) {}
    ~AVLTree()
    {
        if( _root == nullptr )
            return;
        
        std::stack<Node*> nodes;
        Node* current = _root, *last = nullptr;
        while( ( ( !nodes.empty() ) || ( current != nullptr ) ) )
        {
            if( current != nullptr)
            {
                nodes.push(current);
                current = current->_l;
            }
            else
            {
                current = nodes.top();
                if( ( ( current->_l != nullptr ) && ( last != current->_r ) ) )
                    current = current->_r;
                else
                {
                    nodes.pop();
                    last = current;
                    delete current;
                    current = nullptr;
                }
            }
        }
    }

    void push( const T& item, int& position ) { _root = push( _root, item, position ); }

    void pop( const int& position ) { _root = pop(_root, position); } 
};

template <typename T = int>  // Взял с семинара
std::vector<T> buildSolders( std::istream &input )
{
    size_t size;
    input >> size;

    AVLTree avlt;
    std::vector<T> result;

    int item = 0;
    int command = 0, position = 0;

    for( size_t i = 0; i < size; ++i )
    {
        input >> command;
        switch( command )
        {
        case 1:
            position = 0;
            input >> item;
            avlt.push( item, position );
            result.push_back(position);
            break;
        case 2:
            input >> position;
            avlt.pop(position);
            break;
        }
        
    }
    return result;
}

void printVector( const std::vector<int>& vec )
{
    for( auto it : vec )
        std::cout << it << std::endl;
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

class FileNotOpen : public std::exception
{
public:
    explicit FileNotOpen(const std::string &msg) : m_msg(msg) {}
    const char *what() const noexcept override { return m_msg.c_str(); }
private:
    std::string m_msg;
};

template<typename T = int>
bool operator==( const std::vector<T>& v1, const std::vector<T>& v2 )
{
    if( v1.size() != v2.size() )
        return false;

    for( size_t i = 0; i < v1.size(); ++i )
        if( v1[i] != v2[i] )
            return false;

    return true;
}


void makeTest( const std::string& input, const std::string& output )
{
    std::ifstream in( input ), out( output );
    if( !in )
        throw FileNotOpen( input );
    if( !out )
        throw FileNotOpen( output );
    
    auto result = buildSolders(in);
    auto result_true = getVectorFromStream( out );
    assert( result == result_true );
}

#endif

int main()
{
#ifdef MAKETEST
    makeTest( "input/in01.txt", "output/out01.txt" );
#else
    auto vec = buildSolders(std::cin);
    printVector(vec);
#endif
    return 0;
}


