#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <geometry_msgs/Twist.h>
#include <iostream>

#define b 0.3

double r=0.1;

ros::Publisher pubR;
ros::Publisher pubL;


void c_callback(const geometry_msgs::TwistConstPtr &tws){
	std_msgs::Float32 vr, vl;

	ROS_INFO("received twist %f %f",tws->angular.z,tws->linear.x);

    vr.data = (tws->linear.x + ((b/2)*(tws->angular.z)))*10;
    vl.data = (tws->linear.x - ((b/2)*(tws->angular.z)))*10;
  
	pubR.publish(vr);
	pubL.publish(vl);
}
    
int main(int argc, char **argv){
    
    ros::init(argc,argv,"control");
    
    ros::NodeHandle control;

	ros::Subscriber twistSub = control.subscribe("/cross_bone/tws",1,c_callback);
    
    pubR = control.advertise<std_msgs::Float32>("/vrep/cross_bone/motorRightSpeed", 1);
    pubL = control.advertise<std_msgs::Float32>("/vrep/cross_bone/motorLeftSpeed", 1);
    
    ros::spin();
	return 0;
}
