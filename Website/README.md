To run the Flask application:

1) First once in the directory containing app.py, run 

    pip install -r requirements.txt
    
    (use pip3 instead of pip if using python3)

2) Then, Run the following command:

    python app.py

    (use python3 if using python3)

3) Finally, Go to http://127.0.0.1:5000/ on your browser to access the website

Note: Python3 is recommended for the application

The website has been deployed online to https://shivam-sood.github.io/. This website contains all the notes written as well as three solvers ( Travelling salesman problem, Knapsack problem, and a gavel graph creator).

The three solvers have been deployed on a separate domain, to which they are linked to from the main website, since github pages only allow static pages and hence server applications like flask would not work on them.

So, Azure was used to make an web app, which would contain the three solvers at https://shivam-sood.azurewebsites.net/tsp for Travelling salesman problem solver, 
https://shivam-sood.azurewebsites.net/sack for Knapsack problem solver, and https://shivam-sood.azurewebsites.net/graph for the Gavel graph creator.

The free tier of Azure was used due to which there is a daily CPU usage limit, so the azure domain may stop working at any time due to excessive usage, so using the provided flask app is recommended to check working of the simulators.

Also for larger inputs, a large amount of time might be needed for the program to run, so using smaller data sets is recommended to minimize waiting times. Also the server for the free tier of azure uses only 1Gb of ram so online website might function even slower so again using flask locally to run solvers on the site is recommened, with the online deployment of the solvers acting as a proof of concept. 

Since there was a problem with CPU usage limit, I divided the whole website into two parts (static part and dynamic part), and deployed them separately online. So static part(like notes) are on github.io and dynamic part (like the solvers) are on azurewebsites.

To use the Travelling salesman problem solver, First create an input txt file, which contains in the first line the number of cities, n,  in the problem, and the next n lines contains the x and y co-ordinates of the respective city.

So the input format is:

n
x1 y1
x2 y2
.
.
.
xn yn

Then drag and drop the file into the benchmark drop box, and after some processing the website will allow you to download the 'output_tsp.txt'. After downloading the file, drag and drop the output_tsp.txt file downloaded into the solution drop box. You will now be able to see the graph.

The output_tsp.txt contains length of total hamiltonian path, then 1 if our solution is proved to be mathematically optimal and 0 otherwise, and then the order in which the salesman should go to the cities(0-indexed)


Now, For the knapsack problem solver. First create an input txt file, input contains n + 1 lines. The first line contains two integers, the first is the number
of items in the problem, n. The second number is the capacity of the knapsack, K. The remaining
lines present the data for each of the items. Each line, i ∈ 0 . . . n − 1 contains two integers, the
item’s value v_i followed by its weight w_i.

so the input format is: 

n K
v_0 w_0
v_1 w_1
.
.
.
v_n-1 w_n-1

Then drag and drop the file into the benchmark drop box, and after some processing the website will allow you to download the 'output_sack.txt'.

The output contains a knapsack solution and is made of two lines. The first line contains two values
obj and opt. obj is the total value of the items selected to go into the knapsack (i.e. the objective
value). opt should be 1 if our algorithm proved optimality and 0 otherwise. The next line is a list
of n 0/1-values, one for each of the x_i variables(i.e. x_i is 1 if ith item is to be taken into knapsack and 0 otherwise).

So the output format is

obj opt
x_0 x_1 ..... x_n-1


For the gavel graph creator, 
First create an input txt file, input contains n lines, each line is of the format:
v1 v2 w

where v1 is name of vertex 1 (can be any string), v2 is name of vertex 2, and w is weight, and there is a directed edge from v1 to v2 of weight w.

After processing, user can download a graph in png format of the graph formed. This graph can be used to visualize the gavel problem discussed in the Gavel notes.