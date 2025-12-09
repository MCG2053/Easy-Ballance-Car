# Easy-Balance-Car

基于STM32F103C8T6的两轮自平衡小车控制系统,适合江协学习路线的朋友上手

## ✨ 功能特性

- 自平衡控制（并级PID算法）
- GY-25姿态解算
- JGA25-370 12V 620转 编码电机测速 & 驱动
- 蓝牙 & 2.4G遥控
- 无线NRF24L01+调试接口
- OLED实时数据显示

## 📁 项目结构

Easy-Ballance-Car/
|──Hardware
| |── Bluetooth.c #蓝牙硬件驱动
| |── Encoder.c #编码器硬件驱动
| |── GY25.c #陀螺仪硬件驱动
| |── Motor.c #电机硬件驱动
| |── NRF24L01.C #2.4G无线模块硬件驱动 by 江协科技
| └──  OLED.c # 0.96 OLED硬件驱动 by 江协科技
|── Library
| └── ## 标准库片内外设驱动文件,不做过多说明 ##
|── Objects
| └── ## 存放编译后文件 ##  
|── Start
| └──## 标准库核心文件,不做过多说明 ##
|── System
| |── Delay.c #延迟文件
| └── PWM.c #PWM驱动文件
|── User
| |── Control.c #无线控制文件
| |── Menu.c #菜单显示文件
| |── PID.c #PID算法文件
| |── Safety.c #安全装置文件
| └── SysInit.c #系统初始化文件
|── #Keil5项目文件
|── 1
|── 2
|── 3
|└── 清除编译后文件bat

## 🚀 快速开始

### 硬件要求

| 模块 | 型号 | 说明 |
|------|------|------|
| 主控 | STM32F103C8T6 | 最小系统板 |
| 传感器 | GY-25 | 六轴陀螺仪 |
| 电机 | JGA25-370直流减速电机 | 带编码器 |
| 驱动 | TB6612FNG | 电机驱动模块 |
| 通信 | HC-05 | 蓝牙模块 |
| 2.4G | NRF24L01 | 2.4G模块 |
| 显示 | 0.96寸OLED | SSD1306 |

### 软件环境

- Keil uVision5
- STM32标准库（用于初始化配置）
- ST-Link Utility（下载程序）

### 编译下载

1. 克隆项目

bash
git clone https://github.com/yourname/balance-car.git

3. 用Keil打开项目文件 `Balance_Car.uvprojx`

4. 编译项目
   - 点击 **Build** 或按 **F7**

5. 下载到开发板
   - 连接ST-Link调试器
   - 点击 **Download** 或按 **F8**

## 🔧 硬件连接

| STM32引脚 | 外设 | 功能 |
|-----------|------|------|
| PA6, PA7 | 编码器A | 左电机编码器 |
| PB6, PB7 | 编码器B | 右电机编码器 |
| PB10, PB11 | USART | GY-25 (TX, RX) |
| PA0, PA1 | TIM2 | 电机PWM (通道1, 2) |
| PB12, PB13, PB14, PB15 | GPIO | 电机正反转 (左, 右) |
| PA2, PA3 | USART2 | 蓝牙 (TX, RX) |
| PA2, PA3 | USART2 | 蓝牙 (TX, RX) |
| PB8, PB9 | I2C1 | OLED (SCL, SDA) |
| PA9, PA10, PA11, PA12 | SPI | NRF24L01 (CE,CSN,SCK,MOSI,MISO) |

## 📊 PID参数调节

在 `pid.h` 中调整参数：

c

/*角度环PID参数*/
PID AnglePID = {	
	.Kp = -150,// 250*0.6==150,
	.Kd = -800,// 650*0.6==390,
	.target=0,
};

/*速度环PID参数*/ 
PID SpeedPID = {	
	.Vkp = -380,
	.Vki = -380/200,// 340/200==1.7
};

/*转向环PID参数*/
PID TurnPID = {	
	.Tkp = -1,
	.Tkd = -0.1,
};

调节步骤：
1. 先调直立环（Kp增大稳定性，Kd抑制震荡）
2. 再调速度环（Vkp控制速度跟踪，Vki消除静差）
3. 最后调转向环 (Tkp控制转向稳定性,Tkd抑制转向震荡)

## 📱 蓝牙控制

通过手机APP发送指令：

| 指令 | 功能 |
|------|------|
| @FORWARD# | 前进 |
| @BACKWARD# | 后退 |
| @LEFT# | 左转 |
| @RIGHT# | 右转 |
| @STOP# | 停止 |

## 📁 文件说明

| 文件 | 功能 |
|------|------|
| `main.c` | 主程序，初始化，主循环，以及角度获取和PID控制 |
| `PID.c` | 平衡控制核心算法 |
| `GY25.c` | MPU6050驱动与数据处理 |
| `Motor.c` | 电机PWM与方向控制 |
| `Encoder.c` | 编码器计数与速度计算 |
| `OLED.c` | OLED显示驱动 |
| `Bluetooth.c` | 蓝牙数据接收与解析 |
| `NRF24L01.c` | 2.4G数据接收与解析 |
## 🔄 工作流程

mermaid
graph TD
A[OLED初始化] --> B[电机控制初始化]
B --> C[电机编码初始化]
C --> D[蓝牙串口初始化]
D --> E[2.4G无线模块初始化]
E --> F[陀螺仪初始化]
F --> G[陀螺仪角度获取模式设置]
G --> H[PID参数初始化]
H --> I[上电校准角度]
I --> J[主循环]

## 📄 协议

- 蓝牙数据格式：`指令+参数`（如：`F50` 表示前进速度50%）
- 传感器数据：MPU6050原始数据 + 卡尔曼滤波
- 控制周期：1ms（TIM2中断）

## ⚠️ 注意事项

1. 上电前确保电池电量充足
2. 每次运行需进行GY25校准
3. 调试时建议用支架架起小车
4. 先调直立环，再调速度环
5. 避免急加速急减速

## 📈 效果展示

## 当前暂无 ##
![平衡状态](images/balance.jpg)  
*小车保持自平衡状态*

## 暂时未上传 ##
![PCB设计](images/pcb.jpg)  
*自制控制板PCB设计*

## 🤝 贡献

欢迎提交Issue和Pull Request！

1. Fork本项目
2. 创建新分支：`git checkout -b feature/YourFeature`
3. 提交更改：`git commit -m 'Add some feature'`
4. 推送到分支：`git push origin feature/YourFeature`
5. 提交Pull Request

--

⭐ 如果这个项目对你有帮助，请点个Star！
