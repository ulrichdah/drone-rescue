import sys
import os
import numpy as np
import matplotlib.pyplot as plt
import matplotlib
import math
import glob
from statistics import stdev, variance, mean

result_types = ["_10_random.txt", "_15_random.txt", "_20_random.txt", "_10_belief.txt", "_15_belief.txt", "_20_belief.txt"]
TARGET_ID = "target_"
RELAY_ID = "relay_"

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

def get_target_relay_results(area_size):
    cur_dir = os.getcwd()
    file_list = os.listdir(cur_dir)

    check_all_files_exist(TARGET_ID + area_size, file_list)
    results = {}
    relay_results = {}
    for result_type in result_types:
        results[result_type] = []
        relay_results[result_type] = []
        relay_lines = []
        with open(RELAY_ID + area_size + result_type) as file:
            for line in file:
                relay_lines.append(line)
        
        with open(TARGET_ID + area_size + result_type) as file:
            i = 0
            for line in file:
                target_step = int(line.rstrip().split(',')[0])
                results[result_type].append(target_step)
                acc = []
                # Start with second line
                i = i + 1
                relay_line = relay_lines[i].rstrip()
                while relay_line != '----':
                    relay_step = int(relay_line.split(',')[0])
                    acc.append(relay_step - target_step)
                    i = i + 1
                    if i >= len(relay_lines):
                        break
                    relay_line = relay_lines[i].rstrip()
                if len(acc) == 0:
                    relay_results[result_type].append(0)
                else:
                    relay_results[result_type].append(max(acc) if max(acc) > 0 else mean(relay_results[result_type]))
    
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
            e_random.append(stdev(values) if stdev(values) < mean(values) else mean(values))
        elif "belief" in result_type:
            y_belief.append(mean(values))
            x_belief.append(int(result_type[1:3]))
            e_belief.append(stdev(values) if stdev(values) < mean(values) else mean(values))
    
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
    results = {}
    results["x_target_rand"] = x_random
    results["x_target_belief"] = x_belief
    results["y_target_rand"] = y_random
    results["y_target_belief"] = y_belief
    results["e_target_rand"] = e_random
    results["e_target_belief"] = e_belief

    results["x_relay_rand"] = x_random_relay
    results["x_relay_belief"] = x_belief_relay
    results["y_relay_rand"] = y_random_relay
    results["y_relay_belief"] = y_belief_relay
    results["e_relay_rand"] = e_random_relay
    results["e_relay_belief"] = e_belief_relay
    return results

    # # TODO: ADD PLOT FOR RELAY
    # x_r = np.arange(len(x_random_relay))
    # x_b = np.arange(len(x_belief_relay))
    # width = 0.25  # the width of the bars

    # fig, ax = plt.subplots()
    # rects1 = ax.bar(x_r - width/2 - 0.025, y_random_relay, width, yerr=e_random_relay, capsize=2.5, align='center', alpha=0.8, label='random')
    # rects2 = ax.bar(x_b + width/2 + 0.025, y_belief_relay, width, yerr=e_belief_relay, capsize=2.5, align='center', alpha=0.8, label='belief')

    # ax.set_ylabel('Number of steps')
    # ax.set_xlabel('Number of drones')
    # # ax.set_title('Scores by group and gender')
    # ax.set_xticks(x_r)
    # ax.set_xticklabels(x_random_relay)
    # ax.legend()

    # fig.tight_layout()
    # plt.grid(axis = 'y', linestyle = '--', linewidth = 0.5)
    # # plt.savefig(sys.argv[2] + ".png")
    # # plt.show()

    # x_r = np.arange(len(x_random))
    # x_b = np.arange(len(x_belief))
    # width = 0.25  # the width of the bars

    # fig, ax = plt.subplots()
    # rects1 = ax.bar(x_r - width/2 - 0.025, y_random, width, yerr=e_random, capsize=2.5, align='center', alpha=0.8, label='random')
    # rects2 = ax.bar(x_b + width/2 + 0.025, y_belief, width, yerr=e_belief, capsize=2.5, align='center', alpha=0.8, label='belief')

    # ax.set_ylabel('Number of steps')
    # ax.set_xlabel('Number of drones')
    # # ax.set_title('Scores by group and gender')
    # ax.set_xticks(x_r)
    # ax.set_xticklabels(x_random)
    # ax.legend()

    # fig.tight_layout()
    # plt.grid(axis = 'y', linestyle = '--', linewidth = 0.5)
    # # plt.savefig(sys.argv[1] + ".png")
    # # plt.show()

def plot(res, ax, title, is_target):
    plot_type = "target" if is_target else "relay"
    x_rand = np.arange(len(res["x_" + plot_type + "_rand"]))
    x_belief = np.arange(len(res["x_" + plot_type + "_belief"]))
    width = 0.25  # the width of the bars

    if is_target:
        ax.bar(x_rand - width/2 - 0.025, res["y_" + plot_type + "_rand"], width, yerr=res["e_" + plot_type + "_rand"], capsize=2.5, align='center', alpha=0.8, label='random')
        ax.bar(x_belief + width/2 + 0.025, res["y_" + plot_type + "_belief"], width, yerr=res["e_" + plot_type + "_belief"], capsize=2.5, align='center', alpha=0.8, label='belief')
    else:
        ax.bar(x_rand - width/2 - 0.025, res["y_" + plot_type + "_rand"], width, yerr=res["e_" + plot_type + "_rand"], capsize=2.5, align='center', alpha=0.7, label='random', color="green")
        ax.bar(x_belief + width/2 + 0.025, res["y_" + plot_type + "_belief"], width, yerr=res["e_" + plot_type + "_belief"], capsize=2.5, align='center', alpha=0.7, label='belief', color="red")

    ax.set_ylabel('Number of steps')
    ax.set_xlabel('Number of drones')
    ax.set_title(title)
    ax.set_xticks(x_rand)
    ax.set_xticklabels(res["x_" + plot_type + "_rand"])
    ax.legend()
    ax.grid(axis = 'y', linestyle = '--', linewidth = 0.5)

if __name__ == '__main__':
    # if sys.argv[1] == "-h":
    #     print_usage()
    # if len(sys.argv) != 3:
    #     print("Error: Wrong arguments!")
    #     print_usage()
    results_2020 = get_target_relay_results("2020")
    results_3030 = get_target_relay_results("3030")
    results_4040 = get_target_relay_results("4040")
    all = [results_2020, results_3030, results_4040]
    titles = ["20 x 20", "30 x 30", "40 x 40"]
    
    fig_t, (ax1_target, ax2_target, ax3_target) = plt.subplots(1, 3)
    ax_t = [ax1_target, ax2_target, ax3_target]

    for i in range(len(all)):
        plot(all[i], ax_t[i], titles[i], True)

    fig_t.tight_layout()
    # plt.grid(axis = 'y', linestyle = '--', linewidth = 0.5)
    plt.savefig("target" + ".png")

    fig_r, (ax1_relay, ax2_relay, ax3_relay) = plt.subplots(1, 3)
    ax_r = [ax1_relay, ax2_relay, ax3_relay]

    for i in range(len(all)):
        plot(all[i], ax_r[i], titles[i], False)

    fig_r.tight_layout()
    plt.savefig("relay" + ".png")

    # plt.grid(axis = 'y', linestyle = '--', linewidth = 0.5)
    # plt.show()
    
