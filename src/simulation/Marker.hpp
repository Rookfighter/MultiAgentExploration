#ifndef MAE_ANT_MARKER_HPP
#define MAE_ANT_MARKER_HPP

#include <string>
#include <stage.hh>
#include "utils/Odometry.hpp"

namespace mae
{
	class Marker
	{
		friend class MarkerStock;
	private:
		Stg::Model *model_;
		int id_;
		bool highlighted_;
		int value_;
		std::string name_;
		
	public:
		Marker(const int p_id);
		~Marker();
		
		void connect(Stg::World* p_world);
		void disconnect(Stg::World* p_world);
		
		void setPose(const Pose& p_pose);
		void setValue(const int p_value);
		void setHighlighted(const bool p_highlighted);
		
		int getID() const;
		Pose getPose() const;
		int getValue() const;
		bool isHighlighted() const;
		
		void incrementValue();
		
		std::string str();
	};

}

#endif
