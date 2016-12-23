#include <sstream>
#include "iostream"
#include "gestusVisualization.h"
#include "deque"


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
		std::string buffer1, buffer2;
		// todo: adapt visualisation for buffers and iDs
		Visualization visualization;

		std::deque<std::string> buffer;

		buffer.push_back("123 23 94");
		buffer.push_back("154 54 124");
		buffer.push_back("173 74 144");
		buffer.push_back("193 93 194");
		buffer.push_back("123 23 34");

		visualization.visualizeData(buffer);
		visualization.visualizeData(buffer);
		// this should be invoked in new thread
		visualization.run( );

		return EXIT_SUCCESS;
}
