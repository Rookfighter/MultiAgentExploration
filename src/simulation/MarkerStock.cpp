#include <sstream>
#include <iostream>
#include <easylogging++.h>
#include "simulation/MarkerStock.hpp"

namespace mae
{
	MarkerStock::MarkerStock(const StockConfig &p_config)
		:world_(p_config.world), refillCount_(p_config.refillCount),
		markerPool_(), marker_(), currentID_(0)
	{
		refill(refillCount_);
		LOG(DEBUG) << "Initialized MarkerStock: " << refillCount_ << " Marker";
	}

	MarkerStock::~MarkerStock()
	{
		cleanup();
	}

	void MarkerStock::refill(const int p_markerCount)
	{
		markerPool_.resize(p_markerCount);

		for(int i = 0; i < markerPool_.size(); ++i) {
			markerPool_[i] = new Marker(currentID_);
			currentID_++;
		}
	}

	void MarkerStock::cleanup()
	{
		for(int i = 0; i < markerPool_.size(); ++i)
			delete markerPool_[i];
		for(int i = 0; i < marker_.size(); ++i) {
			marker_[i]->disconnect();
			delete marker_[i];
		}

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
			refill(refillCount_);
			
		Marker *marker = markerPool_.back();
		markerPool_.pop_back();
		marker_.push_back(marker);
		marker->setValue(0);
		marker->connect(world_);

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
		marker->disconnect();
	}
	
}
