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

## Compile:
Clone the repository and run **make && make install** from the directory. Special thanks to
**Joseph** for this convenience. This will compile the program and link it to your **$PATH**
Depending on your system, *make install* may require administrative privilege.
This program may be maintained and updated on git. You can pull the changes, and re run the same commands to update your system.

    $ make && make install

## Usage:

    $ stimulize -a {node_name ..} -v {node_name ..} [-s <step_val>] [-t {test_seq ..}]

### Required:

-a,--analyzer {node_name ..}: nodes to view on the analyzer window.

-v,--vector {node_name ..}: nodes to vectorize.

### Optional:

-s,--stepsize <step_val>: set the stepsize to <step_val>, default is 5ns.

-t,--test {test_seq ..}: binary sequences to toggle the vector, default is 0 to ( 2^n ) - 1.

## Examples:

**Example 1:** stepsize: 5ns, analyzer: [ A, B, C, OUT ], vector: [ A, B, C ], from [ 0, 0, 0 ] to [ 1, 1, 1 ] in *Gray Code* order, and pipe into *test.cmd*

    $ stimulize -a A B OUT -v A B > test.cmd

**Example 2:** stepsize: 50ns, analyzer: [ I1, I2, I3, I4, O1, O2 ], vector: [ I1, I2, I3, I4 ] = [ 0, 0, 0, 0 ] [ 1, 1, 1, 0 ] [ 0, 0, 1, 1 ]

    $ stimulize -a I1 I2 I3 O1 O2 -v I1 I2 I3 -s 50 -t 0000 1110 0011

**Example 3:** print help
    $ stimulize -h

## References:

*ELECTRIC* https://www.staticfreesoft.com/jmanual/

*IRSIM* http://opencircuitdesign.com/irsim/reference.html

*GRAY CODE* https://en.wikipedia.org/wiki/Gray_code
