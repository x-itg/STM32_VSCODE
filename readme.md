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
---------------------------------------------------------------------------------

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
- sudo nano /etc/xrdp/sesman.ini   #将`KillDisconnected`的值修改为`true`,保存退出
- sudo systemctl restart xrdp 
- 查看wls中ubuntu的ip：
-  ip addr show eth0
- windows宿主机器powershell：
-  netsh interface portproxy add v4tov4 listenport=3390 listenaddress=0.0.0.0 connectport=3390 connectaddress=192.168.92.156 protocol=tcp
- netsh interface portproxy show all
