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
		RangerConfig();
		RangerConfig(const int p_sensorCount);

		int sensorCount;
		std::vector<Pose> sensorPose;
		std::vector<double> fov;
		std::vector<double> maxRange;

		void setSensorCount(const int p_sensorCount);
		std::string str(const int p_index) const;
	};

	class ExplorationBot
	{
	private:
		PlayerClient *client_;
		Simulation *simulation_;
		PlayerCc::Position2dProxy motor_;
		PlayerCc::RangerProxy ranger_;

		std::string name_;
		RangerConfig config_;

		void updateRangerConfig();
	public:
		ExplorationBot(PlayerClient *p_client,
		               Simulation *p_simulation,
		               const std::string &p_name,
		               const int p_motorIndex,
		               const int p_rangerIndex);
		~ExplorationBot();

		void setPose(const Pose &p_pose);
		Pose getPose() const;
		Pose getAbsolutePose();

		void setVelocity(const Velocity &p_velocity);
		Velocity getVelocity() const;

		double getRangerDistance(const int p_index);
		bool hasValidRangerData();

		int getRangerCount() const;
		const RangerConfig& getRangerConfig() const;

		std::string getName() const;

		void placeMarker(Marker* p_marker);

		virtual Velocity getMaxVelocity() const = 0;
		virtual Velocity getMinVelocity() const = 0;

	};

}

#endif
