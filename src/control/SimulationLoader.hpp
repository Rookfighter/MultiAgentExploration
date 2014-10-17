#ifndef MAE_SIMULATION_LOADER_HPP
#define MAE_SIMULATION_LOADER_HPP

#include "control/ExplorationBot.hpp"
#include "control/MarkerStock.hpp"

namespace mae
{

	class SimulationLoader
	{
	private:
		PlayerClient *client_;
		Simulation *simulation_;
		MarkerStock *stock_;
		std::vector<ExplorationBot*> robots_;
		
		std::string host_;
		int port_;
		SimulationConfig simulationConfig_;
		StockConfig stockConfig_;
		std::vector<RobotConfig> robotConfigs_;
		
		void cleanup();
	public:
		SimulationLoader();
		~SimulationLoader();
		
		void load(const std::string& p_file);
		
		PlayerClient* getClient();
		Simulation* getSimulation();
		MarkerStock* getStock();
		
		const std::vector<ExplorationBot*>& getRobots();
		ExplorationBot* getRobot(const std::string &p_name);
	};

}

#endif
