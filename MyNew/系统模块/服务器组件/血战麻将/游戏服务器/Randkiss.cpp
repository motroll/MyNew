// RandKiss.cpp: implementation of the CRandKiss class.

//

//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "RandKiss.h"



//////////////////////////////////////////////////////////////////////

// Construction/Destruction

//////////////////////////////////////////////////////////////////////



CRandKiss::CRandKiss()

{

	x = 123456789;

	y = 362436;

	z = 521288629;

	c = 7654321;

}



CRandKiss::~CRandKiss()

{



}



unsigned long CRandKiss::RandKiss()

{

	unsigned long long t = 0;

	unsigned long long a = 698769069;



	//����1������ͬ��

	x = 69069 * x + 12345;



	//����2����λ���

	y ^= (y << 13);

	y ^= (y >> 17);

	y ^= (y << 5);



	//����3������λ�ĳ˷�

	t = a * z + c;

	c = (t >> 32);

	return x + y + (z = t);

}


