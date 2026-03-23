List of instructions:
* `ADD dest src1 src2` - `dest = src1 + src2`
* `SUB dest src1 src2` - `dest = src1 - src2`
* `MUL dest src1 src2` - `dest = src1 * src2`
* `DIV dest src1 src2` - `dest = src1 / src2`
* `BZ src where` - if `src` is equal to zero, jumps to instruction on line `where`
* `IN dest` - loads input from user into `dest` variable
* `OUT src` - outputs value of `dest` variable
* `OUTC src` - outputs value of `dest` variable as ASCII

Available variables / registers are: `A, B, ..., Z`