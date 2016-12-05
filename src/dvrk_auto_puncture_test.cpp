/*
 *  dvrk_auto_puncture_test.cpp
 *
 *	Author(s): Tamas D. Nagy
 *	Created on: 2016-12-03
 *
 */

#include <ros/ros.h>
#include <ros/package.h>
#include "std_msgs/String.h"
#include "sensor_msgs/JointState.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <cmath>
#include <numeric>
#include <chrono>
#include "dvrk/arm.hpp"
#include "dvrk/psm.hpp"
#include "dvrk/pose.hpp"
#include "dvrk/trajectory_factory.hpp"
#include "dvrk_automation/puncturer.hpp"


int main(int argc, char **argv)
{

	// Check command line arguments
	if (argc < 4) 
	{
		std::cout << 
		"Use with params: PSM1/PSM2; init_pos/no_init_pos; rate" 
		<< std::endl;
		return 1;
	}
	
	std::istringstream ss1(argv[3]);
	int rate_command;
	ss1 >> rate_command;
	
	double dt = 1.0/ rate_command;
	dvrk::Trajectory<double> to_enable_cartesian;
	dvrk::Trajectory<dvrk::Pose> circle_tr;
	
	
	// Initialize ros node
    ros::init(argc, argv, "irob_dvrk_auto_puncture_test");
    ros::NodeHandle nh;
    
    // Robot control
  	try {
    	dvrk_automation::Puncturer punct(nh, 
    			dvrk::ArmTypes::typeForString(argv[1]),
    			dt);
    	ros::Duration(1.0).sleep();
    	//psm.home(); 
		// Init position if necessary
		if (std::string(argv[2]) == "init_pos")
		{
			// init   	 		
   		}
  
   	
   		// Do preprogrammed movement
   	 	Eigen::Vector2d scanningArea(-50.0 / 1000.0, -50.0 / 1000.0);
		Eigen::Vector2i nLocations(3,3);
		int nTrials=2;
		double depth=5.0 / 1000.0; 
		double speed=1.0 / 1000.0; 
		double T=8.0; 
		std::string fileNameBase = "test";
					
   	 	punct.doPunctureSeries(scanningArea,
					nLocations, nTrials,
					depth, speed, T, 
					fileNameBase);
    
    	//psm.home();
    	ROS_INFO_STREAM("Program finished succesfully, shutting down ...");
    	
    } catch (const std::exception& e) {
  		ROS_ERROR_STREAM(e.what());
  		ROS_ERROR_STREAM("Program stopped by an error, shutting down ...");
  	}
    
    
    // Exit
    ros::shutdown();
	return 0;
}




