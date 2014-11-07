#ifndef MAE_MARKER_STOCK_HPP
#define MAE_MARKER_STOCK_HPP

#include <vector>
#include "utils/Observer.hpp"
#include "simulation/Marker.hpp"
#include "simulation/StockConfig.hpp"

namespace mae
{	
	class MarkerStock
	{
	private:
		Stg::World *world_;
		int refillCount_;
		
		std::vector<Marker*> markerPool_;
		std::vector<Marker*> marker_;
		
		int currentID_;
		
		void refill(const int p_markerCount);
		void cleanup();
	public:
		MarkerStock(const StockConfig &p_config);

		~MarkerStock();

		std::vector<Marker*>& getMarker();

		Marker* acquireMarker();
		void releaseMarker(Marker *p_marker);
	};

}

#endif
