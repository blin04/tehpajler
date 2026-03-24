List of instructions:
* `ADD dest src1 src2` - `dest = src1 + src2`
* `ADDI dest src1 const` - `dest = src1 + const`
* `SUB dest src1 src2` - `dest = src1 - src2`
* `SUBI dest src1 const` - `dest = src1 - const`
* `MUL dest src1 src2` - `dest = src1 * src2`
* `MULI dest src1 const` - `dest = src1 * const`
* `DIV dest src1 src2` - `dest = src1 / src2`
* `DIVI dest src1 const` - `dest = src1 / const`
* `BZ where src` - if `src` is equal to zero, jumps to instruction on line `where`
* `BP where src` - if `src` is positive, jump to instruction on line `where`
* `BN where src` - if `src` is negative, jump to instruction on line `where`
* `IN dest` - loads input from user into `dest` variable
* `OUT src` - outputs value of `dest` variable
* `OUTC ascii` - outputs given integer as ASCII character

There are infinitely many registers available, named `r1, r2, ...`.