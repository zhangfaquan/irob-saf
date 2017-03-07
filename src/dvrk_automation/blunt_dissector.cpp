/*
 *  blunt_dissector.cpp
 *
 *	Author(s): Tamas D. Nagy
 *	Created on: 2017-02-08
 *  
 */

#include "dvrk_automation/blunt_dissector.hpp"
#include "dvrk/trajectory_factory.hpp"
#include <numeric>
#include <chrono>


namespace dvrk_automation {

// Constants
//const double BluntDissector::travelSpeed  = 20.0 / 1000.0; // m/s

BluntDissector::BluntDissector(
	ros::NodeHandle nh, dvrk::ArmTypes arm_typ, double dt,
								 std::string regfile_name): 
							nh(nh), psm(nh, arm_typ, dvrk::PSM::ACTIVE),
							vision(nh, regfile_name), dt(dt)
{
		psm.setRobotState(dvrk::PSM::STATE_POSITION_CARTESIAN);
		waitForTopicsInit();
}

BluntDissector::~BluntDissector()
{
	// TODO Auto-generated destructor stub
}

void BluntDissector::dissect()
{
	try {	// TODO err handling
			
			//toolClose();
			ROS_INFO_STREAM("Tool closed, starting dissection.");
			// Do this while the vision says it is done
			while (!vision.isTaskDone())	
			{
				// Ask for new target
				ROS_INFO_STREAM("Ask for new target");
				vision.sendSubtaskStatusWithAck(
						SubtaskStatus::
						NEW_DISSECTION_TARGET_NEEDED.getCommand(),
						SubtaskStatus::
						NEW_DISSECTION_TARGET_NEEDED.getAck());
				
				// Go to target and make dissection
				ROS_INFO_STREAM("Go to target and make dissection.");
				goToTarget(1.0);
				ros::Duration(1.0).sleep();
				vision.sendSubtaskStatus(SubtaskStatus::
						PERFORMING_DISSECTION.getCommand());
				toolPushIn(5.0);
				ros::Duration(1.0).sleep();
				toolOpen(30.0);
				ros::Duration(1.0).sleep();
				toolPullOut(5.0);
				ros::Duration(1.0).sleep();
				toolClose(0.0, 20.0);
				ros::Duration(1.0).sleep();
				
				// Ask for new distant target
				ROS_INFO_STREAM("Ask for new distant target.");
				vision.sendSubtaskStatusWithAck(
						SubtaskStatus::
						NEW_DISTANT_TARGET_NEEDED.getCommand(),
						SubtaskStatus::
						NEW_DISTANT_TARGET_NEEDED.getAck());
				
				// Go to distant target
				ROS_INFO_STREAM("Go to distant target.");
				goToTarget(1.0);
				ros::Duration(1.0).sleep();
			}		
		
	} catch (const std::exception& e) {
		// Abort task
		vision.sendSubtaskStatus(SubtaskStatus::
						ABORT.getCommand());
  		ROS_ERROR_STREAM(e.what());
  		ROS_ERROR_STREAM(
  		 "Program stopped by an error, send ABORT command to vision node ...");
  	}

}

void BluntDissector::waitForTopicsInit()
{
	// TODO wait for vision init?
	ros::Rate loop_rate(1.0/dt);
	while (ros::ok() && psm.getPoseCurrent().position.norm() < 0.001
			/*&& oforce.getForcesCurrent().norm() < 0.1*/)
  	{
  		loop_rate.sleep();
  	}
}

/**
 * Push tool between tissues in the current direction.
 *
 * @param stepT time for planning subtrajectories in s
 * @param speed base movement speed in mm/s
 */
void BluntDissector::goToTarget(double stepT, double speed /* = 4.0 */)
{
	bool dp_reached = false;
	bool goal_reached = false;
	double step_distance = std::abs(stepT * speed)/1000.0; // m
	
	while (!goal_reached)
	{
		dp_reached = false;
		goal_reached = false;
		vision.sendSubtaskStatusWithAck(
							SubtaskStatus::
							WAITING_FOR_TARGET.getCommand(),
							SubtaskStatus::
							WAITING_FOR_TARGET.getAck());
		while (!vision.isTargetValid()) 
		{
			ros::Duration(0.1).sleep();
		}
		while (!dp_reached)
		{
			dvrk::Pose pose_current = psm.getPoseCurrent();
			dvrk::Pose end_target = vision.getTargetCurrent();
		
								
			double end_distance = (pose_current.dist(end_target)).cartesian;
		
			dvrk::Pose step_target;
		
			if (end_distance > step_distance)
			{
				step_target = dvrk::interpolate(step_distance/end_distance,
											 pose_current, end_target);
				dp_reached = false;
				
				vision.sendSubtaskStatus(SubtaskStatus::
						GOING_TO_TARGET.getCommand());
			}
			else
			{
				step_target = end_target;
				dp_reached = true;
			}
		
			dvrk::Trajectory<dvrk::Pose> to_target 	= dvrk::TrajectoryFactory::
						linearTrajectoryForTime(pose_current,step_target, 
												stepT, dt);
			checkTrajectory(to_target);
			psm.playTrajectory(to_target);
		}
		
		if (vision.getTargetType() == dvrk_vision::TargetType::DP)
		{
			vision.sendSubtaskStatusWithAck(
							SubtaskStatus::
							DP_REACHED.getCommand(),
							SubtaskStatus::
							DP_REACHED.getAck());
			goal_reached = false;
			dp_reached = true;
			ROS_INFO_STREAM("DP reached.");
		} 
		else if (vision.getTargetType() == dvrk_vision::TargetType::GOAL)
		{
			vision.sendSubtaskStatusWithAck(
							SubtaskStatus::
							GOAL_REACHED.getCommand(),
							SubtaskStatus::
							GOAL_REACHED.getAck());
			goal_reached = true;
			dp_reached = true;
			ROS_INFO_STREAM("Goal reached.");
		}
		else
		{
			throw std::runtime_error("Wrong target type received... ");
		}
	}
}

/**
 * Push tool between tissues in the current direction.
 *
 * @param depth insertion depth in mm
 * @param speed insertion speed in mm/s
 */
void  BluntDissector::toolPushIn(double depth, double speed /* = 2.0 */)
{
	Eigen::Vector3d v(0,0,1);
	dvrk::Pose p1 = psm.getPoseCurrent();
	Eigen::Matrix3d R = p1.orientation.toRotationMatrix();
	v = R*v;
	dvrk::Pose p2 = p1+((std::abs(depth)/1000.0)*v);
	double T = std::abs(depth / speed);
	dvrk::Trajectory<dvrk::Pose> tr = dvrk::TrajectoryFactory::
   				linearTrajectoryWithSmoothAcceleration(
   						p1, 
   						p2,
   						T, T*0.1, dt);
   					
   	// Safety
   	checkTrajectory(tr);						
   						
   	psm.playTrajectory(tr); 
}

/**
 * Push tool out of tissues in the current direction.
 *
 * @param depth pull distance in mm
 * @param speed pull speed in mm/s
 */	
void  BluntDissector::toolPullOut(double depth, double speed /* = 2.0 */)
{
	Eigen::Vector3d v(0,0,1);
	dvrk::Pose p1 = psm.getPoseCurrent();
	Eigen::Matrix3d R = p1.orientation.toRotationMatrix();
	v = R*v;
	dvrk::Pose p2 = p1-((std::abs(depth)/1000.0)*v);
	double T = std::abs(depth / speed);
	dvrk::Trajectory<dvrk::Pose> tr = dvrk::TrajectoryFactory::
   				linearTrajectoryWithSmoothAcceleration(
   						p1, 
   						p2,
   						T, T*0.1, dt);
   	
   	// Safety
   	checkTrajectory(tr);						
   						
   	psm.playTrajectory(tr); 
}

/**
 * Open tool to separate tissues.
 *
 * @param angle angle of jaws in deg
 * @param speed opening speed in deg/s
 */	
void  BluntDissector::toolOpen(double angle, double speed /* = 10.0 */)
{
	dvrk::Pose p1 = psm.getPoseCurrent();
	
	double angle_rad = (std::abs(angle)/360.0)* M_PI * 2.0;
	double speed_rad = (std::abs(speed)/360.0)* M_PI * 2.0;
		
	double T = std::abs((angle_rad-p1.jaw) / speed_rad);
	
		
	dvrk::Pose p2 = p1;
	p2.jaw = angle_rad;
	
	dvrk::Trajectory<dvrk::Pose> tr = dvrk::TrajectoryFactory::
   				linearTrajectoryWithSmoothAcceleration(
   						p1, 
   						p2,
   						T, T*0.1, dt);
   	
   	// Safety
   	checkTrajectory(tr);						
   						
   	psm.playTrajectory(tr); 
}

/**
 * Close tool after separated tissues.
 *
 * @param angle angle of jaws in deg, default is fully closed
 * @param speed closing speed in deg/s
 */	
void  BluntDissector::toolClose(double angle /* = 0.0 */, 
								double speed /* = 10.0 */)
{
	dvrk::Pose p1 = psm.getPoseCurrent();
	
	double angle_rad = (std::abs(angle)/360.0)* M_PI * 2.0;
	double speed_rad = (std::abs(speed)/360.0)* M_PI * 2.0;
		
	double T = std::abs((angle_rad-p1.jaw) / speed_rad);
		
	dvrk::Pose p2 = p1;
	p2.jaw = angle_rad;
	
	dvrk::Trajectory<dvrk::Pose> tr = dvrk::TrajectoryFactory::
   				linearTrajectoryWithSmoothAcceleration(
   						p1, 
   						p2,
   						T, T*0.1, dt);
   	// Safety
   	checkTrajectory(tr);					
   						
   	psm.playTrajectory(tr); 
}

/**
 * Rotate tool.
 *
 * @param angle angle of rotation
 * @param speed rotation speed in deg/s
 */	
void  BluntDissector::toolRotate(double angle, double speed /* = 10.0 */)
{
	double angle_rad = (angle/360.0)* M_PI * 2.0;
	double speed_rad = (speed/360.0)* M_PI * 2.0;
		
	double T = std::abs(angle_rad / speed_rad);

	dvrk::Pose p1 = psm.getPoseCurrent();
	Eigen::Vector3d v(0,0,1);
	Eigen::Matrix3d R = p1.orientation.toRotationMatrix();
	v = R*v;
	v.normalize();
	Eigen::AngleAxis<double> t(Eigen::AngleAxis<double>(angle_rad,v));
	
	Eigen::Quaternion<double> ori = p1.orientation;
	
	ori = t * ori;
	
	dvrk::Pose p2 = p1;
	p2.orientation = ori;
	
	dvrk::Trajectory<dvrk::Pose> tr = dvrk::TrajectoryFactory::
   				linearTrajectoryWithSmoothAcceleration(
   						p1, 
   						p2,
   						T, T*0.1, dt);
   	// Safety
   	checkTrajectory(tr);					
   						
   	psm.playTrajectory(tr); 
}


void BluntDissector::safetyCheck()
{
	// Check position, jaw and force
	checkPoseCurrent();
	
}


void  BluntDissector::checkTrajectory(dvrk::Trajectory<dvrk::Pose> tr)
{
	for (int i = 0; i < tr.size(); i++)
		checkPose(tr[i]);
}

void  BluntDissector::checkPoseCurrent()
{
	checkPose(psm.getPoseCurrent());
}



void  BluntDissector::checkPose(dvrk::Pose p)
{
	Eigen::Vector3d position = p.position;
	
	//TODO
	
	//throw std::runtime_error(errstring.str());
}




}