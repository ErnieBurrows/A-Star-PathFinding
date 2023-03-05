#pragma once
#include <glm/vec2.hpp>
#include <glm/common.hpp>

#include "util.h"


glm::vec2 Seek(glm::vec2 target, glm::vec2 seeker, glm::vec2 seekerVelocity, float maxVelocity)
{
	glm::vec2 toTarget = target - seeker;
	toTarget = Util::Normalize(toTarget);
	toTarget *= maxVelocity;

	return  toTarget - seekerVelocity;
}

glm::vec2 Arrive(glm::vec2 target, glm::vec2 seeker, glm::vec2 seekerVelocity, float maxVelocity)
{
	glm::vec2 toTarget = target - seeker;
	float distance = length(toTarget);
	int radius = 100;

	toTarget = Util::Normalize(toTarget);

	if (distance < radius)
	{
		toTarget *= maxVelocity * (distance / radius);
	}
	else
	{
		//Gives us just a direction of vector 
		toTarget *= maxVelocity;
	}


	return  toTarget - seekerVelocity;
}