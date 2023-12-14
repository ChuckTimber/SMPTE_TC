/* 
	MCV-2313
		MIDI to CV converter 

	global MACRO definition Header

	$RCSfile$
	$Revision: 143 $
	$Author  Satoshi "Chuck" Takagi $
	$Date: 2008-12-30 06:22:53 +0900 (火, 30 12 2008) $
 */

#ifndef GLOBAL_H
#define GLOBAL_H

#ifndef F_CPU
#define F_CPU    20000000	/*  20.00MHz processor */
#endif


#define IN       0
#define OUT      1

#define LOW      0
#define HIGH     1

#define OK       1
#define NG       0

#define ON       1
#define OFF      0

#define ENABLE   1
#define DISABLE  0

#define FALLING  0
#define RISING   1

#ifndef NULL
#define NULL 0
#endif

#endif
