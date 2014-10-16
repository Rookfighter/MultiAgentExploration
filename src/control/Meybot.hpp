#ifndef MAE_MEYBOT_HPP
#define MAE_MEYBOT_HPP

#include "control/ExplorationBot.hpp"

namespace mae
{

	class Meybot : public ExplorationBot
	{
	private:
		RangerConfig config_;
		
		void initConfig();
	protected:
		PlayerCc::RangerProxy ranger_;
	public:
		const static int FRONT = 0;
		const static int LEFT = 1;
		const static int RIGHT = 2;
	
		Meybot(PlayerClient *p_client,
		       Simulation *p_simulation,
		       const std::string &p_name,
		       const int p_motorIndex,
			   const int p_rangerIndex);
		~Meybot();

		RangerConfig getRangerConfig();
		int getRangerCount();
		double getRangerDistance(const int p_index);
		bool hasValidRangerData();
		
		Velocity getMaxVelocity() const;
	};

}

#endif
