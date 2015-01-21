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
		unsigned int measurementCount_;
		std::vector<Pose> measurementOrigins_;
		
		void setMeasurementCount(const unsigned int p_count);
	public:
		RangerProperties();
		~RangerProperties();
		
		double getFOV() const;
		double getMaxRange() const;
		unsigned int getMeasurementCount() const;
		const std::vector<Pose>& getMeasurementOrigins() const;
		
		std::string str(const int p_index) const;
		
	};
	
	class Ranger
	{
	private:
		Stg::ModelRanger *ranger_;
		
		RangerProperties properties_;
		
		void updateProperties();
	public:
		Ranger(const RobotConfig &p_config);
		~Ranger();
		
		double getDistance(const int p_index) const;
		bool hasValidData() const;

		const RangerProperties& getProperties() const;
		unsigned int getMeasurementCount() const;
		
		static const int SENSOR_P90F = 0;
		static const int SENSOR_P50 = 1;
		static const int SENSOR_P30 = 2;
		static const int SENSOR_P10 = 3;

		static const int SENSOR_N10 = 4;
		static const int SENSOR_N30 = 5;
		static const int SENSOR_N50 = 6;
		static const int SENSOR_N90F = 7;

		static const int SENSOR_N90B = 8;
        static const int SENSOR_N130 = 9;
        static const int SENSOR_N150 = 10;
        static const int SENSOR_N170 = 11;

        static const int SENSOR_P170 = 12;
        static const int SENSOR_P150 = 13;
        static const int SENSOR_P130 = 14;
        static const int SENSOR_P90B = 15;
	};

}

#endif
