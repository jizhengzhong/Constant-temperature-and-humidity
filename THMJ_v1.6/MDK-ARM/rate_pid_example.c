/**
  ******************************************************************************
  * @file    rate_pid_example.c
  * @brief   微分先行PID升降温速率控制器 - 使用示例
  *
  *          演示内容：
  *          1. 回调函数实现（时间戳/输出/传感器/调试）
  *          2. 控制器初始化与参数配置
  *          3. 升温/降温/恒温控制
  *          4. 运行时切换PID算法
  *          5. FreeRTOS任务集成
  *          6. 自定义滤波器/算法的扩展方式
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "rate_pid.h"
#include <stdio.h>

/* ========================================================================== */
/*                        回调函数实现（用户移植）                              */
/* ========================================================================== */

/**
  * @brief  获取系统时间戳回调
  * @retval 当前时间（秒）
  *
  * 移植说明：
  * - STM32 HAL:     HAL_GetTick() / 1000.0f
  * - FreeRTOS:      xTaskGetTickCount() / (float)configTICK_RATE_HZ
  * - 其他系统:      使用对应的毫秒/秒计数函数
  */
static float Example_GetTimestamp(void)
{
    /* TODO: 替换为实际系统时间函数 */
    /* return HAL_GetTick() / 1000.0f; */
    return 0.0f;
}

/**
  * @brief  获取温度测量值回调（可选）
  * @param  user_data: 用户数据指针
  * @retval 当前温度（℃）
  *
  * 移植说明：
  * - 从ADC采样值转换
  * - 从PT100/热电偶传感器读取
  * - 从全局变量获取
  */
static float Example_GetTemperature(void *user_data)
{
    /* TODO: 替换为实际温度读取函数 */
    /* extern float g_current_temperature; */
    /* return g_current_temperature; */
    (void)user_data;
    return 25.0f;
}

/**
  * @brief  设置输出功率回调
  * @param  output: 输出百分比 (0-100%)
  * @param  user_data: 用户数据指针
  *
  * 移植说明：
  * - PWM控制: 映射到定时器占空比
  * - SSR控制: 时间比例通断
  * - DAC输出: 映射到模拟量
  */
static void Example_SetOutput(float output, void *user_data)
{
    /* 示例：PWM控制 */
    /*
    uint16_t pwm_value = (uint16_t)(output * PWM_PERIOD / 100.0f);
    __HAL_TIM_SET_COMPARE(&htim, TIM_CHANNEL_1, pwm_value);
    */

    /* 示例：SSR时间比例控制 */
    /*
    g_ssr_on_time_ms = (uint16_t)(output * SSR_CYCLE_MS / 100.0f);
    */

    (void)user_data;
    printf("Output: %.1f%%\r\n", output);
}

/**
  * @brief  调试打印回调
  * @param  pid: 控制器指针
  * @param  user_data: 用户数据指针
  */
static void Example_DebugPrint(void *pid, void *user_data)
{
    RatePID_Controller_t *ctrl = (RatePID_Controller_t *)pid;
    RatePID_Status_t status;

    if (ctrl == NULL) return;

    RatePID_GetStatus(ctrl, &status);

    printf("=== Rate PID ===\r\n");
    printf("State: %s\r\n",
           status.state == RATE_PID_IDLE    ? "IDLE" :
           status.state == RATE_PID_HEATING ? "HEATING" :
           status.state == RATE_PID_COOLING ? "COOLING" : "HOLDING");
    printf("Temp: %.1f -> %.1f (err=%.2f)\r\n",
           status.current_temp, status.target_temp, status.temp_error);
    printf("Rate: actual=%.2f target=%.2f (err=%.2f)\r\n",
           status.actual_rate, status.target_rate, status.rate_error);
    printf("Output: %.1f%%\r\n", status.output);
    printf("T-PID: out=%.2f int=%.2f\r\n", status.temp_pid_output, status.temp_pid_integral);
    printf("R-PID: out=%.2f int=%.2f\r\n", status.rate_pid_output, status.rate_pid_integral);
    printf("================\r\n");

    (void)user_data;
}

/* ========================================================================== */
/*                        控制器实例与初始化                                    */
/* ========================================================================== */

static RatePID_Controller_t g_heating_ctrl;

/**
  * @brief  初始化控制器（含完整参数配置）
  */
void Example_RatePID_Init(void)
{
    RatePID_Config_t config;
    memset(&config, 0, sizeof(RatePID_Config_t));

    /* ====== 温度PID参数（微分先行 + 不完全微分） ====== */
    config.temp_pid_params.kp                    = RATE_PID_TEMP_KP_DEFAULT;      /* 2.0 */
    config.temp_pid_params.ki                    = RATE_PID_TEMP_KI_DEFAULT;      /* 0.5 */
    config.temp_pid_params.kd                    = RATE_PID_TEMP_KD_DEFAULT;      /* 0.1 */
    config.temp_pid_params.integral_limit        = RATE_PID_TEMP_INTEGRAL_LIMIT;  /* 50.0 */
    config.temp_pid_params.output_max            = 100.0f;
    config.temp_pid_params.output_min            = 0.0f;
    config.temp_pid_params.deriv_filter_alpha    = RATE_PID_TEMP_DERIV_ALPHA;     /* 0.6 不完全微分系数 */
    config.temp_pid_params.integral_separation   = RATE_PID_TEMP_SEP_THRESHOLD;   /* 20.0 积分分离阈值 */
    config.temp_pid_params.enable_anti_windup    = 1;   /* 启用抗积分饱和 */
    config.temp_pid_params.enable_integral_sep   = 1;   /* 启用积分分离 */

    /* ====== 速率PID参数 ====== */
    config.rate_pid_params.kp                    = RATE_PID_RATE_KP_DEFAULT;      /* 5.0 */
    config.rate_pid_params.ki                    = RATE_PID_RATE_KI_DEFAULT;      /* 1.0 */
    config.rate_pid_params.kd                    = RATE_PID_RATE_KD_DEFAULT;      /* 0.0 */
    config.rate_pid_params.integral_limit        = RATE_PID_RATE_INTEGRAL_LIMIT;  /* 30.0 */
    config.rate_pid_params.output_max            = 100.0f;
    config.rate_pid_params.output_min            = 0.0f;
    config.rate_pid_params.deriv_filter_alpha    = RATE_PID_RATE_DERIV_ALPHA;     /* 0.5 */
    config.rate_pid_params.integral_separation   = 50.0f;
    config.rate_pid_params.enable_anti_windup    = 1;
    config.rate_pid_params.enable_integral_sep   = 0;

    /* ====== PID算法选择 ====== */
    config.temp_pid_alg = PID_ALG_DERIV_ON_MEAS;   /* 温度环：微分先行（推荐） */
    config.rate_pid_alg = PID_ALG_DERIV_ON_MEAS;   /* 速率环：微分先行 */

    /* ====== 控制参数 ====== */
    config.sample_time         = RATE_PID_SAMPLE_TIME_DEFAULT;    /* 0.5秒 */
    config.temp_deadband       = RATE_PID_DEADBAND_DEFAULT;       /* 1.0℃ */
    config.rate_filter_alpha   = RATE_PID_FILTER_ALPHA_DEFAULT;   /* 0.3 */
    config.rate_control_weight = RATE_PID_RATE_WEIGHT_DEFAULT;    /* 0.8 */
    config.output_rate_limit   = RATE_PID_OUTPUT_RATE_LIMIT;      /* 10%/s */

    /* ====== 回调函数 ====== */
    config.get_time_cb         = Example_GetTimestamp;
    config.get_measurement_cb  = Example_GetTemperature;
    config.set_output_cb       = Example_SetOutput;
    config.debug_print_cb      = Example_DebugPrint;
    config.rate_filter_cb      = Filter_LowPass1st;    /* 使用内置一阶低通 */
    config.user_data           = NULL;

    /* ====== 初始化 ====== */
    RatePID_Init(&g_heating_ctrl, &config);
}

/* ========================================================================== */
/*                          控制操作示例                                       */
/* ========================================================================== */

/**
  * @brief  启动升温控制
  * @note   目标120℃，升温速率2℃/s，降温速率3℃/s
  */
void Example_StartHeating(void)
{
    RatePID_SetTarget(&g_heating_ctrl, 120.0f, 2.0f, 3.0f);
    printf("Start: Target=120C, HeatRate=2C/s, CoolRate=3C/s\r\n");
}

/**
  * @brief  启动降温控制
  * @note   从当前温度降温到50℃，降温速率1℃/s
  */
void Example_StartCooling(void)
{
    RatePID_SetTarget(&g_heating_ctrl, 50.0f, 2.0f, 1.0f);
    printf("Start cooling: Target=50C, CoolRate=1C/s\r\n");
}

/**
  * @brief  运行中修改目标温度（微分先行保证无微分冲击）
  * @note   这是微分先行PID的核心使用场景
  */
void Example_ChangeTargetTemp(void)
{
    /* 客户运行中修改目标温度 */
    /* 微分先行PID：D项仅对PV微分，SP突变不会引起微分冲击 */
    RatePID_SetTargetTemp(&g_heating_ctrl, 150.0f);
    printf("Target changed to 150C (no derivative kick)\r\n");
}

/**
  * @brief  运行时切换PID算法
  * @note   演示函数指针策略模式的动态切换能力
  */
void Example_SwitchAlgorithm(void)
{
    /* 从微分先行切换到增量式 */
    RatePID_SetAlgorithm(&g_heating_ctrl, 1, PID_ALG_INCREMENTAL);
    printf("Temp PID switched to Incremental algorithm\r\n");

    /* 切换回微分先行（推荐） */
    RatePID_SetAlgorithm(&g_heating_ctrl, 1, PID_ALG_DERIV_ON_MEAS);
    printf("Temp PID switched back to Derivative-on-Measurement\r\n");
}

/**
  * @brief  停止控制
  */
void Example_Stop(void)
{
    RatePID_Stop(&g_heating_ctrl);
    printf("Control stopped\r\n");
}

/**
  * @brief  定时调用PID计算
  * @param  current_temp: 当前温度
  * @retval 输出功率 (0-100%)
  */
float Example_Update(float current_temp)
{
    return RatePID_Calculate(&g_heating_ctrl, current_temp);
}

/* ========================================================================== */
/*                    仅使用基础DerivPID（单环）示例                             */
/* ========================================================================== */

/**
  * @brief  单环微分先行PID使用示例
  *
  * 如果只需要简单的温度PID控制（无需速率控制），
  * 可以直接使用DerivPID_t，无需RatePID_Controller_t
  */
void Example_SinglePID(void)
{
    DerivPID_t pid;
    PID_Params_t params = {
        .kp = 2.0f,
        .ki = 0.5f,
        .kd = 0.1f,
        .integral_limit = 50.0f,
        .output_max = 100.0f,
        .output_min = 0.0f,
        .deriv_filter_alpha = 0.6f,      /* 不完全微分系数 */
        .integral_separation = 20.0f,    /* 积分分离阈值 */
        .enable_anti_windup = 1,
        .enable_integral_sep = 1
    };

    /* 初始化：传NULL使用默认微分先行算法 */
    DerivPID_Init(&pid, &params, NULL);

    /* 也可显式指定算法 */
    /* DerivPID_Init(&pid, &params, PID_Algorithm_DerivOnMeas); */

    /* 运行时切换算法 */
    /* DerivPID_SetAlgorithm(&pid, PID_ALG_INCREMENTAL); */

    /* 设置输出滤波器 */
    pid.filter = Filter_LowPass1st;
    pid.filter_alpha = 0.3f;

    /* 模拟计算 */
    float setpoint = 100.0f;
    float measurement = 25.0f;
    float dt = 0.5f;

    for (int i = 0; i < 10; i++) {
        float output = DerivPID_Calculate(&pid, setpoint, measurement, dt);
        printf("Step %d: SP=%.1f PV=%.1f Out=%.1f\r\n",
               i, setpoint, measurement, output);
        measurement += output * 0.02f;   /* 简化被控对象模型 */
    }
}

/* ========================================================================== */
/*                       自定义扩展示例                                        */
/* ========================================================================== */

/**
  * @brief  自定义PID算法示例：带前馈的PID
  *
  * 演示如何通过函数指针扩展自定义PID算法
  * 只需实现 PID_AlgorithmFunc_t 签名的函数即可
  */
float PID_Algorithm_Feedforward(DerivPID_t *pid, float setpoint,
                                 float measurement, float dt)
{
    /* 先调用标准微分先行PID */
    float output = PID_Algorithm_DerivOnMeas(pid, setpoint, measurement, dt);

    /* 加入前馈补偿（示例：基于设定值的前馈） */
    float ff_gain = 0.5f;   /* 前馈增益，需根据系统模型调整 */
    output += ff_gain * setpoint;

    /* 输出限幅 */
    if (output > pid->params.output_max) output = pid->params.output_max;
    else if (output < pid->params.output_min) output = pid->params.output_min;

    pid->state.prev_output = output;
    return output;
}

/**
  * @brief  使用自定义算法初始化控制器
  */
void Example_CustomAlgorithm(void)
{
    DerivPID_t pid;
    PID_Params_t params = {
        .kp = 2.0f, .ki = 0.5f, .kd = 0.1f,
        .integral_limit = 50.0f,
        .output_max = 100.0f, .output_min = 0.0f,
        .deriv_filter_alpha = 0.6f,
        .integral_separation = 20.0f,
        .enable_anti_windup = 1,
        .enable_integral_sep = 1
    };

    /* 使用自定义前馈PID算法 */
    DerivPID_Init(&pid, &params, PID_Algorithm_Feedforward);

    printf("Custom feedforward PID initialized\r\n");
}

/* ========================================================================== */
/*                      FreeRTOS任务集成示例                                   */
/* ========================================================================== */

/**
  * @brief  FreeRTOS温控任务示例
  *
  * void TemperatureControlTask(void *argument)
  * {
  *     float current_temp;
  *     float output;
  *
  *     // 初始化控制器
  *     Example_RatePID_Init();
  *
  *     // 启动升温：目标120℃，升温2℃/s，降温3℃/s
  *     RatePID_SetTarget(&g_heating_ctrl, 120.0f, 2.0f, 3.0f);
  *
  *     for (;;) {
  *         // 读取温度传感器
  *         current_temp = ReadTemperature();
  *
  *         // 执行PID计算（也可传-1使用回调获取温度）
  *         output = RatePID_Calculate(&g_heating_ctrl, current_temp);
  *
  *         // 周期性调试输出
  *         static uint8_t debug_cnt = 0;
  *         if (++debug_cnt >= 10) {
  *             debug_cnt = 0;
  *             RatePID_PrintDebug(&g_heating_ctrl);
  *         }
  *
  *         // 延时到下一个采样周期
  *         osDelay(pdMS_TO_TICKS(500));
  *     }
  * }
  */

/* ========================================================================== */
/*                      参数整定指南                                           */
/* ========================================================================== */

/**
  * 温度PID参数整定步骤（微分先行模式）：
  *
  * 1. 先将 Ki=0, Kd=0, deriv_filter_alpha=0（完全微分先行）
  * 2. 逐渐增大Kp，直到系统出现轻微振荡
  * 3. 将Kp设为振荡临界值的60%
  * 4. 逐渐增大Ki，消除稳态误差
  * 5. 增大Kd + deriv_filter_alpha，抑制超调
  *    - deriv_filter_alpha=0.6~0.7 通常效果好
  *    - 先设Kd=0.1，观察效果再微调
  *
  * 速率PID参数整定步骤：
  * 1. Ki=0, Kd=0
  * 2. 设置较大Kp（5~15），快速响应速率偏差
  * 3. 适当增加Ki（0.5~2.0）消除稳态速率误差
  * 4. Kd通常为0（速率信号已有滤波）
  *
  * 常见系统参考参数：
  *
  * 小功率加热 (<1kW):
  *   温度PID: Kp=3.0, Ki=0.8, Kd=0.2, alpha=0.6
  *   速率PID: Kp=8.0, Ki=2.0, Kd=0
  *   升温速率: 1~3℃/s
  *
  * 中功率加热 (1~5kW):
  *   温度PID: Kp=2.0, Ki=0.5, Kd=0.1, alpha=0.6
  *   速率PID: Kp=5.0, Ki=1.0, Kd=0
  *   升温速率: 2~5℃/s
  *
  * 大功率加热 (>5kW):
  *   温度PID: Kp=1.5, Ki=0.3, Kd=0.05, alpha=0.5
  *   速率PID: Kp=3.0, Ki=0.5, Kd=0
  *   升温速率: 3~10℃/s
  *
  * 常见问题排查：
  *
  * Q: 升温速率控制不精确
  * A: 增大速率PID的Kp值；减小rate_filter_alpha加强滤波；
  *    提高采样频率
  *
  * Q: 到达目标温度后超调过大
  * A: 增大温度PID的Kd值；增大deriv_filter_alpha使微分更平滑；
  *    增大rate_control_weight加强速率限制；启用积分分离
  *
  * Q: 修改目标温度时输出突变
  * A: 确认使用的是微分先行算法(PID_ALG_DERIV_ON_MEAS)；
  *    检查积分项是否在目标改变时正确重置
  *
  * Q: 升降温切换时输出抖动
  * A: 增大temp_deadband避免频繁状态切换；
  *    增大output_rate_limit限制输出变化速度
  */
