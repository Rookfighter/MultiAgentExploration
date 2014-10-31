#ifndef MAE_RANGER_HPP
#define MAE_RANGER_HPP

#include <vector>
#include "simulation/RobotConfig.hpp"

namespace mae
{
	class RangerProperties
	{
		friend class Ranger;
	private:
		double fov_;
		double maxRange_;
		int measurementCount_;
		std::vector<Pose> measurementOrigins_;
		
		void setMeasurementCount(const int p_count);
	public:
		RangerProperties();
		~RangerProperties();
		
		double getFOV() const;
		double getMaxRange() const;
		int getMeasurementCount() const;
		const std::vector<Pose>& getMeasurementOrigins() const;
		
		std::string str(const int p_index) const;
		
	};
	
	class Ranger
	{
	private:
		PlayerCc::RangerProxy ranger_;
		
		RangerProperties properties_;
		
		void updateProperties();
	public:
		Ranger(const RobotConfig &p_config);
		~Ranger();
		
		double getDistance(const int p_index) const;
		bool hasValidData() const;

		const RangerProperties& getProperties() const;
		int getMeasurementCount() const;
		
	};

}

#endif
