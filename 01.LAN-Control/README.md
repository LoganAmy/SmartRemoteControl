通过本文，你也可以做到：

> - 使用 Arduino 开发 ESP8266(NodeMCU)模块。
>
> - ESP8266 控制舵机触发空调遥控。
>
> - ESP8266 提供 Web 服务，局域网控制舵机触发空调遥控。

## 工具人

作为工具人，总是无时无刻接受着领导的指令调度。下班回家，还没放下背包，领导就发来了指令：老公，我热，快开空调！！收到指令，我赶紧冲向书桌拿空调遥控器，然鹅遥控器却不见踪影，我东找找西翻翻，不知道被我昨天丢哪了。领导一顿训斥：一天天的，东西不放到该放的位置，就知道乱丢，热死了，快开空调！！我……

![心好累](https://img.soogif.com/rNP5Mv3h0OnieagNOqlbTfrP48y15Noq.gif?imageMogr2/thumbnail/!83.72041225655889p&scope=mdnice)

为了能更好地服务领导（避免被骂），为了工具人的荣耀（被骂很难受），我决定动手做一个`"智能"`空调遥控器。

## 硬件准备

- ESP8266(NodeMCU)模块
- SG90 舵机
- 移动电源

NodeMCU 可以在电商平台购买，价格十几元到二十元不等，可随意挑选，功能都是一样的。需要注意的是，NodeMCU 的 USB 转串口芯片型号，有 CH340、CP2102 等不同型号，在烧录固件的时候需要在电脑上安装对应的驱动，后文会详细说明。

图片

SG90 舵机可以在电商平台购买，也可用其它型号的舵机，用 SG90 舵机，主要是因为便宜（doge）

NodeMCU 与 SG90 舵机管脚连接示意如下图：

NodeMCU 的 D4 与管脚与 SG90 舵机信号线相连。

移动电源为我们的`"智能"`空调遥控器提供电源，任意一款输出电压为 5V 的充电宝都可。

## 软件开发环境准备

### Arduino IDE 配置 ESP8266 环境

我们将使用首先 Arduino 开发`"智能"`空调遥控器，所以首先需要从 [Arduino 官网](https://www.arduino.cc/en/software "Arduino 官网") 下载 Arduino IDE 软件并安装。
安装完成以后，进入首选项（Preferences），找到附加开发板管理器地址（Additional Board Manager URLs），并在其后添加如下信息：

> http://arduino.esp8266.com/stable/package_esp8266com_index.json

之后点击工具 - 开发板 - 开发板管理器，进入开发板管理器界面：

输入 esp8266，选择最新版本并安装。

>> **如果安装过程中，下载比较慢，可以先下载相关依赖包，然后再按上述步骤进行安装。**
>> - esp8266 Arduino 核心库: esp8266-xxxx.zip
>>   https://github.com/esp8266/Arduino/releases
>>   
>> - esp8266 交叉编译工具链 (不同平台需要下载不同的工具链)
>>   https://github.com/earlephilhower/esp-quick-toolchain/releases
>>   - windows下载如下包
>>     - i686-w64-mingw32.mklittlefs-xxxx.zip
>>     - i686-w64-mingw32.mkspiffs-xxxx.zip
>>     - i686-w64-mingw32.xtensa-lx106-elf-xxxx.zip
>>     - python3-3.7.2.post1-embed-win32v2a.zip
>>   - macos下载如下包
>>     - x86_64-apple-darwin14.mklittlefs-xxxx.tar.gz
>>     - x86_64-apple-darwin14.mkspiffs-xxxx.tar.gz
>>     - x86_64-apple-darwin14.xtensa-lx106-elf-xxxx.tar.gz
>>     - python3-macosx-portable.tar.gz
>> - 依赖包存放路径
>>   - windows:%LocalAppData%\Arduino15\staging\packages
>>   - macos:~/Library/Arduino15/staging/packages


### USB 转串口驱动安装

USB 转串口驱动主要在
