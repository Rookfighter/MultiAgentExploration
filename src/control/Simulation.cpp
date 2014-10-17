#include "control/Simulation.hpp"
#include <easylogging++.h>

#define SIZE_ELEMENT_COUNT 3
#define VELOCITY_BOUNDS_SIZE 8
#define SIZE_PROPERTY_NAME "size"
#define COLOR_PROPERTY_NAME "color"
#define VELOCITY_BOUNDS_PROPERTY_NAME "velocity_bounds"

namespace mae
{

	Simulation::Simulation(const SimulationConfig& p_config)
		:client_(p_config.client), simulation_(client_->getClient(), p_config.simulationIndex)
	{
		LOG(DEBUG) << "Connected SimulationProxy: " << p_config.simulationIndex;
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
	
	void Simulation::setColorOf(const std::string &p_name, uint32_t p_color)
	{
		setProperty(p_name, COLOR_PROPERTY_NAME, (void *) &p_color, sizeof(uint32_t));
	}
	
	uint32_t Simulation::getColorOf(const std::string &p_name)
	{
		uint32_t result;
		getProperty(p_name, COLOR_PROPERTY_NAME, (void*) &result, sizeof(uint32_t));
	}
	
	void Simulation::getVelocityBounds(const std::string &p_name, double* p_bounds)
	{
		getProperty(p_name, VELOCITY_BOUNDS_PROPERTY_NAME, (void*) p_bounds, VELOCITY_BOUNDS_SIZE * sizeof(double));
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
