# PJC semester work - finding shortest paths in a graph

In this term paper, I have worked on creating a console application that allows the user to find shortest paths from point X to other points in the graph based on a given graph.
In this version of the application, two algorithms, Dijkstra's algorithm and Bellman-Ford's algorithm, are implemented. The Dijkstra algorithm can only be used on graphs with non-negatively valued edges, whereas the Bellman-Ford algorithm can also be used on these graphs but is slower compared to the Dijkstra algorithm. My implementation is done using several functions and the graphs are stored using vector and pair structures. The functional paradigm was used.\
This message was written to commit:**c6e98d06c136dba2af3119b17fcb8a6a9c4b8c09**

### Assignment Description

The assignment was to write a C++ application to find shortest paths in a graph

### Using the application
The application allows to use several switches to make the application do exactly what we want, the only switch **-f** is mandatory

* --help &emsp; displays the program help 
* -s &emsp; specifies the vertex from which the paths should be calculated (if this switch is not used, the path will be calculated from all vertices)
* **-f** &emsp; **path to the graph file you want to work with**
* -w &emsp; path to the file where we want to save the result of the program
* -t &emsp; measures how long it took to calculate the path
* -a &emsp; the name of the algorithm we want to calculate the path(s) ( without specification the bellman-ford algorithm is used)

### Sample application usage

Consider the following graph:
![graph](img/graph.png)


Let's rewrite this graph into a format supported by this application hence:
```
S:A=10,E=8
A:C=2
B:A=1
C:B=-2
D:A=-4,C=-1
E:D=1
```

or from where:to where=path length, where the comma separates the path destinations. We save this to a file with the extension txt.
Run the compiled program, here semestral, using the command:

./semestral -f ../input.txt -a B -s S

Where we take the input.txt file as input, we use bellman-ford as the algorithm, and we choose the vertex S as the starting point.\
This command will then return the result:
```
This results were calculated using.

Shortest path from vertex: S
| S | 0 | S
| A | 5 | A -> D -> E -> S
| B | 5 | B -> C -> A -> D -> E -> S
| C | 7 | C -> A -> D -> E -> S
| D | 9 | D -> E -> S
| E | 8 | E -> S
```

The measurements were performed on a computer with the following parameters (neofetch program listing):
```
                               OS: Manjaro Linux x86_64 
██████████████████ ████████ Host: 81YQ IdeaPad 5 15ARE05 
████████ ████████ Kernel: 5.13.19-2-MANJARO 
████████ ████████ Uptime: 1 hour, 4 mins 
████████ ████████ ████████ Packages: 1505 (pacman), 30 (snap) 
████████ ████████ ████████ Shell: zsh 5.9 
████████ ████████ Resolution: 1920x1080 
████████ ████████ DE: Plasma 5.25.5 
████████ ████████ ████████ WM: KWin 
████████ ████████ ████████ WM Theme: Sweet-Dark-transparent 
████████ ████████ ████████ Theme: Breath2 2021 Dark [Plasma], Breeze [GTK3] 
████████ ████████ Icons: Vector-Maia-dark [Plasma], Vector-Maia-dark [GTK2/3] 
                               Terminal: console 
                               Terminal Font: Hack 10 
                               CPU: AMD Ryzen 7 4700U with Radeon Graphics (8) @ 2.000GHz 
                               GPU: AMD ATI 03:00.0 Renoir 
                               Memory: 5825MiB / 15362MiB 
```

Tests:\
dijkstra.txt - Dijkstra - 290 microseconds
dijkstra.txt - Bellman-Ford - 547 microseconds
input.txt - Bellman-Ford - 541 microseconds

In the **graphs** folder you will find two test graphs.
