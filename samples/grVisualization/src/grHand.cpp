#include "grHand.h"


// helpers
bool isAllocated( GRHandNode *ptr )
{
        if ( ptr == NULL )
        {
                printf( "Unable to allocate Node" );
                return false;
        }

        return true;
}

// class methods

GRHand::GRHand(bool left)
{
        // todo: get mins and maxes from config
        // GRHand initializing
        this->setMaxAngles(90.f, 180.f, 90.f);
        this->setMinAngles(-120.f, -180.f, -120.f);
        this->isLeft = left;

        // Hand initializing
		this->children = new GRHandNode[1];
        isAllocated( this->children );

		struct GRHandNode *hand = &(this->children[0]);
    hand->isLeft = left;
        hand->setMaxAngles(80.f, 0.f, 80.f);
        hand->setMinAngles(-80.f, 0.f, -40.f);

        // Fingers initializing
		hand->children = new struct GRHandNode[5];
        isAllocated( hand->children );

        for ( int i = 0; i < 5; i++ )
        {
                // Add remaining phalanges for each finger
                float maxFingerAngleX = 85.f, minFingerAngleX = -5.f;

                // initializing of first phalange
				struct GRHandNode *firstPhalange = &( *hand ).children[ i ];
                firstPhalange->setMaxAngles(maxFingerAngleX, 0.f, 0.f);
                firstPhalange->setMinAngles(minFingerAngleX, 0.f, 0.f);
                firstPhalange->isLeft = left;

                // initializing second phalange
				firstPhalange->children = new struct GRHandNode[1];
                isAllocated( firstPhalange->children );
                firstPhalange->children->setMaxAngles(maxFingerAngleX, 1.f, 0.f);
                firstPhalange->children->setMinAngles(minFingerAngleX, 1.f, 0.f);
                firstPhalange->children->isLeft = left;
                if(i != 4)
                {
                    // initializing third phalange
					firstPhalange->children[ 0 ].children = new struct GRHandNode[1];
                    isAllocated( firstPhalange->children[ 0 ].children );
                    firstPhalange->children[0].children->setMaxAngles(maxFingerAngleX, 1.f, 0.f);
                    firstPhalange->children[0].children->setMinAngles(minFingerAngleX, 1.f, 0.f);
                    firstPhalange->children[0].children->isLeft = left;
                }
        }



        int parameter = isLeft ? -1 : 1;

        // initial hand props
		bendArm(-60.f, 0.f, parameter*10.f);
		//bendHand( 10.f, 130.f, parameter*10.f );
		bendHand(0.f, 0.f, 180.f);

		bendFinger( 0, 10.f, 0.f, 0.f );
        bendFinger( 1, 10.f, 0.f, 0.f );
        bendFinger( 2, 10.f, 0.f, 0.f );
        bendFinger( 3, 10.f, 0.f, 0.f );
        bendFinger( 4, 20.f, 20.f, parameter*10.f );
        // TODO: libconfig

        //

        bendFirstPhalange( 0, 10.f, 0.f, parameter*10.f );
        bendFirstPhalange( 1, 10.f, 0.f, parameter*5.f );
        bendFirstPhalange( 2, 10.f, 0.f, parameter*0.f );
        bendFirstPhalange( 3, 10.f, 0.f, parameter*-10.f );

}

GRHand::~GRHand()
{
        // todo: check correctness of this
		GRHandNode *hand = &(this->children[0]);

        for ( int i = 0; i < 5; i++ )
        {
                delete [] (&hand->children[ i ])->children[0].children;
                delete [] (&hand->children[ i ])->children;
        }

        delete [] hand->children;
        delete [] hand;
}


bool GRHand::bendArm( float angleX, float angleY, float angleZ )
{
        if(bend(this, angleX, angleY, angleZ))
        {
                // add info about trajectory
                std::map< char, float > angles;
                std::map< char, float > lastPoint = this->trajectoryAngles.back( );
                angles[ 'X' ] = angleX;// + lastPoint[ 'X' ];
                angles[ 'Y' ] = angleY;// + lastPoint[ 'Y' ];
                angles[ 'Z' ] = angleZ;// + lastPoint[ 'Z' ];

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

bool GRHand::bendHand( float angleX, float angleY, float angleZ )
{
		struct GRHandNode *hand = &(this->children[0]);

        bool bended = bend(hand, angleX, angleY, angleZ);
        return bended;
}

bool GRHand::bendFirstPhalange(int index, float angleX, float angleY, float angleZ )
{
  struct GRHandNode *phalange = &this->children[ 0 ].children[ index ];
      if(!phalange)
      {
        printf("\nUnable to find phanlange with %d index for bending!", index);
        return false;
      };
      bool success = false;

      success = bend(phalange, angleX, angleY, angleZ);

      return success;
}

bool GRHand::bendFinger( int index, float angleX, float angleY, float angleZ )
{
		struct GRHandNode *phalange = &this->children[ 0 ].children[ index ];
        if(!phalange)
        {
          printf("\nUnable to find phanlange with %d index for bending!", index);
          return false;
        };
        bool success = false;

        while ( phalange )
        {
                success = bend(phalange, angleX, angleY, angleZ);
                // angleY = 0.f;
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


bool GRHand::bendHandWithMatrix(GLfloat mat[16])
{
		struct GRHandNode *hand = &(this->children[0]);
		for(int i = 0; i < 16; ++i)
		{
				hand->matrix[i] = mat[i];
		};

		return true;
};
bool GRHand::bendFingerWithMatrix( int index, GLfloat mat[16] )
{
		struct GRHandNode *phalange = &this->children[ 0 ].children[ index ];

		struct GRHandNode *hand = &(this->children[0]);
		if(!phalange)
		{
		  printf("\nUnable to find phanlange with %d index for bending!", index);
		  return false;
		};
		bool success = false;

		while ( phalange )
		{
				//success = bend(phalange, angleX, angleY, angleZ);
			for(int i = 0; i < 16; ++i)
			{
					phalange->matrix[i] = mat[i];
			};
				// angleY = 0.f;
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

bool GRHand::bend( GRHandNode *node, float angleX, float angleY, float angleZ )
{
    /*
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
*/

        // todo: if you want to move on new angle every time (without adding last position) change += to =
        node->angleX = angleX;
        node->angleY = angleY;
        node->angleZ = angleZ;

        return true;
}

// todo: maybe for every GRHand separate motions?
// changed break; to return true;

const std::vector<std::map<char, float>>& GRHand::getTrajectoryAngles() const
{
        return trajectoryAngles;
}
