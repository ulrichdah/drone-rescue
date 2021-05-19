# General setup

- Clone this repo in `/home/dji`

- Copy `robot/bash_aliases` file to `~/.bash_aliases`: run `cp robot/bash_aliases ~/.bash_aliases`

- `cp /home/dji/drone-rescue/robot/object-detection/txdocker /usr/local/bin/`

- Install catkin tools `sudo apt-get install -y --no-install-recommends python-catkin-tools`

- Install camera driver: `sudo apt install ros-kinetic-realsense2-camera` 

- Avoid using sudo for docker: `sudo groupadd docker && sudo usermod -aG docker $USER`

- Place this file https://github.com/IntelRealSense/librealsense/blob/master/config/99-realsense-libusb.rules in `/etc/udev/rules.d/`

# Drone control stack

- Clone `https://git.mistlab.ca/dasto/drones` and checkout branch `ros_drones_ws`. Follow the instructions from there. Use the `build_m100.sh` script to build the drone stack.

# Networking

- Install batman-adv: download the alfred, batctl and batman-adv sources from
  `https://downloads.open-mesh.org/batman/releases/batman-adv-2020.4/`. Extract
  the `.tar.gz` archives and run `make` then `make install` in each.

- Change hostname in `/etc/hostname`

- `sudo cp /home/dji/drone-rescue/robot/config/copymac.sh
  /usr/local/bin/copymac.sh`

- `sudo cp /home/dji/drone-rescue/robot/config/batman-cl@.service
  /etc/systemd/system/batman-cl@.service`

- Make sure `/etc/network/interfaces``contains only:
```
auto lo
iface lo inet loopback
```

- `sudo systemctl enable batman-cl@wlan0`, then reboot.

- Add IP infos for ROS in `~/.bashrc`: Append the following lines: 
```
source /opt/ros/kinetic/setup.bash

export ROS_IP=192.168.143.125
export ROBOT_ID=0
```

- Replace `192.168.143.125` with the bat0 IP address, `0` with the robot ID. Make the changes in `~/.bashrc`

# Containers

- In `robot/object-detection/`, run : `docker build . --tag lajoiepy/object-detection`
- In `robot/bridge/`, run : `docker build . --tag lajoiepy/ros-bridge`

# How to run

All the important commands have an alias in `~/.bash_aliases`.

- `cameras` : launches the cameras
- `objects` : launches the object detection
- `ros2bridge` : launches the ROS1 to ROS2 bridge
