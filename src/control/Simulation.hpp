#ifndef MAE_SIMULATION_HPP
#define MAE_SIMULATION_HPP

#include "control/PlayerClient.hpp"
#include "common/Odometry.hpp"

namespace mae
{
	class Simulation
	{
	private:
		PlayerClient *client_;
		PlayerCc::SimulationProxy simulation_;


		void setProperty(const std::string &p_name,
		                 const std::string &p_property,
		                 void *p_value,
		                 const size_t p_size);
		void getProperty(const std::string &p_name,
		                 const std::string &p_property,
		                 void *p_value,
		                 const size_t p_size);
	public:
		Simulation(PlayerClient *p_client, const int p_simulationIndex);
		~Simulation();

		void setPoseOf(const std::string &p_name, const Pose &p_pose);
		Pose getPoseOf(const std::string &p_name);

		void setSizeOf(const std::string &p_name, double *p_sizes);
		void getSizeOf(const std::string &p_name, double *p_sizes);
	};

}

#endif
