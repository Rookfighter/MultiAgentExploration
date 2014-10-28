#include <sstream>
#include <iostream>
#include <easylogging++.h>
#include "simulation/MarkerStock.hpp"

#define MARKER_SIZE 0.05

namespace mae
{
	
	static void createRegularPolygon(const double p_radius, player_point_2d_t* p_corners, const int p_cornerCount) {
		for(int i = 0; i < p_cornerCount; ++i) {
			p_corners[i].px = p_radius * cos((2 * M_PI * i) / p_cornerCount);
			p_corners[i].py = p_radius * sin((2 * M_PI * i) / p_cornerCount);
		}
	}
	
	MarkerStock::MarkerStock(const StockConfig &p_config)
		:graphics_(p_config.client->getClient(), p_config.graphicsIndex),
		 config_(p_config), markerPool_(), marker_(), currentID_(0)
	{
		LOG(DEBUG) << "Connected Graphics2DProxy: " << p_config.graphicsIndex << " (" << p_config.stockName << ")";
		pose_ = config_.simulation->getPoseOf(config_.stockName);
		LOG(DEBUG) << "Initialized stock pose " << pose_.str() << "(" << p_config.stockName << ")";
		refill(config_.refillCount);
		LOG(DEBUG) << "Initialized MarkerStock: " << config_.refillCount << " Marker (" << p_config.stockName << ")";
		createRegularPolygon(1, rangePolygon_, rangePolygonCount_);
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
		marker->inUse_ = true;

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
		marker->inUse_ = false;
	}
	
	void MarkerStock::notify(void *p_data)
	{
		redrawMarker();
	}

	void MarkerStock::redrawMarker()
	{
		graphics_.Clear();
		
		graphics_.Color(0, 0, 0, 0);
		
		for(Marker *marker : marker_) {
			if(marker->drawRange())
				drawMarkerRange(marker);
			drawMarkerCenter(marker);
		}
	}
	
	void MarkerStock::drawMarkerCenter(Marker *p_marker)
	{
		player_color_t markerColor;
		
		markerColor.red = 200;
		markerColor.blue = 0;
		markerColor.green = 0;
		markerColor.alpha = 0;
		
		player_point_2d_t points[4];
		Vector2 relativePos = p_marker->getPose().position - pose_.position;
		points[0].px = relativePos.x + MARKER_SIZE;
		points[0].py = relativePos.y + MARKER_SIZE;
		
		points[1].px = relativePos.x - MARKER_SIZE;
		points[1].py = relativePos.y + MARKER_SIZE;
		
		points[2].px = relativePos.x - MARKER_SIZE;
		points[2].py = relativePos.y - MARKER_SIZE;
		
		points[3].px = relativePos.x + MARKER_SIZE;
		points[3].py = relativePos.y - MARKER_SIZE;
		
		graphics_.DrawPolygon(points, 4, true, markerColor);
	}
	
	void MarkerStock::drawMarkerRange(Marker *p_marker)
	{
		Vector2 relativePos = p_marker->getPose().position - pose_.position;
		player_color_t rangeColor;
		
		player_point_2d_t points[rangePolygonCount_];
		for(int i = 0; i < rangePolygonCount_; ++i) {
			points[i].px = p_marker->getRange() * rangePolygon_[i].px + relativePos.x;
			points[i].py = p_marker->getRange() * rangePolygon_[i].py + relativePos.y;
		}
		
		graphics_.DrawPolygon(points, rangePolygonCount_, false, rangeColor);
	}
	
}
