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
```

## 事先安装这些软件

```
sudo apt-get install lsb-core
sudo apt-get install build-essential pkg-config autoconf automake libtool libusb-dev libusb-1.0-0-dev libhidapi-dev
sudo apt-get install libtool libsysfs-dev
```

## 下载openocd源码 使能stlink 默认没有的

```
https://blog.csdn.net/daoshengtianxia/article/details/115038674
下载openocd及其子模块后编译安装
./configure --enable-stlink
sudo make
sudo make install
安装在了/usr/local/bin
配置文件在 /usr/local/share/openocd/scripts
```

### Makefile文件中 添加
```
-include $(wildcard $(BUILD_DIR)/*.d)
update:
	openocd -f openocd.cfg -c init -c halt -c "program $(BUILD_DIR)/$(TARGET).hex verify reset exit"
reset:
	openocd -f openocd.cfg -c init -c halt -reset -c shutdown
```

### 在工作目录下添加openocd.cfg文件，内容：
```
source [find /usr/local/share/openocd/scripts/interface/stlink-v2.cfg]
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
            "name": "STM32F103", //调试入口显示的名字，随便起
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



