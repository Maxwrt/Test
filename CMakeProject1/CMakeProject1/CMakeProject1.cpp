// CMakeProject1.cpp: 定义应用程序的入口点。
//

#include "CMakeProject1.h"
#include "math_.h"
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp>
using namespace std;
using namespace boost;
int main()
{
	std::string str("Hello");
	str.append(" CMake.");
	cout << "str: " << str << endl;

	//调用boost的tuple
	boost::tuple<int, char, float> tuple_(0, 'a', 0.0);
	std::cout << "tuple: " << tuple_ << std::endl;
	
	//调用utility库的sum
	std::cout << "sum: " << sum(10, 11) << std::endl;
	return 0;
}
