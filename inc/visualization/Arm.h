//
// Created by vtimofeev on 29/10/16.
//

#ifndef VISUALIZATION_ARM_H
#define VISUALIZATION_ARM_H

#include <SDL2/SDL.h>
#include <vector>
#include <map>

#include "visualization/Node.h"
#include "visualization/Dimensions.h"

class Arm : public Node
{
public:
		Arm();
		~Arm();

		// bending
		void bendArm( float angleX, float angleY, float angleZ );
		void bendHand( float angleX, float angleY, float angleZ );
		void bendFinger( int index, float angleX, float angleY, float angleZ );

		void handleInput( SDL_Keysym keysym );

		const std::vector<std::map<char, float>> & getTrajectoryAngles() const;
private:
		// returns true if node is bended
		bool bend(Node* node, float angleX, float angleY, float angleZ);

		// data for trajectory render
		std::vector<std::map<char, float> > trajectoryAngles
						= { {{'X', angleX}, {'Y', angleY}, {'Z', angleZ}} };
};


#endif // ARM_H
