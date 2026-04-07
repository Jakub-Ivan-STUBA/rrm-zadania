from launch import LaunchDescription
from launch.actions import ExecuteProcess
from launch_ros.actions import Node
from launch.substitutions import Command, PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    return LaunchDescription([
        
        # Joint State Publisher Gui Node
        Node(
            package="joint_state_publisher_gui",
            executable="joint_state_publisher_gui",
            name="joint_state_publisher_gui"
        ),

        # Robot State Publisher Node
        Node(
            package="robot_state_publisher",
            executable="robot_state_publisher",
            name="robot_state_publisher",
            output="screen",
            parameters=[{
                'robot_description': Command([
                    'xacro ', PathJoinSubstitution([
                        FindPackageShare('abb_model'),
                        'urdf', 'abb_irb4600_60_205.xacro'
                    ])
                ])
            }]
        ),

        # RViz2 Node
        Node(
            package="rviz2",
            executable="rviz2",
            name="rviz2",
            output="screen"
        )
    ])
