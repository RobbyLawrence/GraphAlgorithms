# Homework 6
The file attached finds 5-coloring for a planar graph. Instead of leveraging Wernicke's Theorem,
which I was finding really difficult to implement, I instead ran greedy coloring from a different starting vertex
anytime I got more than 5 colors. In the case that I got 5 or less colors, I outputted that graph.

Compile with `g++ -std=c+=11 planar_color.cpp -o planar_color, and you can run with ./planar_color <file_name>.
