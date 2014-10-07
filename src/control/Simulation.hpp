#ifndef MAE_SIMULATION_HPP
#define MAE_SIMULATION_HPP

#include <libplayerc++/playerc++.h>
#include <vector>
#include "common/Marker.hpp"

namespace mae
{
	class SimulationConfiguration
	{
	public:
		std::string host;
		int port;
		int simulationIndex;
		int markerCount;
		std::string markerName;
		Pose unusedPose;
	};
	
	class Simulation
	{
	private:
		PlayerCc::PlayerClient client_;
		PlayerCc::SimulationProxy simulation_;
		
		std::vector<Marker*> usedMarker_;
		std::vector<Marker*> unusedMarker_;
		
		std::string markerName_;
		Pose unusedPose_;
		bool updated_;
		
		void resizeMarkerLists(const int p_count);
		void initMarker();
		void updateMarker();
		void cleanupLists();
	public:
		Simulation(const SimulationConfiguration &p_config);
		~Simulation();
		
		void setPoseOf(const std::string &p_name, const Pose &p_pose);
		Pose getPoseOf(const std::string &p_name);
		
		void placeMarkerAt(const Pose& p_pose);
		void removeMarker(const std::string &p_name);
		
		std::vector<Marker*> getUnusedMarker();
		std::vector<Marker*> getUsedMarker();
		std::vector<Marker*> getMarker();
		
		PlayerCc::PlayerClient* getClient();
		
		void update();
	};

}

#endif
