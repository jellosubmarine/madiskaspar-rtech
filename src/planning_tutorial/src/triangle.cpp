#include <ros/ros.h>

// MoveIt!
#include <moveit/move_group_interface/move_group.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>

#include <moveit_msgs/DisplayRobotState.h>
#include <moveit_msgs/DisplayTrajectory.h>

#include <moveit_msgs/AttachedCollisionObject.h>
#include <moveit_msgs/CollisionObject.h>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "movegroup_triangle");
  ros::NodeHandle node_handle;
  ros::AsyncSpinner spinner(1);
  spinner.start();

  /* This sleep is ONLY to allow Rviz to come up */
  sleep(15.0);

  moveit::planning_interface::MoveGroup group("sia5");

  // We will use the :planning_scene_interface:`PlanningSceneInterface`
  // class to deal directly with the world.
  moveit::planning_interface::PlanningSceneInterface planning_scene_interface;

  // (Optional) Create a publisher for visualizing plans in Rviz.
  ros::Publisher display_publisher = node_handle.advertise<moveit_msgs::DisplayTrajectory>("/move_group/display_planned_path", 1, true);
  moveit_msgs::DisplayTrajectory display_trajectory;

  // We can print the name of the reference frame for this robot.
  ROS_INFO("Reference frame: %s", group.getPlanningFrame().c_str());

  // We can also print the name of the end-effector link for this group.
  ROS_INFO("Reference frame: %s", group.getEndEffectorLink().c_str());

  // Planning to a Pose goal
  // ^^^^^^^^^^^^^^^^^^^^^^^

  group.setJointValueTarget(group.getNamedTargetValues("second_corner"));

  moveit::planning_interface::MoveGroup::Plan my_plan;
  bool success = group.plan(my_plan);
  group.move();


  ROS_INFO("Visualizing plan 1 (pose goal) %s",success?"":"FAILED");
  /* Sleep to give Rviz time to visualize the plan. */
  sleep(2.0);
  display_trajectory.trajectory_start = my_plan.start_state_;
  display_trajectory.trajectory.push_back(my_plan.trajectory_);

  group.setJointValueTarget(group.getNamedTargetValues("third_corner"));
  success = group.plan(my_plan);
  group.move();
  ROS_INFO("Visualizing plan 2 (pose goal) %s",success?"":"FAILED");
  sleep(2.0);
  display_trajectory.trajectory.push_back(my_plan.trajectory_);

  group.setJointValueTarget(group.getNamedTargetValues("home_pose"));
  success = group.plan(my_plan);
  group.move();
  ROS_INFO("Visualizing plan 3 (pose goal) %s",success?"":"FAILED");
  sleep(2.0);
  display_trajectory.trajectory.push_back(my_plan.trajectory_);



  if (1)
  {
    ROS_INFO("Visualizing plan 1 (again)");
    display_publisher.publish(display_trajectory);
    /* Sleep to give Rviz time to visualize the plan. */
    sleep(5.0);
  }

  ros::shutdown();

  return 0;
}
