// CMakeProject1.cpp: 定义应用程序的入口点。
//

#include "CMakeProject1.h"
#include "math_.h"
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp>
#include <boost/filesystem.hpp>
#include <vector>
#include <algorithm>
using namespace std;
using namespace boost::filesystem;

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		cout << "Usage: CMakeProject1 path\n";
		return 1;
	}
	cout << argv[0] << " " << file_size(argv[0]) << endl;
	string str("Hello");
	str.append(" CMake.");
	cout << "str: " << str << endl;

	//调用boost的tuple
	boost::tuple<int, char, float> tuple_(0, 'a', 0.0);
	cout << "tuple: " << tuple_ << endl;
	
	//调用utility库的sum
	cout << "sum: " << sum(10, 11) << endl;

	//调用 boost的filesystem
	directory_entry dir_(argv[1]);
	cout << "root_path:" << dir_.path().root_directory() << endl;
	cout << "root_name:" << dir_.path().root_name() << endl;
	cout << "root_path:" << dir_.path().root_path() << endl;
	cout << "relative_path:" << dir_.path().relative_path() << endl;
	cout << "parent_path:" << dir_.path().parent_path() << endl;
	cout << "stem:" << dir_.path().stem() << endl;
	cout << "filename:" << dir_.path().filename() << endl;
	cout << "extension: " << dir_.path().extension() << endl;
	
	path p(argv[1]);

	try
	{
		if (exists(p))
		{
			if (is_regular_file(p))
				cout << p << " size is " << file_size(p) << "\n";
			else if (is_directory(p))
			{
				cout << p << " is a directory containing:\n";
				vector<string> filev;

				for (auto& x : directory_iterator(p))
					filev.push_back(x.path().filename().string());

				sort(filev.begin(), filev.end());
				for (auto& x : filev)
					cout << "\t" << x << "\n";
			}
			else
				cout << p << " exists but not a regular file or directory\n";
		}
		else
			cout << p.string() << " does not exists" << endl;
	}
	catch (const filesystem_error& ex)
	{
		cout << ex.what() << "\n";
	}


	return 0;
}
