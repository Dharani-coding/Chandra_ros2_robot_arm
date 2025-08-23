from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        # USB camera node
        Node(
            package='camera_ros',
            executable='camera_node',
            name='camera_node',
            output='screen',
            parameters=[
                {'video_device': '/dev/video2'},
                {'width': 640},
                {'height': 480},
                {'frame_rate': 30}
            ]
        ),

        # Box detector node
        Node(
            package='chandra_box_detector',
            executable='box_detector',
            name='box_detector',
            output='screen',
            parameters=[
                {'image_topic': '/camera/image_raw'}  # if you want to parametrize topic
            ]
        )
    ])
