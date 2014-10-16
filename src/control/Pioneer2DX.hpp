#ifndef MAE_PIONEER_2DX_hpp
#define MAE_PIONEER_2DX_hpp

#include "control/ExplorationBot.hpp"

namespace mae
{

	class Pioneer2DX : public ExplorationBot
	{
	private:
		RangerConfig config_;
		PlayerCc::RangerProxy ranger_;

		void initConfig();
	public:
		Pioneer2DX(PlayerClient *p_client,
		           Simulation *p_simulation,
		           const std::string &p_name,
		           const int p_motorIndex,
				   const int p_rangerIndex);
		~Pioneer2DX();

		RangerConfig getRangerConfig();
		int getRangerCount();
		double getRangerDistance(const int p_index);
		bool hasValidRangerData();
		
		Velocity getMaxVelocity() const;

	};

}

#endif
