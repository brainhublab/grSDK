#include "iostream"
#include "gestusVisualization.h"

int main()
{
		std::string id1, id2;
		std::string buffer1, buffer2;
		// todo: adapt visualisation for buffers and iDs
		Visualization visualization( id1, buffer1, id2, buffer2 );

		double d[] = {10, 0, 1};
		double t[] = {100, -30, -88};
		visualization.addPlotData(d);
		visualization.addPlotData(t);

		// this should be invoked in new thread
		visualization.run( );

		return EXIT_SUCCESS;
}
