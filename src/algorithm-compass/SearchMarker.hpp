#ifndef MAE_SEARCH_MARKER_HPP
#define MAE_SEARCH_MARKER_HPP

#include "algorithm/MovementController.hpp"
#include "algorithm-compass/CompassState.hpp"

namespace mae
{

	class SearchMarker : public CompassState
	{
	private:
        MovementController movementController_;
        Marker *newMarker_;
        bool lostAllSignals_;
        
        void updateNewMarker();
	public:
		SearchMarker(const CompassStateProperties &p_properties);
		~SearchMarker();
		
		CompassState* update();
	};

}

#endif
