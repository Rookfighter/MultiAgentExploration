#ifndef MAE_ANT_MARKER_HPP
#define MAE_ANT_MARKER_HPP

#include <string>
#include "utils/Odometry.hpp"
#include "utils/Subject.hpp"

namespace mae
{
	class Marker : public Subject
	{
		friend class MarkerStock;
	private:
		int id_;
		Pose pose_;
		bool highlighted_;
		int value_;
		
		void redraw();
	public:
		Marker(const int p_id);
		~Marker();
		
		void setPose(const Pose& p_pose);
		void setValue(const int p_value);
		void setHighlighted(const bool p_highlighted);
		
		int getID() const;
		const Pose& getPose() const;
		int getValue() const;
		bool isHighlighted() const;
		
		void incrementValue();
		
		std::string str();
	};

}

#endif
