//
// Created by vtimofeev on 29/10/16.
//

#include "arm.h"

// helpers
bool isAllocated( Node *ptr )
{
		if ( ptr == NULL )
		{
				printf( "Unable to allocate Node" );
				exit( 1 );
		}

		return true;
}

// class methods

Arm::Arm()
{
		// todo: get mins and maxes from config
		// Arm initializing
		this->setMaxAngles(90.f, 180.f, 90.f);
		this->setMinAngles(-120.f, -180.f, -120.f);

		// Hand initializing
		this->children = new Node[1];
		isAllocated( this->children );

		struct Node *hand = &(this->children[0]);

		hand->setMaxAngles(80.f, 0.f, 80.f);
		hand->setMinAngles(-80.f, 0.f, -40.f);

		// Fingers initializing
		hand->children = new struct Node[5];
		isAllocated( hand->children );

		for ( int i = 0; i < 5; i++ )
		{
				// Add remaining phalanges for each finger
				float maxFingerAngleX = 85.f, minFingerAngleX = -5.f;

				// initializing of first phalange
				struct Node *firstPhalange = &( *hand ).children[ i ];
				firstPhalange->setMaxAngles(85.f, 0.f, 0.f);
				firstPhalange->setMinAngles(-5.f, 0.f, 0.f);

				// initializing second phalange
				firstPhalange->children = new struct Node[1];
				isAllocated( firstPhalange->children );
				firstPhalange->children->setMaxAngles(85.f, 0.f, 0.f);
				firstPhalange->children->setMinAngles(-5.f, 0.f, 0.f);

				if(i != 4)
				{
					// initializing third phalange
					firstPhalange->children[ 0 ].children = new struct Node[1];
					isAllocated( firstPhalange->children[ 0 ].children );
					firstPhalange->children[0].children->setMaxAngles(85.f, 0.f, 0.f);
					firstPhalange->children[0].children->setMinAngles(-5.f, 0.f, 0.f);
				}
		}

		// initial hand props
		bendHand( 35.f, 0.f, 0.f );
		bendFinger( 0, 10.f, 0.f, 0.f );
		bendFinger( 1, 55.f, 0.f, 0.f );
		bendFinger( 2, 55.f, 0.f, 0.f );
		bendFinger( 3, 10.f, 0.f, 0.f );
		bendFinger( 4, 15.f, 0.f, 0.f );
}

Arm::~Arm()
{
		// todo: check correctness of this
		Node *hand = &(this->children[0]);

		for ( int i = 0; i < 5; i++ )
		{
				delete [] (&hand->children[ i ])->children[0].children;
				delete [] (&hand->children[ i ])->children;
		}

		delete [] hand->children;
		delete hand;
}


bool Arm::bendArm( float angleX, float angleY, float angleZ )
{
		if(bend(this, angleX, angleY, angleZ))
		{
				// add info about trajectory
				std::map< char, float > angles;
				std::map< char, float > lastPoint = this->trajectoryAngles.back( );
				angles[ 'X' ] = angleX + lastPoint[ 'X' ];
				angles[ 'Y' ] = angleY + lastPoint[ 'Y' ];
				angles[ 'Z' ] = angleZ + lastPoint[ 'Z' ];

				this->trajectoryAngles.push_back( angles );

				if ( this->trajectoryAngles.size( ) > TRAJECTORY_LENGTH )
				{
						// clean last angle
						this->trajectoryAngles.erase( this->trajectoryAngles.begin( ));
				}
				return true;
		}

		return false;
}

bool Arm::bendHand( float angleX, float angleY, float angleZ )
{
		struct Node *hand = &(this->children[0]);

		if(bend(hand, angleX, angleY, angleZ))
		{
			return true;
		}
		else
		{
			return false;
		}
}


bool Arm::bendFinger( int index, float angleX, float angleY, float angleZ )
{
		struct Node *phalange = &this->children[ 0 ].children[ index ];
		assert(phalange != nullptr);

		while ( phalange )
		{
				bend(phalange, angleX, angleY, angleZ);
				if (( *phalange ).children != NULL )
				{
						phalange = &( *phalange ).children[ 0 ];
				}
				else
				{
						phalange = NULL;
				}
		}
		return true;
}


bool Arm::bend( Node *node, float angleX, float angleY, float angleZ )
{
		// check if bending is possible
		if(node->maxAngleX != 0.f &&
			 (node->angleX + angleX >= node->maxAngleX
				|| node->angleX + angleX <= node->minAngleX))
		{
				return false;
		}

		if(node->maxAngleY != 0.f &&
			 (node->angleY + angleY >= node->maxAngleY
			 || node->angleY + angleY <= node->minAngleY))
		{
				return false;
		}

		if(node->maxAngleZ != 0.f &&
			 (node->angleZ + angleZ >= node->maxAngleZ
			 || node->angleZ + angleZ <= node->minAngleZ))
		{
				return false;
		}


		// todo: if you want to move on new angle every time (without adding last position) change += to =
		node->angleX += angleX;
		node->angleY += angleY;
		node->angleZ += angleZ;

		return true;
}

// todo: maybe for every arm separate motions?
// changed break; to return true;
bool Arm::handleInput( SDL_Keysym keysym )
{
		switch ( keysym.sym )
		{
				/** handle bending keys **/
				case SDLK_1:
						if ( keysym.mod & KMOD_SHIFT )
						{
								bendFinger(0, -5.f, 0.f, 0.f );
						}
						else
						{
								bendFinger(0, 5.f, 0.f, 0.f );
						}
						return true;
				case SDLK_2:
						if ( keysym.mod & KMOD_SHIFT )
						{
								bendFinger(1, -5.f, 0.f, 0.f );
						}
						else
						{
								bendFinger( 1, 5.f, 0.f, 0.f );
						}
						return true;
				case SDLK_3:
						if ( keysym.mod & KMOD_SHIFT )
						{
								bendFinger( 2, -5.f, 0.f, 0.f );
						}
						else
						{
								bendFinger( 2, 5.f, 0.f, 0.f );
						}
						return true;
				case SDLK_4:
						if ( keysym.mod & KMOD_SHIFT )
						{
								bendFinger( 3, -5.f, 0.f, 0.f );
						}
						else
						{
								bendFinger( 3, 5.f, 0.f, 0.f );
						}
						return true;
				case SDLK_5:
						if ( keysym.mod & KMOD_SHIFT )
						{
								bendFinger( 4, -5.f, 0.f, 0.f );
						}
						else
						{
								bendFinger( 4, 5.f, 0.f, 0.f );
						}
						return true;
				case SDLK_6:
						if ( keysym.mod & KMOD_SHIFT )
						{
								bendHand( -5.f, 0.f, 0.f );
						}
						else
						{
								bendHand( 5.f, 0.f, 0.f );
						}
						return true;

				case SDLK_0:
						if ( keysym.mod & KMOD_SHIFT )
						{
								bendHand( 0.f, -5.f, 0.f );
						}
						else
						{
								bendHand( 0.f, 5.f, 0.f );
						}
						return true;

				case SDLK_7:
						if ( keysym.mod & KMOD_SHIFT )
						{
								bendArm( -5.f, 0.f, 0.f );
						}
						else
						{
								bendArm( 5.f, 0.f, 0.f );
						}
						return true;
				case SDLK_8:
						if ( keysym.mod & KMOD_SHIFT )
						{
								bendArm( 0.f, -5.f, 0.f );
						}
						else
						{
								bendArm( 0.f, 5.f, 0.f );
						}
						return true;
				case SDLK_9:
						if ( keysym.mod & KMOD_SHIFT )
						{
								bendArm( 0.f, 0.f, -5.f );
						}
						else
						{
								bendArm( 0.f, 0.f, 5.f );
						}
						return true;
				default:
						return false;
		}
}

const std::vector<std::map<char, float>>& Arm::getTrajectoryAngles() const
{
		return trajectoryAngles;
}
