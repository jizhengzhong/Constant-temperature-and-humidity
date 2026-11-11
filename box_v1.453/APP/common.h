#ifndef __COMMON_H__
#define __COMMON_H__


#define C_OK 					0x00
#define C_NOTOK					0xff

#define IIC_ACK_ERROR			0X01


#define C_CMDERR          		0x01                // 命令字错误
#define C_PARAERR          		0x02                // 命令参数错误
#define C_CRCERR           		0x03                // 校验和错误
#define C_E2P_READERR        	0x04                // 读EEPROM失败
#define C_E2P_WRITEERR       	0x05                // 写EEPROM失败
#define C_E2P_RDBACK      		0x06				        // EEPROM回读错误
#define C_E2P_CRCERR         	0x07                // EEPROM校验和错误

// 读写操作信息
#define EEPREAD             	0x00
#define EEPWRITE            	0x01
#define EEPCALCRC		   		    0x01
#define EEPREADBACK		   		  0x02
#define EEPDECODE		   		    0x04
#define EEPUNCODE           	0x08
#define EEPCHECKCRC         	0x10



unsigned char EEPRW_Mode(unsigned char cMode, unsigned char cType, unsigned char *cpBuf, unsigned int wLength);

#endif
