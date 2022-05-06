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


def parse_data_file(datafile: str, dims=1, dtype='f8') -> np.ndarray:
    """Load data from a binary data file

    Args:
        datafile (str): filename of binary data

    Returns:
        np.ndarray:
    """

    with open(datafile, 'rb') as fid:
        data_array = np.fromfile(fid, dtype=dtype)
    if dims > 1:
        return data_array.reshape((-1, dims)).T
    else:
        return data_array


def plot_timing(datafile: str, output: str, username: str, step_size: int, width=7, height=7, dpi=200):

    timevec = parse_data_file(datafile, dtype='u4')
    jitter = timevec[1:] - timevec[:-1]

    min_jitter = np.min(jitter) - step_size
    max_jitter = np.max(jitter) - step_size
    stdev_jitter = np.std(jitter)
    mean_jitter = np.average(jitter)

    num_points = timevec.size
    duration = num_points * step_size
    abstime_desired = np.linspace(step_size, duration, num=num_points)
    abstime_actual = timevec
    abstime_error = abstime_actual - abstime_desired

    time_seconds = timevec * 1.0e-6

    # plot
    fig, axs = plt.subplots(2, 1)
    ax0 = axs[0]
    ax1 = axs[1]

    ax0.plot(time_seconds[1:], jitter)
    ax0.set_title('Jitter')
    ax0.set_ylabel('Jitter (us)')
    ax0.set_xlabel('Time (s)')
    ax0.grid(True)

    ax1.plot(time_seconds, abstime_error)
    ax1.set_title('Absolute Error (Latency)')
    ax1.set_ylabel('Latency (us)')
    ax1.set_xlabel('Time (s)')
    ax1.grid(True)

    # filename
    fname = os.path.basename(datafile)
    topstr = fname + '\n' + get_timestamp()
    ax0.text(0.5, 0.97, topstr, transform=ax0.transAxes, fontsize=8,
             verticalalignment='top', horizontalalignment='center')

    # info
    infostr = []
    infostr.extend(['Step size (us)', '{}'.format(step_size)])
    infostr.extend(['', 'Min (us)'])
    infostr.append('{} + {}'.format(min_jitter, step_size))
    infostr.extend(['', 'Max (us)'])
    infostr.append('{} + {}'.format(max_jitter, step_size))
    infostr.extend(['', 'Mean, Stdev (us)'])
    infostr.append('{:.2f}, {:.2f}'.format(mean_jitter, stdev_jitter))
    if username is not None:
        infostr.extend(['', 'User:', '{}'.format(username)])
    # place info text
    infostr = '\n'.join(infostr)
    ax0.text(1.03, 0.97, infostr, transform=ax0.transAxes, fontsize=8,
             verticalalignment='top')

    # save
    if output is None:
        output = os.path.splitext(datafile)[0] + '.png'
    fig.set_size_inches(width, height)
    fig.tight_layout()
    fig.savefig(output, bbox_inches='tight', dpi=dpi)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        prog='plot_jitter.py',
        description='Plot timing information from binray file'
    )
    parser.add_argument('--datafile',
                        type=str,
                        help='<Required> binary data file containing time in numpy format "u4"',
                        required=True)
    parser.add_argument('--step_size',
                        type=int,
                        help='<Required> Desired step size of timer in microseconds',
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

    plot_timing(args.datafile, args.output, args.user, step_size=args.step_size)
