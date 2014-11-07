#ifndef MAE_MARKER_STOCK_HPP
#define MAE_MARKER_STOCK_HPP

#include <vector>
#include "utils/Observer.hpp"
#include "simulation/Marker.hpp"
#include "simulation/StockConfig.hpp"

namespace mae
{	
	class MarkerStock : public Observer
	{
	private:
		static player_color_t BLACK;
		static player_color_t RED;
		static player_color_t GREEN;
	
		PlayerCc::Graphics2dProxy graphics_;
		StockConfig config_;
		
		std::vector<Marker*> markerPool_;
		std::vector<Marker*> marker_;
		
		Pose pose_;
		
		int currentID_;
		
		void refill(const int p_markerCount);
		void cleanup();
		void redrawMarker();
		void drawMarkerCenter(Marker *p_marker);
	public:
		MarkerStock(const StockConfig &p_config);

		~MarkerStock();

		std::vector<Marker*>& getMarker();

		Marker* acquireMarker();
		void releaseMarker(Marker *p_marker);
		
		void notify(void *p_data);
	};

}

#endif
