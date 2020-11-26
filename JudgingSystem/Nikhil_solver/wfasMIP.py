


import math
from ortools.sat.python import cp_model
from ortools.linear_solver import pywraplp
import sys
from random import randrange as rr 

teamIndex = {}
teamID = 0
MAX = 23
cost = [ [0 for _ in range(MAX) ] for __ in range(MAX) ]


def doMIP():

    model = cp_model.CpModel()
    y = {}

    # prepare decision variables
    for i in range(teamID):
        t = {}
        for j in range(i+1, teamID):
            t[j] = model.NewBoolVar('y[%i,%i]' % (i, j))
        y[i] = t 
    # print(y)

    # triangle inequality
    for i in range(teamID):
    	for j in range(i+1, teamID):
    		for k in range(j+1, teamID):
    			model.Add(y[i][j] + y[j][k] - y[i][k] <= 1)
    			model.Add(y[i][k] <= y[i][j] + y[j][k])

    objective_terms = []
    for j in range(teamID):
    	for k in range(teamID):
    		if k < j :
    			objective_terms.append(cost[k][j] * y[k][j])
    		elif k > j :
    			objective_terms.append(cost[k][j] * (1 - y[j][k]) )

    model.Minimize(sum(objective_terms))

    solver = cp_model.CpSolver()
    status = solver.Solve(model)

    if status == cp_model.OPTIMAL or status == cp_model.FEASIBLE:
        print('Total cost = %d' % solver.ObjectiveValue())
        print()

        # assign = [-1]*m 
        # for i in range(n):
        #     for j in range(m):
        #         if solver.BooleanValue(y[i][j]):
        #             if assign[j] >= 0:
        #                 print("Error in modeling!!")
        #             assign[j] = i 


if __name__ == '__main__':
	# f = open("input.in", "r")
	# teams = map(int, f.readline().split())
	# matches = teams * teams 
	# # print(teams, matches)
	# for line in f.readlines():
	# 	if len(line) <= 1 :
	# 		continue 
	# 	team1, team2, winner = line.split()
	# 	# print(team1, team2, winner)

	# 	if team1 not in teamIndex:
	# 		teamIndex[team1] = teamID 
	# 		teamID += 1 

	# 	if team2 not in teamIndex:
	# 		teamIndex[team2] = teamID 
	# 		teamID += 1 

	# 	team1 = teamIndex[team1]
	# 	team2 = teamIndex[team2]
	# 	winner = teamIndex[winner] 
	# 	if team1 == winner:
	# 		cost[team1][team2] += 1 

	# 	else :
	# 		cost[team2][team1] += 1 

	# print(cost)
	# print(teamID)
	# teamID = MAX
	# for i in range(teamID):
	# 	for j in range(teamID):
	# 		if rr(1, 7) == 2 and i != j :
	# 			cost[i][j] = rr(1, 100)

	# list1 = []
	# for i in range(teamID):
	# 	list1.append(' '.join(map(str, cost[i])))
	# matrix = '\n'.join(list1)

	# print(matrix)
	# f = open("input.in", "w")
	# f.write(str(teamID) + "\n")
	# f.write(matrix)
	# f.close()
	teamID = int(input())
	cost = [[0 for _ in range(teamID)] for __ in range(teamID)] 
	for i in range(teamID):
		cost[i] = list(map(int, raw_input().split()))
	doMIP()