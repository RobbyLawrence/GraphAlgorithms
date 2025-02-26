import sys
from gurobipy import Model, GRB, quicksum


def read_graph(filename): # importing the graph from .dim file
    with open(filename, 'r') as file:
        num_vertices, num_edges = map(int, file.readline().split())
        edges = []
        for line in file:
            u, v = map(int, line.split())
            edges.append((u, v))
    return num_vertices, edges


def solve_min_vertex_cover(num_vertices, edges):
    # use gurobi to solve ilp
    try:
        model = Model("min_vertex_cover")
        # had to look up how to do all this lol
        x = model.addVars(num_vertices, vtype=GRB.BINARY, name="x")
        model.setObjective(quicksum(x[i] for i in range(num_vertices)), GRB.MINIMIZE)
        # add constraints
        for u, v in edges:
            model.addConstr(x[u] + x[v] >= 1, f"edge_{u}_{v}")
        model.optimize()

        # check if an optimal solution was actually found
        if model.status == GRB.OPTIMAL:
            cover = [i for i in range(num_vertices) if x[i].X > 0.5]
            print(f"Size of vertex cover: {len(cover)}")
            print("{" + ", ".join(map(str, cover)) + "}")
        else:
            print("0")

    except Exception as e:
        print(f"Error during optimization: {e}")


def main():
    # make sure there are only two command line args
    if len(sys.argv) != 2:
        print("usage: python main.py <file_name>")
        sys.exit(1)

    filename = sys.argv[1]
    num_vertices, edges = read_graph(filename)
    solve_min_vertex_cover(num_vertices, edges)


if __name__ == "__main__":
    main()
