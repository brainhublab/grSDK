//#include <SDL2/SDL.h>
//#include <GL/glut.h>
//#include <cstdlib>
//#include <SDL2/SDL_opengl.h>
//#include <stdio.h>
//#include <string>

#include "gestusVisualization.h"

using namespace std;

Visualization::Visualization(std::string& id1, std::string& buffer1, std::string& id2, std::string& buffer2) : _firstHandId(id1), _firstBuffer(buffer1), _secondHandId(id2), _secondBuffer(buffer2)
{
		// initialize sdl
		if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
		{
				printf( "SDL cannot init: %s\n", SDL_GetError( ));
				exit(EXIT_FAILURE);
		}

		// set opengl version to use in this program
		setOpenGLVersion( );

		// create window
		window = SDL_CreateWindow( "Opengl", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
															 SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
		if ( window == NULL )
		{
				printf( "Cannot create window: %s\n", SDL_GetError( ));
				exit(EXIT_FAILURE);
		}

		// ImGUI initialization
		ImGui_ImplSdl_Init(window);


		// create opengl context
		glContext = SDL_GL_CreateContext( window );
		if ( glContext == NULL )
		{
				printf( "Cannot create OpenGL: %s\n", SDL_GetError( ));
				exit(EXIT_FAILURE);
		}
}

Visualization::~Visualization()
{
		close();
}




void Visualization::run()
{
		Renderer renderer;
		SDL_Event event;
		while ( !quit )
		{
				while ( SDL_PollEvent( &event ) != 0 )
				{
						// handle events for gui
						ImGui_ImplSdl_ProcessEvent(&event);

						if ( event.type == SDL_QUIT )
						{
								quit = true;
						}
						else if ( event.type == SDL_WINDOWEVENT )
						{
								if ( event.window.event == SDL_WINDOWEVENT_RESIZED )
								{
										renderer.setViewport( event.window.data1, event.window.data2 );
								}
						}
						else if ( event.type == SDL_KEYDOWN )
						{
								switch ( event.key.keysym.sym )
								{
										case SDLK_v:
												if ( event.key.keysym.mod & KMOD_SHIFT)
														renderer.setViewport( renderer.getWidth(), renderer.getHeight() );
												else
														renderer.setViewport( SCREEN_WIDTH * 2 / 3, renderer.getHeight() );
												break;
										case SDLK_UP:
												angleX += angleStep;
												break;
										case SDLK_DOWN:
												angleX -= angleStep;
												break;
										case SDLK_LEFT:
												angleY -= angleStep;
												break;
										case SDLK_RIGHT:
												angleY += angleStep;
												break;
										default:
												break;
								}

								if(angleX >= 360)
								{
										angleX = angleX - 360;
								}
								else if(angleX <= -360)
								{
										angleX = angleX + 360;
								}

								if(angleY >= 360)
								{
										angleY = angleY - 360;
								}
								else if(angleY <= -360)
								{
										angleY = angleY + 360;
								}
								if(angleZ >= 360)
								{
										angleZ = angleZ - 360;
								}
								else if(angleZ <= -360)
								{
										angleZ = angleZ + 360;
								}


								// todo: adapt to buffer
								// every frame pop from buffer data and apply to arm
								// if buffer is empty - do nothing (this will save prev state)
								leftArm.handleInput( event.key.keysym );
								rightArm.handleInput( event.key.keysym );
						}
				}


				ImGui_ImplSdl_NewFrame(window);
				// render gui stuff here

				gui::drawMenu( &leftArm, &rightArm, &renderWithHand, &renderWithTrajectory, &angleX, &angleY, &angleZ);

				// clear bg color and depth buffer
				glClearColor( 0.0f, 0.5f, 0.5f, 0.0f );
				glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


				// render stuff here
				renderer.drawScene( angleX, angleY, angleZ );


				if ( renderWithHand )
				{
						renderer.renderArm( &rightArm, angleX, angleY, angleZ, false );
						renderer.renderArm( &leftArm, angleX, angleY, angleZ, true );
				}
				if ( renderWithTrajectory )
				{
						renderer.renderTrajectory( &rightArm, angleX, angleY, angleZ, false );
						renderer.renderTrajectory( &leftArm, angleX, angleY, angleZ, true );
				}

				ImGui::Render();

				// swap window inorder to update openGL
				SDL_GL_SwapWindow( window );
		}
		close();
}


void Visualization::stop()
{
		quit = true;
}

// helpers
void Visualization::setOpenGLVersion()
{
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
}

void Visualization::close()
{
		ImGui_ImplSdl_Shutdown();
		SDL_DestroyWindow( window );
		window = NULL;

		SDL_Quit( );
}
