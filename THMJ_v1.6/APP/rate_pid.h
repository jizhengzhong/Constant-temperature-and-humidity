/**
  ******************************************************************************
  * @file    rate_pid.h
  * @brief   微分先行PID升降温速率控制器 - 头文件
  *
  *          核心特性：
  *          1. 微分先行PID（Derivative-on-Measurement）
  *             - 微分项仅对测量值微分，避免目标值突变引起的微分冲击
  *             - 配合不完全微分滤波，抑制高频噪声
  *
  *          2. 函数指针策略模式
  *             - PID算法可运行时切换（位置式/增量式/微分先行）
  *             - 滤波器策略可替换（一阶低通/滑动平均）
  *             - 硬件回调完全解耦（时间戳/输出/传感器读取）
  *
  *          3. 双环级联控制
  *             - 外环：温度PID → 输出基准
  *             - 内环：速率PID → 速率限制/增强
  *             - 支持升温/降温双向速率控制
  *
  *          4. 抗积分饱和 + 积分分离
  *             - 输出饱和时停止积分累积
  *             - 偏差过大时分离积分项，防止积分饱和超调
  *
  ******************************************************************************
  * @attention
  *
  * 微分先行原理：
  *   传统PID:  D = Kd * d[e(k)]/dt = Kd * d[SP-PV]/dt
  *            当SP突变时，e(k)突变 → D项产生巨大冲击
  *
  *   微分先行: D = -Kd * d[PV(k)]/dt
  *            D项仅响应PV变化，SP突变不影响D项
  *            客户运行中修改目标温度时，系统响应平稳
  *
  * 不完全微分原理：
  *   完全微分: D(k) = Kd * (PV(k)-PV(k-1)) / T
  *            对高频噪声极其敏感
  *
  *   不完全微分: D(k) = α*Td/(α*Td+Ts) * D(k-1)
  *                     + Kd*Ts/(α*Td+Ts) * (PV(k)-PV(k-1))/Ts
  *            α为不完全微分系数(0-1)，越大越接近完全微分
  *            引入一阶惯性滤波，平滑微分输出
  *
  ******************************************************************************
  */

#ifndef __RATE_PID_H__
#define __RATE_PID_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <string.h>

/* ========================================================================== */
/*                            类型定义与宏                                     */
/* ========================================================================== */

/* 控制器运行状态 */
typedef enum {
    RATE_PID_IDLE = 0,        /**< 空闲：未启动 */
    RATE_PID_HEATING,         /**< 升温：当前温度 < 目标温度 */
    RATE_PID_COOLING,         /**< 降温：当前温度 > 目标温度 */
    RATE_PID_HOLDING          /**< 恒温：温度进入死区范围 */
} RatePID_State_t;

/* PID算法类型 */
typedef enum {
    PID_ALG_POSITIONAL = 0,   /**< 位置式PID（传统） */
    PID_ALG_INCREMENTAL,      /**< 增量式PID */
    PID_ALG_DERIV_ON_MEAS     /**< 微分先行PID（默认推荐） */
} PID_Algorithm_t;

/* ========================================================================== */
/*                          函数指针类型定义                                    */
/* ========================================================================== */

/* 前向声明 */
struct DerivPID;

/**
  * @brief  PID算法计算函数指针
  * @param  pid: PID控制器指针
  * @param  setpoint: 目标设定值
  * @param  measurement: 当前测量值
  * @param  dt: 采样时间间隔（秒）
  * @retval PID输出值
  *
  * 策略模式核心：不同的PID算法实现统一接口
  * - PID_ALG_POSITIONAL:   位置式，输出绝对值
  * - PID_ALG_INCREMENTAL:  增量式，输出增量
  * - PID_ALG_DERIV_ON_MEAS: 微分先行，微分项仅对PV微分
  */
typedef float (*PID_AlgorithmFunc_t)(struct DerivPID *pid,
                                      float setpoint,
                                      float measurement,
                                      float dt);

/**
  * @brief  滤波器计算函数指针
  * @param  new_value: 新采样值
  * @param  prev_filtered: 上次滤波输出
  * @param  alpha: 滤波系数（含义由具体实现定义）
  * @retval 滤波后的值
  *
  * 可替换滤波策略：
  * - 一阶低通滤波
  * - 滑动平均滤波
  * - 中值滤波
  */
typedef float (*Filter_Func_t)(float new_value, float prev_filtered, float alpha);

/**
  * @brief  获取系统时间戳回调
  * @retval 当前时间（秒）
  */
typedef float (*GetTime_Callback_t)(void);

/**
  * @brief  获取传感器测量值回调
  * @param  user_data: 用户自定义数据
  * @retval 测量值（如当前温度）
  */
typedef float (*GetMeasurement_Callback_t)(void *user_data);

/**
  * @brief  设置控制输出回调
  * @param  output: 输出值（含义由用户定义，如PWM占空比0-100%）
  * @param  user_data: 用户自定义数据
  */
typedef void (*SetOutput_Callback_t)(float output, void *user_data);

/**
  * @brief  调试输出回调（可选）
  * @param  pid: PID控制器指针
  * @param  user_data: 用户自定义数据
  */
typedef void (*DebugPrint_Callback_t)(void *pid, void *user_data);

/* ========================================================================== */
/*                          PID参数与状态结构体                                 */
/* ========================================================================== */

/**
  * @brief  PID参数结构体
  */
typedef struct {
    float kp;                    /**< 比例系数 */
    float ki;                    /**< 积分系数 */
    float kd;                    /**< 微分系数 */
    float integral_limit;        /**< 积分限幅值（抗积分饱和） */
    float output_max;            /**< 输出上限 */
    float output_min;            /**< 输出下限 */
    float deriv_filter_alpha;    /**< 不完全微分滤波系数（0.0~1.0，越大越接近完全微分） */
    float integral_separation;   /**< 积分分离阈值（偏差超过此值时取消积分） */
    uint8_t enable_anti_windup;  /**< 是否启用抗积分饱和 (0/1) */
    uint8_t enable_integral_sep; /**< 是否启用积分分离 (0/1) */
} PID_Params_t;

/**
  * @brief  PID运行状态结构体
  */
typedef struct {
    float prop_output;           /**< 比例项输出 */
    float int_output;            /**< 积分项输出 */
    float diff_output;           /**< 微分项输出 */
    float integral;              /**< 积分累积值 */
    float prev_measurement;      /**< 上次测量值（微分先行用） */
    float prev_diff_output;      /**< 上次微分输出（不完全微分用） */
    float prev_error;            /**< 上次误差（增量式PID用） */
    float prev_prev_error;       /**< 上上次误差（增量式PID用） */
    float prev_output;           /**< 上次输出（增量式PID用） */
    uint8_t first_run;           /**< 首次运行标志 */
} PID_State_t;

/* ========================================================================== */
/*                       基础微分先行PID控制器                                  */
/* ========================================================================== */

/**
  * @brief  微分先行PID控制器结构体
  *
  * 独立的单环PID控制器，通过函数指针选择算法策略。
  * 可单独使用，也可作为RatePID的内环/外环组件。
  */
typedef struct DerivPID {
    /* 参数与状态 */
    PID_Params_t     params;     /**< PID参数 */
    PID_State_t      state;      /**< 运行状态 */

    /* 算法策略函数指针 */
    PID_AlgorithmFunc_t algorithm;  /**< PID算法计算函数（核心策略） */

    /* 滤波器函数指针 */
    Filter_Func_t    filter;     /**< 滤波器函数（可选，为NULL则不滤波） */
    float            filter_alpha; /**< 滤波系数 */
} DerivPID_t;

/* ========================================================================== */
/*                      升降温速率PID控制器                                    */
/* ========================================================================== */

/**
  * @brief  速率控制器配置结构体
  */
typedef struct {
    /* 双环PID参数 */
    PID_Params_t temp_pid_params;    /**< 温度PID参数 */
    PID_Params_t rate_pid_params;    /**< 速率PID参数 */

    /* PID算法选择 */
    PID_Algorithm_t temp_pid_alg;    /**< 温度PID算法类型 */
    PID_Algorithm_t rate_pid_alg;    /**< 速率PID算法类型 */

    /* 控制参数 */
    float sample_time;               /**< 采样周期（秒） */
    float temp_deadband;             /**< 温度死区（℃） */
    float rate_filter_alpha;         /**< 速率低通滤波系数（0~1） */
    float rate_control_weight;       /**< 速率控制权重（0~1，越大速率环介入越强） */
    float output_rate_limit;         /**< 输出变化率限制（%/秒），0表示不限 */

    /* 回调函数指针 */
    GetTime_Callback_t       get_time_cb;        /**< 获取时间戳 */
    GetMeasurement_Callback_t get_measurement_cb; /**< 获取测量值（可选） */
    SetOutput_Callback_t     set_output_cb;       /**< 设置输出 */
    DebugPrint_Callback_t    debug_print_cb;      /**< 调试输出（可选） */
    Filter_Func_t            rate_filter_cb;      /**< 速率滤波器（可选） */

    /* 用户数据 */
    void *user_data;                 /**< 用户自定义数据指针 */
} RatePID_Config_t;

/**
  * @brief  速率控制器状态信息（只读查询用）
  */
typedef struct {
    RatePID_State_t state;       /**< 运行状态 */
    float current_temp;          /**< 当前温度 */
    float target_temp;           /**< 目标温度 */
    float temp_error;            /**< 温度偏差 */
    float actual_rate;           /**< 实际升降温速率（℃/秒） */
    float target_rate;           /**< 目标升降温速率（℃/秒） */
    float rate_error;            /**< 速率偏差 */
    float output;                /**< 当前输出 */
    float temp_pid_integral;     /**< 温度PID积分累积 */
    float rate_pid_integral;     /**< 速率PID积分累积 */
    float temp_pid_output;       /**< 温度PID输出分量 */
    float rate_pid_output;       /**< 速率PID输出分量 */
} RatePID_Status_t;

/**
  * @brief  升降温速率PID控制器结构体
  *
  * 双环级联控制：
  *   外环 DerivPID_t temp_pid  → 温度偏差 → 输出基准
  *   内环 DerivPID_t rate_pid  → 速率偏差 → 速率修正
  */
typedef struct {
    RatePID_Config_t config;         /**< 配置参数 */

    /* 双环PID控制器 */
    DerivPID_t temp_pid;             /**< 温度PID（外环） */
    DerivPID_t rate_pid;             /**< 速率PID（内环） */

    /* 运行状态 */
    RatePID_State_t state;           /**< 当前运行状态 */
    float target_temperature;        /**< 目标温度（℃） */
    float target_heating_rate;       /**< 目标升温速率（℃/秒，正值） */
    float target_cooling_rate;       /**< 目标降温速率（℃/秒，正值） */

    /* 过程变量 */
    float current_temperature;       /**< 当前温度 */
    float prev_temperature;          /**< 上次温度 */
    float actual_rate;               /**< 实际升降温速率（升温为正，降温为负） */

    /* 输出 */
    float current_output;            /**< 当前输出 */
    float last_output;               /**< 上次输出 */

    /* 时间 */
    float last_timestamp;            /**< 上次时间戳 */
    float dt;                        /**< 实际采样间隔 */
} RatePID_Controller_t;

/* ========================================================================== */
/*                       基础DerivPID API                                      */
/* ========================================================================== */

/**
  * @brief  初始化基础微分先行PID控制器
  * @param  pid: 控制器指针
  * @param  params: PID参数
  * @param  algorithm: PID算法函数指针（传NULL则默认微分先行）
  * @retval 无
  */
void DerivPID_Init(DerivPID_t *pid, const PID_Params_t *params,
                   PID_AlgorithmFunc_t algorithm);

/**
  * @brief  重置PID控制器状态（保留参数）
  * @param  pid: 控制器指针
  * @retval 无
  */
void DerivPID_Reset(DerivPID_t *pid);

/**
  * @brief  执行一次PID计算
  * @param  pid: 控制器指针
  * @param  setpoint: 目标设定值
  * @param  measurement: 当前测量值
  * @param  dt: 时间间隔（秒）
  * @retval PID输出值
  */
float DerivPID_Calculate(DerivPID_t *pid, float setpoint,
                          float measurement, float dt);

/**
  * @brief  运行时切换PID算法策略
  * @param  pid: 控制器指针
  * @param  algorithm: 算法类型枚举
  * @retval 无
  */
void DerivPID_SetAlgorithm(DerivPID_t *pid, PID_Algorithm_t algorithm);

/**
  * @brief  更新PID参数
  * @param  pid: 控制器指针
  * @param  params: 新参数
  * @retval 无
  */
void DerivPID_UpdateParams(DerivPID_t *pid, const PID_Params_t *params);

/* ========================================================================== */
/*                    内置PID算法实现（可直接用作函数指针）                      */
/* ========================================================================== */

/**
  * @brief  位置式PID算法
  * @note   输出 = Kp*e + Ki*∫e*dt + Kd*de/dt
  */
float PID_Algorithm_Positional(DerivPID_t *pid, float setpoint,
                                float measurement, float dt);

/**
  * @brief  增量式PID算法
  * @note   Δout = Kp*(e(k)-e(k-1)) + Ki*e(k) + Kd*(e(k)-2*e(k-1)+e(k-2))
  */
float PID_Algorithm_Incremental(DerivPID_t *pid, float setpoint,
                                 float measurement, float dt);

/**
  * @brief  微分先行PID算法（推荐）
  * @note   P = Kp*e, I = Ki*∫e*dt, D = -Kd*d[PV]/dt
  *         微分项仅对测量值微分，配合不完全微分滤波
  */
float PID_Algorithm_DerivOnMeas(DerivPID_t *pid, float setpoint,
                                 float measurement, float dt);

/* ========================================================================== */
/*                    内置滤波器实现（可直接用作函数指针）                       */
/* ========================================================================== */

/**
  * @brief  一阶低通滤波器
  * @param  new_value: 新采样值
  * @param  prev_filtered: 上次滤波输出
  * @param  alpha: 滤波系数（0~1，越大跟踪越快，噪声抑制越弱）
  * @retval 滤波后的值
  */
float Filter_LowPass1st(float new_value, float prev_filtered, float alpha);

/**
  * @brief  限幅滤波器（消抖）
  * @param  new_value: 新采样值
  * @param  prev_filtered: 上次滤波输出
  * @param  alpha: 限幅阈值（新值与旧值差超过此值才更新）
  * @retval 滤波后的值
  */
float Filter_Clamp(float new_value, float prev_filtered, float alpha);

/* ========================================================================== */
/*                     升降温速率PID控制器 API                                  */
/* ========================================================================== */

/**
  * @brief  初始化升降温速率PID控制器
  * @param  ctrl: 控制器指针
  * @param  config: 配置参数指针
  * @retval 无
  */
void RatePID_Init(RatePID_Controller_t *ctrl, const RatePID_Config_t *config);

/**
  * @brief  设置目标温度和升降温速率
  * @param  ctrl: 控制器指针
  * @param  target_temp: 目标温度（℃）
  * @param  heating_rate: 升温速率（℃/秒，正值）
  * @param  cooling_rate: 降温速率（℃/秒，正值）
  * @retval 无
  */
void RatePID_SetTarget(RatePID_Controller_t *ctrl, float target_temp,
                        float heating_rate, float cooling_rate);

/**
  * @brief  仅修改目标温度（保持当前速率不变）
  * @param  ctrl: 控制器指针
  * @param  target_temp: 新目标温度
  * @retval 无
  */
void RatePID_SetTargetTemp(RatePID_Controller_t *ctrl, float target_temp);

/**
  * @brief  停止控制，输出归零
  * @param  ctrl: 控制器指针
  * @retval 无
  */
void RatePID_Stop(RatePID_Controller_t *ctrl);

/**
  * @brief  重置控制器（清空状态，保留配置）
  * @param  ctrl: 控制器指针
  * @retval 无
  */
void RatePID_Reset(RatePID_Controller_t *ctrl);

/**
  * @brief  执行一次速率PID计算（主循环调用）
  * @param  ctrl: 控制器指针
  * @param  current_temp: 当前温度（℃），若传<0则使用get_measurement_cb回调
  * @retval 控制输出值
  */
float RatePID_Calculate(RatePID_Controller_t *ctrl, float current_temp);

/**
  * @brief  运行时切换温度PID或速率PID的算法策略
  * @param  ctrl: 控制器指针
  * @param  is_temp_pid: 1=温度PID, 0=速率PID
  * @param  algorithm: 算法类型
  * @retval 无
  */
void RatePID_SetAlgorithm(RatePID_Controller_t *ctrl, uint8_t is_temp_pid,
                           PID_Algorithm_t algorithm);

/**
  * @brief  更新温度PID或速率PID参数
  * @param  ctrl: 控制器指针
  * @param  is_temp_pid: 1=温度PID, 0=速率PID
  * @param  params: 新参数指针
  * @retval 无
  */
void RatePID_UpdateParams(RatePID_Controller_t *ctrl, uint8_t is_temp_pid,
                           const PID_Params_t *params);

/**
  * @brief  获取控制器状态信息
  * @param  ctrl: 控制器指针
  * @param  status: 状态结构体指针（输出）
  * @retval 无
  */
void RatePID_GetStatus(const RatePID_Controller_t *ctrl, RatePID_Status_t *status);

/**
  * @brief  调试打印（调用debug_print_cb回调）
  * @param  ctrl: 控制器指针
  * @retval 无
  */
void RatePID_PrintDebug(RatePID_Controller_t *ctrl);

/* ========================================================================== */
/*                            默认参数宏                                       */
/* ========================================================================== */

/* 温度PID默认参数（中功率加热系统 1~5kW） */
#define RATE_PID_TEMP_KP_DEFAULT        2.0f
#define RATE_PID_TEMP_KI_DEFAULT        0.5f
#define RATE_PID_TEMP_KD_DEFAULT        0.1f
#define RATE_PID_TEMP_INTEGRAL_LIMIT    50.0f
#define RATE_PID_TEMP_DERIV_ALPHA       0.6f   /* 不完全微分系数 */
#define RATE_PID_TEMP_SEP_THRESHOLD     20.0f  /* 积分分离阈值 */

/* 速率PID默认参数 */
#define RATE_PID_RATE_KP_DEFAULT        5.0f
#define RATE_PID_RATE_KI_DEFAULT        1.0f
#define RATE_PID_RATE_KD_DEFAULT        0.0f
#define RATE_PID_RATE_INTEGRAL_LIMIT    30.0f
#define RATE_PID_RATE_DERIV_ALPHA       0.5f

/* 控制参数默认值 */
#define RATE_PID_SAMPLE_TIME_DEFAULT    0.5f   /* 采样周期0.5秒 */
#define RATE_PID_DEADBAND_DEFAULT       1.0f   /* 温度死区1℃ */
#define RATE_PID_FILTER_ALPHA_DEFAULT   0.3f   /* 速率滤波系数 */
#define RATE_PID_RATE_WEIGHT_DEFAULT    0.8f   /* 速率控制权重 */
#define RATE_PID_OUTPUT_RATE_LIMIT      10.0f  /* 输出变化率10%/s */

#ifdef __cplusplus
}
#endif

#endif /* __RATE_PID_H__ */
