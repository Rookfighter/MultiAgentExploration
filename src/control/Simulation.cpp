#include "control/Simulation.hpp"

#define SIZE_ELEMENT_COUNT 3
#define SIZE_PROPERTY_NAME "size"

namespace mae
{

	Simulation::Simulation(PlayerClient *p_client, const int p_simulationIndex)
		:client_(p_client), simulation_(client_->getClient(), p_simulationIndex)
	{
	}

	Simulation::~Simulation()
	{
	}

	void Simulation::setPoseOf(const std::string &p_name, const Pose &p_pose)
	{
		char tmpName[p_name.length() + 1];
		strcpy(tmpName, p_name.c_str());
		simulation_.SetPose2d(tmpName, p_pose.x, p_pose.y, p_pose.yaw);
	}

	Pose Simulation::getPoseOf(const std::string &p_name)
	{
		Pose result;
		char tmpName[p_name.length() + 1];
		strcpy(tmpName, p_name.c_str());
		simulation_.GetPose2d(tmpName, result.x, result.y, result.yaw);
		return result;
	}

	void Simulation::setSizeOf(const std::string &p_name, double *p_sizes)
	{
		setProperty(p_name, SIZE_PROPERTY_NAME, (void*) p_sizes,SIZE_ELEMENT_COUNT * sizeof(double));
	}

	void Simulation::getSizeOf(const std::string &p_name, double *p_sizes)
	{
		getProperty(p_name, SIZE_PROPERTY_NAME, (void*) p_sizes, SIZE_ELEMENT_COUNT * sizeof(double));
	}

	void Simulation::setProperty(const std::string &p_name,
	                             const std::string &p_property,
	                             void *p_value,
	                             const size_t p_size)
	{
		char tmpName[p_name.length() + 1];
		char tmpProperty[p_property.length() + 1];
		strcpy(tmpName, p_name.c_str());
		strcpy(tmpProperty, p_property.c_str());

		simulation_.SetProperty(tmpName, tmpProperty, p_value, p_size);
	}

	void Simulation::getProperty(const std::string &p_name,
	                             const std::string &p_property,
	                             void *p_value,
	                             const size_t p_size)
	{
		char tmpName[p_name.length() + 1];
		char tmpProperty[p_property.length() + 1];
		strcpy(tmpName, p_name.c_str());
		strcpy(tmpProperty, p_property.c_str());

		simulation_.GetProperty(tmpName, tmpProperty, p_value, p_size);
	}
}
