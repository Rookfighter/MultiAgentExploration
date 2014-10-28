#include <sstream>
#include <iostream>
#include <easylogging++.h>
#include "simulation/MarkerStock.hpp"

#define MARKER_SIZE 0.05

namespace mae
{

	MarkerStock::MarkerStock(const StockConfig &p_config)
		:graphics_(p_config.client->getClient(), p_config.graphicsIndex),
		 config_(p_config), markerPool_(), marker_(), currentID_(0)
	{
		LOG(DEBUG) << "Connected Graphics2DProxy: " << p_config.graphicsIndex << " (" << p_config.stockName << ")";
		refill(config_.refillCount);
		LOG(DEBUG) << "Initialized MarkerStock: " << config_.refillCount << " Marker (" << p_config.stockName << ")";
	}

	MarkerStock::~MarkerStock()
	{
		cleanup();
	}

	void MarkerStock::refill(const int p_markerCount)
	{
		markerPool_.resize(p_markerCount);

		for(int i = 0; i < markerPool_.size(); ++i) {
			markerPool_[i] = new Marker(config_.markerRange, currentID_);
			markerPool_[i]->addObserver(this);
			currentID_++;
		}
	}

	void MarkerStock::cleanup()
	{
		for(int i = 0; i < markerPool_.size(); ++i)
			delete markerPool_[i];
		for(int i = 0; i < marker_.size(); ++i)
			delete marker_[i];

		markerPool_.clear();
		marker_.clear();
	}

	std::vector<Marker*>& MarkerStock::getMarker()
	{
		return marker_;
	}

	Marker* MarkerStock::acquireMarker()
	{
		if(markerPool_.empty())
			refill(config_.refillCount);
			
		Marker *marker = markerPool_.back();
		markerPool_.pop_back();
		marker_.push_back(marker);
		marker->setValue(0);
		marker->setInUse(true);

		return marker;
	}

	void MarkerStock::releaseMarker(Marker *p_marker)
	{
		assert(!marker_.empty());
		
		std::vector<Marker*>::iterator it;
		for(it = marker_.begin(); it != marker_.end(); ++it)
			if((*it)->getID() == p_marker->getID())
				break;

		Marker *marker = (*it);
		marker_.erase(it);
		markerPool_.push_back(marker);
		marker->setInUse(false);
	}
	
	void MarkerStock::notify(void *p_data)
	{
		redrawMarker();
	}

	void MarkerStock::redrawMarker()
	{
		graphics_.Clear();
		
		player_color_t markerColor;
		
		markerColor.red = 200;
		markerColor.blue = 0;
		markerColor.green = 0;
		markerColor.alpha = 255;
		
		graphics_.Color(200, 0, 0, 255);
		
		for(Marker *marker : marker_) {
			player_point_2d_t points[4];
			
			points[0].px = marker->getPose().position.x + MARKER_SIZE;
			points[0].py = marker->getPose().position.y + MARKER_SIZE;
			
			points[1].px = marker->getPose().position.x - MARKER_SIZE;
			points[1].py = marker->getPose().position.y + MARKER_SIZE;
			
			points[2].px = marker->getPose().position.x - MARKER_SIZE;
			points[2].py = marker->getPose().position.y - MARKER_SIZE;
			
			points[3].px = marker->getPose().position.x + MARKER_SIZE;
			points[3].py = marker->getPose().position.y - MARKER_SIZE;
			
			graphics_.DrawPolygon(points, 4, true, markerColor);
		}
	}

}
