#include "control/MarkerStock.hpp"
#include "control/Meybot.hpp"
#include "easylogging++.h"

_INITIALIZE_EASYLOGGINGPP

namespace mae
{
	class ApplicationNC
	{
	private:
		PlayerClient client_;
		Simulation simulation_;
		MarkerStock stock_;
		Meybot bot_;
		
		void updateClient()
		{
			LOG(INFO) << "Updating client.";
			client_.update();
		}
		
		void printAllMarker()
		{
			LOG(INFO) << "Marker";
			std::vector<Marker*> all = stock_.getAll();
			for(Marker* marker : all)
				LOG(INFO) << "--" << marker->str();
		}
		
		void refreshStock()
		{
			LOG(INFO) << "Refreshing stock";
			stock_.refresh();
		}
		
		void printRobot(ExplorationBot &p_bot)
		{
			LOG(INFO) << "Robot: " << p_bot.getName();
			LOG(INFO) << "-- Pose: " << p_bot.getPose().str();
			LOG(INFO) << "-- Velocity: " << p_bot.getVelocity().str();
			LOG(INFO) << "-- Abs. Pose: " << p_bot.getAbsolutePose().str();
		}
		
		void printRangerConfig(RangerConfig &p_config)
		{
			LOG(INFO) << "RangerConfig ";
			for(int i = 0; i < p_config.count; ++i)
				LOG(INFO) << "-- " << p_config.str(i);
		}
		
		
	public:
		ApplicationNC()
		:client_(), simulation_(&client_, 0), stock_(&simulation_, 1), bot_(&client_, &simulation_, "meybot1", 0)
		{
			
		}
		
		void run()
		{
			updateClient();
			LOG(INFO) << "Before refresh.";
			printAllMarker();
			
			refreshStock();
			LOG(INFO) << "After refresh.";
			printAllMarker();
			
			LOG(INFO) << "Starting Position.";
			printRobot(bot_);
			
			LOG(INFO) << "Teleporting Bot.";
			bot_.setPose(Pose(2,2,1.5));
			printRobot(bot_);
			
			LOG(INFO) << "Getting RangerConfig.";
			RangerConfig config = bot_.getRangerConfig();
			printRangerConfig(config);
				
			LOG(INFO) << "Acquiring Marker.";
			Marker *markerTmp = stock_.acquireMarker();
			LOG(INFO) << "Placing Marker.";
			bot_.placeMarker(markerTmp);
			
			printAllMarker();
			
			LOG(INFO) << "Drving away.";
			bot_.setVelocity(Velocity(2.0,0));
			printRobot(bot_);
		}
	};
}

int main(int argc, char **argv)
{
	std::cout << "App started." << std::endl;
	mae::ApplicationNC app;
	app.run();
	std::cout << "App terminated." << std::endl;
	return 0;
}