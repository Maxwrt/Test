// CMakeProject1.cpp: 定义应用程序的入口点。
//

#include "CMakeProject1.h"
#include <boost/any>
using namespace std;

int main()
{
	std::string str("Hello");
	str.append("  CMake.");
	cout << "str: " << str << endl;
	return 0;
}
