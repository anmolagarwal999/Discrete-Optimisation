#!/usr/bin/python
# -*- coding: utf-8 -*-

import math
from collections import namedtuple
import os
from subprocess import Popen, PIPE

Point = namedtuple("Point", ['x', 'y'])

def length(point1, point2):
    return math.sqrt((point1.x - point2.x)**2 + (point1.y - point2.y)**2)

def get_name(input_data_file):
    send_str=""
    cnt=0
    for ch in input_data_file:
        if cnt ==2:
            send_str+=ch
    
        if ch=='/':
            cnt+=1
    print("send_str is ",send_str)
    return send_str
            


def solve_it(input_data,file_location):
    # Modify this code to run your optimization algorithm

    # parse the input
    # lines = input_data.split('\n')
    # print(lines)

    # nodeCount = int(lines[0])

    # points = []
    # for i in range(1, nodeCount+1):
    #     line = lines[i]
    #     parts = line.split()
    #     points.append(Point(float(parts[0]), float(parts[1])))

    # # build a trivial solution
    # # visit the nodes in the order they appear in the file
    # solution = range(0, nodeCount)

    # # calculate the length of the tour
    # obj = length(points[solution[-1]], points[solution[0]])
    # for index in range(0, nodeCount-1):
    #     obj += length(points[solution[index]], points[solution[index+1]])

    # # prepare the solution in the specified output format
    # output_data = str(obj) + ' ' + str(0) + '\n'
    # output_data += ' '.join(map(str, solution))
    # print(input_data)
    # inp_file = open("input.txt", "w")
    # inp_file.write(input_data)
    # inp_file.close()
    stripped_input_name=get_name(file_location)
    output_file_wanted="./use/use_"+stripped_input_name
    #cmd = './a < ' +file_location +" > "+output_file_wanted
   # print("cmd is ",cmd)
    #os.system(cmd)
    # print("HERE")
    # os.system('ls')
    print(output_file_wanted)
    with open(output_file_wanted,'r') as file:
        output_data= file.read()
    # process = Popen(['./a'], stdout=PIPE)
    # (stdout, stderr) = process.communicate()

    return output_data
    # return 2


import sys

if __name__ == '__main__':
    if len(sys.argv) > 1:
        file_location = sys.argv[1].strip()
        input_data_file = open(file_location, 'r')
        print("File location is ",file_location)
        input_data = ''.join(input_data_file.readlines())
        input_data_file.close()
        #exit()
        # print(input_data)
        # print(solve_it(input_data))
        # print(solve_it(input_data))
        print(solve_it(input_data,file_location))
    else:
        print('This test requires an input file.  Please select one from the data directory. (i.e. python solver.py ./data/tsp_51_1)')

