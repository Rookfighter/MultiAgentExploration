#ifndef TUT_MEYBOT_HPP
#define TUT_MEYBOT_HPP

#include <libplayerc++/playerc++.h>
#include <string>

namespace tut
{
	
	enum MeybotSonar {FRONT=0, LEFT, RIGHT};
	
	class Pose
	{
	public:
		double x;
		double y;
		double yaw;
	};

	class Meybot
	{
	private:
		PlayerCc::PlayerClient robot_;
		PlayerCc::Position2dProxy motor_;
		PlayerCc::RangerProxy rangers_;
	public:
		Meybot();
		Meybot(const std::string &p_host, const int p_port);
		~Meybot();
		
		void setSpeed(const double p_linVel, const double p_angVel);
		double getLinearVelocity() const;
		double getAngularVelocity() const;
		
		Pose getPose() const;
		
		double getSonarDistance(const int p_sensor) const;
		int getSonarCount() const;
		
		void update();
	};

}

#endif
