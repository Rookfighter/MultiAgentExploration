#include "control/Simulation.hpp"

namespace mae
{

	Simulation::Simulation(const SimulationConfiguration &p_config)
		:client_(p_config.host, p_config.port), simulation_(&client_, p_config.simulationIndex), updated_(false),
		 unusedPose_(p_config.unusedPose), markerName_(p_config.markerName)
	{
		resizeMarkerLists(p_config.markerCount);
	}

	Simulation::~Simulation()
	{
		cleanupLists();
	}

	void Simulation::resizeMarkerLists(const int p_count)
	{
		cleanupLists();
		
		unusedMarker_.resize(p_count);
		usedMarker_.reserve(p_count);

		for(int i = 0; i < unusedMarker_.size(); ++i)
			unusedMarker_[i] = new Marker();
			
		initMarker();
	}

	void Simulation::initMarker()
	{
		std::vector<Marker*> all = getMarker();
		for(int i = 0; i < all.size(); ++i) {
			std::stringstream ss;
			ss << markerName_ << (i + 1);
			all[i]->name = ss.str();
			all[i]->value = 0;
		}
		
		for(Marker* marker : unusedMarker_)
			marker->inUse = false;
		
		for(Marker* marker : usedMarker_)
			marker->inUse = true;
			
		updated_ = false;
	}

	void Simulation::cleanupLists()
	{
		std::vector<Marker*> all = getMarker();
		for(int i = 0; i < all.size(); ++i)
			delete all[i];

		unusedMarker_.clear();
		usedMarker_.clear();
	}

	void Simulation::setPoseOf(const std::string &p_name, const Pose &p_pose)
	{
		char tmpName[p_name.length() + 1];
		strcpy(tmpName, p_name.c_str());
		simulation_.SetPose2d(tmpName, p_pose.x, p_pose.y, p_pose.yaw);
	}

	Pose Simulation::getPoseOf(const std::string &p_name)
	{
		Pose result;
		char tmpName[p_name.length() + 1];
		strcpy(tmpName, p_name.c_str());
		simulation_.GetPose2d(tmpName, result.x, result.y, result.yaw);
		return result;
	}

	void Simulation::placeMarkerAt(const Pose& p_pose)
	{
		if(unusedMarker_.size() == 0)
			throw new std::logic_error("Cannot place marker. No unused markers available");

		Marker *marker = unusedMarker_.back();
		unusedMarker_.erase(unusedMarker_.end());
		marker->pose = p_pose;
		setPoseOf(marker->name, marker->pose);
	}

	void Simulation::removeMarker(const std::string &p_name)
	{
		if(usedMarker_.size() == 0)
			throw new std::logic_error("Cannot remove marker. No used markers available");

		std::vector<Marker*>::iterator it;
		for(it = usedMarker_.begin(); it != usedMarker_.end(); ++it)
			if((*it)->name == p_name)
				break;
		Marker *marker = (*it);
		usedMarker_.erase(it);
		setPoseOf(marker->name, marker->pose);
	}

	std::vector<Marker*> Simulation::getUnusedMarker()
	{
		return unusedMarker_;
	}

	std::vector<Marker*> Simulation::getUsedMarker()
	{
		return usedMarker_;
	}

	std::vector<Marker*> Simulation::getMarker()
	{
		std::vector<Marker*> result(unusedMarker_.size() + usedMarker_.size());
		for(int i = 0; i < unusedMarker_.size(); ++i)
			result[i] = unusedMarker_[i];
		for(int i = 0; i < usedMarker_.size(); ++i)
			result[i + unusedMarker_.size()] = usedMarker_[i];

		return result;
	}

	PlayerCc::PlayerClient* Simulation::getClient()
	{
		return &client_;
	}

	void Simulation::update()
	{
		client_.Read();
		if(!updated_)
			updateMarker();
	}

	void Simulation::updateMarker()
	{
		std::vector<Marker*> all = getMarker();
		for(Marker *marker : all)
			marker->pose = getPoseOf(marker->name);
			
		updated_ = true;
	}
}
