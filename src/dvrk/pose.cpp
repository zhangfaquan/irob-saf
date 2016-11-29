/*
 * pose.cpp
 *
 *  Created on: 2016. okt. 27.
 *      Author: tamas
 */

#include <iostream>
#include <math.h>
#include "dvrk/pose.hpp"

namespace dvrk {

 	Pose::Pose(): position(0.0, 0.0, 0.0), 
 			orientation(0.0, 0.0, 0.0, 0.0), jaw(0.0) {}
 			
   	Pose::Pose(double px, double py, double pz, 
   		double ow, double ox, double oy,double oz,
   		double jaw): 
   			position(px, py, pz),
   			orientation( ow, ox, oy, oz), jaw(jaw) {}
   			
   	Pose::Pose(const Pose& other): position(other.position), 
   		orientation(other.orientation), jaw(other.jaw) {}
   		
   	Pose::Pose(const geometry_msgs::Pose& msg, double jaw): 
		position(msg.position.x, msg.position.y, msg.position.z), 			orientation(msg.orientation.w, msg.orientation.x, msg.orientation.y,
				 msg.orientation.z), 
		jaw(jaw){}	
   		
	Pose::Pose(const geometry_msgs::PoseStamped& msg, double jaw): 
		position(msg.pose.position.x, msg.pose.position.y, msg.pose.position.z), 			orientation( msg.pose.orientation.w, msg.pose.orientation.x, msg.pose.orientation.y,
				 msg.pose.orientation.z), 
		jaw(jaw){}	

   	void Pose::swap(Pose& other) 
   	{
   		Pose tmp(*this);
   		
   		position = other.position;
   		orientation = other.orientation;
   		jaw = other.jaw;
   		
   		other.position = tmp.position;
   		other.orientation = tmp.orientation;
   		other.jaw = tmp.jaw;
   	}
   	
   	Pose Pose::operator=(const Pose& other)
   	{
   		Pose tmp(other);
   		this->swap(tmp);
   		return *this;
   	}
   	
   	Pose Pose::operator+=(const Eigen::Vector3d& v) 
   	{
   		position += v;
   		return *this;
   	}
   	
   	Pose Pose::operator-=(const Eigen::Vector3d& v) 
   	{
   		position -= v;   		
   		return *this;
   	}
   
   	
   	Pose Pose::operator+(const Eigen::Vector3d& v) const
   	{
   		Pose tmp(*this);
   		tmp += v;
   		return tmp;
   	}
   	
   	Pose Pose::operator-(const Eigen::Vector3d& v) const
   	{
   		Pose tmp(*this);
   		tmp -= v;
   		return tmp;
   	}   	
   	
   	Pose Pose::interpolate(double a, const Pose& other) const
   	{
   		Pose res;
   		res.orientation = orientation.slerp(a, other.orientation);
   		res.position = ((1.0-a) * position) + ((a) * other.position);
   		res.jaw = ((1.0-a) * jaw) + ((a) * other.jaw);
   		return res;
   	}
   	
   	geometry_msgs::Pose Pose::toRosPose() const
   	{
   		geometry_msgs::Pose ret;
   		ret.position.x = position.x();
   		ret.position.y = position.y();
   		ret.position.z = position.z();
   		ret.orientation.w = orientation.w();
   		ret.orientation.x = orientation.x();
   		ret.orientation.y = orientation.y();
   		ret.orientation.z = orientation.z();
   		return ret;
   	}
   	
   	std_msgs::Float32 Pose::toRosJaw() const
   	{
   	 	std_msgs::Float32 ret;
   	 	ret.data = jaw;
   	 	return ret;
   	}
   	
   	Pose::Distance Pose::dist(const Pose& other) const
   	{
   		Pose::Distance d;
   		d.cartesian = (position - other.position).norm();
   		double cosAlha1_2 = orientation.dot(other.orientation);
   		d.angle = std::abs((acos(cosAlha1_2) * 2.0*360.0)/(2.0*M_PI));
   		d.jaw = std::abs(jaw - other.jaw);
   		return d;
   	}
   	
   	Pose::Distance Pose::dist(const Eigen::Vector3d& otherPos) const
   	{
   		Pose::Distance d;
   		d.cartesian = (position - otherPos).norm();
   		d.angle = 0.0;
   		d.jaw = 0.0;
   		return d;
   	}
   	
   	Pose::Distance Pose::dist(const Eigen::Quaternion<double>& otherOrientation) const
   	{
   		Pose::Distance d;
   		d.cartesian = 0.0;
   		double cosAlha1_2 = orientation.dot(otherOrientation);
   		d.angle = std::abs((acos(cosAlha1_2) * 2.0*360.0)/(2.0*M_PI));
   		d.jaw = 0.0;
   		return d;
   	}
   	
   	Pose::Distance Pose::dist(double otherJaw) const
   	{
   		Pose::Distance d;
   		d.cartesian = 0.0;
   		d.angle = 0.0;
   		d.jaw = std::abs(jaw - otherJaw);
   		return d;
   	}
   	
   	Pose::Distance Pose::Distance::operator*=(double d)
   	{
   		cartesian *= d;
   		angle *= d;
   		jaw *= d;
   		return *this;
   	}
   	
    Pose::Distance Pose::Distance::operator/=(double d)
   	{
   		cartesian /= d;
   		angle /= d;
   		jaw /= d;
   		return *this;
   	}
   	
   	
    Pose::Distance Pose::Distance::operator*(double d) const
    {
    	 Pose::Distance ret(*this);
    	 ret *= d;
    	 return ret;
    }
    
    Pose::Distance Pose::Distance::operator/(double d) const
    {
    	 Pose::Distance ret(*this);
    	 ret /= d;
    	 return ret;
    }
   	
   	std::ostream& operator<<(std::ostream& os, const Pose& p)
   	{
   		return os << p.position.x() <<"\t" << p.position.y() <<"\t"
   					<< p.position.z() <<"\t"<< p.orientation.w() <<"\t"
   					<< p.orientation.x() <<"\t"<< p.orientation.y() <<"\t"
   					<< p.orientation.z() <<"\t" << p.jaw;
   	}
   	
   	std::istream& operator>>(std::istream& is, Pose& p)
   	{

   		is >> p.position.x() >> std::ws >> p.position.y() 
   		>> std::ws >> p.position.z() >> std::ws >> p.orientation.w()
   		>> std::ws >> p.orientation.x() >> std::ws >> p.orientation.y()
   		>> std::ws >> p.orientation.z()>> std::ws >> p.jaw >> std::ws;
   		return is;
   	}
   	
   	std::ostream& operator<<(std::ostream& os, const Pose::Distance& d)
   	{
   		return os << d.cartesian <<"\t" << d.angle <<"\t"
   					<< d.jaw;
   	}
   	
   	
   	
}
   	
   	
   	
   	
   	
   	
   	
   	
   	
   	
   	
   	
   	
   	
   	
   	
   	

