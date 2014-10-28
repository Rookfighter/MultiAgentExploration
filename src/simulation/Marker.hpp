#ifndef MAE_ANT_MARKER_HPP
#define MAE_ANT_MARKER_HPP

#include <string>
#include "common/Odometry.hpp"
#include "common/Subject.hpp"

namespace mae
{
	class Marker : public Subject
	{
	private:
		int id_;
		Pose pose_;
		double range_;
		bool inUse_;
		int value_;
		
		void redraw();
	public:
		Marker(const double p_range, const int p_id);
		~Marker();
		
		void setPose(const Pose& p_pose);
		void setValue(const int p_value);
		void setInUse(const bool p_inUse);
		
		int getID() const;
		const Pose& getPose() const;
		double getRange() const;
		int getValue() const;
		
		bool isInUse() const;
		
		void incrementValue();
		
		std::string str();
	};

}

#endif
