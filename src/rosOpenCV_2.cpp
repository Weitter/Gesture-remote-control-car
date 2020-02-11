#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <stdio.h>
 
int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_publisher");
  ros::NodeHandle nh;
  image_transport::ImageTransport it(nh);//用之前声明的节点句柄初始化it，其实这里的it和nh的功能基本一样，可以像之前一样使用it来发布和订阅相消息。
  image_transport::Publisher pub = it.advertise("camera/image", 1);
 
  cv::Mat image = cv::imread("/home/weitter/Pictures/1.jpg", CV_LOAD_IMAGE_COLOR);
  if(image.empty())
  {
    printf("open error\n");
  }
  sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", image).toImageMsg();//图像格式转换
 
  ros::Rate loop_rate(5);//每秒5帧
  while (nh.ok()) 
  {
    pub.publish(msg);
    ros::spinOnce();
    loop_rate.sleep();
  }
}

