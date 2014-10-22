#ifndef MAE_ANT_MARKER_HPP
#define MAE_ANT_MARKER_HPP

#include <string>
#include "simulation/StockConfig.hpp"
#include "common/Odometry.hpp"

namespace mae
{
	class Marker
	{
	private:
		Simulation *simulation_;
		
		int id_;
		std::string name_;
		Pose pose_;
		double range_;
		bool inUse_;
		int value_;
		
	public:
		Marker(const StockConfig& p_config, const int p_id);
		~Marker();
		
		void setPose(const Pose& p_pose);
		void setValue(const int p_value);
		void setInUSe(const bool p_inUse);
		
		int getID() const;
		const std::string& getName() const;
		const Pose& getPose() const;
		double getRange() const;
		int getValue() const;
		
		bool isInUse() const;
		
		void refreshData();
		void incrementValue();
		
		std::string str();
	};

}

#endif
