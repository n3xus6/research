# The Twos Complement representation

## Abstract
Integer arithmetic by the CPU is done using the **twos complement representation** of the operands. Usually the programmer uses decimal numbers and can leave the details on the bit level to the system. However, being familiar with 2's complement can become useful in low-level programming, and helps to avoid mistakes, especially when bit manipulations are involved. This brief overview wants to explain it's basics. All of the examples and the description assume 8-bit signed integers. The concept is the same for 32-bit and 64-bit wide integers.

## Twos complement representation explained
With n bits 2^n different values can be stored (0 - 2^n-1, if only positive numbers would be allowed).

Zero is represented as 00000000. 2^n-1 values are left. Because this is an odd number, the number of the remaining positive and negative values have to be different.

The twos complement representation of an integer X can be described by the following formula, where x_n is the bit at the n'th position:

``-2^n-1 * x_n-1 + 2^n-2 * x_n-2 + 2^n-3 * x_n-3 + ... + 2^0 * x_0``

The first term has a negative sign. If the left-most bit is set, the integer is negative. Any bits afterwards makes the integer value grow. Meaning that 1000000 (-128) is the smallest number that fits into 8 bits and 01111111 (127) ist the largest number that is possible to store. So for n bits the negative range is from -1 to -2^(n-1) and the positive range is from 0 to 2^(n-1) - 1.

With the formula above and this table it should become more clear:

| Decimal | Twos complement representation |
| --------| -------------------------------|
| 0       | 00000000 |
| 1       | 00000001 |
| -1      | 11111111 |
| 32      | 00100000 |
| -32     | 11100000 |
| 127     | 01111111 |
| -127    | 10000001 |
| -128    | 10000000 |

The left-most bit is called the sign-bit.

## Taking the twos complement

The meaning of "taking the twos complement" is doing the bitwise complement, including the sign-bit, of a number, then adding one.

Example: twos complement of 01100000 (96):

1. Bitwise complement: 10011111
2. add 1:              10100000 (-96)

Example: twos complement of 11100000 (-32):

1. Bitwise complement: 00011111
2. add 1:              00100000 (32)

## Computations in the twos complement representation

### Negation

Negation is done by taking the twos complement. But note that 128 can not be represented:

10000000 (-128) \
01111111 (after bitwise complement) \
10000000 ! (after adding 1)

### Addition

When adding two 1-bits we have to carry over a 1 to the next bit position on the left.

Example: 52 + 53 = 105
```
  00110100
+ 00110101 
= 01101001 (105)
```

Example: -53 + 52 = -1

```
  11001011
+ 00110100 +
= 11111111 (-1)
```

Example with **Overflow** 117 + 11:

```
  01110101
+ 00001011 
= 11111111 Wrong result!
```

If two numbers have the same sign-bit and the result has a different sign-bit, then an overflow occured and the sum is incorrect in the mathematical sense.

### Substraction

`X - Y` can be expressed as an addition operation: `X + take_twos_complement(Y)`.

Example: X = 100, Y = 42.

Twos complement of 42 is -42:

```
00101010 (42)
11010101 (after inverting all bits)
11010110 (after adding one: -42)
```

```
  01100100 (100)
+ 11010110 (-42)
= 00111010 = 58
```

The **carry over** from the last bit is ignored in the result.

### Conclusion

We have seen how the computer does integer arithmetic internally using the twos complement form. If we add/substract integers the result can become incorrect due to overlow. The result becomes clear if we reproduce the calculation steps in twos complement representation. This was just a glimps how twos complement works. In books teaching system architecture and low-level programming you can find further material. Also check the CPU and the Compiler manuals. Take caution with bit manipulation on signed integers. Some border cases are implementation-defined.
