#pragma once
#include "Entity.hpp"

#include "SFX.hpp"

class Actor : public Entity {
public:
	Actor(Map* m);

protected:
	void doPhysics() override;

	SFX sndLand;
	SFX sndLandHard;
};