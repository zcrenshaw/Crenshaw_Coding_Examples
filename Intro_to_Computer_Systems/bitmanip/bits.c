/* 
 * CS:APP Data Lab 
 * 
 * Zack Crenshaw zcrenshaw
 * 
 * bits.c - Source file with your solutions to the project.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the project by
editing the collection of functions in this source file.

CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. The maximum number of ops for each function is given in the
     header comment for each function.

/*
 * STEP 2: Modify the following functions according to the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the btest checker to verify that your solutions produce
 *      the correct answers.
 */


#endif
/* 
 * absVal - absolute value of x
 *   Example: absVal(-1) = 1.
 *   You may assume -TMax <= x <= TMax
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int absVal(int x) {
/* will only change bits if number is negative 
   by producing a mask of 0xFFFFFFFF by arithmetic shift */
	int mask = x >> 31;
	return (mask^x) + (~mask + 1);
}
/* 
 * addOK - Determine if can compute x+y without overflow
 *   Example: addOK(0x80000000,0x80000000) = 0,
 *            addOK(0x80000000,0x70000000) = 1, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int addOK(int x, int y) {
//compares x and y to sum
//if first bit changed, then there was overflow
	int sum = x + y;
  	return !(((sum ^ x) & (sum ^ y)) >> 31);
}
/* 
 * allEvenBits - return 1 if all even-numbered bits in word set to 1
 *   Examples allEvenBits(0xFFFFFFFE) = 0, allEvenBits(0x55555555) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allEvenBits(int x) {
/* makes mask of 0101... bit pattern then compares it to x */
	int evens = 0x55 + (0x55 << 8) + (0x55 << 16) + (0x55 << 24);
	return !((x & evens) ^ evens);
}
/*
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
/* or x against its negative, then add 1
   ~0 + 1 is also zero, so 0 | -0 will be 0
   any other number | its negative is will have a one in the first bit (made into -1 by bitshift) */
	return (((~x + 1) | x) >> 31) + 1;
}
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
/* compares the last digit to 1 then right shifts to compare the next */
	int count = 0;
	count += (x&1);
	x = x >> 1;
	count += (x&1);
	x = x >> 1;
	count += (x&1);
	x = x >> 1;
	count += (x&1);
	x = x >> 1;
	count += (x&1);
	x = x >> 1;
	count += (x&1);
	x = x >> 1;
	count += (x&1);
	x = x >> 1;
	count += (x&1);
	x = x >> 1;
	count += (x&1);
	x = x >> 1;
	count += (x&1);
	x = x >> 1;
	count += (x&1);
	x = x >> 1;
	count += (x&1);
	x = x >> 1;
	count += (x&1);
	x = x >> 1;
	count += (x&1);
	x = x >> 1;
	count += (x&1);
	x = x >> 1;
	count += (x&1);
	x = x >> 1;
	count += (x&1);
	x = x >> 1;
	count += (x&1);
	x = x >> 1;
	count += (x&1);
	x = x >> 1;
	count += (x&1);
	x = x >> 1;
	count += (x&1);
	x = x >> 1;
	count += (x&1);
	x = x >> 1;
	count += (x&1);
	x = x >> 1;
	count += (x&1);
	x = x >> 1;
	count += (x&1);
	x = x >> 1;
	count += (x&1);
	x = x >> 1;
	count += (x&1);
	x = x >> 1;
	count += (x&1);
	x = x >> 1;
	count += (x&1);
	x = x >> 1;
	count += (x&1);
	x = x >> 1;
	count += (x&1);
	x = x >> 1;
	count += (x&1);
	return count;
}
/*
 * bitNor - ~(x|y) using only ~ and & 
 *   Example: bitNor(0x6, 0x5) = 0xFFFFFFF8
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
 */
int bitNor(int x, int y) {
/* compares the inverse of each input to find negative space (zeros) */
	return (~x & ~y);
}

/*
 * byteSwap - swaps the nth byte and the mth byte
 *  Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *            byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *  You may assume that 0 <= n <= 3, 0 <= m <= 3
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 2
 */
int byteSwap(int x, int n, int m) {
/* get the two bytes
   null their places in x
   place them in each other's spot	*/
	int shift_n = n << 3;
	int shift_m = m << 3;

	int nth_byte = ((x >> shift_n) & 0xFF);
	int mth_byte = ((x >> shift_m) & 0xFF);
	
	int n_place = (nth_byte << shift_n);
	int m_place = (mth_byte << shift_m);

	int holy = x & ~n_place & ~m_place; //because it has holes
	return holy | (nth_byte << shift_m) | (mth_byte << shift_n) ;
	
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
/* makes x 0 or 1 then makes a mask of that digit across all bits
   compares mask with x, inverse with y (leaving one all zeros and one the y or z value) */
	int mask = (!!x << 31) >> 31;
	return (mask & y) | ((~mask) & z);
}
/*
 * ezThreeFourths - multiplies by 3/4 rounding toward 0,
 *   Should exactly duplicate effect of C expression (x*3/4),
 *   including overflow behavior.
 *   Examples: ezThreeFourths(11) = 8
 *             ezThreeFourths(-9) = -6
 *             ezThreeFourths(1073741824) = -268435456 (overflow)
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 3
 */
int ezThreeFourths(int x) {
/* (x*3) / 4  with mask to account for signed behavior*/
	int times3 = (x << 1) + x;
	int mask = ((times3 >> 31) & ((1 << 2) + ~0));
	return ((times3 + mask) >> 2);
}
/* 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
/* if number can be shifted over and back and be the same, 
   then the number will fit into a signed n-bit number 
   because first on bit does not change to two's compliment bit */
	int shift_check = 33 + ~n;
	return !(x ^((x <<shift_check) >> shift_check));
}
/*
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
//bit shifts x such that the last byte is the target byte
//then filters out any leading ones
	return ((x >> (n << 3))&0xFF);
}
/* 
 * greatestBitPos - return a mask that marks the position of the
 *               most significant 1 bit. If x == 0, return 0
 *   Example: greatestBitPos(96) = 0x40
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 70
 *   Rating: 4 
 */
int greatestBitPos(int x) {
/* creates mask of all ones starting from greatest bit and compares it to x
   mask will overlap with x on greatest bit
   also checks for edge case of Tmin */
	int mask = x;
	mask |= x >> 1;
	mask |= x >> 2;
	mask |= x >> 3;
	mask |= x >> 4;
	mask |= x >> 5;
	mask |= x >> 6;
	mask |= x >> 7;
	mask |= x >> 8;
	mask |= x >> 9;
	mask |= x >> 10;
	mask |= x >> 11;
	mask |= x >> 12;
	mask |= x >> 13;
	mask |= x >> 14;
	mask |= x >> 15;
	mask |= x >> 16;
	mask |= x >> 17;
	mask |= x >> 18;
	mask |= x >> 19;
	mask |= x >> 20;
	mask |= x >> 21;
	mask |= x >> 22;
	mask |= x >> 23;
	mask |= x >> 24;
	mask |= x >> 25;
	mask |= x >> 26;
	mask |= x >> 27;
	mask |= x >> 28;
	mask |= x >> 29;
	mask |= x >> 30;
	mask |= x >> 31;
	return (x & (~mask >> 1)) | (x&(1 << 31));
}
/*
 * implication - return x -> y in propositional logic - 0 for false, 1
 * for true
 *   Example: implication(1,1) = 1
 *            implication(1,0) = 0
 *   Legal ops: ! ~ ^ |
 *   Max ops: 5
 *   Rating: 2
 */
int implication(int x, int y) {
//only return false if x is true and y is false (0 | 0 with given operators)
	return !x | !!y;
}
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
//checks if second to last nibble is 0x3
//checks if last nibble is less than 10
	int is_3x = !((x >> 4)^0x3);
	int last_nibble = x & 0xF;
	return is_3x & !((last_nibble + 6) >> 4);
}
/* 
 * isEqual - return 1 if x == y, and 0 otherwise 
 *   Examples: isEqual(5,5) = 1, isEqual(4,5) = 0
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int isEqual(int x, int y) {
// equal values should have all ones in the same places
// x^y = 0 means this all ones in same places
	return !(x^y);
}
/*
 * isLess - if x < y  then return 1, else return 0 
 *   Example: isLess(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLess(int x, int y) {
/* x < y if:
   x < 0 && y >= 0
   or
   x - y = negative (and they have the same sign) */
	int xsign = x >> 31;
	int ysign = y >> 31;
	int diff = x + 1 + ~y;
	int diffsign = diff >> 31;
	int turnary = !(xsign^ysign);
	int mask = (turnary << 31) >> 31;
	return (mask & (!!diffsign)) | ((~mask) & (!(y >> 31)));
}
/*
 * isNonNegative - return 1 if x >= 0, return 0 otherwise 
 *   Example: isNonNegative(-1) = 0.  isNonNegative(0) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 3
 */
int isNonNegative(int x) {
// checks to see if first bit turned on (meaning number is negative)
	return !(x >> 31);
}
/*
 * isPower2 - returns 1 if x is a power of 2, and 0 otherwise
 *   Examples: isPower2(5) = 0, isPower2(8) = 1, isPower2(0) = 0
 *   Note that no negative number is a power of 2.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int isPower2(int x) {
//any power of two minus one will be all ones starting one place lower
//anding x and its minus one if power of two will be all zeros
//also catches edge cases Tmin and 0
	return !((x + ~0) & x) & !!(x^(0x1 << 31)) & !!(x^0);
}
/*
 * isTmin - returns 1 if x is the minimum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmin(int x) {
//x is Tmin if added against its self = 0 and it is not 0
	return !((x+x) ^ 0) ^ !(x^0);
}
/*
 * minusOne - return a value of -1 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 2
 *   Rating: 1
 */
int minusOne(void) {
// -1 is all bits turned on (inverse of all bits turned off, i.e zero)
	return (~0);
}
/*
 * rotateLeft - Rotate x to the left by n
 *   Can assume that 0 <= n <= 31
 *   Examples: rotateLeft(0x87654321,4) = 0x76543218
 *   Legal ops: ~ & ^ | + << >> !
 *   Max ops: 25
 *   Rating: 3 
 */
int rotateLeft(int x, int n) {
//shift left by n
// logical right shift by 32 - n to get bytes we want
//patching them together
	int bits_minus_n = 33 + ~n;
	int left = x << n;
	int right = (x >> bits_minus_n) & (~(((1 << 31) >> bits_minus_n) << 1));
	return left | right;
}
/*
 * satMul2 - multiplies by 2, saturating to Tmin or Tmax if overflow
 *   Examples: satMul2(0x30000000) = 0x60000000
 *             satMul2(0x40000000) = 0x7FFFFFFF (saturate to TMax)
 *             satMul2(0x60000000) = 0x80000000 (saturate to TMin)
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int satMul2(int x) {
	return 2;

}

