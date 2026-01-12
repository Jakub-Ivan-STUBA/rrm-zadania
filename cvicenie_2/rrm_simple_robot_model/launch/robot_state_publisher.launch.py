import os

import launch
import launch_ros.actions
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    pkg_share = FindPackageShare('rrr_simple_robot_model').find('rrr_simple_robot_model')
    urdf_dir = os.path.join(pkg_share, 'urdf')
    urdf_file = os.path.join(urdf_dir, 'arm.urdf')
    with open(urdf_file, 'r') as infp:
        robot_desc = infp.read()

    params = {'robot_description': robot_desc}
    rsp = launch_ros.actions.Node(package='robot_state_publisher',
                                  executable='robot_state_publisher',
                                  output='both',
                                  parameters=[params])

    return launch.LaunchDescription([rsp])