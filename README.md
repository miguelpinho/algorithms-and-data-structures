# Algorithms and Data Structures Project

This repository contains the final project for the Algorithms and Data
Structures Course of the 1st Semester of 2015/2016, by Miguel Malaca
([@malaca3](https://github.com/malaca3)) and Miguel Pinho
([@miguelpinho](https://github.com/miguelpinho)).

It passed all the 20 evaluation tests in the course's Mooshak (within memory and
time constraints), and had the final grade of 19 (in 20).

This project consisted in an optimal route planner for an underground car park.
It touched on several concepts lectured in the course, and in particular it
solved the problem efficiently (in terms of memory and run time) using a
modified version of **Dijkstra's algorithm**. It also provides efficient
implementations of the necessary data structures, including **linked lists**,
**heaps**, and **hash tables**.

## Building

In an linux/unix system, the project can be compiled using:

```bash
make autopark
```

It has no dependencies other than the standard libraries.

## Usage

The autopark executable has the following usage:

```bash
./autopark file.cfg file.inp [file.res]
```

Where `file.cfg` is the file with the garage configuration, and `file.inp` is
the file the cars that have to be parked, using the formats described in
`docs/description-pt.pdf`. `file.res` is an optional input file with restriction
events in the park. The output file is generated with the same path as
`file.inp`, but with the extension `.pts`.

## Project tree

- `src`: C source files (`.c`)
- `inc`: C header files (`.h`)
- `samples`: sample input and output files
- `docs`: project description and report
