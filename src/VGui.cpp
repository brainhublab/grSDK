#include <sstream>
#include "VGui.h"

VGui::VGui()
{
		for ( int i = 0; i < sizeof( chartArray ) / sizeof( float ); i++ )
    {
			chartArray[ i ] = ( i & 1 ) ? 1.0f : 0.0f;
		}

}

VGui::~VGui()
{

}

double getAxisAngle(std::string str, size_t index)
{
    int i = 0;
    double n;
    std::stringstream ss(str);
    while(ss >> n)
    {
        if(i == index)
        {
            return n;
        }
        i++;
    }

    return NAN;
}

bool VGui::renderChartFromBuffer(const std::deque< std::string > &buffer, size_t axis)
{
    float min = 0, max = 0;
    float *arr = new float[buffer.size( )];
    for ( int i = 0; i < buffer.size(); ++i )
    {
        float tmp = static_cast<float>(getAxisAngle(buffer[i], axis));
        if(tmp < min)
            min = tmp;
        if(tmp > max)
            max = tmp;

        arr[ i ] = tmp;
    }

    drawChart( arr, buffer.size( ), min, max );

    return true;
}



bool VGui::plotData(const std::deque< std::string > &buffer )
{
//    // Config new window
//    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
//    ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
//    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiSetCond_Once);
//    ImGui::SetNextWindowSize(ImVec2(280, 300), ImGuiSetCond_Once);
//
//    // Begin a new window
//    ImGui::Begin( "", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs);
//
//    static float x, y, z;
//
//    ImGui::InputFloat("X", &x);
//    ImGui::InputFloat("Y", &y);
//    ImGui::InputFloat("Z", &z);
//    if(ImGui::Button("add plot data"))
//    {
////        printf( "%d, %d, %d", ( int ) x, ( int ) y, ( int ) z );
//        std::string tmp;
//        tmp.append(std::to_string(x));
//        tmp.append(" ");
//        tmp.append(std::to_string(y));
//        tmp.append(" ");
//        tmp.append(std::to_string(z));
////        buffer.push_back(tmp);
//    }

    ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
    {
        ImGui::SetNextWindowPos( ImVec2( 0, 0 ), ImGuiSetCond_Once );
        ImGui::SetNextWindowSize( ImVec2( 500, 800 ), ImGuiSetCond_Once );

        // Plots window
        ImGui::Begin( "Plots", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);
        if(buffer.size() > 0)
        {
            // Table
            ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
            if(ImGui::TreeNode("Plot Data"))
            {
                ImGui::Columns(1);
                ImGui::Separator();

                ImGui::Text("Angles: ");
                ImGui::Separator();

                ImGui::Text("X");
                renderChartFromBuffer(buffer, 0);
                ImGui::Separator();
                ImGui::Text("Y");
                renderChartFromBuffer(buffer, 1);
                ImGui::Separator();
                ImGui::Text("Z");
                renderChartFromBuffer(buffer, 2);
                ImGui::Separator();
                ImGui::TreePop();
            }
        }
        ImGui::End();
    }

//    // clear IMGui window
//    ImGui::End();
//    ImGui::PopStyleColor();
//    ImGui::PopStyleColor();
    return true;
}

bool VGui::addPlotData(double data[3])
{
    std::map<char, float> angles;
    angles['X'] = data[0];
    angles['Y'] = data[1];
    angles['Z'] = data[2];

    dataForPlot.push_back(angles);

    if(dataForPlot.size() > 40)
    {
        dataForPlot.erase(dataForPlot.begin());
    }

    return true;
}

bool VGui::drawMenu(Arm* leftArm, Arm* rightArm ,bool* renderWithHand, bool* renderWithTrajectory, float * angleX, float * angleY, float * angleZ)
{
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiSetCond_Once);
    ImGui::SetNextWindowSize(ImVec2(280, 300), ImGuiSetCond_Once);

    // Begin a new window
    ImGui::Begin("Management", nullptr, ImGuiWindowFlags_MenuBar);

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Render"))
        {
            if (ImGui::MenuItem("Charts")) {
                showChartWindow = !showChartWindow;
            }

            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // choises: what elements we need to render
    if (ImGui::CollapsingHeader("Rendering"))
    {
        ImGui::Checkbox( "Renrer trajectory", renderWithTrajectory );
        ImGui::Checkbox( "Render infinite trajectory[under constr]", renderWithTrajectory );
        ImGui::Checkbox( "Renrer hand", renderWithHand );
        ImGui::Checkbox( "Renrer with trajectory chart", &showChartWindow );
        ImGui::Checkbox( "Renrer with data chart", &showDataChartWindow );
    }


    if(showChartWindow)
    {
      ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
        {
            ImGui::SetNextWindowPos( ImVec2( 10, 330 ), ImGuiSetCond_Once );
            ImGui::SetNextWindowSize( ImVec2( 500, 400 ), ImGuiSetCond_Once );

            // Plots window
            ImGui::Begin( "Plot Trajectory", nullptr, ImGuiWindowFlags_MenuBar );
            if ( ImGui::BeginMenuBar( ))
            {
                if ( ImGui::BeginMenu( "Plot" ))
                {
                    bool x = false;
                    ImGui::Checkbox( "Checkbox", &x );
                    ImGui::Checkbox( "Checkbox1", &x );

                    ImGui::EndMenu( );
                }
                ImGui::EndMenuBar( );
            }
            drawTrajectoryChart(leftArm->getTrajectoryAngles(), leftArm);
            ImGui::End();
        }

    }
    // Data chart
    if(showDataChartWindow)
    {
      ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
        {
            ImGui::SetNextWindowPos( ImVec2( 550, 330 ), ImGuiSetCond_Once );
            ImGui::SetNextWindowSize( ImVec2( 500, 400 ), ImGuiSetCond_Once );

            // Plots window
            ImGui::Begin( "Plot Data", nullptr, ImGuiWindowFlags_MenuBar );
            if(dataForPlot.size() > 0)
            {
            	drawDataChart( dataForPlot );
            }
            ImGui::End();
        }

    }

    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    return true;
}

bool VGui::drawDataChart(const std::vector<std::map<char, float>>& data)
{
    float min = -360, max = 360; // range of data plotting
    ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
    if(ImGui::TreeNode("Plot Data"))
    {
        ImGui::Columns(1);
        ImGui::Separator();

        ImGui::Text("Angles: ");
        ImGui::Separator();

        ImGui::Text("X");
        drawTrajectiiryByAngleName('X', data, min, max);
        ImGui::Separator();
        ImGui::Text("Y");
        drawTrajectiiryByAngleName('Y', data, min, max);
        ImGui::Separator();
        ImGui::Text("Z");
        drawTrajectiiryByAngleName('Z', data, min, max);
        ImGui::Separator();
        ImGui::TreePop();
        return true;
    }
    return false;
}

bool VGui::drawTrajectoryChart(const std::vector<std::map<char, float>>& trajectory, Arm* arm)
{
    ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
    if(ImGui::TreeNode("Plot Trajectory"))
    {
        ImGui::Columns(1);
        ImGui::Separator();

        ImGui::Text("Angles: ");
        ImGui::Separator();

        ImGui::Text("X");
        drawTrajectiiryByAngleName('X', trajectory, arm->minAngleX, arm->maxAngleX);
        ImGui::Separator();
        ImGui::Text("Y");
        drawTrajectiiryByAngleName('Y', trajectory, arm->minAngleY, arm->maxAngleY);
        ImGui::Separator();
        ImGui::Text("Z");
        drawTrajectiiryByAngleName('Z', trajectory, arm->minAngleZ, arm->maxAngleZ);
        ImGui::Separator();
        ImGui::TreePop();
        return true;
    }
    return false;
}

bool VGui::drawTrajectiiryByAngleName(char name ,const std::vector<std::map<char, float>>& trajectory, float min, float max)
{

    std::vector< float > angles;
    for ( int i = 0; i < trajectory.size( ); i++ )
    {
        std::map< char, float > angl = trajectory[ i ];
        angles.push_back( angl[ name ] );
    }

    float *arr = new float[angles.size( )];
    for ( int i = 0; i < angles.size( ); ++i )
    {
        arr[ i ] = angles[ i ];

    }

    drawChart( arr, angles.size( ), min, max);
    delete[] arr;

    return true;
}

bool VGui::drawChart(const float* data, size_t size, float min, float max)
{
    for ( int i = 0; i < size; ++i )
    {
        assert( sizeof( data ) / sizeof( float ) > 1 );
        chartArray[ i ] = data[ i ];
    }

    int count = size;
    ImGui::SliderInt( "Trajectory size", &count, 1, size );
    std::string s = "Plot\nmin:";
    s.append( std::to_string( roundf( min )));
    s.append( "\nMax:" );
    s.append( std::to_string( roundf( max )));

    ImGui::PlotLines( s.c_str( ), chartArray, count, 0, NULL, min, max, ImVec2( 0, 80 ));
    s = "Histogram\nmin:";
    s.append( std::to_string( roundf( min )));
    s.append( "\nMax:" );
    s.append( std::to_string( roundf( max )));
    ImGui::PlotHistogram( s.c_str( ), chartArray, count, 0, NULL, min, max,
                              ImVec2( 0, 80 ));

    return true;
}
