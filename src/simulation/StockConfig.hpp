#ifndef MAE_STOCK_CONFIG_HPP
#define MAE_STOCK_CONFIG_HPP

#include <string>
#include "simulation/Simulation.hpp"

namespace mae
{

	class StockConfig
	{
	public:
		PlayerClient *client;
		Simulation *simulation;
		int graphicsIndex;
		std::string stockName;
		double markerRange;
		int refillCount;
		
		StockConfig();
		~StockConfig();
	};

}

#endif
