/**
  ******************************************************************************
  * @file    rate_pid.c
  * @brief   微分先行PID升降温速率控制器 - 实现文件
  *
  *          函数指针策略模式实现：
  *          - PID算法通过函数指针切换（位置式/增量式/微分先行）
  *          - 滤波器通过函数指针替换
  *          - 硬件回调完全解耦
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "rate_pid.h"
#include <math.h>

/* ========================================================================== */
/*                     内置滤波器实现                                          */
/* ========================================================================== */

/**
  * @brief  一阶低通滤波器
  *
  * 公式: y(k) = α * x(k) + (1-α) * y(k-1)
  * α越大跟踪越快但噪声抑制弱，α越小跟踪慢但平滑
  */
float Filter_LowPass1st(float new_value, float prev_filtered, float alpha)
{
    return alpha * new_value + (1.0f - alpha) * prev_filtered;
}

/**
  * @brief  限幅滤波器（消抖）
  *
  * 新值与旧值差超过阈值才更新，否则保持旧值
  */
float Filter_Clamp(float new_value, float prev_filtered, float alpha)
{
    if (fabsf(new_value - prev_filtered) > alpha) {
        return new_value;
    }
    return prev_filtered;
}

/* ========================================================================== */
/*                     内置PID算法实现                                         */
/* ========================================================================== */

/**
  * @brief  位置式PID算法
  *
  * 输出 = Kp*e(k) + Ki*∑e(j)*dt + Kd*(e(k)-e(k-1))/dt
  *
  * 特点：
  * - 输出为绝对值，与历史状态相关
  * - 微分项对设定值变化敏感
  * - 适合执行机构无积分特性的场合
  */
float PID_Algorithm_Positional(DerivPID_t *pid, float setpoint,
                                float measurement, float dt)
{
    float error, output;
    PID_Params_t *p = &pid->params;
    PID_State_t  *s = &pid->state;

    if (dt <= 0.0f) return s->prev_output;

    /* 误差 */
    error = setpoint - measurement;

    /* ---- 比例项 ---- */
    s->prop_output = p->kp * error;

    /* ---- 积分项（含积分分离 + 抗积分饱和） ---- */
    if (p->enable_integral_sep && (fabsf(error) > p->integral_separation)) {
        /* 积分分离：偏差过大时不累积积分 */
    } else {
        if (p->enable_anti_windup) {
            /* 抗积分饱和：输出饱和时停止积分 */
            float temp_out = s->prop_output + p->ki * s->integral + s->diff_output;
            if (!((temp_out > p->output_max && error > 0) ||
                  (temp_out < p->output_min && error < 0))) {
                s->integral += error * dt;
            }
        } else {
            s->integral += error * dt;
        }
    }

    /* 积分限幅 */
    if (s->integral > p->integral_limit)
        s->integral = p->integral_limit;
    else if (s->integral < -p->integral_limit)
        s->integral = -p->integral_limit;

    s->int_output = p->ki * s->integral;

    /* ---- 微分项（传统：对误差微分） ---- */
    if (!s->first_run) {
        float d_error = (error - s->prev_error) / dt;
        s->diff_output = p->kd * d_error;
    }

    /* 更新历史 */
    s->prev_error = error;
    s->prev_measurement = measurement;
    if (s->first_run) s->first_run = 0;

    /* 总输出 */
    output = s->prop_output + s->int_output + s->diff_output;

    /* 输出限幅 */
    if (output > p->output_max) output = p->output_max;
    else if (output < p->output_min) output = p->output_min;

    s->prev_output = output;
    return output;
}

/**
  * @brief  增量式PID算法
  *
  * Δout = Kp*(e(k)-e(k-1)) + Ki*e(k)*dt + Kd*(e(k)-2*e(k-1)+e(k-2))/dt
  * out(k) = out(k-1) + Δout
  *
  * 特点：
  * - 输出为增量，控制平稳
  * - 切换手动/自动时无冲击
  * - 微分项仍对设定值变化敏感
  * - 适合步进电机等积分型执行机构
  */
float PID_Algorithm_Incremental(DerivPID_t *pid, float setpoint,
                                 float measurement, float dt)
{
    float error, delta_output, output;
    float delta_p, delta_i, delta_d;
    PID_Params_t *p = &pid->params;
    PID_State_t  *s = &pid->state;

    if (dt <= 0.0f) return s->prev_output;

    error = setpoint - measurement;

    /* 增量计算 */
    delta_p = p->kp * (error - s->prev_error);
    delta_i = p->ki * error * dt;

    if (!s->first_run) {
        delta_d = p->kd * (error - 2.0f * s->prev_error + s->prev_prev_error) / dt;
    } else {
        delta_d = 0.0f;
    }

    delta_output = delta_p + delta_i + delta_d;

    /* 增量限幅 */
    if (delta_output > 20.0f) delta_output = 20.0f;
    else if (delta_output < -20.0f) delta_output = -20.0f;

    /* 累加输出 */
    output = s->prev_output + delta_output;

    /* 输出限幅 */
    if (output > p->output_max) output = p->output_max;
    else if (output < p->output_min) output = p->output_min;

    /* 记录分量 */
    s->prop_output = delta_p;
    s->int_output  = delta_i;
    s->diff_output = delta_d;
    s->integral += error * dt;   /* 仅用于状态监控 */
    if (s->integral > p->integral_limit) s->integral = p->integral_limit;
    else if (s->integral < -p->integral_limit) s->integral = -p->integral_limit;

    /* 更新历史 */
    s->prev_prev_error = s->prev_error;
    s->prev_error = error;
    s->prev_measurement = measurement;
    s->prev_output = output;
    if (s->first_run) s->first_run = 0;

    return output;
}

/**
  * @brief  微分先行PID算法（推荐）
  *
  * P = Kp * e(k)
  * I = Ki * ∫e(t)dt         （含积分分离 + 抗积分饱和）
  * D = -Kd * d[PV]/dt        （微分先行 + 不完全微分滤波）
  *
  * 不完全微分递推公式：
  *   D(k) = α_d * D(k-1) - Kd * (PV(k) - PV(k-1)) / dt * (1 - α_d)
  *   其中 α_d = deriv_filter_alpha（越大越接近完全微分）
  *
  * 核心优势：
  * - 目标值SP突变时，D项不受影响（仅对PV微分）
  * - 不完全微分平滑D项输出，抑制高频噪声
  * - 适合客户运行过程中频繁修改目标温度的场景
  */
float PID_Algorithm_DerivOnMeas(DerivPID_t *pid, float setpoint,
                                 float measurement, float dt)
{
    float error, output;
    float measurement_deriv;
    float alpha_d;
    PID_Params_t *p = &pid->params;
    PID_State_t  *s = &pid->state;

    if (dt <= 0.0f) return s->prev_output;

    /* 误差 */
    error = setpoint - measurement;

    /* ========== 比例项: P = Kp * e(k) ========== */
    s->prop_output = p->kp * error;

    /* ========== 积分项: I = Ki * ∫e(t)dt ========== */
    if (p->enable_integral_sep && (fabsf(error) > p->integral_separation)) {
        /* 积分分离：偏差超过阈值时不累积积分，防止大偏差时积分饱和 */
    } else {
        if (p->enable_anti_windup) {
            /* 抗积分饱和：预判输出是否饱和 */
            float pred_int_output = p->ki * (s->integral + error * dt);
            float pred_output = s->prop_output + pred_int_output + s->prev_diff_output;

            if (!((pred_output > p->output_max && error > 0.0f) ||
                  (pred_output < p->output_min && error < 0.0f))) {
                s->integral += error * dt;
            }
        } else {
            s->integral += error * dt;
        }
    }

    /* 积分限幅 */
    if (s->integral > p->integral_limit)
        s->integral = p->integral_limit;
    else if (s->integral < -p->integral_limit)
        s->integral = -p->integral_limit;

    s->int_output = p->ki * s->integral;

    /* ========== 微分先行 + 不完全微分 ========== */
    if (!s->first_run) {
        /* 计算测量值的变化率 */
        measurement_deriv = (measurement - s->prev_measurement) / dt;

        /*
         * 不完全微分递推公式：
         * D(k) = α * D(k-1) - Kd * (1-α) * d[PV]/dt
         *
         * 当 α=0 时退化为完全微分先行
         * 当 α=1 时D项恒为D(k-1)，失去微分作用
         * 推荐 α=0.5~0.7
         */
        alpha_d = p->deriv_filter_alpha;
        if (alpha_d < 0.0f) alpha_d = 0.0f;
        if (alpha_d > 0.99f) alpha_d = 0.99f;

        s->diff_output = alpha_d * s->prev_diff_output
                       - p->kd * (1.0f - alpha_d) * measurement_deriv;
    } else {
        /* 首次运行，初始化微分输出 */
        s->diff_output = 0.0f;
        s->first_run = 0;
    }

    /* 保存微分输出用于下次不完全微分递推 */
    s->prev_diff_output = s->diff_output;

    /* 更新历史测量值（微分先行核心：保存PV而非误差） */
    s->prev_measurement = measurement;
    s->prev_error = error;

    /* ========== 总输出: u(k) = P + I + D ========== */
    output = s->prop_output + s->int_output + s->diff_output;

    /* 输出限幅 */
    if (output > p->output_max) output = p->output_max;
    else if (output < p->output_min) output = p->output_min;

    s->prev_output = output;
    return output;
}

/* ========================================================================== */
/*                     算法函数指针查找表                                       */
/* ========================================================================== */

/**
  * @brief  根据算法类型枚举获取对应的函数指针
  */
static PID_AlgorithmFunc_t GetAlgorithmFunc(PID_Algorithm_t alg)
{
    switch (alg) {
        case PID_ALG_POSITIONAL:   return PID_Algorithm_Positional;
        case PID_ALG_INCREMENTAL:  return PID_Algorithm_Incremental;
        case PID_ALG_DERIV_ON_MEAS:
        default:                   return PID_Algorithm_DerivOnMeas;
    }
}

/* ========================================================================== */
/*                     基础DerivPID API 实现                                   */
/* ========================================================================== */

/**
  * @brief  初始化基础PID控制器
  */
void DerivPID_Init(DerivPID_t *pid, const PID_Params_t *params,
                   PID_AlgorithmFunc_t algorithm)
{
    if (pid == NULL || params == NULL) return;

    /* 清零状态 */
    memset(&pid->state, 0, sizeof(PID_State_t));
    pid->state.first_run = 1;

    /* 复制参数 */
    pid->params = *params;

    /* 设置算法函数指针 */
    pid->algorithm = (algorithm != NULL) ? algorithm : PID_Algorithm_DerivOnMeas;

    /* 默认滤波器 */
    pid->filter = NULL;
    pid->filter_alpha = 0.0f;
}

/**
  * @brief  重置PID控制器状态（保留参数和算法选择）
  */
void DerivPID_Reset(DerivPID_t *pid)
{
    if (pid == NULL) return;

    memset(&pid->state, 0, sizeof(PID_State_t));
    pid->state.first_run = 1;
}

/**
  * @brief  执行一次PID计算
  */
float DerivPID_Calculate(DerivPID_t *pid, float setpoint,
                          float measurement, float dt)
{
    float output;

    if (pid == NULL || pid->algorithm == NULL) return 0.0f;

    /* 调用策略函数指针 */
    output = pid->algorithm(pid, setpoint, measurement, dt);

    /* 可选：对输出做滤波 */
    if (pid->filter != NULL) {
        output = pid->filter(output, pid->state.prev_output, pid->filter_alpha);
    }

    return output;
}

/**
  * @brief  运行时切换PID算法
  */
void DerivPID_SetAlgorithm(DerivPID_t *pid, PID_Algorithm_t algorithm)
{
    if (pid == NULL) return;

    pid->algorithm = GetAlgorithmFunc(algorithm);

    /* 切换算法后重置状态，避免历史数据干扰 */
    DerivPID_Reset(pid);
}

/**
  * @brief  更新PID参数
  */
void DerivPID_UpdateParams(DerivPID_t *pid, const PID_Params_t *params)
{
    if (pid == NULL || params == NULL) return;
    pid->params = *params;
}

/* ========================================================================== */
/*                     升降温速率PID控制器 实现                                */
/* ========================================================================== */

/**
  * @brief  初始化升降温速率PID控制器
  */
void RatePID_Init(RatePID_Controller_t *ctrl, const RatePID_Config_t *config)
{
    if (ctrl == NULL || config == NULL) return;

    /* 清零整个控制器 */
    memset(ctrl, 0, sizeof(RatePID_Controller_t));

    /* 保存配置 */
    ctrl->config = *config;

    /* 初始化双环PID */
    DerivPID_Init(&ctrl->temp_pid, &config->temp_pid_params,
                  GetAlgorithmFunc(config->temp_pid_alg));
    DerivPID_Init(&ctrl->rate_pid, &config->rate_pid_params,
                  GetAlgorithmFunc(config->rate_pid_alg));

    /* 设置速率滤波器 */
    if (config->rate_filter_cb != NULL) {
        ctrl->rate_pid.filter = config->rate_filter_cb;
        ctrl->rate_pid.filter_alpha = config->rate_filter_alpha;
    }

    /* 初始化时间戳 */
    if (ctrl->config.get_time_cb != NULL) {
        ctrl->last_timestamp = ctrl->config.get_time_cb();
    }

    ctrl->state = RATE_PID_IDLE;
}

/**
  * @brief  设置目标温度和升降温速率
  */
void RatePID_SetTarget(RatePID_Controller_t *ctrl, float target_temp,
                        float heating_rate, float cooling_rate)
{
    if (ctrl == NULL) return;

    ctrl->target_temperature = target_temp;
    ctrl->target_heating_rate = (heating_rate > 0.0f) ? heating_rate : 0.0f;
    ctrl->target_cooling_rate = (cooling_rate > 0.0f) ? cooling_rate : 0.0f;

    /* 重置积分项，避免历史数据影响新目标 */
    DerivPID_Reset(&ctrl->temp_pid);
    DerivPID_Reset(&ctrl->rate_pid);

    /* 重新初始化时间戳 */
    if (ctrl->config.get_time_cb != NULL) {
        ctrl->last_timestamp = ctrl->config.get_time_cb();
    }

    /* 根据当前温度与目标的关系设置初始状态 */
    if (ctrl->current_temperature < target_temp - ctrl->config.temp_deadband) {
        ctrl->state = RATE_PID_HEATING;
    } else if (ctrl->current_temperature > target_temp + ctrl->config.temp_deadband) {
        ctrl->state = RATE_PID_COOLING;
    } else {
        ctrl->state = RATE_PID_HOLDING;
    }
}

/**
  * @brief  仅修改目标温度（微分先行保证无微分冲击）
  */
void RatePID_SetTargetTemp(RatePID_Controller_t *ctrl, float target_temp)
{
    if (ctrl == NULL) return;

    ctrl->target_temperature = target_temp;

    /*
     * 微分先行PID的核心优势：
     * 修改目标温度时，微分项仅对测量值微分，不受SP突变影响
     * 因此不需要重置PID状态，实现无扰切换
     *
     * 但仍需重置积分项以防止旧积分累积导致超调
     */
    ctrl->temp_pid.state.integral = 0.0f;
    ctrl->rate_pid.state.integral = 0.0f;

    /* 更新运行状态 */
    if (ctrl->current_temperature < target_temp - ctrl->config.temp_deadband) {
        ctrl->state = RATE_PID_HEATING;
    } else if (ctrl->current_temperature > target_temp + ctrl->config.temp_deadband) {
        ctrl->state = RATE_PID_COOLING;
    } else {
        ctrl->state = RATE_PID_HOLDING;
    }
}

/**
  * @brief  停止控制
  */
void RatePID_Stop(RatePID_Controller_t *ctrl)
{
    if (ctrl == NULL) return;

    ctrl->state = RATE_PID_IDLE;
    ctrl->last_output = 0.0f;
    ctrl->current_output = 0.0f;

    /* 调用输出回调归零 */
    if (ctrl->config.set_output_cb != NULL) {
        ctrl->config.set_output_cb(0.0f, ctrl->config.user_data);
    }
}

/**
  * @brief  重置控制器
  */
void RatePID_Reset(RatePID_Controller_t *ctrl)
{
    if (ctrl == NULL) return;

    DerivPID_Reset(&ctrl->temp_pid);
    DerivPID_Reset(&ctrl->rate_pid);

    ctrl->prev_temperature = 0.0f;
    ctrl->actual_rate = 0.0f;
    ctrl->last_output = 0.0f;
    ctrl->current_output = 0.0f;
    ctrl->dt = 0.0f;
}

/**
  * @brief  计算实际升降温速率
  * @retval 实际速率（升温为正，降温为负）
  */
static float CalculateActualRate(RatePID_Controller_t *ctrl,
                                  float current_temp, float dt)
{
    float rate;

    if (dt <= 0.0f) return 0.0f;

    /* 基本速率计算: ΔT / Δt */
    rate = (current_temp - ctrl->prev_temperature) / dt;

    /* 速率滤波（一阶低通或用户自定义滤波器） */
    if (ctrl->config.rate_filter_cb != NULL) {
        rate = ctrl->config.rate_filter_cb(rate, ctrl->actual_rate,
                                            ctrl->config.rate_filter_alpha);
    } else {
        /* 默认一阶低通 */
        rate = Filter_LowPass1st(rate, ctrl->actual_rate,
                                  ctrl->config.rate_filter_alpha);
    }

    return rate;
}

/**
  * @brief  主计算函数 - 双环级联控制
  *
  * 控制策略：
  *
  * 升温阶段（HEATING）：
  *   外环温度PID → 输出基准
  *   内环速率PID → 当升温过快时降低输出
  *   final = temp_out - rate_out * weight
  *
  * 降温阶段（COOLING）：
  *   外环温度PID → 输出基准（偏差为负，输出较小或为零）
  *   内环速率PID → 当降温过快时增加输出（抑制降温速率）
  *   final = temp_out + |rate_out| * weight
  *   注：降温阶段输出通常表示加热功率，增大输出可减缓降温
  *
  * 恒温阶段（HOLDING）：
  *   仅温度PID控制，维持温度
  */
float RatePID_Calculate(RatePID_Controller_t *ctrl, float current_temp)
{
    float temp_error, rate_error;
    float temp_output, rate_output, final_output;
    float current_time;

    if (ctrl == NULL) return 0.0f;

    /* 空闲状态不输出 */
    if (ctrl->state == RATE_PID_IDLE) return ctrl->last_output;

    /* 检查时间戳回调 */
    if (ctrl->config.get_time_cb == NULL) return ctrl->last_output;

    /* 如果传值<0，使用回调获取测量值 */
    if (current_temp < 0.0f && ctrl->config.get_measurement_cb != NULL) {
        current_temp = ctrl->config.get_measurement_cb(ctrl->config.user_data);
    }

    /* 计算采样间隔 */
    current_time = ctrl->config.get_time_cb();
    ctrl->dt = current_time - ctrl->last_timestamp;

    /* 时间异常处理 */
    if (ctrl->dt <= 0.0f || ctrl->dt > 10.0f) {
        ctrl->dt = ctrl->config.sample_time;
    }
    ctrl->last_timestamp = current_time;

    /* 更新当前温度 */
    ctrl->current_temperature = current_temp;

    /* 计算实际升降温速率 */
    ctrl->actual_rate = CalculateActualRate(ctrl, current_temp, ctrl->dt);
    ctrl->prev_temperature = current_temp;

    /* 温度误差 */
    temp_error = ctrl->target_temperature - current_temp;

    /* 状态切换逻辑 */
    if (fabsf(temp_error) < ctrl->config.temp_deadband) {
        ctrl->state = RATE_PID_HOLDING;
    } else if (temp_error > 0.0f) {
        ctrl->state = RATE_PID_HEATING;
    } else {
        ctrl->state = RATE_PID_COOLING;
    }

    /* ====== 根据状态执行双环控制 ====== */
    switch (ctrl->state) {

    case RATE_PID_HEATING:
        /* 外环：温度PID（微分先行） */
        temp_output = DerivPID_Calculate(&ctrl->temp_pid,
                                          ctrl->target_temperature,
                                          current_temp, ctrl->dt);

        /* 内环：升温速率控制 */
        if (ctrl->target_heating_rate > 0.0f) {
            rate_error = ctrl->target_heating_rate - ctrl->actual_rate;

            if (ctrl->actual_rate > ctrl->target_heating_rate) {
                /* 升温过快：速率PID介入降低输出 */
                rate_output = DerivPID_Calculate(&ctrl->rate_pid,
                                                  ctrl->target_heating_rate,
                                                  ctrl->actual_rate, ctrl->dt);
                final_output = temp_output - rate_output * ctrl->config.rate_control_weight;
            } else {
                /* 升温未超速：仅温度PID */
                final_output = temp_output;
                /* 仍执行速率PID计算以维持状态连续性 */
                DerivPID_Calculate(&ctrl->rate_pid,
                                   ctrl->target_heating_rate,
                                   ctrl->actual_rate, ctrl->dt);
            }
        } else {
            /* 未设定升温速率限制，仅温度PID */
            final_output = temp_output;
        }
        break;

    case RATE_PID_COOLING:
        /* 外环：温度PID（微分先行） */
        temp_output = DerivPID_Calculate(&ctrl->temp_pid,
                                          ctrl->target_temperature,
                                          current_temp, ctrl->dt);

        /* 内环：降温速率控制 */
        if (ctrl->target_cooling_rate > 0.0f) {
            /*
             * 降温速率控制逻辑：
             * - actual_rate < 0 表示正在降温
             * - 降温过快 = |actual_rate| > target_cooling_rate
             *   即 actual_rate < -target_cooling_rate
             * - 降温过快时需要增加加热功率来减缓降温
             */
            if (ctrl->actual_rate < -ctrl->target_cooling_rate) {
                /* 降温过快：速率PID输出修正量 */
                rate_error = (-ctrl->target_cooling_rate) - ctrl->actual_rate;
                rate_output = DerivPID_Calculate(&ctrl->rate_pid,
                                                  -ctrl->target_cooling_rate,
                                                  ctrl->actual_rate, ctrl->dt);
                /*
                 * 降温过快时，rate_output为负值（因为actual_rate < setpoint）
                 * 减去负值 = 增加输出 → 增加加热功率，减缓降温
                 */
                final_output = temp_output - rate_output * ctrl->config.rate_control_weight;
            } else {
                /* 降温速率正常 */
                final_output = temp_output;
                DerivPID_Calculate(&ctrl->rate_pid,
                                   -ctrl->target_cooling_rate,
                                   ctrl->actual_rate, ctrl->dt);
            }
        } else {
            final_output = temp_output;
        }
        break;

    case RATE_PID_HOLDING:
        /* 恒温阶段：仅温度PID（微分先行） */
        final_output = DerivPID_Calculate(&ctrl->temp_pid,
                                           ctrl->target_temperature,
                                           current_temp, ctrl->dt);
        break;

    default:
        final_output = 0.0f;
        break;
    }

    /* ====== 输出变化率限制 ====== */
    if (ctrl->config.output_rate_limit > 0.0f) {
        float max_change = ctrl->config.output_rate_limit * ctrl->dt;
        float output_change = final_output - ctrl->last_output;

        if (output_change > max_change) {
            final_output = ctrl->last_output + max_change;
        } else if (output_change < -max_change) {
            final_output = ctrl->last_output - max_change;
        }
    }

    /* 输出限幅 0~100% */
    if (final_output > 100.0f) final_output = 100.0f;
    else if (final_output < 0.0f) final_output = 0.0f;

    /* 保存输出 */
    ctrl->last_output = final_output;
    ctrl->current_output = final_output;

    /* 调用输出回调 */
    if (ctrl->config.set_output_cb != NULL) {
        ctrl->config.set_output_cb(final_output, ctrl->config.user_data);
    }

    return final_output;
}

/**
  * @brief  运行时切换算法策略
  */
void RatePID_SetAlgorithm(RatePID_Controller_t *ctrl, uint8_t is_temp_pid,
                           PID_Algorithm_t algorithm)
{
    if (ctrl == NULL) return;

    if (is_temp_pid) {
        DerivPID_SetAlgorithm(&ctrl->temp_pid, algorithm);
    } else {
        DerivPID_SetAlgorithm(&ctrl->rate_pid, algorithm);
    }
}

/**
  * @brief  更新PID参数
  */
void RatePID_UpdateParams(RatePID_Controller_t *ctrl, uint8_t is_temp_pid,
                           const PID_Params_t *params)
{
    if (ctrl == NULL || params == NULL) return;

    if (is_temp_pid) {
        DerivPID_UpdateParams(&ctrl->temp_pid, params);
    } else {
        DerivPID_UpdateParams(&ctrl->rate_pid, params);
    }
}

/**
  * @brief  获取控制器状态信息
  */
void RatePID_GetStatus(const RatePID_Controller_t *ctrl, RatePID_Status_t *status)
{
    if (ctrl == NULL || status == NULL) return;

    status->state             = ctrl->state;
    status->current_temp      = ctrl->current_temperature;
    status->target_temp       = ctrl->target_temperature;
    status->temp_error        = ctrl->target_temperature - ctrl->current_temperature;
    status->actual_rate       = ctrl->actual_rate;
    status->target_rate       = (ctrl->state == RATE_PID_COOLING)
                                 ? -ctrl->target_cooling_rate
                                 : ctrl->target_heating_rate;
    status->rate_error        = status->target_rate - ctrl->actual_rate;
    status->output            = ctrl->current_output;
    status->temp_pid_integral = ctrl->temp_pid.state.integral;
    status->rate_pid_integral = ctrl->rate_pid.state.integral;
    status->temp_pid_output   = ctrl->temp_pid.state.prop_output
                              + ctrl->temp_pid.state.int_output
                              + ctrl->temp_pid.state.diff_output;
    status->rate_pid_output   = ctrl->rate_pid.state.prop_output
                              + ctrl->rate_pid.state.int_output
                              + ctrl->rate_pid.state.diff_output;
}

/**
  * @brief  调试打印
  */
void RatePID_PrintDebug(RatePID_Controller_t *ctrl)
{
    if (ctrl == NULL || ctrl->config.debug_print_cb == NULL) return;
    ctrl->config.debug_print_cb(ctrl, ctrl->config.user_data);
}
