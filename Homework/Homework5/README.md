# Homework 5
I've attached files greedy_clique.cpp and min_vc.cpp. They can be compiled with
`g++ -std=c++11 greedy_clique.cpp -o greedy_clique` and `g++ -std=c++11 min_vc.cpp -o min_vc` respectively.

For the greedy clique program, I used recursion and checked through the neighbors of a vertex in a BFS-y
kind of way, except I used a vector instead of a queue so that I could maintain access to indexes.
If a vertex didn't work out, it got popped out of the current clique, and if it turned out to be a maximal clique, then
I added it to a vector of cliques.

For the minimum vertex cover program, I used recursion again (much to my chagrin). Originally, I thought about using
a vector of edges and removing from this vector until it was empty, but I kept coming up with counterexamples for the
algorithm and couldn't nail them all down. The program I came up with is VERY VERY slow, especially for large graphs,
as it uses recursive backtracking. The main time is spent in the find_vertex_cover function, where I try a vertex, and if
it leads to a valid cover, I leave it in; otherwise, I take it out. That's why the program is so slow. I PROMISE IT WORKS THOUGH!
Only for small graphs though ;-:

For problem 2b, I included a pdf where I wrote my thoughts on the problem.

For problem 2c, I attached a program main.py that implements the ideas in my pdf
using Gurobi in Python. I had to watch a lot of YouTube videos to get this to work...
While formulating the problem was fun, I did NOT enjoy actually writing the code and having
to work through Gurobi's quirks. I'm not much of a Python wizard, so I had to use PyCharm and wrangle Gurobi
from there. I'm not sure how to include Gurobi library; it works on my machine, but I don't know how to run
it on the Linux machines. If we need to talk about it to get it to work, that's cool with me; just reach out.
email is rlawren9@vols.utk.edu.
