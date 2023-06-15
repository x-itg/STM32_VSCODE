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
# Ubuntu下环境搭建
```
https://blog.csdn.net/zhengyangliu123/article/details/54783443?spm=1001.2101.3001.6661.1&utm_medium=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7ERate-1-54783443-blog-124376993.2$
https://developer.arm.com/downloads/-/gnu-a
https://blog.csdn.net/daoshengtianxia/article/details/115038674
https://blog.csdn.net/qq_30623591/article/details/88015282
```

## 下载armgccgdb添加环境变量

```
https://developer.arm.com/downloads/-/gnu-a
https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu-a/10.3-2021.07/binrel/gcc-arm-10.3-2021.07-x86_64-arm-none-eabi.tar.xzhttps://armkeil.blob.core.windows.net/developer/Files/downloads/gnu-a/10$
sudo nano /etc/profile
文末添加： export PATH=$PATH:/home/gg/mycode/gcc-arm-10.3-2021.07-x86_64-arm-none-eabi/bin
命令行使得环境便利生效：source /etc/profile
sudo apt-get install gcc-arm-none-eabi
sudo apt-get remove gcc-arm-none-eabi
arm-none-eabi-gcc -v

```

## 事先安装这些软件

```
sudo apt-get install lsb-core
sudo apt-get install build-essential pkg-config autoconf automake libtool libusb-dev libusb-1.0-0-dev libhidapi-dev
sudo apt-get install libtool libsysfs-dev    
sudo apt-get install libnewlib-arm-none-eabi #出现错误时用arm-none-eabi/bin/ld: cannot find -lc_nano
```

## 下载openocd源码 使能stlink 默认没有的

```
ubuntu下安装stm32cubeclt工具不要忘记：https://www.st.com/en/development-tools/stm32cubeclt.html
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

### 在.vscode文件夹下添加launch.json文件
```
{
    // 使用 IntelliSense 了解相关属性。 
    // 悬停以查看现有属性的描述。
    // 欲了解更多信息，请访问: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
        {
            "name": "wsl", //调试入口显示的名字，随便起
            "cwd": "${workspaceRoot}", //工作目录，根路径
            "executable": "${workspaceFolder}/build/f.elf", //调试文件
            "request": "launch",
            "runToMain": true,
            "type": "cortex-debug", //配置为使用插件调试
            "servertype": "openocd", //映射openocd
            "configFiles": [
                "${workspaceRoot}/openocd.cfg"
            ], //openocd配置
            "postDebugTask": "Reset" //同上，调试结束执行的任务
        }
    ]
}
```
### vscode安装cortex-debug插件




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

# WSL USB驱动
- 安装usb驱动： https://kgithub.com/dorssel/usbipd-win
- ubunutu下安装：sudo apt install linux-tools-generic hwdata
- ubunutu下安装：sudo update-alternatives --install /usr/local/bin/usbip usbip /usr/lib/linux-tools/*-generic/usbip 20
- powershell下：usbipd wsl list
- powershell下连接usb连到wsl：usbipd wsl attach --busid 2-1
- powershell下连接usb断开wsl：usbipd wsl detach --busid 2-1
- ubuntu下：lsusb

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


