/*
 * 	palpate.cpp
 *
 *	Author(s): Tamas D. Nagy
 *	Created on: 2020-01-27
 *
 */

#include <irob_subtask_logic/palpate.hpp>


namespace saf {


Palpate::Palpate(ros::NodeHandle nh, ros::NodeHandle priv_nh,
             std::vector<std::string> arm_names):
  AutosurgAgent(nh, priv_nh, arm_names), optoforce(nh)
{	
  dt = 0.05;
}

Palpate::~Palpate()
{
  // TODO Auto-generated destructor stub
}


void Palpate::palpateSample(std::string filename)
{



  // NaN pose received, until the vision node starts
  Eigen::Vector3d force = makeNaN<Eigen::Vector3d>();
  while (isnan(force)
         && ros::ok())
  {
    force = optoforce.getForcesCurrent();
    ros::Duration(0.1).sleep();
  }
  ROS_INFO_STREAM("Forces received: " << force);
  ROS_INFO_STREAM("Calibrating sensor...");
  optoforce.calibrateOffsets();
  ROS_INFO_STREAM("Sensor calibration done...");



  // Find tissue surface
  ROS_INFO_STREAM("Finding tissue surface...");
  double surface_threshold = 10.0;
  force = optoforce.getForcesCurrent();
  double find_surface_step = 0.1;
  Eigen::Vector3d find_surface_displacement = find_surface_step *
                          BaseDirections<CoordinateFrame::ROBOT,
                          Eigen::Vector3d>::DOWN;
  while(force.z() < surface_threshold)
  {
    // Send navigate surgeme action to the surgeme server.
    arms[0] -> manipulate(find_surface_displacement, 10.0);
    while(!arms[0] -> isSurgemeDone() && ros::ok())
    {
      ros::Duration(0.05).sleep();
    }
    force = optoforce.getForcesCurrent();
  }
  ROS_INFO_STREAM("Tissue surface found successfully.");


  // Do palpation
  ROS_INFO_STREAM("Starting palpation...");
  double palpation_depth = 8.0;
  Pose p = arms[0] -> getPoseCurrent();
  Eigen::Vector3d palpation_displacement = palpation_depth *
                            BaseDirections<CoordinateFrame::ROBOT,
                            Eigen::Vector3d>::DOWN;

  // Send navigate surgeme action to the surgeme server.
  std::vector<double> d, f;
  ros::Rate loop_rate(1.0/dt);
  arms[0] -> manipulate(palpation_displacement, 10.0);
  while(!arms[0] -> isSurgemeDone() && ros::ok())
  {
    Pose p_curr = arms[0] -> getPoseCurrent();
    d.push_back(p_curr.dist(p).cartesian);
    f.push_back(optoforce.getForcesCurrent().z());
    loop_rate.sleep();
  }
  ROS_INFO_STREAM("Palpation executed succesfully.");
  ROS_INFO_STREAM(d);
  ROS_INFO_STREAM(f);
  writeData(filename, "comment", d, f);

}

void Palpate::writeData(std::string filename, std::string comment,
          std::vector<double>& depths, std::vector<double>& forces)
{
  std::ofstream logfile;
    logfile.open (filename.c_str(), std::ofstream::out | std::ofstream::app);

    if (!logfile.is_open())
    throw std::runtime_error("Cannot open file " + filename);

  // The first line is always the comment
  logfile << comment << std::endl;
  // The second line is always the dt
  logfile << dt << std::endl;
  for (int i = 0; i < depths.size(); i++)
    logfile << depths[i] << "\t" << forces[i]<< std::endl;

  logfile.flush();
  logfile.close();
  ROS_INFO_STREAM("Measured data saved to " << filename);
}


}

using namespace saf;

/**
 * Maneuver server main
 */
int main(int argc, char **argv)
{

  // Initialize ros node
  ros::init(argc, argv, "dummy_grasp");
  ros::NodeHandle nh;
  ros::NodeHandle priv_nh("~");

  std::vector<std::string> arm_names;
  priv_nh.getParam("arm_names", arm_names);

  std::string filename;
  priv_nh.getParam("filename", filename);



  // Start autonomous agent
  try {
    Palpate pnp(nh, priv_nh, arm_names);

    pnp.palpateSample(filename);

    ROS_INFO_STREAM("Program finished succesfully, shutting down ...");

  } catch (const std::exception& e) {
    ROS_ERROR_STREAM(e.what());
    ROS_ERROR_STREAM("Program stopped by an error, shutting down ...");
  }


  // Exit
  ros::shutdown();
  return 0;
}


























