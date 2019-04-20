
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技MK66FX机器人开发平台主控板
【修    改】CHIUSIR
【备    注】本模块参考@author 山外科技
【软件版本】V1.0
【最后更新】2016年08月20日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【交流邮箱】chiusir@163.com
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#ifndef __MK66_SPI_H__
#define __MK66_SPI_H__

extern uint32 core_clk_M;//单位MHZ
extern uint32 bus_clk_M;//单位MHZ

/**********************************  SPI   ***************************************/
//PCS接口，不用的时候需要注释，就不会进行初始化对应的管脚
//      模块通道    端口          可选范围                  建议

#define SPI0_SCK    PTD1       // PTA15、PTC5、PTD1        全部都是 ALT2
#define SPI0_SOUT   PTD2       // PTA16、PTC6、PTD2        全部都是 ALT2
#define SPI0_SIN    PTA17       // PTA17、PTC7、PTD3        全部都是 ALT2

#define SPI0_PCS0   PTD0      // PTA14、PTC4、PTD0、      全部都是 ALT2
//一般情况下用不到那么多的从机，以下片选管脚注释掉，如果有需要可以开启
#define SPI0_PCS1   PTC3        // PTC3、PTD4               全部都是 ALT2
#define SPI0_PCS2   PTC2        // PTC2、PTD5               全部都是 ALT2
#define SPI0_PCS3   PTC1        // PTC1、PTD6               全部都是 ALT2
#define SPI0_PCS4   PTC0        // PTC0、                   全部都是 ALT2
#define SPI0_PCS5   PTB23       // PTB23                    ALT3


#define SPI1_SCK    PTE2       // PTE2、PTB11、            全部都是 ALT2
#define SPI1_SOUT   PTE1       // PTE1、PTB16、            全部都是 ALT2
#define SPI1_SIN    PTB17       // PTE3、PTB17、            全部都是 ALT2

#define SPI1_PCS0   PTE4   

// PTE4、PTB10、            全部都是 ALT2
#define SPI1_PCS1   PTE0        // PTE0、PTB9、             全部都是 ALT2
#define SPI1_PCS2   PTE5        // PTE5、                   全部都是 ALT2
#define SPI1_PCS3   PTE6        // PTE6、                   全部都是 ALT2


#define SPI2_SCK    PTB21       // PTB21、PTD12             全部都是 ALT2
#define SPI2_SOUT   PTB22       // PTB22、PTD13             全部都是 ALT2
#define SPI2_SIN    PTB23       // PTB23、PTD14             全部都是 ALT2
#define SPI2_PCS0   PTB20       // PTB20、PTD11             全部都是 ALT2
#define SPI2_PCS1   PTD15       // PTD15                    全部都是 ALT2

//定义主从机模式
typedef enum
{
    MASTER,    //主机模式
    SLAVE      //主机模式
} SPI_CFG;

//定义SPI模块号
typedef enum
{
    SPI_0,
    SPI_1,
    SPI_2
} SPIn_e;

//定义SPI模块片选号
typedef enum
{
    SPIn_PCS0 = 1 << 0,
    SPIn_PCS1 = 1 << 1,
    SPIn_PCS2 = 1 << 2,
    SPIn_PCS3 = 1 << 3,
    SPIn_PCS4 = 1 << 4,
    SPIn_PCS5 = 1 << 5,
} SPIn_PCSn_e;


uint32 spi_init       (SPIn_e, SPIn_PCSn_e , SPI_CFG,uint32 baud);                                        //SPI初始化，设置模式

//主机接收发送函数
void spi_mosi       (SPIn_e spin, SPIn_PCSn_e pcs,                              uint8 *modata, uint8 *midata,               uint32 len);    //SPI发送接收函数,发送databuff数据，并把接收到的数据存放在databuff里(注意，会覆盖原来的databuff)
void spi_mosi_cmd   (SPIn_e spin, SPIn_PCSn_e pcs, uint8 *mocmd , uint8 *micmd , uint8 *modata, uint8 *midata, uint32 cmdlen , uint32 len); //SPI发送接收函数,与spi_mosi相比，多了先发送cmd 缓冲区的步骤，即分开两部分发送



#endif  // __MK60_SPI_H__