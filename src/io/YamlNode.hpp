#ifndef MAE_YAML_NODE_HPP
#define MAE_YAML_NODE_HPP

#include <string>

namespace mae
{

    namespace YamlNode
    {
        const std::string import = "import";
        
        const std::string world = "world";
        const std::string worldFile = "file";
        const std::string worldGui = "gui";
        
        const std::string stock = "stock";
        const std::string refillCount = "refill_count";
        
        const std::string robots = "robots";
        const std::string name = "name";
        const std::string rangerIndex = "ranger_index";
        const std::string maxVelocity = "max_velocity";
        const std::string minVelocity = "min_velocity";
        const std::string markerSensorMaxRange = "marker_sensor_max_range";
        
        const std::string algorithm = "algorithm";
        const std::string type = "type";
        const std::string obstacleStopDistance = "obstacle_stop_distance";
        const std::string obstacleAvoidDistance = "obstacle_avoid_distance";
        const std::string obstacleMarkerDistance = "obstacle_marker_distance";
        const std::string markerDeployDistance = "marker_deploy_distance";
        const std::string collisionResolveDistance = "collision_resolve_distance";
        const std::string markerTooCloseDistance = "marker_too_close_distance";
        
        const std::string experiment = "experiment";
        const std::string terminationMinutes = "termination_minutes";
        const std::string terminationCoverage = "termination_coverage";

        const std::string statistic = "statistic";
        const std::string floorplanName = "floorplan_name";
        const std::string tileSize = "tile_size";
        const std::string coverageEvents = "coverage_events";
    }
}

#endif
