#ifndef MAE_SELECT_ANT_TARGET_HPP
#define MAE_SELECT_ANT_TARGET_HPP

#include <vector>
#include "control/ExplorationBot.hpp"

namespace mae
{
	
	typedef struct {
		Marker *marker;
		double angleToTurn;
	} AntTarget;
	
	class SelectAntTarget
	{
	private:
		ExplorationBot &robot_;
		bool directionMarked_[4];
		std::vector<Marker*> markerInRange_;
		
		void getMarkerInRange(const std::vector<Marker*> &p_availableMarker);
	public:
		SelectAntTarget(ExplorationBot &p_robot);
		~SelectAntTarget();
		
		AntTarget execute(const std::vector<Marker*> &p_availableMarker);

	};

}

#endif
