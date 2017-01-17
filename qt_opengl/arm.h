
#ifndef ARM_H
#define ARM_H

#include <assert.h>
#include <vector>
#include <map>

#include "node.h"
#include "dimensions.h"

class Arm : public Node
{
public:
        Arm();
        ~Arm();

        // bending
        bool bendArm( float angleX, float angleY, float angleZ );
        bool bendHand( float angleX, float angleY, float angleZ );
        bool bendFinger( int index, float angleX, float angleY, float angleZ );



        const std::vector<std::map<char, float>> & getTrajectoryAngles() const;
private:
        // returns true if node is bended
        bool bend(Node* node, float angleX, float angleY, float angleZ);

        // data for trajectory render
        std::vector<std::map<char, float> > trajectoryAngles
                        = { {{'X', angleX}, {'Y', angleY}, {'Z', angleZ}} };
};


#endif // ARM_H
