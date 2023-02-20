
/**--------------------------------------------------------------------------**/
/**使用说明：                                                                **/
/**    1、声明GPIO模拟I2C对象实体（如：Soft_I2CTypeDef si2c），并调用函数     **/
/**       Soft_I2C_Init初始化对象实体。初始化时，给定工作频率1-500K  **/
/**       以kHz为单位，最小始终周期为2微秒，最大为500k微秒。                 **/
/**    2、实现形如void f(Soft_I2CPinValue op)的函数，并将其赋值               **/
/**       给Soft_I2CTypeDef对象实体的SetSCLPin函数指针。用以操作SCL引脚。     **/
/**    3、实现形如void f(Soft_I2CPinValue op)的函数，并将其赋值               **/
/**       给Soft_I2CTypeDef对象实体的SetSDAPin函数指针。用以操作SDA引脚。     **/
/**    4、实现形如uint8_t f(void)的函数，并将其赋值给Soft_I2CTypeDef对象实体  **/
/**       的ReadSDAPin函数指针。用以读取SDA引脚。                            **/
/**    5、实现形如void f(volatile uint32_t period)的函数，并将其赋值         **/
/**       给Soft_I2CTypeDef对象实体的Delayus函数指针。用于周期延时，单位为微秒**/
/**    6、调用相应的函数实现需要的操作。                                     **/
/**--------------------------------------------------------------------------**/


#include "stddef.h"
#include "soft_i2c.h"

/* 模拟I2C通讯起始操作 */
static void Soft_I2CStart();

/* 模拟I2C通讯起始操作 */
static void Soft_I2CStop();

/* 模拟I2C等待确认 */
static Soft_I2CStatus Soft_I2CWaitAck(uint16_t timeOut);

/* 模拟I2C产生一个应答*/
static void IIC_Ack();

/* 模拟I2C产生一个非应答 */
static void IIC_NAck();

/* 通过模拟I2C发送一个字节 */
static void SendByteBySoft_I2C(uint8_t byte);

/* 通过模拟I2C接收一个字节 */
static uint8_t RecieveByteBySoft_I2C();

/* GPIO模拟I2C通讯初始化 */
void Soft_I2C_Init()
{
  /*拉高总线，使处于空闲状态*/
  SetSCLPin(IIC_SET);
  SetSDAPin(IIC_SET);
}

/* 模拟I2C通讯起始操作 */
static void Soft_I2CStart()
{
  /*拉高总线，使处于空闲状态*/
  SetSDAPin(IIC_SET);
  SetSCLPin(IIC_SET);
  /*当SCL为高时，SDA一个下降代表开始*/
  SetSDAPin(IIC_RESET);
  /*嵌住SCL以便发送数据*/
  SetSCLPin(IIC_RESET);

}

/* 模拟I2C通讯停止操作 */
static void Soft_I2CStop()
{
  SetSCLPin(IIC_RESET);
  SetSDAPin(IIC_RESET);
  SetSCLPin(IIC_SET);
  SetSDAPin(IIC_SET);

}

/* 模拟I2C等待确认 */
static Soft_I2CStatus Soft_I2CWaitAck(uint16_t timeOut)
{
  SetSDAPin(IIC_SET);//拉高SDA
  SetSCLPin(IIC_SET);//拉高SCL
  while(ReadSDAPin())           //读取数据引脚
  {
    if(--timeOut)
    {
      Soft_I2CStop();
      return I2C_ERROR;
    }
  }
  SetSCLPin(IIC_RESET);
  return I2C_OK;
}

/* 模拟I2C产生一个应答*/
static void IIC_Ack()
{
  SetSDAPin(IIC_RESET);
  SetSCLPin(IIC_SET);
  SetSCLPin(IIC_RESET);
  SetSDAPin(IIC_SET);
}

/* 模拟I2C产生一个非应答 */
static void IIC_NAck()
{
  SetSCLPin(IIC_RESET);
  SetSDAPin(IIC_SET);
  SetSCLPin(IIC_SET);
  SetSCLPin(IIC_RESET);

}

/* 通过模拟I2C发送一个字节 */
static void SendByteBySoft_I2C(uint8_t byte)
{
  /*拉低SCL引脚准备数据传输*/
  SetSCLPin(IIC_RESET);
  for(uint8_t count = 0; count < 8; count++)
  {
    if(byte & 0x80)     //每次发送最高位
    {
      SetSDAPin(IIC_SET);
    }
    else
    {
      SetSDAPin(IIC_RESET);
    }
    byte <<= 1;         //发送一位后，左移一位
    SetSCLPin(IIC_SET);
    SetSCLPin(IIC_RESET);
  }
//
//  SetSDAPin(IIC_SET);
}

/* 通过模拟I2C接收一个字节 */
static uint8_t RecieveByteBySoft_I2C()
{
  uint8_t receive = 0;
  SetSDAPin(IIC_SET);//引脚配置为开漏输出时，需要置高才能读取数据
  for(uint8_t count = 0; count < 8; count++ )
  {
//    SetSCLPin(IIC_RESET);
//
    SetSCLPin(IIC_SET);
    receive <<= 1;
    if(ReadSDAPin())
    {
      receive++;
    }
    SetSCLPin(IIC_RESET);
  }
  return receive;
}
/* 通过模拟I2C向从站写数据 */
Soft_I2CStatus WriteDataBySoft_I2C(uint8_t deviceAddress,uint8_t *wData,uint16_t wSize)
{
  //启动通讯
  Soft_I2CStart();
  //发送从站地址（写）
  SendByteBySoft_I2C(deviceAddress);
  if(Soft_I2CWaitAck(5000))
  {
    return I2C_ERROR;
  }
  // IIC_Ack(&soft_i2c);
  while(wSize--)
  {
    SendByteBySoft_I2C(*wData);
    if(Soft_I2CWaitAck(5000))
    {
      return I2C_ERROR;
    }
    // IIC_Ack(&soft_i2c);
    wData++;
    // ->Delayus(10);
  }
  Soft_I2CStop();
  return I2C_OK;
}

/* 通过模拟I2C自从站读数据 */
Soft_I2CStatus ReadDataBySoft_I2C(uint8_t deviceAddress,uint8_t *rData, uint16_t rSize)
{
  //启动通讯
  Soft_I2CStart();
  //发送从站地址（读）
  SendByteBySoft_I2C(deviceAddress+1);
  if(Soft_I2CWaitAck(5000))
  {
    return I2C_ERROR;
  }
  while(rSize--)
  {
    *rData=RecieveByteBySoft_I2C();
    rData++;
    if(rData==0)
    {
      IIC_NAck();
    }
    else
    {
      IIC_Ack();
    }
  }
  //结束通讯
  Soft_I2CStop();
  return I2C_OK;
}

/* 通过模拟I2C实现对从站先写数据紧接读数据组合操作 */
Soft_I2CStatus WriteReadDataBySoft_I2C(uint8_t deviceAddress, uint8_t *wData,uint16_t wSize,uint8_t *rData, uint16_t rSize)
{
	//启动通讯
  Soft_I2CStart();
  //发送从站地址（写）
  SendByteBySoft_I2C(deviceAddress);
  if(Soft_I2CWaitAck(5000))
  {
    return I2C_ERROR;
  }
  while(wSize--)
  {
    SendByteBySoft_I2C(*wData);
    if(Soft_I2CWaitAck(5000))
    {
      return I2C_ERROR;
    }
    wData++;
  }

	//再启动
  Soft_I2CStart();
	//发送从站地址（读）
  SendByteBySoft_I2C(deviceAddress+1);
  if(Soft_I2CWaitAck(5000))
  {
    return I2C_ERROR;
  }
  while(rSize--)
  {
    *rData=RecieveByteBySoft_I2C();
    rData++;
    if(rSize==0)
    {
      IIC_NAck();
    }
    else
    {
      IIC_Ack();
    }
  }
	//结束通讯
  Soft_I2CStop();
  return I2C_OK;
}
