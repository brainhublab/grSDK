#include <sstream>
#include "visualGui.h"

VisualGui::VisualGui()
{
		for ( int i = 0; i < sizeof( chartArray ) / sizeof( float ); i++ )
    {
			chartArray[ i ] = ( i & 1 ) ? 1.0f : 0.0f;
		}

}

VisualGui::~VisualGui()
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

bool VisualGui::renderChartFromBuffer(std::string name, const std::deque< std::string > &buffer, size_t axis)
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

void setPlotColors(int i)
{

    const ImVec4 pink{1.000, 0.753, 0.796, 1.f};
    const ImVec4 pink_hovered{1.000, 0.412, 0.706, 1.f};
    const ImVec4 green{0.000, 0.980, 0.604, 1.f};
    const ImVec4 green_hovered{0.196, 0.804, 0.196, 1.f};


    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, i%2 == 0 ? pink : green);
    ImGui::PushStyleColor(ImGuiCol_PlotLines, i%2 == 0 ? pink : green);
    ImGui::PushStyleColor(ImGuiCol_PlotHistogramHovered, i%2 == 0 ? pink_hovered : green_hovered);
    ImGui::PushStyleColor(ImGuiCol_PlotLinesHovered, i%2 == 0 ? green_hovered : pink_hovered);
}

void popPlotColors()
{
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
}


bool VisualGui::plotData(std::map< std::string, std::deque<std::string>*>& buffers )
{

    int i = 0;
    for ( auto buffer : buffers )
    {
        setPlotColors(i);


        ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.902, 0.902, 0.980, .3f));
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.847, 0.749, 0.847, 0.3f));

        // Plot window
        ImGui::SetNextWindowPos( ImVec2( 200 * i, 0 ), ImGuiSetCond_Once );
        ImGui::SetNextWindowSize( ImVec2( 500, 800 ), ImGuiSetCond_Once );
        i++;

        ImGui::Begin( buffer.first.c_str( ), nullptr );
        if ( buffer.second->size() > 0 )
        {
            // Table
            ImGui::SetNextTreeNodeOpen( true, ImGuiSetCond_Once );
            if ( ImGui::TreeNode( "Plot Data" ))
            {
                ImGui::Columns( 1 );
                ImGui::Separator( );

                ImGui::Text( "Angles: " );
                ImGui::Separator( );

                ImGui::Text( "X" );
                renderChartFromBuffer( buffer.first, *buffer.second, 0 );
                ImGui::Separator( );
                ImGui::Text( "Y" );
                renderChartFromBuffer( buffer.first, *buffer.second, 1 );
                ImGui::Separator( );
                ImGui::Text( "Z" );
                renderChartFromBuffer( buffer.first, *buffer.second, 2 );
                ImGui::Separator( );
                ImGui::TreePop( );
            }
        }
        ImGui::End( );


        ImGui::PopStyleColor();
        ImGui::PopStyleColor();

        popPlotColors();
    }

//    // clear IMGui window
//    ImGui::End();
//    ImGui::PopStyleColor();
//    ImGui::PopStyleColor();
    return true;
}

bool VisualGui::addPlotData(double data[3])
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

bool VisualGui::drawMenu(Arm* leftArm, Arm* rightArm ,bool* renderWithHand, bool* renderWithTrajectory, float * angleX, float * angleY, float * angleZ)
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

bool VisualGui::drawDataChart(const std::vector<std::map<char, float>>& data)
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

bool VisualGui::drawTrajectoryChart(const std::vector<std::map<char, float>>& trajectory, Arm* arm)
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

bool VisualGui::drawTrajectiiryByAngleName(char name ,const std::vector<std::map<char, float>>& trajectory, float min, float max)
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

bool VisualGui::drawChart(const float* data, size_t size, float min, float max)
{
    for ( int i = 0; i < size; ++i )
    {
        assert( sizeof( data ) / sizeof( float ) > 1 );
        chartArray[ i ] = data[ i ];
    }

    int count = size;
    ImGui::SliderInt( "Data size", &count, 1, size );
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
