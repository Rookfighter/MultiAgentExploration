#ifndef MAE_BASIC_TEST_HPP
#define MAE_BASIC_TEST_HPP

#include "simulation/MarkerStock.hpp"
#include "simulation/ExplorationBot.hpp"

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
		void printRangerProperties(RangerProperties &p_properties);
	public:
		BasicTest(PlayerClient &p_client,
		          Simulation &p_simulation,
		          MarkerStock &p_stock,
		          ExplorationBot &p_bot);
		~BasicTest();
		
		void execute();
	};

}

#endif
