from setuptools import find_packages, setup

package_name = 'chandra_box_detector'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        ('share/' + package_name + '/launch', ['launch/box_detector_launch.py']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='dharani-saravanan',
    maintainer_email='dharanisara14@gmail.com',
    description='TODO: Package description',
    license='TODO: License declaration',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'box_detector = chandra_box_detector.box_detector:main',
        ],
    },
)
