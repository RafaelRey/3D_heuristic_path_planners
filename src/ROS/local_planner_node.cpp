#include <rclcpp/rclcpp.hpp>
#include <LocalPlanner.hpp>

using namespace PathPlanners;


int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<LocalPlanner>();

    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;

/*
//     tf2_ros::Buffer tfBuffer;
//     tf2_ros::TransformListener tfListener(tfBuffer);
    std::unique_ptr<tf2_ros::Buffer> tfBuffer_;
    std::shared_ptr<tf2_ros::TransformListener> tfListener_{nullptr};
    tfListener_ = std::make_shared<tf2_ros::TransformListener>(*tfBuffer_);

    LocalPlanner lcPlanner(&tfBuffer);
    
    dynamic_reconfigure::Server<theta_star_2d::LocalPlannerConfig> server;
  	dynamic_reconfigure::Server<theta_star_2d::LocalPlannerConfig>::CallbackType f;

    // JAC: This will be executed in configureAlgorithm when LocalPlanner lcPlanner(&tfBuffer) is run, so it would not be needed.
  	f = boost::bind(&LocalPlanner::dynRecCb,&lcPlanner,  _1, _2); 
  	server.setCallback(f);

	ros::Rate loop_rate(30);
    while(ros::ok()){
        
        ros::spinOnce();
        
        // JAC: This is equivalent to requestPathService in planner_ros_node
        // JAC: Define lcPlanner.requestPathService or use it from planner_ros_node
        lcPlanner.plan();
        
        loop_rate.sleep();
    } */
    return 0;
}




//////////////////////////////
//     auto node = std::make_shared<LocalPlanner>();

//     // node->preset_loop_frequency(30);

//     rclcpp::spin(node);
//     // rclcpp::shutdown();

//     return 0;
// }

