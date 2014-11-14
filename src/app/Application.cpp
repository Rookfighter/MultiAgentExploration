#include <easylogging++.h>
#include "app/Application.hpp"
#include "algorithm/ExperimentLoader.hpp"
#include "statistics/StatisticLoader.hpp"

namespace mae
{
    static int applicationCallback(Stg::World* world, void* userarg)
    {
        Application *app = (Application*) userarg;
        app->update();

        return 0;
    }

    Application::Application(int argc, char** argv)
        : experiment_(NULL),
          statistic_(NULL),
          run_(true)
    {
        assert(argc > 1);

        try {
            Stg::Init(&argc, &argv);
            experiment_ = ExperimentLoader::load(argv[1]);
            statistic_ = StatisticLoader::load(argv[1], experiment_);
            experiment_->getSimulation()->getWorld()->AddUpdateCallback(applicationCallback, this);
        } catch(std::exception &e) {
            LOG(ERROR) << "Loading exception: " << e.what();
            run_ = false;
        } catch(...) {
            LOG(ERROR) << "Loading exception: Catched unkown instance.";
            run_ = false;
        }
    }

    Application::~Application()
    {
        if(statistic_ != NULL)
            delete statistic_;
        delete experiment_;
    }

    void Application::update()
    {
        experiment_->update();
        if(statistic_ != NULL)
            statistic_->update();
    }

    void Application::run()
    {
        if(!run_)
            return;

        LOG(INFO) << "Running Application";
        try {
            experiment_->getSimulation()->getWorld()->Run();
        } catch(std::exception &e) {
            LOG(ERROR) << "Catched exception: " << e.what();
        } catch(...) {
            LOG(ERROR) << "Catched unkown instance.";
        }
        LOG(INFO) << "Application terminated.";
    }

}
