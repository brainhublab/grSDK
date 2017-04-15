
#ifndef GR_HAND_H
#define GR_HAND_H

#include <assert.h>
#include <vector>
#include <map>

#include "grHandNode.h"
#include "renderConfig.h"

class GRHand : public GRHandNode
{
public:
        explicit GRHand(bool left);
        ~GRHand();

        // 
        bool bendHandWithMatrix(GLfloat mat[16])
        {
				struct GRHandNode *hand = &(this->children[0]);
                for(int i = 0; i < 16; ++i)
                {
                        hand->matrix[i] = mat[i];
                };

                return true;
        };
		bool bendFingerWithMatrix( int index, GLfloat mat[16] )
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
        // bending
        bool bendArm( float angleX, float angleY, float angleZ );
        bool bendHand( float angleX, float angleY, float angleZ );
        bool bendFinger( int index, float angleX, float angleY, float angleZ );
        bool bendFirstPhalange(int index, float angleX, float angleY, float angleZ);



        const std::vector<std::map<char, float>> & getTrajectoryAngles() const;
private:
        // returns true if node is bended
        bool bend(GRHandNode* node, float angleX, float angleY, float angleZ);

        // data for trajectory render
        std::vector<std::map<char, float> > trajectoryAngles
                        = { {{'X', angleX}, {'Y', angleY}, {'Z', angleZ}} };
};


#endif // GR_HAND_H
