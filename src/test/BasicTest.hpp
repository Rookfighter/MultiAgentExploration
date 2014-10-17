#ifndef MAE_BASIC_TEST_HPP
#define MAE_BASIC_TEST_HPP

#include "control/MarkerStock.hpp"
#include "control/ExplorationBot.hpp"

namespace mae
{

	class BasicTest
	{
	private:
		PlayerClient &client_;
		Simulation &simulation_;
		MarkerStock &stock_;
		ExplorationBot &bot_;

		void updateClient();
		void printAllMarker();
		void refreshStock();
		void printRobot(ExplorationBot &p_bot);
		void printRangerConfig(RangerConfig &p_config);
	public:
		BasicTest(PlayerClient &p_client,
		          Simulation &p_simulation,
		          MarkerStock &p_stock,
		          ExplorationBot &p_bot);
		~BasicTest();
		
		void step();
	};

}

#endif
