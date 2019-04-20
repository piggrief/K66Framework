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

void SysTick_delay(u32 tm)
{
   SysTick->LOAD |=tm;
   SysTick->VAL  |=0;
   SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk
                   |SysTick_CTRL_CLKSOURCE_Msk;
   while( !(SysTick->CTRL&SysTick_CTRL_COUNTFLAG_Msk) );
}
void SysTick_delay_ms(uint32 ms)
{
    while(ms--)
    {
        SysTick_delay(core_clk_M*1000);
    }
}
void SysTick_delay_us(u32 tm)//精准延时1us
{
  tm=(u32)(tm*core_clk_M);
  SysTick_delay(tm);
}
