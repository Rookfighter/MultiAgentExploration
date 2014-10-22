#include <easylogging++.h>
#include "test/BasicTest.hpp"

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
		RangerProperties properties = bot_.getRanger().getProperties();
		printRangerProperties(properties);

		LOG(INFO) << "Dropping Marker.";
		bot_.dropMarker();

		printAllMarker();

		LOG(INFO) << "Drving away.";
		bot_.getMotor().setVelocity(Velocity(2.0,0));
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
		LOG(INFO) << "-- Pose: " << p_bot.getMotor().getPose().str();
		LOG(INFO) << "-- Velocity: " << p_bot.getMotor().getVelocity().str();
		LOG(INFO) << "-- Max Velocity: " << p_bot.getMotor().getMaxVelocity().str();
		LOG(INFO) << "-- Abs. Pose: " << p_bot.getAbsolutePose().str();
	}

	void BasicTest::printRangerProperties(RangerProperties &p_properties)
	{
		LOG(INFO) << "RangerProperties ";
		for(int i = 0; i < p_properties.getMeasurementCount(); ++i)
			LOG(INFO) << "-- " << p_properties.str(i);
	}
}
