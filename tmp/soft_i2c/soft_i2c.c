
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
static void Soft_I2CStart(Soft_I2CObjectType *soft_I2CInstance);

/* 模拟I2C通讯起始操作 */
static void Soft_I2CStop(Soft_I2CObjectType *soft_I2CInstance);

/* 模拟I2C等待确认 */
static Soft_I2CStatus Soft_I2CWaitAck(Soft_I2CObjectType *soft_I2CInstance,uint16_t timeOut);

/* 模拟I2C产生一个应答*/
static void IIC_Ack(Soft_I2CObjectType *soft_I2CInstance);

/* 模拟I2C产生一个非应答 */
static void IIC_NAck(Soft_I2CObjectType *soft_I2CInstance);

/* 通过模拟I2C发送一个字节 */
static void SendByteBySoft_I2C(Soft_I2CObjectType *soft_I2CInstance,uint8_t byte);

/* 通过模拟I2C接收一个字节 */
static uint8_t RecieveByteBySoft_I2C(Soft_I2CObjectType *soft_I2CInstance);

/* GPIO模拟I2C通讯初始化 */
void Soft_I2C_Init
(
  Soft_I2CObjectType *soft_I2CInstance,
  uint32_t speed,
  Soft_I2CSetPin setSCL,
  Soft_I2CSetPin setSDA,
  Soft_I2CReadSDAPin readSDA,
  Soft_I2CDelayus delayus
)
{
  if((soft_I2CInstance==NULL)||(setSCL==NULL)||(setSDA==NULL)||(readSDA==NULL)||(delayus==NULL))
  {
    return;
  }

  soft_I2CInstance->SetSCLPin=setSCL;
  soft_I2CInstance->SetSDAPin=setSDA;
  soft_I2CInstance->ReadSDAPin=readSDA;
  soft_I2CInstance->Delayus=delayus;

  /*初始化速度,默认100K*/
  if((speed>0)&&(speed<=400))
  {
    soft_I2CInstance->period=500/speed;
  }
  else
  {
    soft_I2CInstance->period=5;
  }

  /*拉高总线，使处于空闲状态*/
  soft_I2CInstance->SetSDAPin(Set);
  soft_I2CInstance->SetSCLPin(Set);
}

/* 模拟I2C通讯起始操作 */
static void Soft_I2CStart(Soft_I2CObjectType *soft_I2CInstance)
{
  /*拉高总线，使处于空闲状态*/
  soft_I2CInstance->SetSDAPin(Set);
  soft_I2CInstance->SetSCLPin(Set);
  soft_I2CInstance->Delayus(soft_I2CInstance->period);

  /*当SCL为高时，SDA一个下降代表开始*/
  soft_I2CInstance->SetSDAPin(Reset);
  soft_I2CInstance->Delayus(soft_I2CInstance->period);

  /*嵌住SCL以便发送数据*/
  soft_I2CInstance->SetSCLPin(Reset);
  soft_I2CInstance->Delayus(soft_I2CInstance->period);
}

/* 模拟I2C通讯起始操作 */
static void Soft_I2CStop(Soft_I2CObjectType *soft_I2CInstance)
{
  soft_I2CInstance->SetSCLPin(Reset);
  soft_I2CInstance->Delayus(soft_I2CInstance->period);

  soft_I2CInstance->SetSDAPin(Reset);
  soft_I2CInstance->Delayus(soft_I2CInstance->period);

  soft_I2CInstance->SetSCLPin(Set);
  soft_I2CInstance->Delayus(soft_I2CInstance->period);

  soft_I2CInstance->SetSDAPin(Set);
  soft_I2CInstance->Delayus(soft_I2CInstance->period);
}

/* 模拟I2C等待确认 */
static Soft_I2CStatus Soft_I2CWaitAck(Soft_I2CObjectType *soft_I2CInstance,uint16_t timeOut)
{
  soft_I2CInstance->SetSDAPin(Set);//拉高SDA
  soft_I2CInstance->Delayus(soft_I2CInstance->period);
  soft_I2CInstance->SetSCLPin(Set);//拉高SCL
  soft_I2CInstance->Delayus(soft_I2CInstance->period);

  while(soft_I2CInstance->ReadSDAPin())           //读取数据引脚
  {
    if(--timeOut)
    {
      Soft_I2CStop(soft_I2CInstance);
      return I2C_ERROR;
    }
    soft_I2CInstance->Delayus(soft_I2CInstance->period);
  }
  soft_I2CInstance->SetSCLPin(Reset);
  soft_I2CInstance->Delayus(soft_I2CInstance->period);
  return I2C_OK;
}

/* 模拟I2C产生一个应答*/
static void IIC_Ack(Soft_I2CObjectType *soft_I2CInstance)
{
  soft_I2CInstance->SetSDAPin(Reset);
  soft_I2CInstance->Delayus(soft_I2CInstance->period);

  soft_I2CInstance->SetSCLPin(Set);
  soft_I2CInstance->Delayus(soft_I2CInstance->period);

  soft_I2CInstance->SetSCLPin(Reset);
  soft_I2CInstance->Delayus(soft_I2CInstance->period);

  soft_I2CInstance->SetSDAPin(Set);
}

/* 模拟I2C产生一个非应答 */
static void IIC_NAck(Soft_I2CObjectType *soft_I2CInstance)
{
  soft_I2CInstance->SetSCLPin(Reset);
  soft_I2CInstance->SetSDAPin(Set);
  soft_I2CInstance->Delayus(soft_I2CInstance->period);

  soft_I2CInstance->SetSCLPin(Set);
  soft_I2CInstance->Delayus(soft_I2CInstance->period);
  soft_I2CInstance->SetSCLPin(Reset);
  soft_I2CInstance->Delayus(soft_I2CInstance->period);
}

/* 通过模拟I2C发送一个字节 */
static void SendByteBySoft_I2C(Soft_I2CObjectType *soft_I2CInstance,uint8_t byte)
{
  /*拉低SCL引脚准备数据传输*/
  soft_I2CInstance->SetSCLPin(Reset);

  for(uint8_t count = 0; count < 8; count++)
  {
    if(byte & 0x80)     //每次发送最高位
    {
      soft_I2CInstance->SetSDAPin(Set);
    }
    else
    {
      soft_I2CInstance->SetSDAPin(Reset);
    }
    byte <<= 1;         //发送一位后，左移一位

    soft_I2CInstance->Delayus(soft_I2CInstance->period);
    soft_I2CInstance->SetSCLPin(Set);
    soft_I2CInstance->Delayus(soft_I2CInstance->period);
    soft_I2CInstance->SetSCLPin(Reset);
  }

//  soft_I2CInstance->Delayus(soft_I2CInstance->period);
//  soft_I2CInstance->SetSDAPin(Set);
}

/* 通过模拟I2C接收一个字节 */
static uint8_t RecieveByteBySoft_I2C(Soft_I2CObjectType *soft_I2CInstance)
{
  uint8_t receive = 0;

  soft_I2CInstance->SetSDAPin(Set);//引脚配置为开漏输出时，需要置高才能读取数据

  for(uint8_t count = 0; count < 8; count++ )
  {
//    soft_I2CInstance->SetSCLPin(Reset);
//    soft_I2CInstance->Delayus(soft_I2CInstance->period);
    soft_I2CInstance->SetSCLPin(Set);
    soft_I2CInstance->Delayus(soft_I2CInstance->period);

    receive <<= 1;

    if(soft_I2CInstance->ReadSDAPin())
    {
      receive++;
    }
    soft_I2CInstance->SetSCLPin(Reset);
    soft_I2CInstance->Delayus(soft_I2CInstance->period);
  }
  return receive;
}

/* 通过模拟I2C向从站写数据 */
Soft_I2CStatus WriteDataBySoft_I2C(Soft_I2CObjectType *soft_I2CInstance,uint8_t deviceAddress,uint8_t *wData,uint16_t wSize)
{
  //启动通讯
  Soft_I2CStart(soft_I2CInstance);
  //发送从站地址（写）
  SendByteBySoft_I2C(soft_I2CInstance,deviceAddress);
  if(Soft_I2CWaitAck(soft_I2CInstance,5000))
  {
    return I2C_ERROR;
  }
  // IIC_Ack(&soft_i2c);
  while(wSize--)
  {
    SendByteBySoft_I2C(soft_I2CInstance,*wData);
    if(Soft_I2CWaitAck(soft_I2CInstance,5000))
    {
      return I2C_ERROR;
    }
    // IIC_Ack(&soft_i2c);
    wData++;
    // soft_I2CInstance->Delayus(10);
  }

  Soft_I2CStop(soft_I2CInstance);
  return I2C_OK;
}

/* 通过模拟I2C自从站读数据 */
Soft_I2CStatus ReadDataBySoft_I2C(Soft_I2CObjectType *soft_I2CInstance,uint8_t deviceAddress,uint8_t *rData, uint16_t rSize)
{
  //启动通讯
  Soft_I2CStart(soft_I2CInstance);

  //发送从站地址（读）
  SendByteBySoft_I2C(soft_I2CInstance,deviceAddress+1);
  if(Soft_I2CWaitAck(soft_I2CInstance,5000))
  {
    return I2C_ERROR;
  }

  soft_I2CInstance->Delayus(1000);

  while(rSize--)
  {
    *rData=RecieveByteBySoft_I2C(soft_I2CInstance);
    rData++;
    if(rData==0)
    {
      IIC_NAck(soft_I2CInstance);
    }
    else
    {
      IIC_Ack(soft_I2CInstance);
      soft_I2CInstance->Delayus(1000);
    }
  }

  //结束通讯
  Soft_I2CStop(soft_I2CInstance);
  return I2C_OK;
}

/* 通过模拟I2C实现对从站先写数据紧接读数据组合操作 */
Soft_I2CStatus WriteReadDataBySoft_I2C(Soft_I2CObjectType *soft_I2CInstance,uint8_t deviceAddress, uint8_t *wData,uint16_t wSize,uint8_t *rData, uint16_t rSize)
{
	//启动通讯
  Soft_I2CStart(soft_I2CInstance);
  //发送从站地址（写）
  SendByteBySoft_I2C(soft_I2CInstance,deviceAddress);
  if(Soft_I2CWaitAck(soft_I2CInstance,5000))
  {
    return I2C_ERROR;
  }

  while(wSize--)
  {
    SendByteBySoft_I2C(soft_I2CInstance,*wData);
    if(Soft_I2CWaitAck(soft_I2CInstance,5000))
    {
      return I2C_ERROR;
    }
    wData++;
    soft_I2CInstance->Delayus(10);
  }

	//再启动
  Soft_I2CStart(soft_I2CInstance);
	//发送从站地址（读）
  SendByteBySoft_I2C(soft_I2CInstance,deviceAddress+1);
  if(Soft_I2CWaitAck(soft_I2CInstance,5000))
  {
    return I2C_ERROR;
  }

  while(rSize--)
  {
    *rData=RecieveByteBySoft_I2C(soft_I2CInstance);
    rData++;
    if(rSize==0)
    {
      IIC_NAck(soft_I2CInstance);
    }
    else
    {
      IIC_Ack(soft_I2CInstance);
    }
  }
	//结束通讯
  Soft_I2CStop(soft_I2CInstance);
  return I2C_OK;
}
