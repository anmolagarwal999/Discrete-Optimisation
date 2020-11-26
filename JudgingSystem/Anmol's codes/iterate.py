import os
import pandas as pd
rootdir = './lolib_2010/'

# for subdir, dirs, files in os.walk(rootdir):
#     for file in files:
#         print(subdir)
#         # print(os.path.join(subdir, file))

path = './lolib_2010'
# The immediate file path


def solve_it(input_data_path, executable_file_path):

    # inp_file = open("input.txt", "w")
    # inp_file.write(input_data)
    # inp_file.close()

    cmd = executable_file_path + " > output.txt < " + input_data_path
    os.system(cmd)
    # print("HERE")
    # os.system('ls')
    with open('output.txt', 'r') as file:
        output_data = file.readline()
        for last_line in file:
            pass
    # process = Popen(['./a'], stdout=PIPE)
    # (stdout, stderr) = process.communicate()

    return int(output_data)
    # return 2


executable_path_list = ["./naive", "./kl2", "./chanas","./chanas_alt"]
# df = pd.DataFrame(columns=executable_path_list, index=['x','y','z'])
df = pd.read_excel('do.xlsx')
# df.set_index('file_name')

# df = pd.DataFrame(columns=executable_path_list)
print(df)

directory_contents = os.listdir(path)
for i in directory_contents:
    print("Name of folder is ", i)
    # if i != "IO":
    #     continue
    if i != "IO" and i != "SGB" and i != "MB":
        continue
    arr = os.listdir(path + "/" + i)
    for j in arr:
        input_file_path = path + "/" + i + "/" + j
        map_obj = {}
        # map_obj["Name"]=j
        send_name = input_file_path
        pos = input_file_path.rfind('/')
        input_file_path = input_file_path[pos + 1:]
        idx = df[df["file_name"] == input_file_path].index.values
        print("idx is ",idx)
        
        for exec_name in executable_path_list:
            ans = solve_it(send_name, exec_name)

            loc_got = df.columns.get_loc(exec_name)
            print("loc got is ", loc_got)
            # df.iloc[idx, loc_got] = ans
            # df.set_value(idx, exec_name, ans)
            df.at[idx, exec_name] = ans


            # map_obj[exec_name]=ans

        # print("initila is ",input_file_path)

        # do_insert=df.loc[["Avery Bradley", "R.J. Hunter"],
        #            ["Team", "Number", "Position"]]
        # do_insert=df.loc[[input_file_path],
        #            ["best"]]

        # print("it is ",df.loc[input_file_path])

        # do_insert=df.ix[input_file_path]['best']
        # map_obj["best"]=do_insert
        print(pos)
        print(input_file_path)
        print("--------------")
        # df.loc[input_file_path] = pd.Series(map_obj)

        # feed input to ./<executable-name>
        # redirect output to output.txt
        # take first token as answer and store

    print("--------------------------------")

df.to_excel("do1.xlsx")