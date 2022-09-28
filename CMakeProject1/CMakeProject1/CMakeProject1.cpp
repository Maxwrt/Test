// CMakeProject1.cpp: 定义应用程序的入口点。
//

#include "CMakeProject1.h"
#include "math_.h"
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp>
#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/support/date_time.hpp>
#include <vector>
#include <algorithm>
using namespace std;
using namespace boost::filesystem;
namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace expr = boost::log::expressions;
void init_log()
{
	logging::add_file_log
	(
		keywords::file_name = "sample_%N.log",
		keywords::rotation_size = 10 * 1024 * 1024,
		keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
		keywords::format = (expr::stream << "["
										 << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d-%H:%M:%S.%f ")
										 << path(__FILE__).filename() << ":" << __LINE__
								         << "]<"
										 << boost::log::trivial::severity 
										 << ">:"
										 << expr::smessage),
		keywords::auto_flush = true,
		keywords::open_mode = std::ios_base::app
	);
	logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::trace);
	logging::add_common_attributes();
}
int main(int argc, char *argv[])
{
#if 0
	if (argc < 2)
	{
		cout << "Usage: CMakeProject1 path\n";
		return 1;
	}
#endif
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
	directory_entry dir_("./");
	cout << "root_path:" << dir_.path().root_directory() << endl;
	cout << "root_name:" << dir_.path().root_name() << endl;
	cout << "root_path:" << dir_.path().root_path() << endl;
	cout << "relative_path:" << dir_.path().relative_path() << endl;
	cout << "parent_path:" << dir_.path().parent_path() << endl;
	cout << "stem:" << dir_.path().stem() << endl;
	cout << "filename:" << dir_.path().filename() << endl;
	cout << "extension: " << dir_.path().extension() << endl;
	
	path p("./");

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

	init_log();
	BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
	BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
	BOOST_LOG_TRIVIAL(info) << "An info severity message";
	BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
	BOOST_LOG_TRIVIAL(error) << "An error severity message";
	BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";
	/*
	*	|    数据包         | CRC校验码 |
	*	| 01 03 00 12 00 10 | 03 E4     |
	*/
	std::vector<uint8_t> array = { 0x01, 0x03, 0x00, 0x12, 0x00, 0x10 };
	uint16_t cal_crc = Crc16ForModbus(array);
	if (cal_crc == 0x03E4)
		cout << "data correct\n";
	else
		cout << "data error\n";
	return 0;
}
