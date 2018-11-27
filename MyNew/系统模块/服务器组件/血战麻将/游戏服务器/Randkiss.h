#pragma once
#ifndef __RANDKISS_H__
#define __RANDKISS_H__

class CRandKiss
{
public:
	CRandKiss();
	virtual ~CRandKiss();

	unsigned long x;
	unsigned long y;
	unsigned long z;
	unsigned long c;

	unsigned long RandKiss();

	void SrandKiss(unsigned int iSeed)
	{
		c = iSeed & 0x1FFFFFFF;
	}
};

#endif // __RANDKISS_H__

