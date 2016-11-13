//
// Created by vtimofeev on 29/10/16.
//

#ifndef VISUALIZATION_NODE_H
#define VISUALIZATION_NODE_H

struct Node
{
		struct Node *children = nullptr;
		float angleX = 0.f;
		float angleY = 0.f;
		float angleZ = 0.f;

		float maxAngleX = 0, maxAngleY = 0, maxAngleZ = 0;
		float minAngleX = 0, minAngleY = 0, minAngleZ = 0;

		void setMaxAngles( float angleX, float angleY, float angleZ)
		{
				maxAngleX = angleX;
				maxAngleY = angleY;
				maxAngleZ = angleZ;
		}
		void setMinAngles( float angleX, float angleY, float angleZ)
		{
				minAngleX = angleX;
				minAngleY = angleY;
				minAngleZ = angleZ;
		}
};

#endif // NODE_H
