#include "simulation/TwoBitCompass.hpp"
#include "utils/Math.hpp"

#define CARDINAL_DIRECTION_FOV (M_PI / 4) // 45°
#define EAST_ANGLE 0
#define NORTH_ANGLE (M_PI / 2) // 90 °
#define WEST_ANGLE (M_PI) // 180 °
#define SOUTH_ANGLE (-M_PI / 2) // -90°

namespace mae
{
    static int getNorthDiffTo(const CardinalDirection p_dir)
    {
        switch(p_dir) {
            case NORTH:
                return 0;
            case EAST:
                return 1;
            case WEST:
                return -1;
            default:
                return 2;
        }
    }
    
    static int getSouthDiffTo(const CardinalDirection p_dir)
    {
        switch(p_dir) {
            case NORTH:
                return 2;
            case EAST:
                return -1;
            case WEST:
                return 1;
            default:
                return 0;
        }
    }
    
    static int getEastDiffTo(const CardinalDirection p_dir)
    {
        switch(p_dir) {
            case NORTH:
                return -1;
            case EAST:
                return 0;
            case WEST:
                return 2;
            default:
                return 1;
        }
    }
    
    static int getWestDiffTo(const CardinalDirection p_dir)
    {
        switch(p_dir) {
            case NORTH:
                return 1;
            case EAST:
                return 2;
            case WEST:
                return 0;
            default:
                return -1;
        }
    }
    
    int getDirectionDiff(const CardinalDirection p_dir1, const CardinalDirection p_dir2)
    {
        switch(p_dir1) {
        case NORTH:
            return getNorthDiffTo(p_dir2);
        case EAST:
            return getEastDiffTo(p_dir2);
        case WEST:
            return getWestDiffTo(p_dir2);
        default:
            return getSouthDiffTo(p_dir2);
        }
    }
    
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
