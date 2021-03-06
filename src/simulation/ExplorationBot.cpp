#include <sstream>
#include <easylogging++.h>
#include "simulation/ExplorationBot.hpp"
#include "utils/Math.hpp"

namespace mae
{
    ExplorationBot::ExplorationBot(const RobotConfig &p_config)
        :model_(NULL),
         name_(p_config.name),
         absolutePose_(),
         motor_(p_config),
         ranger_(p_config),
         markerSensor_(p_config),
         compass_(),
         stock_(p_config.stock)
    {
        model_ = p_config.world->GetModel(name_);
        compass_.connect(model_);
        LOG(DEBUG) << "Initialized Robot (" << name_ << ")";
    }

    ExplorationBot::~ExplorationBot()
    {
        compass_.disconnect();
    }

    void ExplorationBot::setPose(const Pose &p_pose)
    {
        model_->SetGlobalPose(p_pose.asStagePose());
    }

    Pose ExplorationBot::getAbsolutePose()
    {
        Pose pose;
        pose.fromStagePose(model_->GetGlobalPose());
        return pose;
    }
    
     bool ExplorationBot::collidedWithObstacle() const
     {
         return model_->Stalled();
     }

    Motor& ExplorationBot::getMotor()
    {
        return motor_;
    }

    Ranger& ExplorationBot::getRanger()
    {
        return ranger_;
    }

    MarkerSensor& ExplorationBot::getMarkerSensor()
    {
        return markerSensor_;
    }

    TwoBitCompass& ExplorationBot::getCompass()
    {
        return compass_;
    }

    std::string ExplorationBot::getName() const
    {
        return name_;
    }

    Marker* ExplorationBot::dropMarker()
    {
        Pose markerPose = getAbsolutePose();
        markerPose.yaw = 0;
        Marker *marker = stock_->acquireMarker();
        marker->setPose(markerPose);

        return marker;
    }
}
