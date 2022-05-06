#!/usr/bin/env python3
import re
import argparse
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import os
import time

# global plot style
mpl.rcParams['lines.linewidth'] = 0.8


def get_timestamp():
    """Get formatted time string

    Returns:
        str: Formatted time string
    """
    return time.strftime('%Y-%m-%d_%Hh%Mm%Ss', time.gmtime(time.time()))


def parse_cyclictest_file(datafile: str):
    """Load data from text file containing contents of output from `cyclictest`

    Args:
        datafile (str): filename of text containing output from `cyclictest`

    Returns:
        dict: dictionary of latency info with the following structure:
              {
                  'latency':
              }
    """

    # return data dict
    data = {}

    # load data
    data_matrix = np.loadtxt(datafile, dtype=np.int32,
                             comments='#', skiprows=1)
    data['latency'] = data_matrix[:, 0]
    data['samples'] = data_matrix[:, 1:]

    with open(datafile, 'r') as fp:
        line = fp.readline()
        while line:
            latencies_result = re.match(
                '# ([A-Za-z]+) Latencies: ([0-9 ]+)', line)
            if latencies_result:
                key = latencies_result.groups()[0]
                values = latencies_result.groups()[1]
                data[key.lower()] = [int(x) for x in values.split(' ')]
            total_result = re.match('# Total: ([0-9 ]+)', line)
            if total_result:
                values = total_result.groups()[0]
                data['total'] = [int(x) for x in values.split(' ')]
            overflows_result = re.match('# Histogram Overflows: ([0-9 ]+)', line)
            if overflows_result:
                values = overflows_result.groups()[0]
                data['overflows'] = [int(x) for x in values.split(' ')]
            line = fp.readline()

    return data


def plot_latency_histogram(datafile: str, output: str, username: str, width=7, height=5, dpi=200):
    # parse data
    data = parse_cyclictest_file(datafile)
    # plot
    fig, ax = plt.subplots(1, 1)
    for cpu in range(data['samples'].shape[1]):
        ax.step(data['latency'], data['samples']
                [:, cpu], label='CPU {}'.format(cpu))
    ax.set_yscale('log')
    ax.set_title('Cyclictest Latency')
    ax.set_ylabel('Number of samples')
    ax.set_xlabel('Latency (us)')
    ax.legend()
    ax.grid(True)
    # filename
    fname = os.path.basename(datafile)
    topstr = fname + '\n' + get_timestamp()
    ax.text(0.5, 0.97, topstr, transform=ax.transAxes, fontsize=8,
            verticalalignment='top', horizontalalignment='center')
    # info
    infostr = ['Cycles:', '{}'.format(data['total'][0] + data['overflows'][0])]
    infostr.extend(['', 'Min (us)'])
    for cpu in range(data['samples'].shape[1]):
        infostr.append('CPU {}: {}'.format(cpu, data['min'][cpu]))
    infostr.extend(['', 'Avg (us)'])
    for cpu in range(data['samples'].shape[1]):
        infostr.append('CPU {}: {}'.format(cpu, data['avg'][cpu]))
    infostr.extend(['', 'Max (us)'])
    for cpu in range(data['samples'].shape[1]):
        infostr.append('CPU {}: {}'.format(cpu, data['max'][cpu]))
    if username is not None:
        infostr.extend(['', 'User:', '{}'.format(username)])

    # place info text
    infostr = '\n'.join(infostr)
    ax.text(1.03, 0.97, infostr, transform=ax.transAxes, fontsize=8,
            verticalalignment='top')

    # save
    if output is None:
        output = os.path.splitext(datafile)[0] + '.png'
    fig.set_size_inches(width, height)
    fig.tight_layout()
    fig.savefig(output, bbox_inches='tight', dpi=dpi)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        prog='plot_cyclictest.py',
        description='Plot histogram data from cyclictest'
    )
    parser.add_argument('--datafile',
                        type=str,
                        help='<Required> Text file containing output from cyclictest',
                        required=True)
    parser.add_argument('--output',
                        type=str,
                        help='Image filename for histogram plot',
                        default=None)
    parser.add_argument('--user',
                        type=str,
                        help='Username',
                        default=None)
    args = parser.parse_args()

    plot_latency_histogram(args.datafile, args.output, args.user)
    plt.show()
