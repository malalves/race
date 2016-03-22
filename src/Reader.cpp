#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <geometry_msgs/Point.h>

ros::Publisher fPointPub;
geometry_msgs::Point fPoint;

void rCall ( const std_msgs::Float32ConstPtr &right){
	fPoint.x = right->data;
}

void lCall ( const std_msgs::Float32ConstPtr &left){
	fPoint.y = left->data;
}

void fCall ( const std_msgs::Float32ConstPtr &front){
	fPoint.z = front->data;

	fPointPub.publish (fPoint);
	ROS_INFO("published fPoint");
}

int main(int argc, char **argv){
	ros::init(argc,argv,"reader");
	ros::NodeHandle reader;
	ros::Subscriber rSub = reader.subscribe("/vrep/cross_bone/rightSonar",1,rCall);
	ros::Subscriber lSub = reader.subscribe("/vrep/cross_bone/leftSonar",1,lCall);
	ros::Subscriber fSub = reader.subscribe("/vrep/cross_bone/frontSonar",1,fCall);
	fPointPub = reader.advertise<geometry_msgs::Point>("/cross_bone/fPoint",1);
	ros::spin();

	return 0;
}

/* equivalencias sonar fake point
	rightSonar - x
	leftSonar - y
	frontSonar - z
*/
