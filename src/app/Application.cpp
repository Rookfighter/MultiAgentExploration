#include <easylogging++.h>
#include <signal.h>
#include "app/Application.hpp"

#include "io/ExperimentLoader.hpp"
#include "io/ImportYaml.hpp"
#include "io/StatisticLoader.hpp"

#define STATISTIC_SAVE_DIR "../plot"

namespace mae
{
    static int applicationCallback(Stg::World* world, void* userarg)
    {
        Application *app = (Application*) userarg;
        return app->update();
    }

    Application::Application(int argc, char** argv)
            : experiment_(NULL),
              run_(true),
              keepRunning_(true),
              exitCode_(0),
              configFile_("")
    {
        assert(argc > 1);

        Stg::Init(&argc, &argv);
        configFile_ = argv[1];
    }

    Application::~Application()
    {
        if(experiment_ != NULL)
            delete experiment_;
    }

    int Application::update()
    {
        if(experiment_->terminated())
            stop();
        if(experiment_->getSimulation()->getWorld()->TestQuit()) {
            saveStatistics();
            keepRunning_ = false;
            experiment_->getSimulation()->getWorld()->Stop();
            return -1;
        }

        experiment_->update();

        return 0;
    }

    void Application::saveStatistics()
    {
        if(experiment_->getStatistic() == NULL)
            return;

        try {
            experiment_->getStatistic()->saveToDirectory(STATISTIC_SAVE_DIR);
        } catch(std::exception &e) {
            LOG(ERROR)<< "Catched exception: " << e.what();
        } catch(...) {
            LOG(ERROR) << "Catched unkown instance.";
        }
    }

    int Application::run()
    {
        exitCode_ = 0;
        if(init()) {
            LOG(INFO)<< "Running Application";
            try {
                loop();
            } catch(std::exception &e) {
                LOG(ERROR)<< "Catched exception: " << e.what();
                exitCode_ = -2;
            } catch(...) {
                LOG(ERROR) << "Catched unkown instance.";
                exitCode_ = -2;
            }
            LOG(INFO)<< "Application terminated.";
        }

        return exitCode_;

    }

    bool Application::init()
    {
        try {
            ImportYaml importer;
            ExperimentLoader experimentLoader;
            importer.resolveImports(configFile_);
            experimentLoader.load(importer.getRoot());
            experiment_ = experimentLoader.create();
            experiment_->getSimulation()->getWorld()->AddUpdateCallback(applicationCallback,
                                                                        this);
        } catch(std::exception &e) {
            LOG(ERROR)<< "Loading exception: " << e.what();
            exitCode_ = -1;
        } catch(...) {
            LOG(ERROR) << "Loading exception: Catched unkown instance.";
            exitCode_ = -1;
        }

        return exitCode_ == 0;
    }

    void Application::loop()
    {
        keepRunning_ = true;
        if(experiment_->getSimulation()->getWorld()->IsGUI())
            loopGUI();
        else
            loopNonGUI();
    }

    void Application::loopGUI()
    {
        LOG(INFO)<< "Running in GUI mode";
        Stg::World::Run();
    }

    void Application::loopNonGUI()
    {
        LOG(INFO)<< "Running in NonGUI mode";
        while(keepRunning_) {
            experiment_->getSimulation()->getWorld()->Update();
        }
    }

    void Application::stop()
    {
        LOG(INFO)<< "Stopping application";
        experiment_->getSimulation()->getWorld()->Quit();
    }

    int Application::getExitCode()
    {
        return exitCode_;
    }

}
