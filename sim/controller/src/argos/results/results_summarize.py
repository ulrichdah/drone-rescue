import sys
import os
import numpy as np
import matplotlib.pyplot as plt
import matplotlib
import math
import glob
from statistics import stdev, variance, mean

result_types = ["_10_random.txt", "_15_random.txt", "_20_random.txt", "_10_belief.txt", "_15_belief.txt", "_20_belief.txt"]
def print_usage():
    print("This script takes the experiment ID (ex:target_2020), find all the needed result files\
 (with each line containing \"step,drone_id\"), and will create a statistical graph.")
    print("")
    print("Usage: results_summarize <experiment_ID>")
    exit()

def check_all_files_exist(experiment_ID, file_list):
    for result_type in result_types:
        if experiment_ID + result_type not in file_list:
            print(experiment_ID + result_type + "do not exist!")
            exit()

if __name__ == '__main__':
    if sys.argv[1] == "-h":
        print_usage()
    if len(sys.argv) != 3:
        print("Error: Wrong arguments!")
        print_usage()
    
    cur_dir = os.getcwd()
    file_list = os.listdir(cur_dir)

    check_all_files_exist(sys.argv[1], file_list)
    results = {}
    relay_results = {}
    for result_type in result_types:
        results[result_type] = []
        relay_results[result_type] = []
        relay_lines = []
        with open(sys.argv[2] + result_type) as file:
            for line in file:
                relay_lines.append(line)
        
        with open(sys.argv[1] + result_type) as file:
            i = 0
            for line in file:
                target_step = int(line.rstrip().split(',')[0])
                results[result_type].append(target_step)
                acc = []
                # Start with second line
                i = i + 1
                relay_line = relay_lines[i]
                while relay_line != '----':
                    relay_step = int(line.rstrip().split(',')[0])
                    acc.append(relay_step - target_step)
                    i = i + 1
                    relay_line = relay_lines[i]
                if len(acc) == 0:
                    relay_results[result_type].append(0)
                else:
                    relay_results[result_type].append(max(acc))
    
    x_random = []
    y_random = []
    e_random = []
    x_belief = []
    y_belief = []
    e_belief = []
    for result_type,values in results.items():
        if "random" in result_type:
            y_random.append(mean(values))
            x_random.append(int(result_type[1:3]))
            e_random.append(stdev(values))
        elif "belief" in result_type:
            y_belief.append(mean(values))
            x_belief.append(int(result_type[1:3]))
            e_belief.append(stdev(values))
    
    x_random_relay = []
    y_random_relay = []
    e_random_relay = []
    x_belief_relay = []
    y_belief_relay = []
    e_belief_relay = []
    for result_type,values in relay_results.items():
        if "random" in result_type:
            y_random_relay.append(mean(values))
            x_random_relay.append(int(result_type[1:3]))
            e_random_relay.append(stdev(values))
        elif "belief" in result_type:
            y_belief_relay.append(mean(values))
            x_belief_relay.append(int(result_type[1:3]))
            e_belief_relay.append(stdev(values))
    
    # TODO: ADD PLOT FOR RELAY
    
    x_r = np.arange(len(x_random))
    x_b = np.arange(len(x_belief))
    width = 0.25  # the width of the bars

    fig, ax = plt.subplots()
    rects1 = ax.bar(x_r - width/2 - 0.025, y_random, width, yerr=e_random, capsize=2.5, align='center', alpha=0.8, label='random')
    rects2 = ax.bar(x_b + width/2 + 0.025, y_belief, width, yerr=e_belief, capsize=2.5, align='center', alpha=0.8, label='belief')

    ax.set_ylabel('Number of steps')
    ax.set_xlabel('Number of drones')
    # ax.set_title('Scores by group and gender')
    ax.set_xticks(x_r)
    ax.set_xticklabels(x_random)
    ax.legend()

    fig.tight_layout()
    plt.grid(axis = 'y', linestyle = '--', linewidth = 0.5)
    plt.show()
    
