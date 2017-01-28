#ifndef GESTUS_HAND_NODE_H
#define GESTUS_HAND_NODE_H

struct GestusHandNode
{
		struct GestusHandNode *children = nullptr;
        float angleX = 0.f;
        float angleY = 0.f;
        float angleZ = 0.f;

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
