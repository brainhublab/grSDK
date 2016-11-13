//
// Created by vtimofeev on 06/11/16.
//

#ifndef VISUALIZATION_GUIMETHODS_H
#define VISUALIZATION_GUIMETHODS_H

#include "visualization/Arm.h"
#include "visualization/ui/imgui_impl_sdl.h"

namespace gui
{
		static void drawChart(const float* data, size_t size, float min, float max)
		{

				{
						static float arr2[600];
						for ( int i = 0; i < size; ++i )
						{
								assert( sizeof( data ) / sizeof( float ) > 1 );
								arr2[ i ] = data[ i ];
						}
						static bool init = false;
						if ( !init )
						{
								init = true;
								for ( int i = 0; i < sizeof( arr2 ) / sizeof( float ); i++ )
										arr2[ i ] = ( i & 1 ) ? 1.0f : 0.0f;
						}
						int count = size;
						ImGui::SliderInt( "Trajectory size", &count, 1, size );
						std::string s = "Plot\nmin:";
						s.append( std::to_string( roundf( min )));
						s.append( "\nMax:" );
						s.append( std::to_string( roundf( max )));

						ImGui::PlotLines( s.c_str( ), arr2, count, 0, NULL, min, max, ImVec2( 0, 80 ));
						s = "Histogram\nmin:";
						s.append( std::to_string( roundf( min )));
						s.append( "\nMax:" );
						s.append( std::to_string( roundf( max )));
						ImGui::PlotHistogram( s.c_str( ), arr2, count, 0, std::to_string( max ).c_str( ), min, max,
																	ImVec2( 0, 80 ));
				}

		}
		static void drawChartByAngleName(char name ,const std::vector<std::map<char, float>>& trajectory, float min, float max)
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
		}
		static void drawChartWindow(const std::vector<std::map<char, float>>& trajectory, Arm* arm)
		{
				ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
				if(ImGui::TreeNode("Plot Trajectory"))
				{
						ImGui::Columns(1);
						ImGui::Separator();

						ImGui::Text("Angles: ");
						ImGui::Separator();

						ImGui::Text("X");
						drawChartByAngleName('X', trajectory, arm->minAngleX, arm->maxAngleX);
						ImGui::Separator();
						ImGui::Text("Y");
						drawChartByAngleName('Y', trajectory, arm->minAngleY, arm->maxAngleY);
						ImGui::Separator();
						ImGui::Text("Z");
						drawChartByAngleName('Z', trajectory, arm->minAngleZ, arm->maxAngleZ);
						ImGui::Separator();
						ImGui::TreePop();
				}
		}
		// Menu
		static void drawMenu(Arm* leftArm, Arm* rightArm ,bool* renderWithHand, bool* renderWithTrajectory, float * angleX, float * angleY, float * angleZ)
		{
				static bool w = true;
				//ImGui::ShowTestWindow(&w);

				ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
				ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiSetCond_Once);
				ImGui::SetNextWindowSize(ImVec2(280, 300), ImGuiSetCond_Once);

				static bool withChart = false;

				ImGui::Begin("Management", nullptr, ImGuiWindowFlags_MenuBar);

				if (ImGui::BeginMenuBar()) {
						if (ImGui::BeginMenu("Render"))
						{
								if (ImGui::MenuItem("Rendering")) {

								}
								if (ImGui::MenuItem("Charts")) {
										withChart = !withChart;
								}

								ImGui::EndMenu();
						}
						ImGui::EndMenuBar();
				}

				if (ImGui::CollapsingHeader("Rendering"))
				{
						ImGui::Checkbox( "Renrer trajectory", renderWithTrajectory );
						ImGui::Checkbox( "Render infinite trajectory[under constr]", renderWithTrajectory );
						ImGui::Checkbox( "Renrer hand", renderWithHand );
						ImGui::Checkbox( "Renrer with chart", &withChart );
				}

				static bool angleXchart = false, angleYchart = false, angleZchart = false;
				if(withChart)
				{
						ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
						if (ImGui::TreeNode("Chart"))
						{
								if (ImGui::TreeNode("Plot angles"))
								{
										ImGui::Checkbox( "AngleX", &angleXchart );
										ImGui::Checkbox( "AngleY", &angleYchart );
										ImGui::Checkbox( "AngleZ", &angleZchart );
										ImGui::TreePop( );
								}
								if (ImGui::TreeNode("Plot Arm"))
								{
										ImGui::Button("under constructionn");
										ImGui::TreePop( );
								}
								ImGui::TreePop();
						}

						{
								ImGui::SetNextWindowPos( ImVec2( 10, 330 ), ImGuiSetCond_Once );
								ImGui::SetNextWindowSize( ImVec2( 500, 400 ), ImGuiSetCond_Once );

								// Plots window
								ImGui::Begin( "Plots", nullptr, ImGuiWindowFlags_MenuBar );
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


								drawChartWindow(leftArm->getTrajectoryAngles(), leftArm);

								ImGui::End();
						}

				}
				ImGui::End();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
		}
// IMGUI end

}
#endif //SOME_GUIMETHODS_H
