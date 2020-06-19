#!/usr/bin/env python3

from graphviz import render
import subprocess
import sys

def build_graph(package):
    command = "bazel query --notool_deps --noimplicit_deps \"deps(" + str(package) + ")\" --output graph"
    results = subprocess.run(command, shell=True, stdout=subprocess.PIPE).stdout
    return results

def is_edge(line):
    return "->" in line

def is_node(line):
    return not is_edge(line)

def get_node_name(text):
    return text[3: text.index(":")]

def get_node_names_from_edge(text):
    words = text.split()
    return get_node_name(words[0]), get_node_name(words[-1])

def internal_edge(name1, name2):
    return name1 == name2

def external_edge(name1, name2):
    return not internal_edge(name1, name2)

def tabs(k):
    return "\t"*k

def extract_and_rebuild(raw_input):
    lines = [x.decode('UTF-8').strip() for x in raw_input.split(b'\n')]
    lines = [x for x in lines if x[:3] == "\"//"]

    nodes = [x for x in lines if is_node(x)]
    edges = [x for x in lines if is_edge(x)]
    packages = list(set([get_node_name(x) for x in nodes]))
    external_edges = [x for x in edges if external_edge(*get_node_names_from_edge(x))]
    internal_edges = [x for x in edges if internal_edge(*get_node_names_from_edge(x))]

    output = []
    output.append("digraph mygraph {")
    output.append(tabs(1) + "node [shape=box, style=filled];")

    for p in packages:
        output.append(tabs(1) + "subgraph cluster_" + p + "{")
        output.append(tabs(2) + "label = \"//" + p + "\"")
        output.append(tabs(2) + "fontsize = 20")
        output.append(tabs(2) + "fontcolor = blue")
        output.append(tabs(2) + "color = " + ("red" if p == "main" else "blue"))
        output.append(tabs(2) + "")
        for x in [x for x in nodes if get_node_name(x) == p]:
            output.append(tabs(2) + x)
        output.append(tabs(2) + "")
        for e in [x for x in internal_edges if get_node_name(x) == p]:
            output.append(tabs(2) + e)
        
        output.append(tabs(1) + "}")
        output.append(tabs(1))

    for e in external_edges:
        output.append(tabs(1) + e)
    output.append("}")

    return output

def main():
    if len(sys.argv) == 2:
        results_raw = build_graph(sys.argv[1])
        print("")
        output_lines = extract_and_rebuild(results_raw)
        for line in output_lines:
            print(line)
        with open("architecture.gz", "w") as outfile:
            outfile.writelines(output_lines)
        print("\nSaved to architecture.gz\n")
        print("Visit: http://www.webgraphviz.com/")

        render('dot', 'png', 'architecture.gz')
        subprocess.run("open architecture.gz.png", shell=True)
        
    else:
        print("Invocation:\n\t./bazel-graph.py [packagename]")
    

if __name__ == "__main__":
	main()