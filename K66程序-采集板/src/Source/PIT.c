/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技MK66FX1M0VLQ18核心板
【编    写】CHIUSIR
【备    注】
【软件版本】V1.0
【最后更新】2016年08月20日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【交流邮箱】chiusir@163.com
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "include.h"
#include "PIT.h"

//-------------------------------------------------------------------------*
//函数名: pit_init
//功  能: 初始化PIT
//参  数: pitn:模块名PIT0或PIT1或PIT2或PIT3
//        cnt 中断时间，单位1ms
//返  回: 无
//简  例: pit_init(PIT0,1000); PIT0中断，1000ms，即1s进入PIT0_interrupt()一次
//-------------------------------------------------------------------------*
void PIT_Init(PITn pitn, u32 cnt)
{
    //PIT 用的是 Bus Clock 总线频率

    /* 开启时钟*/
    SIM_SCGC6       |= SIM_SCGC6_PIT_MASK;                            //使能PIT时钟

    /* PIT模块控制 PIT Module Control Register (PIT_MCR) */
    PIT_MCR         &= ~(PIT_MCR_MDIS_MASK | PIT_MCR_FRZ_MASK );      //使能PIT定时器时钟 ，调试模式下继续运行

    /* 定时器加载值设置 Timer Load Value Register (PIT_LDVALn) */
    PIT_LDVAL(pitn)  = cnt*40*1000;                                          //设置溢出中断时间

    //定时时间到了后，TIF 置 1 。写1的时候就会清0
    PIT_Flag_Clear(pitn);                                             //清中断标志位

    /* 定时器控制寄存器 Timer Control Register (PIT_TCTRL0) */
    PIT_TCTRL(pitn) |= ( PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK );   //使能 PITn定时器,并开PITn中断

    NVIC_EnableIRQ((IRQn_Type)(pitn + 48));			                                //开接收引脚的IRQ中断
}

/*!
 *  @brief      PITn计时开始
 *  @param      PITn        模块号（PIT0~PIT3）
 *  @since      v5.0
 *  Sample usage:
                    pit_time_start(PIT0);                          //PIT0计时开始
 */
void pit_time_start(PITn pitn)
{
    //PIT 用的是 Bus Clock 总线频率
    //溢出计数 = 总线频率 * 时间

    SIM_SCGC6       |= SIM_SCGC6_PIT_MASK;                          //使能PIT时钟

    PIT_MCR         &= ~(PIT_MCR_MDIS_MASK | PIT_MCR_FRZ_MASK );    //使能PIT定时器时钟 ，调试模式下继续运行

    PIT_TCTRL(pitn) &= ~( PIT_TCTRL_TEN_MASK );                     //禁用PIT ，以便设置加载值生效

    PIT_LDVAL(pitn)  = ~0;                                          //设置溢出中断时间

    PIT_Flag_Clear(pitn);                                           //清中断标志位

    PIT_TCTRL(pitn) &= ~ PIT_TCTRL_TEN_MASK;                        //禁止PITn定时器（用于清空计数值）
    PIT_TCTRL(pitn)  = ( 0
                         | PIT_TCTRL_TEN_MASK                        //使能 PITn定时器
                         //| PIT_TCTRL_TIE_MASK                      //开PITn中断
                       );
}

/*!
 *  @brief      获取 PITn计时时间(超时时会关闭 定时器)
 *  @param      PITn        模块号（PIT0~PIT3）
 *  @since      v5.0
 *  Sample usage:
                        uint32 time = pit_time_get(PIT0);                         //获取 PITn计时时间
                        if(time != ~0)       //没超时
                        {
                            printf("\n计时时间为：%d us",time*1000/bus_clk_khz);
                        }
 */
uint32 pit_time_get(PITn pitn)
{
    uint32 val;

    val = (~0) - PIT_CVAL(pitn);

    if(PIT_TFLG(pitn)& PIT_TFLG_TIF_MASK)                           //判断是否时间超时
    {
        PIT_Flag_Clear(pitn);                                       //清中断标志位
        PIT_TCTRL(pitn) &= ~ PIT_TCTRL_TEN_MASK;                    //禁止PITn定时器（用于清空计数值）
        return ~0;
    }

    if(val == (~0))
    {
        val--;              //确保 不等于 ~0
    }
    return val;
}

/*!
 *  @brief      关闭 pit
 *  @param      PITn        模块号（PIT0~PIT3）
 *  @since      v5.0
 *  Sample usage:
                        pit_close(PIT0);                         //关闭PIT
 */
void pit_close(PITn pitn)
{
    PIT_Flag_Clear(pitn);                                       //清中断标志位
    PIT_TCTRL(pitn) &= ~ PIT_TCTRL_TEN_MASK;                    //禁止PITn定时器（用于清空计数值）
}

//-------------------------------------------------------------------------*
//函数名: PIT0_interrupt
//功  能: PIT中断函数
//参  数: 无
//返  回: 无
//简  例: 由初始化决定，多长时间进入一次
//-------------------------------------------------------------------------*
short speed=0;
//void PIT0_Interrupt()
//{
//  PIT_Flag_Clear(PIT0);       //清中断标志位
// /*用户添加所需代码*/
//}
//
//void PIT1_Interrupt()
//{
//  PIT_Flag_Clear(PIT1);       //清中断标志位
// /*用户添加所需代码*/
//}
//
//void PIT2_Interrupt()
//{
//  PIT_Flag_Clear(PIT2);       //清中断标志位
//  /*用户添加所需代码*/
//}
//
//void PIT3_Interrupt()
//{
//  PIT_Flag_Clear(PIT3);       //清中断标志位
//  /*用户添加所需代码*/
//}