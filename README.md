# Stimulize

## Purpose:
The goal of this program is to make a great discomfort something comfortable. The intent is to make *EE457 — Digital Integrated Circuits* easier by means of automating the process of toggling *IRSIM* inputs on *ELECTRIC*. Toggling inputs in *IRSIM* using the built in vecotr commands will not make the boxes, but using commands *h* and *l* will. This program acts as a wrapper to do that easily.

This program can of course also be used to generate command files for terminal *IRSIM*.

This program will take input arguments and can then be piped to *.cmd* file, which can be imported into
*ELECTRIC* by going to *TOOLS->Simulation (Built-in)->Restore Stimuli From Disk ..*

This program sets the analyzer window to the desired nodes you wish to track. Then, it generates a sequence of *IRSIM* commands to toggle a desired set of inputs **n** going through all **(2^n)-1** states. It can also generate *IRSIM* commands for particular test sequence of inputs instead.

The default mode is to scroll through inputs using **Gray Code**, changing only one bit at a time so the outputs are more stable. It will still toggle through all input combinations.

**Example 1:** If you have 3 inputs [ A, B, C ], the program will generate commands to toggle them from [ 0, 0, 0 ] to [ 1, 1, 1 ], in **Gray Code** order.

**Example 2:** If you have 4 inputs [ I1, I2, I3, I4 ], and you wish to test [ 0, 0, 0, 0 ] [ 1, 1, 1, 0 ], and [ 0, 0, 1, 1 ], it will generate commands for only those three as desired.

## Compile & Install:
Clone the repository and run **make && make install** from the directory. Special thanks to
**Joseph** for this convenience. This will compile the program and link it to your **$PATH**, and install the man page.
Depending on your system, *make install* may require administrative privileges.
This program may be maintained and updated on git. You can pull the changes, and re run the same commands to update your system.

    $ make && make install

To uninstall the program, run:

    $ make clean && make uninstall

### Man Page:
Upon runing **make install**, the man page should be copied to your local manpage directory. but if it fails may have to either 
adjsut your **$MANPATH** or change the make file.
If everything succeeds you can can now see examples and more detailed information using

    man stimulize

If installation of the manpage fails, you can still see the manpage locally by running: 

    man ./stimulize.man

while in this directory.

## Usage:

    $ stimulize -a {node_name ..} -v {node_name ..} [-s <step_val>] [-t {test_seq ..}]

### Required:

-a,--analyzer {node_name ..}: nodes to view on the analyzer window. Node lists may be given in format NAME[<num1>:<num2>] and will be auto expanded. See Example 3

-v,--vector {node_name ..}: nodes to vectorize. Node lists may be given in format NAME[<num1>:<num2>] and will be auto expanded. See Example 3

### Optional:

-s,--stepsize <step_val>: set the stepsize to <step_val>, default is 5ns.

-t,--test {test_seq ..}: binary sequences to toggle the vector, default is 0 to ( 2^n ) - 1.

-h, run this flag alone for help. Not as nicely formatted as man, but functional.

## Examples:

**Example 1:** Stepsize: default - 5ns, View [A,B,C,OUT] in the analyzer, Vectorize [A,B,C] as
inputs, and step through all combinations in Gray Code order.


    $ stimulize -a A B C OUT -v A B C

**Example 2:** Stepsize: 50ns, View [I0,I1,I2,I3,O0,O1] in the analyzer, Vectorize [I0,I1,I2,I3] as inputs, and step through the
specific cases:

[I0,I1,I2,I3] = [0,0,0,0], [1,1,1,0], [0,0,1,1], [1,1,1,1]

Pipe the output to test.cmd

    $ stimulize -a I0 I1 I2 I3 O0 O1 -v I0 I1 I2 I3 -s 50  -t  0000  1110  0011 1111 > test.cmd

**Example 3:**        Stepsize: 50ns, View [A3,A2,A1,A0,B3,B2,B1,B0,F3,F2,F1,F0] in the analyzer, Vectorize [A3,A2,A1,A0,B3,B2,B1,B0] as inputs, and step through specific cases:

[A3,A2,A1,A0,B3,B2,B1,B0]=[0,0,0,0,0,0,0,0], [1,1,1,1,0,0,0,0,], [0,0,0,0,1,1,1,1]

    $ stimulize -a A[3:0] B[3:0] F[3:0] -v A[3:0] B[3:0] -s 50 -t 00000000 11110000 00001111

## References:

*ELECTRIC* https://www.staticfreesoft.com/jmanual/

*IRSIM* http://opencircuitdesign.com/irsim/reference.html

*GRAY CODE* https://en.wikipedia.org/wiki/Gray_code