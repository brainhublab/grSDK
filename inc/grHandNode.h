#ifndef GR_HAND_NODE_H
#define GR_HAND_NODE_H

#include <GL/gl.h>

struct GRHandNode
{
				explicit GRHandNode(bool left = true)
				{
					isLeft = left;
				}

				struct GRHandNode *children = nullptr;
				bool isLeft;
        float angleX = 0.f;
        float angleY = 0.f;
        float angleZ = 0.f;


        GLfloat matrix[16];

        float maxAngleX = 0, maxAngleY = 0, maxAngleZ = 0;
        float minAngleX = 0, minAngleY = 0, minAngleZ = 0;

        bool setMaxAngles( float angleX, float angleY, float angleZ)
        {
                maxAngleX = angleX;
                maxAngleY = angleY;
                maxAngleZ = angleZ;
                return true;
        }
        bool setMinAngles( float angleX, float angleY, float angleZ)
        {
                minAngleX = angleX;
                minAngleY = angleY;
                minAngleZ = angleZ;
                return true;
        }
};

#endif // NODE_H
