import os
from collections import namedtuple
from flask import *
import sys
import math 
import matplotlib.pyplot as plt
import networkx as nx
from constraint import *
import re
app = Flask(__name__)

# Function For main page
@app.route('/')
def main():
    return render_template('index.html') # Shows given html page to user
 
# Function for Local search notes site
@app.route('/local')
def local():
    return render_template('local.html')  # Shows given html page to user


# Function for Linear programming notes site
@app.route('/lp')
def lp():
    return render_template('lp.html') # Shows given html page to user

# Function for Travelling Salesman problem solver and graph maker site
@app.route('/tsp', methods=['GET','POST'])
def tsp():
    if request.method == "POST":     # When input file is dropped into box, Javascipt makes POST request to flask using JQuery ajax
        input_data = request.get_json()  # Retrieve JSON sent by Javascript in the jquery ajax request, which contains data of input file given by user
        inp_file = open("input_tsp.txt", "w") # Open the file which will contain the input for the problem
        inp_file.write(input_data['data']) # Write retrieved input data to file
        inp_file.close()
        tmp = "/tsp/succ/" + input_data['data1'] # tmp contains the next url to which we will redirect to
        return jsonify({"redirect": tmp}) # Return the next url to javascipt in JSON form, with key 'redirect', also signals success of ajax request
    else:
        return render_template('tsp.html') # Shows given html page to user

# Function for knapsack problem solver site
@app.route('/sack', methods=['GET','POST'])
def sack():
    if request.method == "POST": # When input file is dropped into box, Javascipt makes POST request to flask using JQuery ajax
        input_data = request.get_json()  # Retrieve JSON sent by Javascript in the jquery ajax request, which contains data of input file given by user
        inp_file = open("input_sack.txt", "w") # Open the file which will contain the input for the problem
        inp_file.write(input_data['data']) # Write retrieved input data to file
        inp_file.close()
        tmp = "/sack/succ/"  # tmp contains the next url to which we will redirect to
        return jsonify({"redirect": tmp}) # Return the next url to javascipt in JSON form, with key 'redirect', also signals success of ajax request

    else:
        return render_template('sack.html')  # Shows given html page to user

# Function to which TSP site will redirect to upon success of ajax request when input data in entered and written into input file
@app.route('/tsp/succ/<x>')
def succ(x):
    # x variable is used to show choice of method user wants to solve tsp problem wit
    if x == "a": 
        cmd = './a.out' # Greedy method
    elif x == "b":
        cmd = './b.out' # Simulated annealing method
    elif x == "c":
        cmd = './c.out' # Local Guided Search method
    elif x == "d":
        cmd = './d.out' # E_approx_algo_2opt method
    elif x == "e":
        cmd = './e.out' # B approx_algo method

    os.system(cmd) # Execute the given command, the c++ files use the input file to get input and write output to the output file.
    return send_file('output_tsp.txt', as_attachment=True, cache_timeout=0) # Make the user download the output file formed, without caching the output file.

# Function to which Knapsack solver site will redirect to upon success of ajax request when input data in entered and written into input file
@app.route('/sack/succ/')
def succ2():
    cmd = './sack' # Executable for the c++ code which solves Knapsack problem using input_sack.txt as input and writing output to output_sack.txt
    os.system(cmd)  # Execute the given command
    return send_file('output_sack.txt', as_attachment=True, cache_timeout=0) # Make the user download the output file formed, without caching the output file.

# Function to the Gavel graph creator
@app.route('/graph', methods=['GET','POST'])
def graph():
    if request.method == "POST": # When input file is dropped into box, Javascipt makes POST request to flask using JQuery ajax
        inp = request.get_json()['data']  # Retrieve JSON sent by Javascript in the jquery ajax request, which contains data of input file given by user
        G = nx.DiGraph() # Create A directional graph G
        inp1 = inp.split('\n') # Split input to parse it
        for x in inp1:
            tmp = x.split()
            if len(tmp) < 2: # Remove incorrect input data
                continue
            G.add_edge(tmp[0], tmp[1], weight=tmp[2])  # Add edge as given by input file
             
        pos = nx.spring_layout(G,k=10000000,iterations=20) # Positions nodes using Fruchterman-Reingold force-directed algorithm. where k is optimal distance between nodes, and iterations is number of iterations taken.
        nx.draw_networkx_nodes(G, pos) # Draw the nodes
        nx.draw_networkx_labels(G, pos) # Draw the labels on the nodes
        nx.draw_networkx_edges(G, pos) # Draw the edges
        labels = nx.get_edge_attributes(G,'weight') # Make variable for the weights of the edges
        nx.draw_networkx_edge_labels(G,pos,edge_labels=labels, font_size = 7) # Add weights to the edges.
        fig = plt.gcf() 
        fig.set_size_inches((16, 9), forward=False) # Set size of graph
        fig.savefig("graph.png", dpi=500) # Save the graph as png with given dpi
        fig.clear() # Clear graph for next use
        G.clear() # Clear graph for next use
        tmp = "/graph/succ/"  # tmp contains the next url to which we will redirect to
        return jsonify({"redirect": tmp}) # Return the next url to javascipt in JSON form, with key 'redirect', also signals success of ajax request
    else:
        return render_template('graph.html') # Shows given html page to user

# Function to which Gavel Graph creator site will redirect to upon success of ajax request when input data in entered and written into input file
@app.route('/graph/succ/')
def succ3():
    return send_file('graph.png', as_attachment=True, cache_timeout=0) # Make the user download the graph file formed, without caching the graph file.

# Function for Constraint programming notes site
@app.route('/cp')
def cp():
    return render_template('cp.html') # Shows given html page to user
        
def dataNormalize (data):
    ans=""
    sudoku_nums = [ eachPos[1] for eachPos in sorted( data[0].items() ) ]
    for step in range(0,81,9):
        # print(sudoku_nums[step:step+9])
        ans = ans+str(sudoku_nums[step:step+9])+"<br>"
    return ans

def sudoku_solve (inp):


    puzzleNums = "9 0 5 0 0 0 0 0 8\n4 0 0 5 7 0 1 0 6\n0 2 7 6 0 0 0 4 0\n0 9 6 0 0 3 5 1 2\n7 0 4 0 1 0 3 0 0\n2 1 0 9 8 0 0 0 4\n0 8 1 0 0 4 0 9 0\n3 0 0 8 0 0 0 5 1\n0 0 2 0 0 7 0 6 0"
    inplist = re.split(' |\n', inp)
    # print(len(inplist))
    if len(inplist) == 81:
        puzzleNums = inp
    # print("ok")
    # print(puzzleNums)
    # stores the numbers in a list. Ex: [1,2,9,0,3...]
    puzzleNums = [ int(eachNum) for eachNum in puzzleNums.split() ]
    # Problem instance created.
    # Recursive backtracking is used here
    sudoku = Problem( RecursiveBacktrackingSolver() )
    # List of 9x9 sudoku puzzle indices. Ex: [(0,0), (0,1).. (9,9)]
    sudokuIndex = [ (row, col) for row in range(9) for col in range(9) ]
    # adding variables to the sudoku instance
    for eachIndex,eachNum in zip(sudokuIndex, puzzleNums):
        # If empty location is found, its range is set to 1-10
        if eachNum == 0:
            sudoku.addVariable(eachIndex, range(1,10) )
        # If not an empty location, its a value is assigned
        else:
            sudoku.addVariable(eachIndex, [eachNum] )

    # constraints for each row and column
    # counting from 0-9 (numeber of rows/ columns)
    var = 0
    for aCount in range(9):
        # A list of locations present in a row.
        rowIndices = [ (var, col) for col in range(9) ]
        # Adding constraint
        # No two numbers in a row should be same
        sudoku.addConstraint( AllDifferentConstraint(), rowIndices )
        # A list of locations present in a column
        colIndices = [ (row, var) for row in range(9) ]
        # Adding constraint
        # No two numbers in a column should be same
        sudoku.addConstraint( AllDifferentConstraint(), colIndices )
        var+=1

    # constraints for each block (3x3) of board
    # Finding all boxes in sudoku board. Its 9 in this case
    rowStep = 0
    colStep = 0
    while rowStep < 9:
        colStep = 0
        while colStep < 9:
            # list of locations present in a box
            boxIndices = [ (row, col) for row in range(rowStep, rowStep+3) \
                            for col in range(colStep, colStep+3)]
            # Adding constraint
            # No two numbers in the box should be same
            sudoku.addConstraint( AllDifferentConstraint(), boxIndices )
            colStep+=3
        rowStep+=3
    # return the solution
    return sudoku.getSolutions()


@app.route('/ajax', methods = ['POST'])
def ajax_request():
    inp = request.form['inp']
    # print(inp)
    # print(sudoku_solve(inp))
    return jsonify(username=dataNormalize( sudoku_solve(inp) ))
    
# Function for Branch and Bound notes site
@app.route('/branch')
def branch():
    return render_template("branch.html")# Shows given html page to user

# Function for Travelling salesman person notes site
@app.route('/tsp_note')
def tsp_note():
    return render_template("tsp_note.html") # Shows given html page to user

# Function for gavel problem site
@app.route('/gavel')
def gavel():
    return render_template("gavel.html") # Shows given html page to user
    
# Main function
if __name__ == '__main__':    
    app.run(debug = True) # Run flask app
