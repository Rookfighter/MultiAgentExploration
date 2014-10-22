#include <easylogging++.h>
#include <sstream>
#include "simulation/Ranger.hpp"

namespace mae
{
	/*=================
	 * RangerProperties
	 *=================*/

	RangerProperties::RangerProperties()
	{
	}

	RangerProperties::~RangerProperties()
	{
	}

	void RangerProperties::setMeasurementCount(const int p_count)
	{
		measurementCount_ = p_count;
		measurementOrigins_.resize(p_count);
	}

	double RangerProperties::getFOV() const
	{
		return fov_;
	}

	double RangerProperties::getMaxRange() const
	{
		return maxRange_;
	}

	int RangerProperties::getMeasurementCount() const
	{
		return measurementCount_;
	}

	const std::vector<Pose>& RangerProperties::getMeasurementOrigins() const
	{
		return measurementOrigins_;
	}

	std::string RangerProperties::str(const int p_index) const
	{
		std::stringstream ss;
		ss.precision(2);

		ss << "id=" << p_index << ",orig=" << measurementOrigins_[p_index].str() << ",fov=" << fov_ << ",mr=" << maxRange_;

		return ss.str();
	}

	/*========
	 * Ranger
	 *========*/
	Ranger::Ranger(const RobotConfig &p_config)
		:ranger_(p_config.client->getClient(), p_config.rangerIndex)
	{
		LOG(DEBUG) << "Connected RangerProxy: " << p_config.rangerIndex << " (" << p_config.name << ")";

		ranger_.RequestGeom();
		ranger_.RequestConfigure();
		LOG(DEBUG) << "Requested Geometry and Configure for RangerProxy " << p_config.rangerIndex << " (" << p_config.name << ")";

		updateProperties();
		LOG(DEBUG) << "Initialized MeasurementProperties " << p_config.rangerIndex << " (" << p_config.name << ")";
	}

	void Ranger::updateProperties()
	{
		properties_.setMeasurementCount(ranger_.GetElementCount());

		properties_.maxRange_ = ranger_.GetMaxRange();
		properties_.fov_ = ranger_.GetMaxAngle() - ranger_.GetMinAngle();

		for(int i = 0; i < properties_.getMeasurementCount(); ++i) {
			player_pose3d_t pose = ranger_.GetElementPose(i);

			properties_.measurementOrigins_[i].set(pose.px, pose.py, pose.pyaw);
		}
	}

	Ranger::~Ranger()
	{

	}

	double Ranger::getDistance(const int p_index) const
	{
		return ranger_.GetRange(p_index);
	}

	bool Ranger::hasValidData() const
	{
		return ranger_.IsValid();
	}

	const RangerProperties& Ranger::getProperties() const
	{
		return properties_;
	}

	int Ranger::getMeasurementCount() const
	{
		return properties_.getMeasurementCount();
	}


}
