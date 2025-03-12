# Homework 7
For this program, I focused on using classes to do what I needed
The approach I used mainly followed the algorithm found on
the Wikipedia page on the Kernighan-Lin algorithm. I chose to use 10 passes instead of |V| / 2
per iteration, since that seemed overzealous and ran for a lot longer.

The program is called `kernighan-lin.cpp`, and can be compiled with `g++ std=c++11 kernighan-lin.cpp -o kernighan-lin`.
I've also attached a graph with 100 vertices and a density of 0.25 for testing, named `testing.dim`.
