#include "BoxMuller.h"

double BoxMuller::genDouble()
{
	std::random_device _engine;
    double random =  std::uniform_real_distribution<double>( 0, _MAX_DOUBLE )(_engine);
	return ( random / _MAX_DOUBLE );
}

BoxMuller::BoxMuller( const size_t& size ) : _MAX_DOUBLE( std::numeric_limits<double>::max() )
{
	for( size_t i = 0; i < size; ++i )
    {
		double x = 0.0, y = 0.0, s = 0.0;
        while( ( ( s > 1 ) || ( s == 0 ) ) )
		{
            x = genDouble();
            y = genDouble();
            s = ( std::pow( x, 2 ) + std::pow( y, 2 ) );
		}
        z.emplace_back(std::pair<double, double>( ( x * std::sqrt(-2 * std::log(s) / s) ),
											      ( y * std::sqrt(-2 * std::log(s) / s) ) ));
    }
}

double BoxMuller::operator()( const size_t& from, const size_t& to )
{
	if( from == to )
		return 0;
    return std::sqrt(
        std::pow( z[from].first - z[to].second, 2 ) + std::pow( z[from].second - z[to].second, 2 )
	);
}


