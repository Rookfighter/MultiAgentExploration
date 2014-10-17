#include <stdexcept>
#include "control/SimulationLoader.hpp"
#include "BasicTest.hpp"
#include "HeisenbergTest.hpp"
#include "WanderTest.hpp"

_INITIALIZE_EASYLOGGINGPP

namespace mae
{
	class TestApplication
	{
	private:
		SimulationLoader loader_;
		BasicTest *basicTest_;
		HeisenbergTest *heisenbergTest_;
		WanderTest *wanderTest_;

	public:
		TestApplication() {
			loader_.load("test_simulation.yaml");
			basicTest_ = new BasicTest(*loader_.getClient(),
			                           *loader_.getSimulation(),
			                           *loader_.getStock(),
			                           *loader_.getRobot("pioneer1"));
			heisenbergTest_ = new HeisenbergTest(*loader_.getClient(),
			                                     *loader_.getRobot("pioneer1"));
			wanderTest_ = new WanderTest(*loader_.getClient(),
			                             *loader_.getRobot("pioneer1"));
		}

		~TestApplication() {
			delete wanderTest_;
			delete heisenbergTest_;
			delete wanderTest_;
		}

		void run() {
			LOG(INFO) << "Running Test";
			try {
				basicTest_->step();
			} catch(std::exception &e) {
				LOG(WARNING) << "Catched exception: " << e.what();
			} catch(PlayerCc::PlayerError &e) {
				LOG(WARNING) << "Catched player error: " << e.GetErrorStr();
			}
		}
	};
}

int main(int argc, char **argv)
{
	el::Configurations conf("easylog.conf");
	el::Loggers::reconfigureLogger("default", conf);

	LOG(INFO) << "App started.";
	mae::TestApplication app;
	app.run();
	LOG(INFO) << "App terminated.";
	return 0;
}
