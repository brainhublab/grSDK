
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

        // benging with matrix
		bool bendHandWithMatrix(GLfloat mat[16]);
        bool bendFingerWithMatrix( int index, GLfloat mat[16] );

        // bending with euler angles
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
