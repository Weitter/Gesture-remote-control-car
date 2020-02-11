#include"ros/ros.h"
#include"std_msgs/String.h"
#include"rosOpenCV/roscamera.h"
#include<sstream>
#include<string.h>
int main(int argc, char** argv)
{

	ros::init(argc, argv, "talker");  // 节点名称
	ros::NodeHandle n;

	// 话题topic的名称
	ros::Publisher roscamera_pub = n.advertise<rosOpenCV::roscamera>("roscamera_msg", 1000);
	ros::Rate loop_rate(10);
	int i=1;
	while( ros::ok() )
	{
		rosOpenCV::roscamera msg;
		msg.up = i+1;
		msg.down = i+2;
		msg.left = i+3;
		msg.right = i+4;
		i++;		
		roscamera_pub.publish(msg);

		ros::spinOnce();
		loop_rate.sleep();

	}
	
	return 0;
}

