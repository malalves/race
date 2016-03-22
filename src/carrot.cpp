#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <tf/tf.h>
#include <cmath>
#include <std_msgs/Float64.h>

ros::Publisher dYawPub;
double x[]={6.025,9.9, 11.125, 11.175, 11.925, 15.525, 8.175, -5.625, -14.4, -12.975, -1.2, -1.589, -12.765, -12.975, -9.8}, y[]={0.5, 0.2, -1.275, -8.65, -10.875, -14.987, -19.825, -19.825, -14.675, -12.675, -13, -11.091, -10.74, -2.425, 0};
double delta = 1;
double ru, theta, th, betha, r, psi, xc, yc;
int i=0,ra, n=15/*=numero de waypoints*/;
std_msgs::Float64 dYaw;

double normAng (double ang){
	while(ang/M_PI > 1){
		ang = ang - 2*M_PI;
	}
	while(ang/M_PI <= -1){
		ang = ang + 2*M_PI;
	}
	return ang;
}

void callback(const nav_msgs::OdometryConstPtr &od){
	ra = (i+1)%n;	
	ROS_INFO("counter+1 %i",ra);
	double yaw = tf::getYaw(od->pose.pose.orientation);
	if(sqrt(pow((x[ra]-od->pose.pose.position.x),2) + pow((y[ra] - od->pose.pose.position.y),2))<0.3) i = ra;
	
	ru = sqrt(pow((x[i]-od->pose.pose.position.x),2)+pow((y[i]-od->pose.pose.position.y),2));
	th = std::atan2((y[i]-od->pose.pose.position.y),(x[i]-od->pose.pose.position.x));
	theta = std::atan2((y[ra]-y[i]),(x[ra]-x[i]));
	betha = normAng(theta - th);
	r = sqrt((ru*ru)-pow(ru*std::sin(betha),2));

	xc = x[i]+((r+delta)*std::cos(theta));
	yc = y[i]+((r+delta)*std::sin(theta));
	psi = std::atan2((yc-od->pose.pose.position.y),(xc-od->pose.pose.position.x));	
	dYaw.data = normAng(psi-yaw);

	dYawPub.publish(dYaw);
	ROS_INFO("published dYaw");
}

int main(int argc, char **argv){
	ros::init(argc,argv,"carrot");
	ros::NodeHandle chaser;
	ros::Subscriber odSub = chaser.subscribe("/vrep/cross_bone/odometry",1, callback);
	dYawPub = chaser.advertise <std_msgs::Float64>("/cross_bone/dYaw",1);
	ros::spin();
	return 0;
}
