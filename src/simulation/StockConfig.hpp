#ifndef MAE_STOCK_CONFIG_HPP
#define MAE_STOCK_CONFIG_HPP

#include <string>
#include "simulation/Simulation.hpp"

namespace mae
{

	class StockConfig
	{
	public:
		Stg::World *world;
		
		std::string stockName;
		int refillCount;
		
		StockConfig() { }
		~StockConfig() { }
	};

}

#endif
