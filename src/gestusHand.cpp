#include "gestusHand.h"


// helpers
bool isAllocated( GestusHandNode *ptr )
{
        if ( ptr == NULL )
        {
                printf( "Unable to allocate Node" );
                exit( 1 );
        }

        return true;
}

// class methods

GestusHand::GestusHand()
{
        // todo: get mins and maxes from config
        // GestusHand initializing
        this->setMaxAngles(90.f, 180.f, 90.f);
        this->setMinAngles(-120.f, -180.f, -120.f);

        // Hand initializing
		this->children = new GestusHandNode[1];
        isAllocated( this->children );

		struct GestusHandNode *hand = &(this->children[0]);

        hand->setMaxAngles(80.f, 0.f, 80.f);
        hand->setMinAngles(-80.f, 0.f, -40.f);

        // Fingers initializing
		hand->children = new struct GestusHandNode[5];
        isAllocated( hand->children );

        for ( int i = 0; i < 5; i++ )
        {
                // Add remaining phalanges for each finger
                float maxFingerAngleX = 85.f, minFingerAngleX = -5.f;

                // initializing of first phalange
				struct GestusHandNode *firstPhalange = &( *hand ).children[ i ];
                firstPhalange->setMaxAngles(maxFingerAngleX, 0.f, 0.f);
                firstPhalange->setMinAngles(minFingerAngleX, 0.f, 0.f);

                // initializing second phalange
				firstPhalange->children = new struct GestusHandNode[1];
                isAllocated( firstPhalange->children );
                firstPhalange->children->setMaxAngles(maxFingerAngleX, 0.f, 0.f);
                firstPhalange->children->setMinAngles(minFingerAngleX, 0.f, 0.f);

                if(i != 4)
                {
                    // initializing third phalange
					firstPhalange->children[ 0 ].children = new struct GestusHandNode[1];
                    isAllocated( firstPhalange->children[ 0 ].children );
                    firstPhalange->children[0].children->setMaxAngles(maxFingerAngleX, 0.f, 0.f);
                    firstPhalange->children[0].children->setMinAngles(minFingerAngleX, 0.f, 0.f);
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

GestusHand::~GestusHand()
{
        // todo: check correctness of this
		GestusHandNode *hand = &(this->children[0]);

        for ( int i = 0; i < 5; i++ )
        {
                delete [] (&hand->children[ i ])->children[0].children;
                delete [] (&hand->children[ i ])->children;
        }

        delete [] hand->children;
        delete [] hand;
}


bool GestusHand::bendArm( float angleX, float angleY, float angleZ )
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

bool GestusHand::bendHand( float angleX, float angleY, float angleZ )
{
		struct GestusHandNode *hand = &(this->children[0]);

        bool bended = bend(hand, angleX, angleY, angleZ);
        return bended;
}


bool GestusHand::bendFinger( int index, float angleX, float angleY, float angleZ )
{
		struct GestusHandNode *phalange = &this->children[ 0 ].children[ index ];
        if(!phalange)
        {
          printf("\nUnable to find phanlange with %d index for bending!", index);
          return false;
        };
        bool success = false;
        while ( phalange )
        {
                success = bend(phalange, angleX, angleY, angleZ);
                if (( *phalange ).children != NULL )
                {
                        phalange = &( *phalange ).children[ 0 ];
                }
                else
                {
                        phalange = NULL;
                }
        }
        return success;
}


bool GestusHand::bend( GestusHandNode *node, float angleX, float angleY, float angleZ )
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

// todo: maybe for every GestusHand separate motions?
// changed break; to return true;

const std::vector<std::map<char, float>>& GestusHand::getTrajectoryAngles() const
{
        return trajectoryAngles;
}
