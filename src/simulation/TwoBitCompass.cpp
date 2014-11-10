#include "simulation/TwoBitCompass.hpp"
#include "utils/Math.hpp"

#define CARDINAL_DIRECTION_FOV (M_PI / 4) // 45°
#define EAST_ANGLE 0
#define NORTH_ANGLE (M_PI / 2) // 90 °
#define WEST_ANGLE (M_PI) // 180 °
#define SOUTH_ANGLE (-M_PI / 2) // -90°

namespace mae
{

	TwoBitCompass::TwoBitCompass()
		:model_(NULL)
	{
	}

	TwoBitCompass::~TwoBitCompass()
	{
	}

	void TwoBitCompass::connect(Stg::Model *p_model)
	{
		assert(model_ == NULL);

		model_ = p_model;
	}

	void TwoBitCompass::disconnect()
	{
		assert(model_ != NULL);

		model_ = NULL;
	}

	CardinalDirection TwoBitCompass::getFacingDirection() const
	{
		// TODO put some noise on yaw
		double yaw = model_->GetGlobalPose().a;
		
		if(angleIsBetween(yaw,
		                  EAST_ANGLE - (CARDINAL_DIRECTION_FOV / 2),
		                  EAST_ANGLE + (CARDINAL_DIRECTION_FOV / 2)))
			return EAST;
		else if(angleIsBetween(yaw,
		                  NORTH_ANGLE - (CARDINAL_DIRECTION_FOV / 2),
		                  NORTH_ANGLE + (CARDINAL_DIRECTION_FOV / 2)))
			return NORTH;
		else if(angleIsBetween(yaw,
		                  WEST_ANGLE - (CARDINAL_DIRECTION_FOV / 2),
		                  WEST_ANGLE + (CARDINAL_DIRECTION_FOV / 2)))
			return WEST;
		else
			return SOUTH;
	}


}
