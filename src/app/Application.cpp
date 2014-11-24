#include <easylogging++.h>
#include <signal.h>
#include "app/Application.hpp"
#include "loading/ExperimentLoader.hpp"
#include "loading/StatisticLoader.hpp"
#include "loading/ImportYaml.hpp"

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
          statistic_(NULL),
          run_(true),
          keepRunning_(true)
    {
        assert(argc > 1);

        try {
            Stg::Init(&argc, &argv);
            
            ImportYaml importer;
            importer.resolveImports(argv[1]);
            statistic_ = StatisticLoader::load(importer.getRoot());
            if(statistic_ != NULL) {
                experiment_ = statistic_->getExperiment();
            } else {
                experiment_ = ExperimentLoader::load(importer.getRoot());
            }
            
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
        if(experiment_ != NULL)
            delete experiment_;
    }

    int Application::update()
    {
        experiment_->update();
        if(statistic_ != NULL)
            statistic_->update();
            
        if(experiment_->getSimulation()->getWorld()->TestQuit()) {
            saveStatistics();
            keepRunning_ = false;
            experiment_->getSimulation()->getWorld()->Stop();
            return -1;
        }
        
        return 0;
    }
    
    void Application::saveStatistics()
    {
         if(statistic_ == NULL)
             return;
             
        try {
            statistic_->saveToDirectory(STATISTIC_SAVE_DIR);
        } catch(std::exception &e) {
            LOG(ERROR) << "Catched exception: " << e.what();
        } catch(...) {
            LOG(ERROR) << "Catched unkown instance.";
        }
    }


    void Application::run()
    {
        if(!run_)
            return;

        LOG(INFO) << "Running Application";
        try {
            loop();
        } catch(std::exception &e) {
            LOG(ERROR) << "Catched exception: " << e.what();
        } catch(...) {
            LOG(ERROR) << "Catched unkown instance.";
        }
        LOG(INFO) << "Application terminated.";
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
        Stg::World::Run();
    }
    
    void Application::loopNonGUI()
    {
        LOG(INFO) << "Loop non GUI";
        while(keepRunning_) {
             experiment_->getSimulation()->getWorld()->Update();
        }
    }
    
    void Application::stop()
    {
        LOG(INFO) << "Stopping application";
        experiment_->getSimulation()->getWorld()->Quit();
    }

}
