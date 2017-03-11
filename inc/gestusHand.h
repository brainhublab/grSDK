
#ifndef GESTUS_HAND_H
#define GESTUS_HAND_H

#include <assert.h>
#include <vector>
#include <map>

#include "gestusHandNode.h"
#include "dimensions.h"

class GestusHand : public GestusHandNode
{
public:
        explicit GestusHand(bool left);
        ~GestusHand();

        // bending
        bool bendArm( float angleX, float angleY, float angleZ );
        bool bendHand( float angleX, float angleY, float angleZ );
        bool bendFinger( int index, float angleX, float angleY, float angleZ );
        bool bendFirstPhalange(int index, float angleX, float angleY, float angleZ);



        const std::vector<std::map<char, float>> & getTrajectoryAngles() const;
private:
        // returns true if node is bended
        bool bend(GestusHandNode* node, float angleX, float angleY, float angleZ);

        // data for trajectory render
        std::vector<std::map<char, float> > trajectoryAngles
                        = { {{'X', angleX}, {'Y', angleY}, {'Z', angleZ}} };
};


#endif // GESTUS_HAND_H
