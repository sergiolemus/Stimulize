# Stimulize
----

## Purpose:
----
The goal of this program is to make a great discomfort something comfortable. In the intent of making *EE457 — Digital Integrated Circuits* easier by means of automating the process of toggling *IRSIM* inputs on *ELECTRIC*.

This program sets the analyzer window to the desired nodes you wish to track. Then, it generates a sequence of *IRSIM* commands to toggle a desired set of inputs **n** starting at **0** and ending at **(2^n)-1**. It can also generate *IRSIM* commands for particular set of binary sequences the user desires.

Ex: If you have 3 inputs [ A, B, C ], the program will generate commands to toggle them from [ 0, 0, 0 ] up to [ 1, 1, 1 ], that being from 0 to 7.

Ex: If you have 4 inputs [ A, B, C, D ], and you wish to test [ 0, 0, 1, 1 ], [ 1, 1, 1, 0 ], and [ 0, 0, 0, 0 ], it will generate commands for only those three as desired.

## Compile:
----
Clone the repository and run **make** from the directory. Special thanks to **Joseph** for this convenience.

    $ make

## Usage:
----

    $ stimulize -a {node_name ..} -v {node_name ..} [-s <step_val>] [-t {test_seq ..}]

### Required:

-a,--analyzer {node_name ..}: nodes to view on the analyzer window.
-v,--vector {node_name ..}: nodes to vectorize.

### Optional:

-s,--stepsize <step_val>: set the stepsize to <step_val>, default is 5ns.
-t,--test {test_seq ..}: binary sequences to toggle the vector, default is 0 to ( 2^n ) - 1.

## Examples:
----

**Example 1:** stepsize: 5ns, analyzer: [ A, B, OUT ], vector: [ A, B ], from [ 0, 0 ] to [ 1, 1 ].

    $ stimulize -a A B OUT -v A B

**Example 2:** stepsize: 50ns, analyzer: [ I1, I2, I3, O1, O2 ], vector: [ I1, I2, I3 ] = [ 0, 0, 0 ] [ 0, 1, 0 ] [ 1, 1, 1 ]

    $ stimulize -a I1 I2 I3 O1 O2 -v I1 I2 I3 -s 50 -t 000 010 111
