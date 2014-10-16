#include "BasicTest.hpp"
#include "easylogging++.h"

namespace mae
{

	BasicTest::BasicTest(PlayerClient &p_client,
	                     Simulation &p_simulation,
	                     MarkerStock &p_stock,
	                     ExplorationBot &p_bot)
		: client_(p_client), simulation_(p_simulation), stock_(p_stock), bot_(p_bot)
	{

	}

	BasicTest::~BasicTest()
	{
	}

	void BasicTest::execute()
	{
		updateClient();
		LOG(INFO) << "Marker before refresh.";
		printAllMarker();

		refreshStock();
		LOG(INFO) << "Marker after refresh.";
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
		
		/*LOG(INFO) << "Setting color.";
		uint32_t color = 0xff00ff00;
		simulation_.setColorOf(markerTmp->name, color);*/
	}

	void BasicTest::updateClient()
	{
		LOG(INFO) << "Updating client.";
		client_.update();
	}

	void BasicTest::printAllMarker()
	{
		LOG(INFO) << "Marker";
		std::vector<Marker*> all = stock_.getAll();
		for(Marker* marker : all)
			LOG(INFO) << "--" << marker->str();
	}

	void BasicTest::refreshStock()
	{
		LOG(INFO) << "Refreshing stock";
		stock_.refresh();
	}

	void BasicTest::printRobot(ExplorationBot &p_bot)
	{
		LOG(INFO) << "Robot: " << p_bot.getName();
		LOG(INFO) << "-- Pose: " << p_bot.getPose().str();
		LOG(INFO) << "-- Velocity: " << p_bot.getVelocity().str();
		LOG(INFO) << "-- Max Velocity: " << p_bot.getMaxVelocity().str();
		LOG(INFO) << "-- Abs. Pose: " << p_bot.getAbsolutePose().str();
	}

	void BasicTest::printRangerConfig(RangerConfig &p_config)
	{
		LOG(INFO) << "RangerConfig ";
		for(int i = 0; i < p_config.sensorCount; ++i)
			LOG(INFO) << "-- " << p_config.str(i);
	}
}
