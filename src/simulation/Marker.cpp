#include <sstream>
#include "simulation/Marker.hpp"
#include "utils/Convert.hpp"
#include "utils/Math.hpp"

#define MARKER_SIZE 0.05

namespace mae
{
	Marker::Marker(const int p_id)
		:id_(p_id),
		 model_(NULL),
		 value_(0),
		 highlighted_(false),
		 compass_(),
		 directionInfos_(4)
	{
	}

	Marker::~Marker()
	{
	}

	void Marker::connect(Stg::World* p_world)
	{
		assert(model_ == NULL);

		model_ = new Stg::Model(p_world, NULL, "marker", name_);

		model_->AddBlockRect(0, 0, MARKER_SIZE, MARKER_SIZE, 0.01);
		model_->SetColor(Stg::Color::red);

		Stg::Geom geom = model_->GetGeom();
		geom.size = Stg::Size(MARKER_SIZE, MARKER_SIZE, 0.01);
		model_->SetGeom(geom);

		model_->SetBlobReturn(false);
		model_->SetGripperReturn(false);
		model_->SetObstacleReturn(false);
		model_->SetRangerReturn(0.0);
		model_->SetFiducialReturn(false);

		compass_.connect(model_);
		
		directionInfos_[0].direction = EAST;
		directionInfos_[1].direction = NORTH;
		directionInfos_[2].direction = WEST;
		directionInfos_[3].direction = SOUTH;
		for(DirectionInfo info : directionInfos_) {
			info.lastMsec = 0L;
			info.state = OPEN;
		}
	}

	void Marker::disconnect()
	{
		assert(model_ != NULL);

		compass_.disconnect();
		model_->GetWorld()->RemoveModel(model_);
		delete model_;
		model_ = NULL;
	}

	void Marker::setPose(const Pose& p_pose)
	{
		assert(model_ != NULL);

		model_->SetGlobalPose(Stg::Pose(p_pose.position.x, p_pose.position.y, 0, p_pose.yaw));
	}

	void Marker::setValue(const double p_value)
	{
		value_ = p_value;
	}

	void Marker::setHighlighted(const bool p_highlighted)
	{
		assert(model_ != NULL);

		highlighted_ = p_highlighted;
		if(highlighted_)
			model_->SetColor(Stg::Color::green);
		else
			model_->SetColor(Stg::Color::red);
	}
	
	void Marker::setDirectionState(const CardinalDirection p_direction, const DirectionState p_state)
	{
		for(DirectionInfo &info : directionInfos_) {
			if(info.direction == p_direction) {
				info.state = p_state;
				if(info.state == EXPLORED)
					info.lastMsec = model_->GetWorld()->SimTimeNow() / 1000L;
			}
		}
	}
	
	void Marker::exploreDirection(const CardinalDirection p_direction)
	{
		setDirectionState(p_direction, EXPLORED);
	}

	bool Marker::isHighlighted() const
	{
		return highlighted_;
	}

	int Marker::getID() const
	{
		return id_;
	}

	Pose Marker::getPose() const
	{
		assert(model_ != NULL);

		Stg::Pose pose = model_->GetGlobalPose();
		return Pose(pose.x, pose.y, pose.a);
	}

	double Marker::getValue() const
	{
		return value_;
	}

	const TwoBitCompass& Marker::getCompass() const
	{
		return compass_;
	}
	
	CardinalDirection Marker::getRecommendedDirection() const
	{
		int oldest = -1;
		for(int i = 0; i < directionInfos_.size(); ++i) {
			// check if direction is still open
			if(directionInfos_[i].state == OPEN)
				return directionInfos_[i].direction;
			
			// check if exploration of this direction is older
			if(oldest == -1 || directionInfos_[i].lastMsec < directionInfos_[oldest].lastMsec)
				oldest = i; 
		}
		
		return directionInfos_[oldest].direction;
	}
    
    DirectionState Marker::getDirectionState(const CardinalDirection p_direction) const
    {
        for(DirectionInfo info : directionInfos_) {
            if(info.direction == p_direction)
                return info.state;
        }
        
        return OPEN;
    }
    
    long Marker::getDirectionLastVisit(const CardinalDirection p_direction) const
    {
        for(DirectionInfo info : directionInfos_) {
            if(info.direction == p_direction)
                return info.lastMsec;
        }
        
        return 0L;
    }

	void Marker::changeValueBy(const double p_toChange)
	{
		value_ += p_toChange;
	}

	bool Marker::hasSameValueAs(const Marker *p_marker, const double p_eps)
	{
		return sameDouble(getValue(), p_marker->getValue(), p_eps);
	}

	std::string Marker::str()
	{
		std::stringstream ss;
		ss.precision(2);
		ss << "id: " << id_ << " pos=" << getPose().str() << ",val=" << value_;

		return ss.str();
	}
}
