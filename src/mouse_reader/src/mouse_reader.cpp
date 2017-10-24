#include "ros/ros.h"
#include "geometry_msgs/Point32.h"
#include <linux/input.h>
#include <string.h>
#include <map>
#include <vector>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <glob.h>	// for counting files in a dir (needed for counting event files in /dev/input)
#include <sstream>


/** Main function and a ROS publisher */
int main(int argc, char *argv[]) {

  // ROS init
  ros::init(argc, argv, "mouse_reader");
  // Use async spinner
  ros::AsyncSpinner spinner(1);
  spinner.start();

  // ROS node handle
  ros::NodeHandle nh;

  // Creates publisher that advertises Key messages on rostopic /keyboard
  ros::Publisher pub_mouse = nh.advertise<geometry_msgs::Point32>("mouseXY", 1);

  // Message for publishing key press events
  geometry_msgs::Point32 mouseXY;

  float x = 0;
  float y = 0;
  float z = 0;

  int fd = open("/dev/input/event18", O_RDONLY);
  int events;

  struct input_event ibuffer[64];
  mouseXY.z = 0;
  int i,r;
  while(ros::ok())
  {
    r = read(fd, ibuffer, sizeof(struct input_event) * 64);
    printf("%i\n", r);
    if( r > 0 )
    {
        events = r / sizeof(struct input_event);				// getting the number of events
        printf("%i\n", events);
        for(i=0; i<events; i++)						// going through all the read events
        {
          printf("for\n");
          switch(ibuffer[i].type)				// switch to a case based on the event type
          {
            case EV_SYN:				// this event is always present but no need to do anything
              printf("EV_SYN: code=0x%04x, value=0x%08x\n", ibuffer[i].code, ibuffer[i].value);
              break;
            case EV_MSC:				// this event is always present but no need to do anything
              printf("EV_MSC: code=0x%04x, value=0x%08x\n", ibuffer[i].code, ibuffer[i].value);
              break;
            case EV_REL:
              printf("EV_REL: code=0x%04x, value=0x%08x\n", ibuffer[i].code, ibuffer[i].value);
              if (ibuffer[i].code == 0) {
                x += ibuffer[i].value;
              }
              if (ibuffer[i].code == 1) {
                y += ibuffer[i].value;
              mouseXY.x = x;
              mouseXY.y = y;
              pub_mouse.publish(mouseXY);

              break;
              }
        }
      }
    }
    // mouseXY.x = x;
    // mouseXY.y = y;
    // pub_mouse.publish(mouseXY);
  } // end while
  return 0;
} //end main
