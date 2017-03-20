/*
 * commontypes.h
 *
 *  Created on: 11.12.2015
 *      Author: uidg6243
 */
#ifndef COMMONTYPES_H_
#define COMMONTYPES_H_

typedef unsigned char                  	uint8;	//0-255
typedef char						  	int8;	//-128 - 127
typedef unsigned short                 	uint16;	//0-65535
typedef short                 			int16;	//-32768 - 32767
typedef unsigned long                   uint32;	//0 - 4 294 967 295
typedef long                   			int32;	//-2147483648 - 2147483647
typedef unsigned long long int         	uint64;
typedef long long int         			int64;	//-9223372036854775808 - 9223372036854775807
typedef uint8							bool;
#define false 0
#define true 1

//uint16 map (uint16 x, uint16 in_min, uint16 in_max, uint16 out_min, uint16 out_max)
//{
//	uint32 tmpValue;
//	tmpValue  = (x - in_min) * (out_max - out_min);
//	tmpValue /= (in_max - in_min);
//	tmpValue += out_min;
//	return (uint16)tmpValue;
//}
#endif /* COMMONTYPES_H_ */
