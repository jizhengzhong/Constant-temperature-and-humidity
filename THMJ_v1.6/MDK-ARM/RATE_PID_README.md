# 升温速率PID控制程序（微分先行版）

## 📋 目录
- [项目概述](#项目概述)
- [核心特性：微分先行PID](#核心特性微分先行pid)
- [设计思路](#设计思路)
- [核心特性](#核心特性)
- [文件说明](#文件说明)
- [快速开始](#快速开始)
- [API文档](#api文档)
- [移植指南](#移植指南)
- [参数整定](#参数整定)
- [常见问题](#常见问题)

---

## 项目概述

这是一个专门用于控制升温速率的PID控制程序。与传统PID控制器不同，本程序增加了升温速率控制功能，可以精确限制温度上升的速度，避免温度超调和加热过快导致的问题。

### 🎯 核心升级：微分先行PID（v2.0）

**v2.0版本从传统PID升级为微分先行PID，解决了客户在运行过程中修改目标温度时的积分饱和问题！**

#### 传统PID的问题
```
场景：设备正在从50°C升温到100°C
客户突然修改目标温度：100°C → 120°C

传统PID响应：
❌ 误差突变：50°C → 70°C
❌ 微分项产生巨大冲击
❌ 积分项快速累积导致饱和
❌ 输出剧烈波动，系统振荡
```

#### 微分先行PID的优势
```
同样场景：目标温度从100°C → 120°C

微分先行PID响应：
✅ 微分项只对当前温度微分，不受目标温度影响
✅ 输出平滑过渡，无冲击
✅ 无积分饱和现象
✅ 系统稳定，无振荡
```

### 应用场景
- 实验室加热设备
- 工业温控系统
- 灭菌设备
- 材料热处理
- 任何需要控制升温速率的场合
- **需要频繁修改目标温度的场合（重点！）**

---

## 设计思路

### 1. 传统PID的局限性
传统PID控制器只考虑当前温度与目标温度的偏差：
```
输出 = Kp×误差 + Ki×积分 + Kd×微分
```

**存在的问题：**
- 无法限制升温速率，可能加热过快
- 大功率系统容易产生温度超调
- 升温过程不可控

### 2. 双环控制架构

本程序采用**双环PID控制**架构：

```
┌─────────────────────────────────────────┐
│           外环：温度PID                   │
│  输入：目标温度 - 当前温度                 │
│  输出：基础加热功率                       │
└──────────────┬──────────────────────────┘
               │
               ↓
┌─────────────────────────────────────────┐
│           内环：升温速率PID                │
│  输入：目标速率 - 实际速率                 │
│  作用：升温过快时降低输出                   │
└──────────────┬──────────────────────────┘
               │
               ↓
        最终输出功率
```

### 3. 控制策略

#### 升温阶段（RUNNING状态）
```c
// 1. 计算温度PID输出
temp_output = TemperaturePID_Calculate(temp_error);

// 2. 判断是否需要速率控制
if (actual_rate > target_rate) {
    // 升温过快，速率PID介入
    rate_output = RatePID_Calculate(rate_error);
    final_output = temp_output - rate_output × weight;
} else {
    // 升温正常，使用温度PID输出
    final_output = temp_output;
}
```

#### 恒温阶段（HOLDING状态）
```c
// 到达目标温度，只使用温度PID
final_output = TemperaturePID_Calculate(temp_error);
```

### 4. 函数指针封装

通过回调函数实现与硬件的解耦：

```c
typedef struct {
    // 获取时间戳
    float (*get_timestamp_cb)(void);
    
    // 设置输出功率（0-100%）
    void (*set_output_cb)(float output, void *user_data);
    
    // 调试打印（可选）
    void (*debug_print_cb)(void *controller, void *user_data);
    
    // 用户自定义数据
    void *user_data;
} RatePID_Config_t;
```

**优点：**
- ✅ 与硬件平台无关
- ✅ 易于移植到不同MCU
- ✅ 支持多种控制方式（PWM、继电器、DAC等）
- ✅ 便于单元测试

---

## 核心特性

### ✅ 功能特性
- **双环PID控制**：温度环 + 速率环
- **升温速率限制**：精确控制升温速度
- **抗积分饱和**：防止积分项过大导致超调
- **输出变化率限制**：保护加热设备
- **温度死区控制**：避免在目标温度附近频繁切换
- **速率滤波**：一阶低通滤波减少噪声
- **状态管理**：自动切换升温和恒温模式

### ✅ 移植特性
- **函数指针封装**：完全解耦硬件依赖
- **配置化设计**：通过结构体配置所有参数
- **零全局变量**：支持多实例运行
- **标准C语言**：兼容所有C编译器
- **无外部依赖**：只需math.h和string.h

---

## 文件说明

```
rate_pid/
├── rate_pid.h              # 头文件：结构体定义和函数声明
├── rate_pid.c              # 源文件：PID算法实现
├── rate_pid_example.c      # 示例文件：使用示例和回调函数实现
└── README.md               # 本说明文档
```

### 核心文件

#### 1. rate_pid.h
- 数据结构定义
- API函数声明
- 枚举和常量定义

#### 2. rate_pid.c
- PID算法实现
- 状态管理逻辑
- 回调函数调用

#### 3. rate_pid_example.c
- 回调函数示例
- 初始化配置示例
- 使用示例代码
- 参数整定建议

---

## 快速开始

### 步骤1：复制文件到项目
```bash
将以下文件复制到您的项目：
- rate_pid.h
- rate_pid.c
- rate_pid_example.c
```

### 步骤2：实现回调函数
```c
// 1. 实现获取时间戳函数
float My_GetTimestamp(void)
{
    return HAL_GetTick() / 1000.0f;  // STM32示例
}

// 2. 实现设置输出函数
void My_SetOutput(float output, void *user_data)
{
    // 控制加热器，例如设置PWM占空比
    uint16_t pwm = (uint16_t)(output * 1000 / 100.0f);
    __HAL_TIM_SET_COMPARE(&htim, TIM_CHANNEL_1, pwm);
}
```

### 步骤3：初始化控制器
```c
RatePID_Controller_t controller;
RatePID_Config_t config;

// 配置PID参数
config.temp_pid_params.kp = 2.0f;
config.temp_pid_params.ki = 0.5f;
config.temp_pid_params.kd = 0.1f;
// ... 其他参数配置

// 配置回调函数
config.get_timestamp_cb = My_GetTimestamp;
config.set_output_cb = My_SetOutput;

// 初始化
RatePID_Init(&controller, &config);
```

### 步骤4：启动控制
```c
// 启动升温：目标120°C，速率2°C/s
RatePID_SetTarget(&controller, 120.0f, 2.0f);
```

### 步骤5：周期性调用
```c
// 在定时器或任务中调用（建议周期：0.5秒）
float current_temp = ReadTemperature();
float output = RatePID_Calculate(&controller, current_temp);
```

---

## API文档

### 核心函数

#### RatePID_Init
```c
void RatePID_Init(RatePID_Controller_t *ctrl, const RatePID_Config_t *config);
```
初始化控制器。

**参数：**
- `ctrl`: 控制器指针
- `config`: 配置参数指针

---

#### RatePID_SetTarget
```c
void RatePID_SetTarget(RatePID_Controller_t *ctrl, float target_temp, float target_rate);
```
设置目标温度和升温速率。

**参数：**
- `ctrl`: 控制器指针
- `target_temp`: 目标温度（°C）
- `target_rate`: 目标升温速率（°C/秒）

---

#### RatePID_Calculate
```c
float RatePID_Calculate(RatePID_Controller_t *ctrl, float current_temp);
```
执行PID计算，返回控制输出。

**参数：**
- `ctrl`: 控制器指针
- `current_temp`: 当前温度（°C）

**返回值：**
- 输出功率（0-100%）

---

#### RatePID_Stop
```c
void RatePID_Stop(RatePID_Controller_t *ctrl);
```
停止PID控制，输出归零。

---

#### RatePID_Reset
```c
void RatePID_Reset(RatePID_Controller_t *ctrl);
```
重置控制器状态（积分项、历史数据等）。

---

#### RatePID_UpdateParams
```c
void RatePID_UpdateParams(RatePID_Controller_t *ctrl, 
                           RatePID_Type_t pid_type,
                           const PID_Params_t *params);
```
动态更新PID参数。

---

#### RatePID_GetStatus
```c
void RatePID_GetStatus(RatePID_Controller_t *ctrl, RatePID_Status_t *status);
```
获取控制器运行状态。

---

## 移植指南

### STM32 + HAL库移植示例

#### 1. 时间戳回调
```c
static float STM32_GetTimestamp(void)
{
    // HAL_GetTick()返回毫秒，转换为秒
    return HAL_GetTick() / 1000.0f;
}
```

#### 2. 输出回调（PWM控制）
```c
static void STM32_SetOutputPWM(float output, void *user_data)
{
    // 将0-100%映射到PWM周期
    uint16_t pwm_value = (uint16_t)(output * __HAL_TIM_GET_AUTORELOAD(&htim2) / 100.0f);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pwm_value);
}
```

#### 3. 输出回调（继电器控制）
```c
// 全局变量
static float relay_output = 0;
static uint32_t relay_cycle_counter = 0;

static void STM32_SetOutputRelay(float output, void *user_data)
{
    relay_output = output;
    // 在定时器中断中实现时间比例控制
}

// 在定时器中断中（例如1秒中断一次）
void TIM_IRQHandler(void)
{
    relay_cycle_counter++;
    
    // 时间比例控制：周期10秒
    if (relay_cycle_counter < (relay_output / 10.0)) {
        HEATER_ON();   // 导通
    } else {
        HEATER_OFF();  // 断开
    }
    
    if (relay_cycle_counter >= 10) {
        relay_cycle_counter = 0;
    }
}
```

#### 4. 在FreeRTOS任务中使用
```c
void HeatingTask(void *argument)
{
    RatePID_Controller_t controller;
    RatePID_Config_t config;
    float current_temp;
    
    // 初始化配置
    memset(&config, 0, sizeof(RatePID_Config_t));
    config.temp_pid_params.kp = 2.0f;
    config.temp_pid_params.ki = 0.5f;
    config.temp_pid_params.kd = 0.1f;
    config.get_timestamp_cb = STM32_GetTimestamp;
    config.set_output_cb = STM32_SetOutputPWM;
    
    RatePID_Init(&controller, &config);
    RatePID_SetTarget(&controller, 120.0f, 2.0f);
    
    while (1) {
        current_temp = ReadTemperature();
        RatePID_Calculate(&controller, current_temp);
        osDelay(pdMS_TO_TICKS(500));  // 500ms周期
    }
}
```

---

## 参数整定

### 温度PID参数

| 参数 | 作用 | 调整建议 |
|------|------|----------|
| Kp | 比例响应速度 | 增大→响应快，但可能振荡 |
| Ki | 消除稳态误差 | 增大→消除误差快，但可能超调 |
| Kd | 抑制超调 | 增大→抑制超调，但对噪声敏感 |

**整定步骤：**
1. Ki=0, Kd=0，逐渐增大Kp直到系统开始振荡
2. Kp = 振荡值 × 0.6
3. 逐渐增大Ki直到稳态误差消除
4. 如果有超调，适当增大Kd

### 速率PID参数

| 参数 | 作用 | 调整建议 |
|------|------|----------|
| Kp | 速率偏差响应 | 通常较大（5-10），快速介入 |
| Ki | 速率稳态误差 | 可以较小或为0 |
| Kd | 速率变化率 | 通常为0 |

### 控制参数

| 参数 | 推荐值 | 说明 |
|------|--------|------|
| sample_time | 0.5s | 采样周期 |
| temp_deadband | 1.0°C | 温度死区 |
| rate_filter_alpha | 0.3 | 速率滤波（0.1-0.5） |
| rate_control_weight | 0.8 | 速率控制权重（0.5-1.0） |
| output_rate_limit | 10%/s | 输出变化率限制 |

### 不同功率系统参考参数

#### 小功率系统（<1kW）
```c
温度PID: Kp=3.0, Ki=0.8, Kd=0.2
速率PID: Kp=8.0, Ki=2.0, Kd=0
升温速率: 1-3°C/s
```

#### 中功率系统（1-5kW）
```c
温度PID: Kp=2.0, Ki=0.5, Kd=0.1
速率PID: Kp=5.0, Ki=1.0, Kd=0
升温速率: 2-5°C/s
```

#### 大功率系统（>5kW）
```c
温度PID: Kp=1.5, Ki=0.3, Kd=0.05
速率PID: Kp=3.0, Ki=0.5, Kd=0
升温速率: 3-10°C/s
```

---

## 常见问题

### Q1: 升温速率控制不精确
**症状：** 实际升温速率与设定值偏差较大

**解决方案：**
1. 增大速率PID的Kp值（例如从5增加到10）
2. 减小`rate_filter_alpha`（例如从0.3减小到0.2）增强滤波
3. 提高采样频率（减小`sample_time`）
4. 检查温度传感器响应速度

---

### Q2: 到达目标温度后超调过大
**症状：** 温度超过目标值2-5°C

**解决方案：**
1. 增大温度PID的Kd值（例如从0.1增加到0.2）
2. 减小`temp_deadband`（例如从2.0减小到1.0）
3. 增大`rate_control_weight`（例如从0.5增加到0.8）
4. 检查是否存在较大的纯滞后

---

### Q3: 升温过程中输出波动大
**症状：** 输出功率频繁变化，加热器频繁开关

**解决方案：**
1. 减小温度PID的Kp值
2. 增大`output_rate_limit`的限制（例如从5增加到15）
3. 检查温度传感器是否有噪声
4. 增大`rate_filter_alpha`增强滤波

---

### Q4: 升温速率过慢
**症状：** 实际升温速率远低于设定值

**解决方案：**
1. 检查加热功率是否足够
2. 增大目标升温速率设定值
3. 减小速率PID的介入强度（减小`rate_control_weight`）
4. 检查系统是否存在较大的热损失

---

### Q5: 积分饱和导致响应迟缓
**症状：** 温度变化后系统响应慢

**解决方案：**
1. 确保`anti_windup`设置为1
2. 减小`integral_limit`值
3. 适当增大Ki加快积分响应

---

## 技术支持

如有问题，请检查：
1. ✅ 回调函数是否正确实现
2. ✅ 采样周期是否稳定
3. ✅ 温度传感器数据是否准确
4. ✅ PID参数是否匹配系统特性
5. ✅ 输出限幅是否合理

---

## 版本历史

- **v1.0.0** (2026-05-20)
  - 初始版本
  - 实现双环PID控制
  - 完整的函数指针封装
  - 详细的文档和示例

---

## 许可证

本代码可以自由使用和修改。

---

**祝您使用愉快！** 🎉
