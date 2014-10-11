#ifndef MAE_ANT_BOT_HPP
#define MAE_ANT_BOT_HPP

#include "control/PlayerClient.hpp"
#include "control/Simulation.hpp"
#include "control/Marker.hpp"
#include "common/Odometry.hpp"

namespace mae
{
	class RangerConfig
	{
	public:
		RangerConfig(const int p_sensorCount);
		
		int count;
		std::vector<Pose> sensorPose;
		std::vector<double> fov;
		std::vector<double> maxRange;
		
		std::string str(const int p_index) const;
	};
	
	class ExplorationBot
	{
	protected:
		std::string name_;
		PlayerClient *client_;
		Simulation *simulation_;
		PlayerCc::Position2dProxy motor_;

	public:
		ExplorationBot(PlayerClient *p_client,
		               Simulation *p_simulation,
		               const std::string &p_name,
		               const int p_motorIndex);
		~ExplorationBot();

		void setPose(const Pose &p_pose);
		Pose getPose() const;
		Pose getAbsolutePose();

		void setVelocity(const Velocity &p_velocity);
		Velocity getVelocity() const;

		std::string getName() const;
		
		void placeMarker(Marker* p_marker);
		
		virtual RangerConfig getRangerConfig() = 0;
		virtual int getRangerCount() = 0;
		virtual double getRangerDistance(const int p_index) = 0;

	};

}

#endif
