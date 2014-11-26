#ifndef MAE_ANT_MARKER_HPP
#define MAE_ANT_MARKER_HPP

#include <string>
#include "simulation/TwoBitCompass.hpp"
#include "utils/Odometry.hpp"

namespace mae
{
	enum DirectionState { OPEN, EXPLORED };
	
	struct DirectionInfo {
		CardinalDirection direction;
		long lastMsec;
		DirectionState state;
	};
	
	class Marker
	{
		friend class MarkerStock;
	private:
		Stg::Model *model_;
		int id_;
		bool highlighted_;
		std::string name_;
        bool isLocked_;
		
		double value_;
		TwoBitCompass compass_;
		std::vector<DirectionInfo> directionInfos_;
		
	public:
		Marker(const int p_id);
		~Marker();
		
		void connect(Stg::World* p_world);
		void disconnect();
		
		void setPose(const Pose& p_pose);
		void setValue(const double p_value);
		void setHighlighted(const bool p_highlighted);
		void setDirectionState(const CardinalDirection p_direction, const DirectionState p_state);
		void exploreDirection(const CardinalDirection p_direction);
        bool lock();
        void unlock();
        void setAsTarget();
        void releaseAsTarget();
		
		int getID() const;
		Pose getPose() const;
		double getValue() const;
		bool isHighlighted() const;
		const TwoBitCompass& getCompass() const;
		CardinalDirection getRecommendedDirection() const;
        DirectionState getDirectionState(const CardinalDirection p_direction) const;
        long getDirectionLastVisit(const CardinalDirection p_direction) const;
        bool isLocked() const;
		
		void changeValueBy(const double p_toChange);
		
		bool hasSameValueAs(const Marker *p_marker, const double p_eps = 0.01);
		
		std::string str();
	};

}

#endif
