#include "ros/ros.h"
#include "std_msgs/String.h"
#include"rosOpenCV/roscamera.h"
void chatterCalllback(const rosOpenCV::roscamera msg)
{
	ROS_INFO("Header/\n");
	ROS_INFO("	up [%d]\n",msg.up);
	ROS_INFO("	down[%d]\n", msg.down);
	ROS_INFO("	left[%d]\n",msg.left);
	ROS_INFO("	right[%d]\n", msg.right);

}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "listener");
	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe( "roscamera_msg", 1000, chatterCalllback );
	ros::spin();
	return 0;
}

