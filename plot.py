# plot.py: To plot the results of placement
import os
from pathlib import Path
import matplotlib.pyplot as plt
import matplotlib.patches as patches

def plot_rec(ax, x1, y1, x2, y2):
    print('draw: ( ' + str(x1) + ' , ' + str(y1) + ') , w =' + str(x2 - x1) + ' , h = ' + str(y2-y1))
    ax.add_patch(
        patches.Rectangle(
            (x1, y1),
            (x2 - x2),
            (y2 - y1),
            fill = True,
        )
    )

def readfile_and_plot(fileName):
    file = open(fileName, 'r')
    testcaseName = Path(fileName).stem
    
    fig, ax = plt.subplots()
    ax.set_title('Case: ' + testcaseName)
    ax.set_xlabel('x-axis')
    ax.set_ylabel('y-axis')
    ax.grid(which = 'major', axis = 'both')
    plt.rc('font', size = 7)

    #ax.plot([0], [0])  # grid: dynamic change with chipsize
    ax.plot(25, 25)     # grid: set as fixed x = 0~25 ; y = 0~25

    # skip final cost
    file.readline()
    # skip total wirelength
    file.readline()
    # skip chip area
    file.readline()
    # chip width & chip height
    chipW, chipH = file.readline().split()
    chipW = float(chipW)
    chipH = float(chipH)

    ax.plot(chipW, chipH, 'g*')     # tap green star on chip's top right corner
    plt.text(chipW, chipH, '({}, {})'.format(chipW, chipH))

    # skip program run time
    file.readline()

    # block name, x1, y1, x2, y2
    x_list = []
    y_list = []
    for thisLine in list(file):
        name, x1, y1, x2, y2 = thisLine.split()
        x1 = float(x1)
        y1 = float(y1)
        x2 = float(x2)
        y2 = float(y2)
        
        x_list.extend([x1])
        y_list.extend([y1])
        w = x2 - x1
        h = y2 - y1
        centerX = x1 + w / 2.0
        centerY = y1 + h / 2.0
        rect = patches.Rectangle((x1,y1), w, h, edgecolor = 'black', fill = True, facecolor = 'green', alpha = 0.5)
        nameTag = name + '\n' + str(w) + ', ' + str(h)
        ax.annotate(nameTag, (centerX, centerY), color = 'white', ha = 'center', va = 'center')
        ax.add_patch(rect)

    ax.plot(x_list, y_list, 'r*')   # tap red star on each block's left bottom corner
    for x, y in zip(x_list, y_list):
        plt.text(x, y, '({},{})'.format(x,y))

    # save figures
    plotName = './figures/' + testcaseName + '.png'
    plt.savefig(plotName)
    print('[Finished] Plot ' + plotName)

def main():
    directory = 'results'
    for fileName in os.listdir(directory):
        file = os.path.join(directory, fileName)
        
        if(os.path.isfile(file)):
            print("Current File: " + file)
            readfile_and_plot(file)

def __init__(self):
    print("Enter plot.py __init__()")

if __name__ == '__main__': main()