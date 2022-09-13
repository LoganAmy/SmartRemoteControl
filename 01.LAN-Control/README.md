通过本文，你也可以做到：

> - 使用 Arduino 开发 ESP8266(NodeMCU)模块。
>
> - ESP8266 控制舵机触发空调遥控。
>
> - ESP8266 提供 Web 服务，局域网控制舵机触发空调遥控。

## 工具人

作为工具人，总是无时无刻接受着领导的指令调度。下班回家，还没放下背包，领导就发来了指令：老公，我热，快开空调！！收到指令，我赶紧冲向书桌拿空调遥控器，然鹅遥控器却不见踪影，我东找找西翻翻，不知道被我昨天丢哪了。领导一顿训斥：一天天的，东西不放到该放的位置，就知道乱丢，热死了，快开空调！！我……

![](https://img.soogif.com/rNP5Mv3h0OnieagNOqlbTfrP48y15Noq.gif?imageMogr2/thumbnail/!83.72041225655889p&scope=mdnice)

为了能更好地服务领导（避免被骂），为了工具人的荣耀（被骂很难受），我决定动手做一个`"智能"`空调遥控器。

## 方案是这样的

![](https://files.mdnice.com/user/34369/3dec419e-cf5b-4cee-8f73-43c15b1676b3.png)
`手机`通过 WiFi，发送指令给可以联网的`NodeMCU`模块，`NodeMCU`控制`舵机`旋转按下`空调遥控器`的开关，从而启动`空调`。

看起来应该挺不错的~

![](https://wximg.yiban.io/img_proxy?src=https://img.soogif.com/vGNqMO9zyestygIOhLEoQZIYYUfrl3j7.gif)

## 硬件准备

- ESP8266(NodeMCU)模块
- SG90 舵机
- 移动电源

NodeMCU 可以在电商平台购买，价格十几元到二十元不等，可随意挑选，功能都是一样的。需要注意的是，NodeMCU 的 USB 转串口芯片型号，有 CH340、CP2102 等不同型号，在烧录固件的时候需要在电脑上安装对应的驱动，后文会详细说明。
![](https://files.mdnice.com/user/34369/9cf6af1c-6e59-4a7d-857a-fdc7dc1b5244.png)

\
SG90 舵机可以在电商平台购买，也可用其它型号的舵机，用 SG90 舵机，主要是因为便宜 😁😆🤪
![](https://files.mdnice.com/user/34369/3883bb9c-9088-4379-9df8-f37c9535a5be.png)

\
移动电源为`"智能"`空调遥控器提供电源，任意一款输出电压为 5V 的充电宝都可。

## 软件开发环境准备

### Arduino IDE 配置 ESP8266 环境

我们将使用首先 Arduino 开发`"智能"`空调遥控器，所以首先需要从 Arduino 官网(`https://www.arduino.cc/en/software`) 下载 Arduino IDE 软件并安装。
安装完成以后，进入首选项（Preferences），找到附加开发板管理器地址（Additional Board Manager URLs），并在其后添加如下信息：

> http://arduino.esp8266.com/stable/package_esp8266com_index.json

![](https://files.mdnice.com/user/34369/fc90521a-90bc-4b72-85c6-83d1b997ded9.png)

之后点击：工具 - 开发板 - 开发板管理器，进入开发板管理器界面：
输入 esp8266，选择最新版本并安装。

::: block-1
如果安装过程中下载比较慢，可以先下载文末 `Arduino esp8266 相关依赖包`，然后再按上面的步骤进行安装。
:::

安装好后，依次点击：工具 - 开发板 - ESP8266 Boards - NodeMCU 1.0

![](https://files.mdnice.com/user/34369/a13c150f-06df-45d5-9b16-6117e2f06946.png)

### USB 转串口驱动安装

USB 转串口驱动作用是将电脑上编译好的程序通过 USB 线烧录到 NodeMCU。需要根据购买的 NodeMCU 的 USB 转串口芯片型号安装对应驱动。

例如：
| USB 转串口芯片| 下载地址 | windows 下载包 | macos 下载包 |
| :------------: | :-- | :-----------: | :--------: |
| CH340 | https://www.wch.cn/products/CH340.html| CH341SER.EXE | CH34XSER_MAC.ZIP |
| CP2102 | https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers?tab=downloads| CP210x_VCP_Windows.zip | Mac_OSX_VCP_Driver.zip |

\
安装好驱动软件后，用 USB 线连接 NodeMCU 和电脑，然后依次点击 Arduino IDE 的：工具 - 端口，可以看到新增的端口，选择它。

![](https://files.mdnice.com/user/34369/f7f246a4-9e7c-4ce9-9b83-40b28d22c059.png)

\
自此，软硬件都已准备好，先测试一下，点个灯看看。

## 点个灯试试看

1. 依次点击 Arduino IDE 的：文件 - 示例 - 01.Basics - Blink。可打开 LED 闪烁的示例程序
   ![](https://files.mdnice.com/user/34369/d48d8f79-f81d-4dc5-b5d1-c886b650229c.png)

2. 点击菜单栏的`上传`按钮，等待下方的状态栏显示“上传成功”，LED 闪烁示例程序便成功上传到 NodeMCU。
   ![](https://files.mdnice.com/user/34369/b9d36548-ee0e-4c1f-b759-b56d6b650394.png)

此时可看到 NodeMCU 上的 LED 以亮 1 秒灭 1 秒的频率闪烁。

![](https://files.mdnice.com/user/34369/82818e31-3de9-4809-9fc4-8a3bde94883e.gif)

## 正式发车

### 1. 遥控器程序开发（复制粘贴）

首先，打开如下网址，复制程序代码（可点击右侧的按钮，一键复制代码。如果由于网络问题一键复制代码失败，手动选择代码文本，然后 ctrl + c 也是可以哒）。

> https://github.com/LoganAmy/SmartRemoteControl/blob/main/01.LAN-Control/src/LanSwitch/LanSwitch.ino

![](https://files.mdnice.com/user/34369/8f0a3d1e-d103-4118-b858-c9629a2b438d.png)

在 Arduino IDE 中，依次点击：文件 - 新建。将刚刚复制的代码粘贴（ctrl + v）到新文件中。

然后将程序代码里面的 WiFi 名称和密码，修改成家里的 WiFi 名称和密码。比如我的 WiFi 名称是`youtoo`，密码是`10242048`，那就按下图所示进行修改。修改完成后，将文件保存（ctrl + s）。

![](https://files.mdnice.com/user/34369/04355f53-7581-41b2-9ae1-be28d0afd0d3.png)

最后，和之前上传 LED 闪烁示例程序的步骤一样，将`智能`遥控器程序上传到 NodeMCU。

### 2. 遥控器硬件开发（组装积木）

如下图所示，通过导线将 NodeMCU 与 SG90 舵机相连。

> > ![](https://files.mdnice.com/user/34369/8944aab1-a570-4efc-81fd-c0e918c651d9.png)
> >
> > SG90 舵机信号线(`橙色线`)需要与 NodeMCU 的 D5 管脚相连 (也可修改代码，使用其它管脚) ，而电源线(`红色线`)与 NodeMCU 任意输出 3.3V 管脚相连，地线(`黑色线`)可与 NodeMCU 任意 GND 管脚相连。

向老婆借一根头绳（以一杯奶茶的代价 😒），将舵机绑到空调遥控器上。（如果舵机摇臂短了，可以吃一根冰棍，把冰棍的木棒绑在摇臂上）

![](https://files.mdnice.com/user/34369/32d980e9-7f9b-40cd-addc-d3220a64497b.jpg)

最后，用移动电源对 NodeMCU 进行供电。自此`智能`空调遥控器制作完成。

## 看看效果

`智能`空调遥控器，连接电源后，指示灯会亮起，成功连接 WIFI 后，指示灯熄灭。

手机连接与`智能`空调遥控器连接的同一个 WIFI，然后手机浏览器访问`http://youtoo-switch.local`。此时指示灯闪烁一下，同时舵机转动，触发空调遥控器开关，打开空调。

::: block-1
需要注意的是，并不是所有手机都支持在浏览器直接访问`http://youtoo-switch.local`。这取决于手机系统是否支持 mDNS 解析，iPhone 系列手机都支持 mDNS，Android 12 及以上版本才支持 mDNS。如何在不支持的手机上进行控制呢？可查看文末附录工具包的`通过IP控制遥控器`
:::

## 附录工具包

::: block-1
<span style="color:#ff3502;font-weight: bold;">Arduino esp8266 依赖包</span>

- esp8266 Arduino 核心库: esp8266-xxxx.zip
  https://github.com/esp8266/Arduino/releases
- esp8266 交叉编译工具链 (不同平台需要下载不同的工具链)
  https://github.com/earlephilhower/esp-quick-toolchain/releases
  - windows 下载如下包
    - i686-w64-mingw32.mklittlefs-xxxx.zip
    - i686-w64-mingw32.mkspiffs-xxxx.zip
    - i686-w64-mingw32.xtensa-lx106-elf-xxxx.zip
    - python3-3.7.2.post1-embed-win32v2a.zip
  - macos 下载如下包
    - x86_64-apple-darwin14.mklittlefs-xxxx.tar.gz
    - x86_64-apple-darwin14.mkspiffs-xxxx.tar.gz
    - x86_64-apple-darwin14.xtensa-lx106-elf-xxxx.tar.gz
    - python3-macosx-portable.tar.gz
- 依赖包存放路径
  - windows:%LocalAppData%\Arduino15\staging\packages
  - macos:~/Library/Arduino15/staging/packages
:::

::: block-1
<span style="color:#ff3502;font-weight: bold;">通过IP控制遥控器</span>

**1. 获取NodeMCU的IP地址**  
打开Arduino IDE的串口监视器，依次点击：工具 - 串口监视器
![](https://files.mdnice.com/user/34369/61ee8e8f-1b9e-46bf-9233-92e00a38ff78.png)
\
在串口监视器中，选择`115200 波特率`，之后上方窗口会打印NodeMCU的运行信息（如果一直不显示，可以按NodeMCU上的 RST 按钮或者重新插拔USB线）。NodeMCU连接WiFi后，会在窗口打印IP地址，如下图中的：192.168.1.25
![](https://files.mdnice.com/user/34369/0aec2634-03cf-4c6e-9537-6cd3d7352d44.png)

**2. 手机浏览器访问IP地址控制遥控器**  
在手机浏览器中，输入上一步骤获取的IP地址，即可控制遥控器。如输入：http://192.168.1.25
![](https://files.mdnice.com/user/34369/a92dcf7f-fe33-4445-9b60-a06f189947bb.jpg)
:::


\
\
为进一步降低操作门槛，让**你也可以**体验动手的乐趣，文中提到的相关软件、驱动、代码，可扫码关注『**YouToo**』微信公众号后，回复 `智能遥控器01` 即可打包获取。

![](https://files.mdnice.com/user/34369/0fbf9c9f-dbb0-4c23-9cbe-e4a5d9724cdc.jpg)
