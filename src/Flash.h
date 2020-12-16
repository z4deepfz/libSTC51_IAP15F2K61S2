#ifndef FLASH_H
#define FLASH_H

#define SDA P40
#define SCL P55

#define ENABLE_IICACK_DELAY
/*
//#define delay();        for(unsigned i=0; i<64; ++i){}
#define FlashI2Cinit()  SCL=1; delay(); SDA=1; delay()
#define flash_start()   SDA=1;delay();SCL=1;delay();SDA=0;delay()
#define flash_stop()    SDA=0;delay();SCL=1;delay();SDA=1;delay();
*/

static void delay()
{
    for(unsigned i=0; i<64; ++i){}
}

void FlashI2Cinit()     //I2C总线初始化
{
    SCL = 1;
    delay();
    SDA = 1;
    delay();
}

static void flash_start()        //主机启动信号
{
    SDA = 1;
    delay();
    SCL = 1;
    delay();
    SDA = 0;
    delay();
}

static void flash_stop()         //停止信号
{
    SDA = 0;
    delay();
    SCL = 1;
    delay();
    SDA = 1;
    delay();
}

static void flashWaitACK()      //从机应答信号
{
    SCL = 1;
    delay();
#ifdef ENABLE_IICACK_DELAY
    for(unsigned i=0; i<255&&SDA==1; ++i){}
#else
    while(SDA==1){}
#endif // ENABLE_IICACK_DELAY
    SCL = 0;
    delay();
}

static void flashWrite( unsigned data ) // write to current address
{
    for(unsigned i = 0; i < 8; i++ ){
        //data = data << 1; // 利用溢出位从高到低提取比特位
        SCL = 0;    // set SCL=0 to change SDA
        delay();
        SDA = 1 & (data>>(7-i));
        delay();    // wait voltage stable
        SCL = 1;    // pull SCL high to notice slave save this bit
        delay();    // wait slave saving
    }
    SCL = 0;
    delay();
    SDA = 1;
    delay();
}

static unsigned flashRead()  // read from current address
{
    SCL = 0;
    delay();
    SDA = 1;
    delay();
    unsigned data = 0;
    for(unsigned i = 0; i < 8; i++){
        SCL = 1;
        delay();
        data = ( data << 1 ) | SDA;
        delay();
        SCL = 0;
        delay();
    }
    delay();
    return data;
}

void FlashWrite(const unsigned addr, const unsigned data) //对24C02的地址addr，写入一个数据date
{
    DisableMasterInterrupt();
    flash_start();

    flashWrite( 0xa0 );
    flashWaitACK();

    flashWrite( addr );
    flashWaitACK();

    flashWrite( data );
    flashWaitACK();

    flash_stop();
    EnableMasterInterrupt();
}

void __FlashWrite(const unsigned addr, const unsigned data) //对24C02的地址addr，写入一个数据date
{
    flash_start();

    flashWrite( 0xa0 );
    flashWaitACK();

    flashWrite( addr );
    flashWaitACK();

    flashWrite( data );
    flashWaitACK();

    flash_stop();
}

unsigned FlashRead( const unsigned addr )      //从24C02的addr地址，读一个字节数据
{
    DisableMasterInterrupt();
    flash_start();

    flashWrite( 0xa0 );
    flashWaitACK();

    flashWrite( addr );
    flashWaitACK();

    flash_start();
    flashWrite( 0xa1 );
    flashWaitACK();

    unsigned data = flashRead();
    flash_stop();
    EnableMasterInterrupt();
    return data;
}

unsigned __FlashRead( const unsigned addr )      //从24C02的addr地址，读一个字节数据
{
    flash_start();

    flashWrite( 0xa0 );
    flashWaitACK();

    flashWrite( addr );
    flashWaitACK();

    flash_start();
    flashWrite( 0xa1 );
    flashWaitACK();

    unsigned data = flashRead();
    flash_stop();
    return data;
}

#undef delay

#endif // FLASH_H
