/*
 * AtmegaPins.h
 *
 * Created: 3/5/2013 3:39:07 PM
 *  Author: mfolz
 */ 


#ifndef ATMEGAPINS_H_INCLUDED_
#define ATMEGAPINS_H_INCLUDED_

#define SETBIT(ADDRESS,BIT) (ADDRESS |= (1<<BIT))
#define CLEARBIT(ADDRESS,BIT) (ADDRESS &= ~(1<<BIT))
#define FLIPBIT(ADDRESS,BIT) (ADDRESS ^= (1<<BIT))
#define CHECKBIT(ADDRESS,BIT) (ADDRESS & (1<<BIT))



#endif /* ATMEGAPINS_H_ */