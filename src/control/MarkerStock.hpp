#ifndef MAE_MARKER_STOCK_HPP
#define MAE_MARKER_STOCK_HPP

#include <vector>
#include "control/Simulation.hpp"
#include "control/Marker.hpp"

namespace mae
{

	class MarkerStock
	{
	private:
		Simulation *simulation_;
		std::string markerName_;
		std::vector<Marker*> availableMarker_;
		std::vector<Marker*> inUseMarker_;
		
		void resize(const int p_markerCount);
		void cleanup();
		void init();
	public:
		MarkerStock(Simulation *p_simulation,
		            const int p_markerCount,
		            const std::string &p_markerName="marker");

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
