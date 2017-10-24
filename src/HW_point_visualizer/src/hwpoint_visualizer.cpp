#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/PointStamped.h"
#include "geometry_msgs/Point32.h"
#include <sstream>
float x;
float y;

void mouseCallback(const geometry_msgs::Point32 point) {
  x = point.x;
  y = point.y;
}

/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
int main(int argc, char **argv)
{
  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line.
   * For programmatic remappings you can use a different version of init() which takes
   * remappings directly, but for most command-line programs, passing argc and argv is
   * the easiest way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
  ros::init(argc, argv, "hwpoint_visualizer");
  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle n;

  ros::Publisher hwpointpublisher = n.advertise<geometry_msgs::PointStamped>("pointXY", 1);
  ros::Subscriber hwpointsubscriber = n.subscribe("mouseXY",1, mouseCallback);
  ros::Rate loop_rate(100);
  // ros::AsyncSpinner spinner(1);
  // spinner.start();
  /**
   * A count of how many messages we have sent. This is used to create
   * a unique string for each message.
   */
  float count = 0;
  while (ros::ok())
  {
    /**
     * This is a message object. You stuff it with data, and then publish it.
     */
    geometry_msgs::PointStamped point;

    point.header.seq = count;
    point.header.stamp = ros::Time::now();
    point.header.frame_id = "box";
    point.point.x = x/100;
    point.point.y = y/100;
    point.point.z = 0;

    //ROS_INFO("%s", point.data.c_str());

    /**
     * The publish() function is how you send messages. The parameter
     * is the message object. The type of this object must agree with the type
     * given as a template parameter to the advertise<>() call, as was done
     * in the constructor above.
     */
    hwpointpublisher.publish(point);

    ros::spinOnce();

    loop_rate.sleep();
    count++;
  }


  return 0;
}
