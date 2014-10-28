#ifndef MAE_MARKER_STOCK_HPP
#define MAE_MARKER_STOCK_HPP

#include <vector>
#include "common/Observer.hpp"
#include "simulation/Marker.hpp"
#include "simulation/StockConfig.hpp"

namespace mae
{	
	class MarkerStock : public Observer
	{
	private:
		static const int rangePolygonCount_ = 12;
		player_point_2d_t rangePolygon_[rangePolygonCount_];
	
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
		void drawMarkerRange(Marker *p_marker);
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
