import sys
import os
import numpy as np
import matplotlib.pyplot as plt
import matplotlib
import math
import glob
from statistics import stdev, variance, mean
import pandas as pd
import seaborn as sns
from collections import OrderedDict
#sns.set(style="darkgrid")
#sns.set(style="whitegrid")
#sns.set_style("white")
sns.set(style="whitegrid",font_scale=2)
import matplotlib.collections as clt
import ptitprince as pt

result_types = ["_15_random.txt", "_20_random.txt", "_25_random.txt", "_15_belief.txt", "_20_belief.txt", "_25_belief.txt"]
TARGET_ID = "target_"
RELAY_ID = "relay_"
DATASIZE_ID = "datasize_"

def print_usage():
    print("")
    print("This srcipt uses result files in the same directory as this one and produces \n \
     - RESULT TYPE 0: 2 images: one for the target find time and one for the relay establishment time. The files should be named\
     like: relay_2020_15_random.txt and target_2020_15_random.txt\n \
     - RESULT TYPE 1: an image of the bandwidth evolution for one experiment. The file should be named for example datasize_3030_25_random.txt.\
     in that case the script is called with the file id: 3030_25_random\n \
     - RESULT TYPE 2: an image for the field tests. The tests id should also be given: The files should be named target_<TEST_ID>_random.txt, etc.")
    print("")
    print("Usage: python3 results_summarize <RESULT_TYPE> <TEST_ID>")
    print("<TEST_ID>: is only needed for result type 1 and 2.")
    exit()

def check_all_files_exist(experiment_ID, file_list):
    for result_type in result_types:
        if experiment_ID + result_type not in file_list:
            print(experiment_ID + result_type + "do not exist!")
            exit()

def get_target_relay_results_from_id(test_id):
    cur_dir = os.getcwd()
    file_list = os.listdir(cur_dir)
    file_ids = [test_id + "_random.txt", test_id + "_belief.txt"]
    for f_id in file_ids:
        if TARGET_ID + f_id not in file_list or RELAY_ID + f_id not in file_list:
            print(f_id + " is missing!")
            print_usage()
    
    results = {}
    relay_results = {}
    for f_id in file_ids:
        results[f_id] = []
        relay_results[f_id] = []
        relay_lines = []
        with open(RELAY_ID + f_id) as file:
            for line in file:
                relay_lines.append(line)
        
        with open(TARGET_ID + f_id) as file:
            i = 0
            for line in file:
                target_step = int(line.rstrip().split(',')[0])
                results[f_id].append(target_step)
                # Start with second line
                i = i + 1
                acc = []
                relay_line = relay_lines[i].rstrip()
                while relay_line != '----':
                    relay_step = int(relay_line.split(',')[0])
                    acc.append(relay_step)
                    i = i + 1
                    if i >= len(relay_lines):
                        break
                    relay_line = relay_lines[i].rstrip()
                relay_results[f_id].append(max(acc) - target_step)
    x_random = []
    y_random = []
    x_belief = []
    y_belief = []
    for f_id,values in results.items():
        if "random" in f_id:
            for i, v in enumerate(values):
                y_random.append(v)
                x_random.append(i + 1)
        elif "belief" in f_id:
            for i, v in enumerate(values):
                y_belief.append(v)
                x_belief.append(i + 1)
    
    x_random_relay = []
    y_random_relay = []
    x_belief_relay = []
    y_belief_relay = []
    for f_id,values in relay_results.items():
        if "random" in f_id:
            for i, v in enumerate(values):
                y_random_relay.append(v)
                x_random_relay.append(i + 1)
        elif "belief" in f_id:
            for i, v in enumerate(values):
                y_belief_relay.append(v)
                x_belief_relay.append(i + 1)
    results = {}
    results["x_target_rand"] = x_random
    results["x_target_belief"] = x_belief
    results["y_target_rand"] = y_random
    results["y_target_belief"] = y_belief

    results["x_relay_rand"] = x_random_relay
    results["x_relay_belief"] = x_belief_relay
    results["y_relay_rand"] = y_random_relay
    results["y_relay_belief"] = y_belief_relay
    return results


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
                    acc.append(relay_step)
                    i = i + 1
                    if i >= len(relay_lines):
                        break
                    relay_line = relay_lines[i].rstrip()
                # if len(acc) == 0:
                #     relay_results[result_type].append(0)
                # else:
                relay_results[result_type].append(sum(acc))
    
    x_random = []
    y_random = []
    e_random = []
    x_belief = []
    y_belief = []
    e_belief = []
    data = {"Number of steps": [], "Number of robots": [], "Search method": []}
    for result_type,values in results.items():
        if "random" in result_type:
            data["Number of steps"] += values
            data["Search method"] += (["random"] * 30)
            data["Number of robots"] += ([result_type[1:3]] * 30)
            y_random.append(mean(values))
            x_random.append(int(result_type[1:3]))
            e_random.append(stdev(values) if stdev(values) < mean(values) else mean(values))
        elif "belief" in result_type:
            data["Number of steps"] += values
            data["Search method"] += (["belief"] * 30)
            data["Number of robots"] += ([result_type[1:3]] * 30)
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
            # data["Number of steps"] += values
            # data["Search method"] += (["random"] * 30)
            # data["Number of robots"] += ([result_type[1:3]] * 30)
            y_random_relay.append(mean(values))
            x_random_relay.append(int(result_type[1:3]))
            e_random_relay.append(stdev(values))
        elif "belief" in result_type:
            # data["Number of steps"] += values
            # data["Search method"] += (["belief"] * 30)
            # data["Number of robots"] += ([result_type[1:3]] * 30)
            y_belief_relay.append(mean(values))
            x_belief_relay.append(int(result_type[1:3]))
            e_belief_relay.append(stdev(values))
    results = {}
    results["df"] = pd.DataFrame(data)
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
    ax.set_title(title, fontweight='bold')
    ax.set_xticks(x_rand)
    ax.set_xticklabels(res["x_" + plot_type + "_rand"])
    ax.legend()
    ax.grid(axis = 'y', linestyle = '--', linewidth = 0.5)

def get_datasize_results(result_id):
    cur_dir = os.getcwd()
    file_list = os.listdir(cur_dir)
    if DATASIZE_ID + result_id + ".txt" not in file_list:
            print(DATASIZE_ID + result_id + " do not exist!")
            exit()
    datasize_results = {}
    stop = False
    with open(DATASIZE_ID + result_id + ".txt") as file:
            for line in file:
                if "---" in line.rstrip() and stop:
                    break
                elif "---" in line.rstrip():
                    stop = True
                    continue
                
                step = int(line.rstrip().split(',')[0])
                bandwidth = int(line.rstrip().split(',')[1])
                r_id = line.rstrip().split(',')[2]
                if r_id not in datasize_results:
                    datasize_results[r_id] = {}
                datasize_results[r_id][step] = bandwidth
    return datasize_results

def plot_datasize(result_id):
    datasize_results_random = get_datasize_results(result_id + "_random")
    datasize_results_belief = get_datasize_results(result_id + "_belief")

    futur_root = -1
    min_len = float("inf")
    for r_id in datasize_results_random:
        if len(datasize_results_random[r_id]) < min_len:
            min_len = len(datasize_results_random[r_id])
            futur_root = r_id
    datasize_results_random.pop(futur_root)

    futur_root = -1
    min_len = float("inf")
    for r_id in datasize_results_belief:
        if len(datasize_results_belief[r_id]) < min_len:
            min_len = len(datasize_results_belief[r_id])
            futur_root = r_id
    datasize_results_belief.pop(futur_root)

    f, ax_b = plt.subplots()
    # for r_id in datasize_results_random:
    #     ax_r.plot(np.array(list(datasize_results_random[r_id].keys())), np.array(list(datasize_results_random[r_id].values())), label=r_id)
    #     ax_r.legend(loc='lower right')
    #     ax_r.set_ylabel('Size of Buzz message queue (in bytes)')
    #     ax_r.set_xlabel('Number of steps')
    #     ax_r.set_title("Random", fontweight='bold')
    
    for r_id in datasize_results_belief:
        ax_b.plot(np.array(list(datasize_results_belief[r_id].keys())), np.array(list(datasize_results_belief[r_id].values())), label=r_id)
        ax_b.legend(loc='lower right')
        ax_b.set_ylabel('Size of Buzz message queue (in bytes)')
        ax_b.set_xlabel('Number of steps')
        # ax_b.set_title("Belief", fontweight='bold')
        # ax.set_ylim(ymin=0, ymax=500)
        # ax.set_xlim(xmin=0)
    # plt.legend()
    plt.legend(prop={'size': 12})
    plt.show()

def plot_field_results(results, ax, title, is_target):
    plot_type = "target" if is_target else "relay"

    ax.plot(np.array(results["x_" + plot_type + "_belief"]), np.array(results["y_" + plot_type + "_belief"]), marker = 'o' if is_target else 'X', color = 'b' if is_target else 'r')
    ax.set_ylabel('Number of steps')
    ax.set_xlabel('Experiment number')
    ax.set_title(title, fontweight='bold')

if __name__ == '__main__':
    if len(sys.argv) == 2 and sys.argv[1] == "-h":
        print_usage()
    if len(sys.argv) < 2 or len(sys.argv) > 3:
        print("Error: Wrong arguments!")
        print_usage()

    # 0: Simulation target and relay results (saved to 2 different files: relay.png and target.png); 
    # 1: Simulation bandwidth plot (not saved); 
    # 2: field tests results 
    wanted_results = 1
    
    if wanted_results == 0:
        results_2020 = get_target_relay_results("2020")
        results_3030 = get_target_relay_results("3030")
        results_4040 = get_target_relay_results("4040")
        all = [results_2020]#, results_3030, results_4040]
        titles = ["20m x 20m", "30m x 30m", "40m x 40m"]
        # plt.rcParams['font.size'] = '8'
        # fig_t, (ax1_target, ax2_target, ax3_target) = plt.subplots(1, 3)
        # ax_t = [ax1_target, ax2_target, ax3_target]
        
        # for i in range(len(all)):
        #     plot(all[i], ax_t[i], titles[i], True)

        # fig_t.tight_layout()
        # # plt.grid(axis = 'y', linestyle = '--', linewidth = 0.5)
        # plt.savefig("target" + ".png")

        # fig_r, (ax1_relay, ax2_relay, ax3_relay) = plt.subplots(1, 3)
        # ax_r = [ax1_relay, ax2_relay, ax3_relay]

        # for i in range(len(all)):
        #     plot(all[i], ax_r[i], titles[i], False)

        # fig_r.tight_layout()
        # plt.savefig("relay" + ".png")

        # # plt.grid(axis = 'y', linestyle = '--', linewidth = 0.5)
        # # plt.show()
        # belief = []
        # for i in len(results_2020["y_target_belief"]):
        # t = rr["df"].type
        # s = rr["df"].step
        # sns.violinplot(x = t, y = s, data=rr["df"])
        # sns.swarmplot(x=t, y=s, data=rr["df"], color="k", alpha=0.8)
        f, ax_1 = plt.subplots()
        axs = [ax_1]
        dy="Number of steps"
        dx="Number of robots"
        ort="v"
        pal = "Set2"
        dhue = "Search method"
        sigma = .2
        for i in range(len(all)):
            axs[i] = pt.RainCloud(x = dx, y = dy, hue = dhue, data = all[i]["df"], palette = pal, bw = sigma,
                    width_viol = .7, ax = axs[i], orient = ort, alpha = .65, dodge = True)
            axs[i].set_title(titles[0], fontweight='bold')
        # plt.legend(bbox_to_anchor=(0.5, 1), loc='upper right', borderaxespad=0)
        handles, labels = plt.gca().get_legend_handles_labels()
        by_label = OrderedDict(zip(labels, handles))
        plt.legend(by_label.values(), by_label.keys(), loc=1, prop={'size': 6})
        # plt.legend(loc=2, prop={'size': 6})
        plt.savefig('raincloud_target_20_n.png', bbox_inches='tight',dpi=100)
        plt.show()
        
    elif wanted_results == 1:
        plot_datasize(sys.argv[2])
    elif wanted_results == 2:
        results = get_target_relay_results_from_id(sys.argv[2])
        fig_t, (ax_target, ax_relay) = plt.subplots(1, 2)
        plot_field_results(results, ax_target, "(a) Target discovery time", True)
        ax_target.grid(axis = 'y', linestyle = '--', linewidth = 0.5)
        plot_field_results(results, ax_relay, "(b) Relay formation time", False)
        ax_relay.grid(axis = 'y', linestyle = '--', linewidth = 0.5)
        plt.show()
    
