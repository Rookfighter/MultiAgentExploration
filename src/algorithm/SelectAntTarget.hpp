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
		std::vector<Marker*> markerInRange_;
		AntTarget target_; 
		
		void getMarkerInRange(Marker *p_currentMarker,
		                      const std::vector<Marker*> &p_availableMarker);
		bool checkBlankSpace();
		void getMarkerTarget();
	public:
		SelectAntTarget(ExplorationBot &p_robot);
		~SelectAntTarget();

		AntTarget execute(Marker *p_currentMarker,
		                  const std::vector<Marker*> &p_availableMarker);

	};

}

#endif
