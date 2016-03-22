#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Twist.h>

#define b 0.3

ros::Publisher twsPub;
geometry_msgs::Twist tws;
double dYaw;

/* equivalencias sonar fake point
	rightSonar - x
	leftSonar - y
	frontSonar - z
*/

double lSpeedSet(double w){
	if(1.5 + b*w/2 >= 1.5 - b*w/2){
		return 	1.5 - b*w/2;
	}
	else{
		return 	1.5 + b*w/2;
	}
	return 1;
}

void yawCall(const std_msgs::Float64ConstPtr &msg){
	dYaw = msg->data;
}

void fpCall(const geometry_msgs::PointConstPtr &fPoint){
	if(fPoint->x==0 && fPoint->y==0 && fPoint->z==0){
		tws.angular.z = 3*dYaw;
		tws.linear.x = lSpeedSet(tws.angular.z);
	}
	else if(fPoint->x!=0 && fPoint->y!=0 && fPoint->z!=0){
		tws.angular.z = 2.5*dYaw;
		tws.linear.x = (4*fPoint->z - 1)*lSpeedSet(tws.angular.z);
	}
	else if(fPoint->x!=0 && fPoint->y==0 && fPoint->z==0){
		if(dYaw>=-0.5){
			tws.angular.z = 2.5*dYaw;
			tws.linear.x = lSpeedSet(tws.angular.z);
		}
		else{
			tws.angular.z = 2.5*dYaw+(0.6-fPoint->x);
			tws.linear.x = lSpeedSet(tws.angular.z);
		}
	}
	else if(fPoint->x==0 && fPoint->y!=0 && fPoint->z==0){
		if(dYaw<=0.5){
			tws.angular.z = 2.5*dYaw;
			tws.linear.x = lSpeedSet(tws.angular.z);
		}
		else{
			tws.angular.z = 2.5*dYaw-(0.6-fPoint->y);
			tws.linear.x = lSpeedSet(tws.angular.z);
		}
	}
	else if(fPoint->x==0 && fPoint->y==0 && fPoint->z!=0){
		if(dYaw>=0.5){
			tws.angular.z = 2.5*dYaw-(0.6-fPoint->z);
			tws.linear.x = lSpeedSet(tws.angular.z)*(4*fPoint->z - 1);
		}
		else{
			tws.angular.z = 2.5*dYaw+(0.6-fPoint->z);
			tws.linear.x = lSpeedSet(tws.angular.z)*(4*fPoint->z - 1);
		}
	}
	else if(fPoint->x!=0 && fPoint->y==0 && fPoint->z!=0){
		tws.angular.z = 2.5*dYaw+(0.6-fPoint->x);
		tws.linear.x = lSpeedSet(tws.angular.z)*(4*fPoint->z - 1);
	}
	else if(fPoint->x==0 && fPoint->y!=0 && fPoint->z!=0){
		tws.angular.z = 2.5*dYaw-(0.6-fPoint->y);
		tws.linear.x = lSpeedSet(tws.angular.z)*(4*fPoint->z - 1);
	}
	twsPub.publish(tws);
}

int main(int argc, char **argv){
	ros::init(argc,argv,"CPU");
	ros::NodeHandle cpu;
	ros::Subscriber dYawSub = cpu.subscribe("/cross_bone/dYaw",1,yawCall);
	ros::Subscriber fPointSub = cpu.subscribe("/cross_bone/fPoint",1,fpCall);
	twsPub = cpu.advertise <geometry_msgs::Twist>("/cross_bone/tws",1);
	ros::spin();
	return 0;
}
