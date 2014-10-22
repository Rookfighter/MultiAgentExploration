#include <sstream>
#include <iostream>
#include "simulation/MarkerStock.hpp"

namespace mae
{

	MarkerStock::MarkerStock(const StockConfig &p_config)
		:config_(p_config), simulation_(config_.simulation),
		 availableMarker_(), inUseMarker_()
	{
		resize(config_.markerCount);
	}

	MarkerStock::~MarkerStock()
	{
		cleanup();
	}

	void MarkerStock::resize(const int p_markerCount)
	{
		cleanup();

		availableMarker_.resize(p_markerCount);
		inUseMarker_.reserve(p_markerCount);

		for(int i = 0; i < availableMarker_.size(); ++i)
			availableMarker_[i] = new Marker(config_, i);
	}

	void MarkerStock::cleanup()
	{
		std::vector<Marker*> all = getAll();
		for(int i = 0; i < all.size(); ++i)
			delete all[i];

		availableMarker_.clear();
		inUseMarker_.clear();
	}

	std::vector<Marker*> MarkerStock::getAll()
	{
		std::vector<Marker*> result(availableMarker_.size() + inUseMarker_.size());
		for(int i = 0; i < availableMarker_.size(); ++i)
			result[i] = availableMarker_[i];
		for(int i = 0; i < inUseMarker_.size(); ++i)
			result[i + availableMarker_.size()] = inUseMarker_[i];

		return result;
	}

	std::vector<Marker*>& MarkerStock::getAvailable()
	{
		return availableMarker_;
	}

	std::vector<Marker*>& MarkerStock::getInUse()
	{
		return inUseMarker_;
	}

	Marker* MarkerStock::acquireMarker()
	{
		if(availableMarker_.size() == 0)
			throw new std::logic_error("Cannot acquire marker. No markers available");

		Marker *marker = availableMarker_.back();
		availableMarker_.pop_back();
		inUseMarker_.push_back(marker);
		marker->setValue(0);
		marker->setInUSe(true);

		return marker;
	}

	void MarkerStock::releaseMarker(Marker *p_marker)
	{
		if(inUseMarker_.size() == 0)
			throw new std::logic_error("Cannot release marker. No markers in use");

		std::vector<Marker*>::iterator it;
		for(it = inUseMarker_.begin(); it != inUseMarker_.end(); ++it)
			if((*it)->getID() == p_marker->getID())
				break;

		Marker *marker = (*it);
		inUseMarker_.erase(it);
		availableMarker_.push_back(marker);
		marker->setInUSe(false);
	}

	void MarkerStock::refresh()
	{
		std::vector<Marker*> all = getAll();
		for(Marker *marker : all)
			marker->refreshData();
	}

}
