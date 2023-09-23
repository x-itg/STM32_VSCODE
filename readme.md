##### 一、工程搭建

- makefile文件由stm32cube生成后修改
- 事先安装的软件：vscode cortex-debug插件；tup-latest、 openocd 、arm-gdb/gcc 、GNU MCU Eclipse安装到C:\Program Files文件夹，它们的bin文件夹路径加入到path环境变量当中。
- 修改的stm32f1discovery.cfg文件放到C:\Program Files\OpenOCD\0.10.0-13\scripts\board的文件夹；这个make upload烧入的时候用到

##### 二、编译指令

`make`

##### 三、烧入指令

`make upload`

##### 四、进入调试

`Fn+F5`
---

## 事先安装这些软件

```
sudo apt-get install lsb-core
sudo apt-get install build-essential pkg-config autoconf automake libtool libusb-dev libusb-1.0-0-dev libhidapi-dev
sudo apt-get install libtool libsysfs-dev  
```

# WSL USB驱动 我用wsl下的ubuntu

- 安装usb驱动： https://kgithub.com/dorssel/usbipd-win
- ubunutu下安装：sudo apt install linux-tools-generic hwdata
- ubunutu下安装：sudo update-alternatives --install /usr/local/bin/usbip usbip /usr/lib/linux-tools/*-generic/usbip 20
- powershell下：usbipd wsl list
- powershell下连接usb连到wsl：usbipd wsl attach --busid 2-1
- powershell下连接usb断开wsl：usbipd wsl detach --busid 2-1
- ubuntu下查看：lsusb

## 下载armgccgdb添加环境变量vscode安装cortex-debug插件

```

ubuntu下安装stm32cubeclt之前有安装过的可以卸载掉：https://www.st.com/en/development-tools/stm32cubeclt.html

下载好后 sudo bash stm32cubeclt.sh 安装

arm-none-eabi-gcc -v
.vscode文件夹下settings.json：
{
    "cortex-debug.armToolchainPath": "/opt/st/stm32cubeclt_1.12.1/GNU-tools-for-STM32/bin/arm-none-eabi-gcc",
    "cortex-debug.gdbPath": "/opt/st/stm32cubeclt_1.12.1/GNU-tools-for-STM32/bin/arm-none-eabi-gdb" 
}

.vscode文件夹下launch.json:
{
    // See https://go.microsoft.com/fwlink/?LinkId=733558
    // for the documentation about the tasks.json format
    "version": "0.2.0",
  
    "configurations": [
        {
            "name": "wsl-ubuntu1804-stm32f1-openocd", //调试入口显示的名字，随便起
            "cwd": "${workspaceRoot}", //工作目录，根路径
            "armToolchainPath": "${config:cortex-debug.armToolchainPath}",  
            "gdbPath": "${config:cortex-debug.gdbPath}",
            "executable": "${workspaceFolder}/build/EC20103RB.elf", //调试文件
            "request": "launch",
            "runToMain": true,
            "type": "cortex-debug", //配置为使用插件调试
            "servertype": "openocd", //映射openocd
            "configFiles": [
                "${workspaceRoot}/openocd.cfg"
            ], //openocd配置
            "postDebugTask": "Reset" //同上，调试结束执行的任务
        }
        ,{
            // For the Cortex-Debug extension
            "type": "cortex-debug",
            "servertype": "openocd-openocd",
            "request": "launch",
            "name": "windows-stm32f1",
            "executable": "${workspaceRoot}/build/EC20103RB.elf",
            "configFiles": [
                "C:/Program Files/OpenOCD/0.10.0-13/scripts/interface/stlink-v2.cfg",
                "C:/Program Files/OpenOCD/0.10.0-13/scripts/target/stm32f1x_stlink.cfg",
            ],
            "cwd": "${workspaceRoot}"
        }
    ]
}
```

## 下载openocd源码 使能stlink 默认没有的

```

根据这篇文章 安装openocd：https://blog.csdn.net/daoshengtianxia/article/details/115038674
git clone https://gitee.com/daoshengtianxia/openocd.git
下载openocd及其子模块后编译安装
sudo ./bootstrap
./configure --enable-stlink
sudo make
sudo make install
安装在了/usr/local/bin
配置文件在 /usr/local/share/openocd/scripts
```

### Makefile文件中 添加 可以使用make update命令烧入代码

```
update:
	openocd -f openocd.cfg -c init -c halt -c "program $(BUILD_DIR)/$(TARGET).hex verify reset exit"
reset:
	openocd -f openocd.cfg -c init -c halt -reset -c shutdown
```

### 在工作目录下添加openocd.cfg文件，内容：## stlink-v2.cfg 不对劲直接使用 stlink.cfg

```
source [find /usr/local/share/openocd/scripts/interface/stlink.cfg]
source [find /usr/local/share/openocd/scripts/target/stm32f1x.cfg]
```

# WSL 图形界面

- https://learn.microsoft.com/zh-cn/windows/wsl/install-manual#step-4--download-the-linux-kernel-update-package
- 管理员运行powershell：dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart
- 管理员运行powershell：dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart
- 内核更新下载安装：https://wslstorestorage.blob.core.windows.net/wslblob/wsl_update_x64.msi
- 管理员运行powershell：wsl --set-default-version 2
- 下载ubuntu1804： https://wslstorestorage.blob.core.windows.net/wslblob/Ubuntu_1804.2019.522.0_x64.appx
- 管理员运行powershell：Add-AppxPackage .\Ubuntu_1804.2019.522.0_x64.appx
- 双击安装Ubuntu_1804.2019.522.0_x64.appx

## wsl2应用程序直接打开gui

- windows下载VxSrv安装时注意勾选Disable access control:    https://nchc.dl.sourceforge.net/project/vcxsrv/vcxsrv/1.20.14.0/vcxsrv-64.1.20.14.0.installer.exe
- sudo apt install -y x11-apps
- echo 'export DISPLAY=172.23.80.1:0' >> ~/.bashrc
- source ~/.bashrc

## 远程桌面连接

- ubuntu安装SYSTEMCTL：git clone https://github.com/DamionGans/ubuntu-wsl2-systemd-script.git
- ubuntu安装SYSTEMCTL：cd ubuntu-wsl2-systemd-script/
- ubuntu安装SYSTEMCTL：bash ubuntu-wsl2-systemd-script.sh --force
- ubuntu安装SYSTEMCTL：wsl --shutdown  #去windows cmd下重启wsl
- ubuntu安装SYSTEMCTL：wsl #启动ubuntu
- ubuntu安装SYSTEMCTL：sudo apt update
- sudo apt install -y ubuntu-desktop
- sudo apt install -y xubuntu-desktop
- sudo apt install -y xrdp
- sudo adduser xrdp ssl-cert
- sudo ufw allow 3390
- sudo sed -i 's/port=3389/port=3390/g' /etc/xrdp/xrdp.ini
- sudo echo xfce4-session > ~/.xsession
- sudo nano /etc/xrdp/sesman.ini   #将 `KillDisconnected`的值修改为 `true`,保存退出
- sudo systemctl restart xrdp

# windows端口转发

- 查看wls中ubuntu的ip：
- ip addr show eth0
- windows宿主机器powershell：
- netsh interface portproxy add v4tov4 listenport=3390 listenaddress=0.0.0.0 connectport=3390 connectaddress=192.168.92.156 protocol=tcp
- netsh interface portproxy add v4tov4 listenport=2222 listenaddress=0.0.0.0 connectport=2222 connectaddress=192.168.92.156
- netsh interface portproxy add v4tov4 listenport=22 listenaddress=0.0.0.0 connectport=22 connectaddress=192.168.92.156
- netsh interface portproxy show all

# UBUNTU ssh server

1. 查看是否安装SSHServer：ps -e|grep ssh
2. 安装SSHServer：sudo apt-get install openssh-server
3. 修改sshd_config: sudo nano /etc/ssh/sshd_config

- port 22
- PermitRootLogin prohibit-password
- PermitRootLogin yes

4. 启动SSH：/etc/init.d/ssh start
5. 设置开机自启SSH：sudo systemctl enable ssh

# git ssh密钥 

1. ssh-keygen -C “572981033@qq.com” -t rsa
2. git config --global credential. Helper store




#### 1、Python

把pip的安装源设置为国内的清华源

pip config set global.index-url https://pypi.tuna.tsinghua.edu.cn/simple/

#### 2、WSL2 下 ROS 添加ROS官方源添加密钥

[(14条消息) win10 wsl2 + ubuntu20.04 配置 ROS-Noetic_lainegates的博客-CSDN博客_wsl2 rosnoetic](https://blog.csdn.net/LaineGates/article/details/120910628)
```
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
sudo apt-key adv --keyserver 'hkp://keyserver.ubuntu.com:80' --recv-key C1CF6E31E6BADE8868B172B4F42ED6FBAB17C654
echo "source /opt/ros/noetic/setup.bash" >> ~/.bashrc
source ~/.bashrc
```
##### 3、安装配套工具初始化rosdep开机自动进行ROS环境配置
```
sudo apt install python3-rosdep python3-rosinstall python3-rosinstall-generator python3-wstool build-essential
sudo apt-get install python3-pip
sudo pip install rosdepc
sudo apt-get install python3-roslaunch
sudo apt-get install ros-noetic-roslaunch
sudo rosdepc init
rosdepc update
sudo apt install python3-rosnode #安装节点查看工具rosnode list #查看节点
sudo apt-get install ros-noetic-rqt #rqt_graph
sudo apt-get install ros-noetic-rqt-common-plugins #rqt_graph
sudo apt-get install ros-noetic-rqt ros-noetic-rqt-common-plugins ros-noetic-turtlesim
source /opt/ros/noetic/setup.bash
```


- 安装ros-tutorials程序包
- 软件包查找如
- 软件包定位如
```
sudo apt-get install ros-noetic-ros-tutorials
rospack find roscpp
roscd roscpp
roscd roscpp/cmake
```


- 创建软件包 快速开始
```
my_package/
  CMakeLists.txt#配置Catkin元包的CMakeLists.txt模板文件
  package.xml#提供软件包元信息

mkdir -p ~/catkin_ws/src
cd ~/catkin_ws/src
git clone https://gitee.com/ros1-learning/beginner_tutorials.git
cd ~/catkin_ws/
catkin_make
roscore
rosrun beginner_tutorials listener
rosrun beginner_tutorials talker
```
#### 创建Catkin工作空间

```
cd ~/catkin_ws/
catkin_make #构建一个catkin工作区并生效配置文件
source devel/setup.bash
cd ~/catkin_ws/src
catkin_create_pkg beginner_tutorials std_msgs rospy roscpp #在catkin_ws/src创建软件包
rospack depends1 beginner_tutorials #查看包依赖
roscore #开启核心节点
rosrun turtlesim turtlesim_node #启动小乌龟节点
rosrun turtlesim turtle_teleop_key #开启小乌龟遥控节点
rosrun rqt_graph rqt_graph #打开画图图形
rosmsg show geometry_msgs/Twist #查看消息类型的详细信息

rostopic echo /turtle1/cmd_vel ##### 显示话题内容
rostopic type /turtle1/cmd_vel ##### 查看话题消息类型
rostopic pub -1 /turtle1/cmd_vel geometry_msgs/Twist -- '[2.0, 0.0, 0.0]' '[0.0, 0.0, 1.8]'##### 发布消息
rostopic pub /turtle1/cmd_vel geometry_msgs/Twist -r 1 -- '[2.0, 0.0, 0.0]' '[0.0, 0.0, -1.8]'##### 1Hz频率发送话题消息

rosrun rqt_plot rqt_plot # rqt_plot命令可以在滚动时间图上显示发布到某个话题上的数据。这里我们将使用rqt_plot命令来绘制正被发布到/
rosrun rqt_console rqt_console #连接到了ROS的日志框架
rosrun rqt_logger_level rqt_logger_level #节点运行时改变输出信息的详细级别
 
rosservice list #服务
rosservice call /clear #调用/call服务

rosparam list #参数服务器
rosparam set /turtlesim/background_r 150
rosparam get /turtlesim/background_r 
rosservice call /clear
rosparam get / #获取参数服务器上所有内容
--------------------------------------------------------------
mkdir msg
echo "int64 num" > msg/Num.msg
msg package.xml 加入依赖项：
<build_depend>message_generation</build_depend>
<exec_depend>message_runtime</exec_depend>

msg CMakeLists.txt find_package中加入message_generation
# 不要直接复制这一大段，只需将message_generation加在括号闭合前即可
find_package(catkin REQUIRED COMPONENTS
   roscpp
   rospy
   std_msgs
   message_generation #加入这个
)
#添加编译
add_message_files(
  FILES
  Num.msg
)
#查看消息是否在ROS中起作用了
rosmsg show beginner_tutorials/Num

-------------------------------------------------------------
roscd beginner_tutorials
mkdir srv
roscp rospy_tutorials AddTwoInts.srv srv/AddTwoInts.srv
#package.xml中确保开启message创建和message运行依赖
<build_depend>message_generation</build_depend
msg CMakeLists.txt find_package中加入message_generation
# 不要直接复制这一大段，只需将message_generation加在括号闭合前即可
find_package(catkin REQUIRED COMPONENTS
   roscpp
   rospy
   std_msgs
   message_generation #加入这个
)
#添加编译
add_service_files(
  FILES
  AddTwoInts.srv
)
generate_messages(
  DEPENDENCIES
  std_msgs
)
#查看服务
rossrv show beginner_tutorials/AddTwoInts

```

## 5.2图形界面

- [(14条消息) 超详细Windows10/Windows11 子系统（WSL2）安装Ubuntu20.04（带桌面环境）_萌褚的博客-CSDN博客_wsl ubuntu 桌面](https://blog.csdn.net/m0_60028455/article/details/125316625)
- [(14条消息) wsl安装xrdp（可视化界面并远程），解决闪退、黑屏_xrdp闪退_daboluo520的博客-CSDN博客](https://blog.csdn.net/guorong520/article/details/124749625)
- [(14条消息) WSL（Ubuntu20.04）与其图形界面安装配置_sandonz的博客-CSDN博客_wsl ubuntu图形界面](https://blog.csdn.net/sandonz/article/details/120854876?spm=1001.2101.3001.6650.1&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EOPENSEARCH%7ERate-1-120854876-blog-113616883.pc_relevant_vip_default&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EOPENSEARCH%7ERate-1-120854876-blog-113616883.pc_relevant_vip_default&utm_relevant_index=2)
- git clone https://github.com/DamionGans/ubuntu-wsl2-systemd-script.git
- cd ubuntu-wsl2-systemd-script/
- bash ubuntu-wsl2-systemd-script.sh --force
- wsl --shutdown  #去windows cmd下重启wsl
- wsl #启动ubuntu
- systemctl 
- sudo apt update
- sudo apt install -y xubuntu-desktop
- sudo apt install -y xrdp
- sudo adduser xrdp ssl-cert
- sudo ufw allow 3390
- sudo sed -i 's/port=3389/port=3390/g' /etc/xrdp/xrdp.ini
- sudo echo xfce4-session > ~/.xsession
- sudo nano /etc/xrdp/sesman.ini   #将`KillDisconnected`的值修改为`true`,保存退出
- sudo systemctl restart xrdp 
