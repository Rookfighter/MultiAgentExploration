#ifndef MAE_MARKER_STOCK_HPP
#define MAE_MARKER_STOCK_HPP

#include <vector>
#include "simulation/Marker.hpp"

namespace mae
{	
	class MarkerStock
	{
	private:
		Simulation *simulation_;
		
		StockConfig config_;
		
		std::vector<Marker*> availableMarker_;
		std::vector<Marker*> inUseMarker_;
		
		void resize(const int p_markerCount);
		void cleanup();
	public:
		MarkerStock(const StockConfig &p_config);

		~MarkerStock();

		std::vector<Marker*> getAll();
		std::vector<Marker*>& getAvailable();
		std::vector<Marker*>& getInUse();

		Marker* acquireMarker();
		void releaseMarker(Marker *p_marker);

		void refresh();
	};

}

#endif
