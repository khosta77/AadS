#include "Test.h"

ListGraph Test::fillGraph( const size_t& verticesCount )
{
	BoxMuller BMT(verticesCount);
    ListGraph graph(verticesCount);
	for( int from = 0; from < verticesCount; ++from )
        for( int to = ( from + 1 ); to < verticesCount; ++to )
			graph.AddEdge( from, to, BMT( from, to ) );
    return graph;        
}

void Test::tester( const size_t& verticesCount )
{
	auto graph = fillGraph(verticesCount); 
    double PrimeValue = DSU::calcTotalTime( DSU::findPrima( graph ) );
	double enumerationValue = enumeration( graph);
    testResult.push_back( enumerationValue / PrimeValue );
}

double Test::calcAvg()
{
	double avg = 0;
    for( const double& it : testResult )
		avg += it;
    avg /= testResult.size();
    return avg;
}

double Test::calcDispersion( const double& avg )
{
	double dispersion = 0;
    for( const double& it : testResult)
		dispersion += std::pow( ( it - avg ), 2 );
    dispersion /= testResult.size();
    return dispersion;
}

void Test::printResult( const size_t& verticesCount )
{
	double avg = calcAvg();
    double dispersion = calcDispersion(avg);
	std::cout << std::setw(2) << verticesCount << ") disp^2 = " << std::setw(9) << dispersion << "; avg = " 
              << std::setw(9) << avg << std::endl;
}

void Test::make()
{
	for( size_t i = _LEFT; i <= _RIGHT; ++i )
    {
		for( size_t j = 0; j < _TEST_SIZE; ++j )
            tester(i);
        printResult(i);
	}
}


