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
		:ranger_(NULL)
	{
		Stg::Model* parent = p_config.world->GetModel(p_config.name);
		std::stringstream ss;
		ss << "ranger:" << p_config.rangerIndex;
		ranger_ = reinterpret_cast<Stg::ModelRanger*>(parent->GetChild(ss.str()));
		ranger_->Subscribe();
		LOG(DEBUG) << "Connected ModelRanger: " << ss.str() << " (" << p_config.name << ")";

		updateProperties();
		LOG(DEBUG) << "Initialized MeasurementProperties " << p_config.rangerIndex << " (" << p_config.name << ")";
	}

	void Ranger::updateProperties()
	{
		assert(ranger_->GetSensors().size() != 0);
		properties_.setMeasurementCount(ranger_->GetSensors().size());
		
		properties_.maxRange_ = ranger_->GetSensors()[0].range.max;
		properties_.fov_ = ranger_->GetSensors()[0].fov;

		for(int i = 0; i < properties_.getMeasurementCount(); ++i) {
			Stg::Pose pose = ranger_->GetSensors()[i].pose;

			properties_.measurementOrigins_[i].set(pose.x, pose.y, pose.a);
		}
	}

	Ranger::~Ranger()
	{
		ranger_->Unsubscribe();
	}

	double Ranger::getDistance(const int p_index) const
	{
		return ranger_->GetSensors()[p_index].ranges[0];
	}

	bool Ranger::hasValidData() const
	{
		return true;
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
