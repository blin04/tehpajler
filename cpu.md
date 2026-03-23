List of instructions:
* `ADD dest src1 src2` - `dest = src1 + src2`
* `ADDI dest src1 const` - `dest = src1 + const`
* `SUB dest src1 src2` - `dest = src1 - src2`
* `SUBI dest src1 const` - `dest = src1 - const`
* `MUL dest src1 src2` - `dest = src1 * src2`
* `MULI dest src1 const` - `dest = src1 * const`
* `DIV dest src1 src2` - `dest = src1 / src2`
* `DIVI dest src1 const` - `dest = src1 / const`
* `BZ src where` - if `src` is equal to zero, jumps to instruction on line `where`
* `IN dest` - loads input from user into `dest` variable
* `OUT src` - outputs value of `dest` variable
* `OUTC src` - outputs value of `dest` variable as ASCII

There are infinitely many registers available, named `r1, r2, ...`.