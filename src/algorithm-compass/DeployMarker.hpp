#ifndef MAE_DEPLOY_MARKER_HPP
#define MAE_DEPLOY_MARKER_HPP

#include "algorithm-compass/CompassState.hpp"

namespace mae
{
	
	class DeployMarker : public CompassState
	{
	public:
		DeployMarker(const CompassStateProperties &p_properties);
		~DeployMarker();
		
		CompassState* update();

	};

}

#endif
