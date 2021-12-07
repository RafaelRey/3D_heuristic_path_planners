#include "utils/ros/ROSInterfaces.hpp"

namespace Planners
{
    namespace utils
    {

        Vec3i discretePoint(const pcl::PointXYZ &_point, const double &_res)
        { //Take care of negative values

            return {static_cast<int>(std::round(_point.x / _res)),
                    static_cast<int>(std::round(_point.y / _res)),
                    static_cast<int>(std::round(_point.z / _res))};
        }
        Vec3i discretePoint(const geometry_msgs::Point &_msg, const double &_res)
        {

            return {static_cast<int>(std::round(_msg.x / _res)),
                    static_cast<int>(std::round(_msg.y / _res)),
                    static_cast<int>(std::round(_msg.z / _res))};
        }
        Vec3i discretePose(const geometry_msgs::Pose &_msg, const double &_res)
        {   
            //TODO Take a look to this overloading
            // return discretePoint(_msg.position); 
            return {static_cast<int>(std::round(_msg.position.x / _res)),
                    static_cast<int>(std::round(_msg.position.y / _res)),
                    static_cast<int>(std::round(_msg.position.z / _res))};
        }
        Vec3i discretePose(const geometry_msgs::PoseStamped &_msg, const double &_res)
        {
            //TODO Take a look to this overloading
            // return discretePose(_msg.pose);
            return {static_cast<int>(std::round(_msg.pose.position.x / _res)),
                    static_cast<int>(std::round(_msg.pose.position.y / _res)),
                    static_cast<int>(std::round(_msg.pose.position.z / _res))};

        }
        geometry_msgs::Point continousPoint(const Vec3i &_vec, const double &_res)
        {
            geometry_msgs::Point ret;

            ret.x = _vec.x * _res;
            ret.y = _vec.y * _res;
            ret.z = _vec.z * _res;

            return ret;
        }
        geometry_msgs::PoseStamped continousPose(const Vec3i &_vec, const double &_res)
        {
            geometry_msgs::PoseStamped ret;

            ret.pose.position.x = _vec.x * _res;
            ret.pose.position.y = _vec.y * _res;
            ret.pose.position.z = _vec.z * _res;
            
            return ret;
        }
        inline Vec3i indexToXY(const unsigned int &_index, const unsigned int _grid_width)
        {

            return {static_cast<int>(std::floor(_index % _grid_width)),
                    static_cast<int>(std::floor(_index / _grid_width)),
                    0};
        }

        bool configureWorldFromOccupancy(const nav_msgs::OccupancyGrid &_grid, PathGenerator &_algorithm, bool _set_size)
        {

            if (_set_size)
            {
                Vec3i world_size;
                world_size.x = std::floor(_grid.info.width / _grid.info.resolution);
                world_size.y = std::floor(_grid.info.height / _grid.info.resolution);
                world_size.z = 1;
                _algorithm.setWorldSize(world_size, _grid.info.resolution);
            }

            Vec3i cell;
            for (long unsigned int i = 0; i < _grid.data.size(); ++i)
            {
                // 100 should be costmap_2d::LETHAL_OBSTACLE but by the values that map server publishes are between 0 and 100
                if (_grid.data[i] == 100)
                {
                    cell = indexToXY(i, _grid.info.width);
                    _algorithm.addCollision(cell);
                }
            }

            return true;
        }
        bool configureWorldFromOccupancyWithCosts(const nav_msgs::OccupancyGrid &_grid, PathGenerator &_algorithm, bool _set_size)
        {

            if (_set_size)
            {
                Vec3i world_size;
                world_size.x = std::floor(_grid.info.width / _grid.info.resolution);
                world_size.y = std::floor(_grid.info.height / _grid.info.resolution);
                world_size.z = 1;
                _algorithm.setWorldSize(world_size, _grid.info.resolution);
            }

            Vec3i cell;
            for (long unsigned int i = 0; i < _grid.data.size(); ++i)
            {
                // int data = _grid.data[i];
                // std::cout << "Cost: " << data << std::endl;
                // 100 should be costmap_2d::LETHAL_OBSTACLE but by the values that map server publishes are between 0 and 100
                cell = indexToXY(i, _grid.info.width);
                if (_grid.data[i] >= 99)
                    _algorithm.addCollision(cell);
                _algorithm.configureCellCost( cell, _grid.data[i] );
            }

            return true;
        }
        bool configureWorldFromPointCloud(const pcl::PointCloud<pcl::PointXYZ>::ConstPtr &_points, PathGenerator &_algorithm, const double &_resolution)
        {

            for (auto &it : *_points)
                _algorithm.addCollision(discretePoint(it, _resolution));

            return true;
        }

        bool configureWorldCosts(Grid3d &_grid, PathGenerator &_algorithm)
        {

            auto world_size = _algorithm.getWorldSize();
            auto resolution = _algorithm.getWorldResolution();

            for (int i = 0; i < world_size.x; i++)
            {
                for (int j = 0; j < world_size.y; j++)
                {
                    for (int k = 0; k < world_size.z; k++)
                    {
                        //JAC: Precision
                        // auto cost = _grid.getCellCost(i * resolution, j * resolution, k * resolution);
                        float cost = _grid.getCellCost(i * resolution, j * resolution, k * resolution);
                        // std::cout << "Cost: " << cost << std::endl;   
                        _algorithm.configureCellCost({i, j, k}, cost);
                    }
                }
            }

            return true;
        }

        bool coordinateListToROSPlan(const CoordinateList &_path, 
                                     const double &_resolution, 
                                     std::vector<geometry_msgs::PoseStamped> &_ros_plan)
        {
            if(!_ros_plan.empty())
                return false;

            for(const auto &it: _path)
                _ros_plan.push_back(continousPose(it, _resolution));
            
            return true;
        }


    } //ns utils
} //ns planners