#include <sstream>
#include <iostream>
#include "control/MarkerStock.hpp"

namespace mae
{

	MarkerStock::MarkerStock(const StockConfig &p_config)
		:simulation_(p_config.simulation), markerName_(p_config.markerName), availableMarker_(), inUseMarker_()
	{
		resize(p_config.markerCount);
		
		for(Marker* marker: availableMarker_)
			marker->range = p_config.markerRange;
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
			availableMarker_[i] = new Marker();

		init();
	}

	void MarkerStock::cleanup()
	{
		std::vector<Marker*> all = getAll();
		for(int i = 0; i < all.size(); ++i)
			delete all[i];

		availableMarker_.clear();
		inUseMarker_.clear();
	}

	void MarkerStock::init()
	{
		for(int i = 0; i < availableMarker_.size(); ++i) {
			std::stringstream ss;
			ss << markerName_ << (i + 1);
			availableMarker_[i]->id = i;
			availableMarker_[i]->name = ss.str();
			availableMarker_[i]->value = 0;
			availableMarker_[i]->inUse = false;
		}
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
		marker->value = 0;
		marker->inUse = true;

		return marker;
	}

	void MarkerStock::releaseMarker(Marker *p_marker)
	{
		if(inUseMarker_.size() == 0)
			throw new std::logic_error("Cannot release marker. No markers in use");

		std::vector<Marker*>::iterator it;
		for(it = inUseMarker_.begin(); it != inUseMarker_.end(); ++it)
			if((*it)->name == p_marker->name)
				break;

		Marker *marker = (*it);
		inUseMarker_.erase(it);
		availableMarker_.push_back(marker);
		marker->inUse = false;
	}

	void MarkerStock::refresh()
	{
		std::vector<Marker*> all = getAll();
		for(Marker *marker : all) {

			marker->pose = simulation_->getPoseOf(marker->name);
			/*double size[3];
			simulation_->getSizeOf(marker->name, size);
			marker->range = size[0];*/
		}
	}

}
