#include <stdexcept>
#include "control/Pioneer2DX.hpp"
#include "BasicTest.hpp"
#include "HeisenbergTest.hpp"
#include "WanderTest.hpp"

_INITIALIZE_EASYLOGGINGPP

namespace mae
{
	class TestApplication
	{
	private:
		PlayerClient client_;
		Simulation simulation_;
		MarkerStock stock_;
		Pioneer2DX pioneer_;
		BasicTest basicTest_;
		HeisenbergTest heisenbergTest_;
		WanderTest wanderTest_;

	public:
		TestApplication()
			:client_(), simulation_(&client_, 0), stock_(&simulation_, 1),
			 pioneer_(&client_, &simulation_, "pioneer1", 0, 0),
			 basicTest_(client_,simulation_, stock_, pioneer_),
			 heisenbergTest_(client_, pioneer_),
			 wanderTest_(client_, pioneer_){

		}
		
		~TestApplication()
		{
			
		}

		void run() {
			LOG(INFO) << "Running Test";
			try {
				wanderTest_.execute();
			} catch (std::exception &e) {
				LOG(WARNING) << "Catched exception: " << e.what();
			} catch (PlayerCc::PlayerError &e) {
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
