/*
	7 segment LED difinitions
*/
/*
#define LED_null 0xff
#define LED_0 0xc0
#define LED_1 0xf9
#define LED_2 0xa4
#define LED_3 0xb0
#define LED_4 0x99
#define LED_5 0x92
#define LED_6 0x82
#define LED_7 0xf8
#define LED_8 0x80
#define LED_9 0x90
#define LED_dot 0x7f
#define LED_o 0x3A
#define LED_A 0xEE
#define LED_b 0x3E
#define LED_c 0x1A
#define LED_C 0x9C
#define LED_d 0x7A
#define LED_E 0x9E
#define LED_F 0x8E
#define LED_G 0xBC
#define LED_h 0x2E
#define LED_H 0x6E
#define LED_J 0x78
#define LED_L 0x1C
#define LED_t 0x1E
#define LED_r 0x0A
#define LED_n 0x2A
#define LED_U 0x7C
#define LED_P 0xCE
#define LED_dash 0x02
*/
/*
//		                        a b c d e f g p
//                              0 1 2 3 4 5 6 7
#define LED_null 0x00	// []	0 0 0 0 0 0 0 0
#define LED_0 0x3F		// 0	1 1 1 1 1 1 0 0
#define LED_1 0x06		// 1	0 1 1 0 0 0 0 0
#define LED_2 0x5B		// 2	1 1 0 1 1 0 1 0
#define LED_3 0x4F		// 3	1 1 1 1 0 0 1 0
#define LED_4 0x66		// 4	0 1 1 0 0 1 1 0
#define LED_5 0x6D		// 5	1 0 1 1 0 1 1 0
#define LED_6 0x7D		// 6	1 0 1 1 1 1 1 0
#define LED_7 0x27		// 7	1 1 1 0 0 1 0 0
#define LED_8 0x7F		// 8	1 1 1 1 1 1 1 0
#define LED_9 0x6F		// 9	1 1 1 1 0 1 1 0
#define LED_dot 0x80	// .	0 0 0 0 0 0 0 1
#define LED_o 0x5C		// o	0 0 1 1 1 0 1 0
#define LED_A 0x77		// A	1 1 1 0 1 1 1 0
#define LED_b 0x7C		// b	0 0 1 1 1 1 1 0
#define LED_c 0x68		// c	0 0 0 1 1 0 1 0
#define LED_C 0x39		// C	1 0 0 1 1 1 0 0
#define LED_d 0x5E		// d	0 1 1 1 1 0 1 0
#define LED_E 0x79		// E	1 0 0 1 1 1 1 0
#define LED_F 0x71		// F	1 0 0 0 1 1 1 0
#define LED_G 0x3D		// G	1 0 1 1 1 1 0 0
#define LED_h 0x74		// h	0 0 1 0 1 1 1 0
#define LED_H 0x76		// H	0 1 1 0 1 1 1 0
#define LED_J 0x1E		// J	0 1 1 1 1 0 0 0
#define LED_L 0x38		// L	0 0 0 1 1 1 0 0
#define LED_t 0x78		// t	0 0 0 1 1 1 1 0
#define LED_r 0x50		// r	0 0 0 0 1 0 1 0
#define LED_n 0x54		// n	0 0 1 0 1 0 1 0
#define LED_U 0x3E		// U	0 1 1 1 1 1 0 0
#define LED_P 0x73		// P	1 1 0 0 1 1 1 0
#define LED_dash 0x40	// -	0 0 0 0 0 0 1 0
*/
//		                        a b c d e f g p
//                              7 6 5 4 3 2 1 0
#define LED_null 0x00	// []	0 0 0 0 0 0 0 0
#define LED_0 0xFC 		// 0	1 1 1 1 1 1 0 0
#define LED_1 0x60 		// 1	0 1 1 0 0 0 0 0
#define LED_2 0xDA	 	// 2	1 1 0 1 1 0 1 0
#define LED_3 0xF2		// 3	1 1 1 1 0 0 1 0
#define LED_4 0x66		// 4	0 1 1 0 0 1 1 0
#define LED_5 0xB6		// 5	1 0 1 1 0 1 1 0
#define LED_6 0xBE		// 6	1 0 1 1 1 1 1 0
#define LED_7 0xE4		// 7	1 1 1 0 0 1 0 0
#define LED_8 0xFE		// 8	1 1 1 1 1 1 1 0
#define LED_9 0xF6		// 9	1 1 1 1 0 1 1 0
#define LED_dot 0x01	// .	0 0 0 0 0 0 0 1
#define LED_o 0x3A		// o	0 0 1 1 1 0 1 0
#define LED_A 0xEE		// A	1 1 1 0 1 1 1 0
#define LED_b 0x3E		// b	0 0 1 1 1 1 1 0
#define LED_c 0x1A		// c	0 0 0 1 1 0 1 0
#define LED_C 0x9C		// C	1 0 0 1 1 1 0 0
#define LED_d 0x7A		// d	0 1 1 1 1 0 1 0
#define LED_E 0x9E		// E	1 0 0 1 1 1 1 0
#define LED_F 0x8E		// F	1 0 0 0 1 1 1 0
#define LED_G 0xBC		// G	1 0 1 1 1 1 0 0
#define LED_h 0x2E		// h	0 0 1 0 1 1 1 0
#define LED_H 0x6E		// H	0 1 1 0 1 1 1 0
#define LED_J 0x78		// J	0 1 1 1 1 0 0 0
#define LED_L 0x1C		// L	0 0 0 1 1 1 0 0
#define LED_t 0x1E		// t	0 0 0 1 1 1 1 0
#define LED_r 0x0A		// r	0 0 0 0 1 0 1 0
#define LED_n 0x2A		// n	0 0 1 0 1 0 1 0
#define LED_U 0x7C		// U	0 1 1 1 1 1 0 0
#define LED_P 0xCE		// P	1 1 0 0 1 1 1 0
#define LED_dash 0x02	// -	0 0 0 0 0 0 1 0

/*
		a b c d e f g p
	[]	0 0 0 0 0 0 0 0 -> 0x00
	0	1 1 1 1 1 1 0 0 -> 0xFC
	1	0 1 1 0 0 0 0 0 -> 0x60
	2	1 1 0 1 1 0 1 0 -> 0xDA
	3	1 1 1 1 0 0 1 0 -> 0xF2
	4	0 1 1 0 0 1 1 0 -> 0x66
	5	1 0 1 1 0 1 1 0 -> 0xB6
	6	1 0 1 1 1 1 1 0 -> 0xBE
	7	1 1 1 0 0 1 0 0 -> 0xE4
	8	1 1 1 1 1 1 1 0 -> 0xFE
	9	1 1 1 1 0 1 1 0 -> 0xF6
	.	0 0 0 0 0 0 0 1 -> 0x01
	o	0 0 1 1 1 0 1 0 -> 0x3A
	A	1 1 1 0 1 1 1 0 -> 0xEE
	b	0 0 1 1 1 1 1 0 -> 0x3E
	c	0 0 0 1 1 0 1 0 -> 0x1A
	C	1 0 0 1 1 1 0 0 -> 0x9C
	d	0 1 1 1 1 0 1 0 -> 0x7A
	E	1 0 0 1 1 1 1 0 -> 0x9E
	F	1 0 0 0 1 1 1 0 -> 0x8E
	G	1 0 1 1 1 1 0 0 -> 0xBC
	h	0 0 1 0 1 1 1 0 -> 0x2E
	H	0 1 1 0 1 1 1 0 -> 0x6E
	J	0 1 1 1 1 0 0 0 -> 0x78
	L	0 0 0 1 1 1 0 0 -> 0x1C
	t	0 0 0 1 1 1 1 0 -> 0x1E
	r	0 0 0 0 1 0 1 0 -> 0x0A
	n	0 0 1 0 1 0 1 0 -> 0x2A
	U	0 1 1 1 1 1 0 0 -> 0x7C
	P	1 1 0 0 1 1 1 0 -> 0xCE
	-	0 0 0 0 0 0 1 0 -> 0x02
	
*/
