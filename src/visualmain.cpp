#include <sstream>
#include "iostream"
#include "gestusVisualization.h"
#include "deque"
#include "map"


void splitSensorData(std::string str, double arr[3])
{
		int i = 0;
		double n;
		std::stringstream ss(str);
		while(ss >> n)
		{
				arr[i] = n;
				i++;
		}
}

int main()
{
		std::string id1, id2;
		// todo: adapt visualisation for buffers and iDs
		Visualization visualization;

		std::deque<std::string> buffer;
		buffer.push_back("123 23 94");
		buffer.push_back("154 54 124");
		buffer.push_back("173 74 144");
		buffer.push_back("193 93 194");
		buffer.push_back("123 23 34");

		std::deque<std::string> buffer1;
		buffer1.push_back(buffer.front());
		buffer1.push_back(buffer.back());

		std::deque<std::string> buffer2;
		buffer2.push_back(buffer.back());
		buffer2.push_back(buffer.front());

		std::map< std::string, std::deque<std::string>*> buffers;
		buffers["buffer"] = &buffer;
		buffers["second buffer"] = &buffer1;
		buffers["first buffer"] = &buffer2;


		visualization.visualizeData(buffers);

		// read convention file
		// make a map key - buffer name, value - pointer to buffer

		// this should be invoked in new threa
		// visualization.run();

		return EXIT_SUCCESS;
}
