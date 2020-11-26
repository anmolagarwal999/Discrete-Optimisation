from flask import (Flask, request, jsonify, render_template)
from constraint import *
import re

app = Flask(__name__)

def dataNormalize (data):
    """
    args: data; output from sudoku_solve()
    returns: Normalized output of input - Diplayed at console
    """
    ans=""
    sudoku_nums = [ eachPos[1] for eachPos in sorted( data[0].items() ) ]
    for step in range(0,81,9):
        print(sudoku_nums[step:step+9])
        ans = ans+str(sudoku_nums[step:step+9])+"<br>"
    #for step in range(0,81,9):
    return ans

# Solves the sudoku puzzle
def sudoku_solve (inp):
    """
    args: None
    returns: Sudoku solution
    return type: list
    description:
        * Creates problem instance, sudoku = Problem()
        * Adds sudoku input and their indices as variables
        * Adds constraints to the problem
            1. No two numbers in a row should be same
            2. No two numbers in a column should be same
            3. No two numbers in a 3x3 box shoud be same
        * returns the solution as a list
    """

    puzzleNums = "9 0 5 0 0 0 0 0 8\n4 0 0 5 7 0 1 0 6\n0 2 7 6 0 0 0 4 0\n0 9 6 0 0 3 5 1 2\n7 0 4 0 1 0 3 0 0\n2 1 0 9 8 0 0 0 4\n0 8 1 0 0 4 0 9 0\n3 0 0 8 0 0 0 5 1\n0 0 2 0 0 7 0 6 0"
    inplist = re.split(' |\n', inp)
    print(len(inplist))
    if len(inplist) == 81:
        puzzleNums = inp
    print("ok")
    print(puzzleNums)
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

@app.route('/')
def index():
    return render_template('Constraint Programming.html')
        
        
@app.route('/ajax', methods = ['POST'])
def ajax_request():
    inp = request.form['inp']
    print(inp)
    print(sudoku_solve(inp))
    return jsonify(username=dataNormalize( sudoku_solve(inp) ))
    
    
if __name__ == "__main__":
    app.run(debug = True)