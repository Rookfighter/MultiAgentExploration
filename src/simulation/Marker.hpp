#ifndef MAE_ANT_MARKER_HPP
#define MAE_ANT_MARKER_HPP

#include <string>
#include "common/Odometry.hpp"
#include "common/Subject.hpp"

namespace mae
{
	class Marker : public Subject
	{
		friend class MarkerStock;
	private:
		int id_;
		Pose pose_;
		double range_;
		bool inUse_;
		bool drawRange_;
		int value_;
		
		void redraw();
	public:
		Marker(const double p_range, const int p_id);
		~Marker();
		
		void setPose(const Pose& p_pose);
		void setValue(const int p_value);
		void setDrawRange(const bool p_drawRange);
		
		int getID() const;
		const Pose& getPose() const;
		double getRange() const;
		int getValue() const;
		bool isInUse() const;
		bool drawRange() const;
		
		void incrementValue();
		
		std::string str();
	};

}

#endif
