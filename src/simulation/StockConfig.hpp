#ifndef MAE_STOCK_CONFIG_HPP
#define MAE_STOCK_CONFIG_HPP

#include <string>
#include "simulation/Simulation.hpp"

namespace mae
{

	class StockConfig
	{
	public:
		Simulation *simulation;
		std::string markerName;
		int markerCount;
		double markerRange;
		
		StockConfig();
		~StockConfig();
	};

}

#endif
