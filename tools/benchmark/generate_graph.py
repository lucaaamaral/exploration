#!/usr/bin/env python3

import os
import sys
import pandas as pd
import matplotlib.pyplot as plt

def generate_graph(input_file):
    """
    Generate a line plot from the benchmark data in the input CSV file.
    Each operation is plotted as a line with run numbers on the x-axis corresponding to all available data points.
    The first row of the CSV is ignored as it may contain header information.
    The output graph is saved as a PNG file in the same directory as the input file.
    """
    # Read the CSV file, skipping the first row and not assuming headers
    try:
        df = pd.read_csv(input_file, header=None, skiprows=1)
    except Exception as e:
        print(f"Error: Failed to read {input_file}: {e}")
        sys.exit(1)

    # Prepare to plot data
    plt.figure(figsize=(14, 7))
    max_runs = 0
    for index, row in df.iterrows():
        operation = row.iloc[0]  # Access the first column by index since headers are not assumed
        deltas = row.iloc[1:].dropna().values  # Skip the first column
        if len(deltas) > 0:
            run_numbers = list(range(1, len(deltas) + 1))
            plt.plot(run_numbers, deltas, label=operation)
            max_runs = max(max_runs, len(deltas))
            print(f"Plotted operation: {operation} with {len(deltas)} data points")
        else:
            print(f"No data points found for operation: {operation}")

    if max_runs == 0:
        print("Error: No data to plot from the input file.")
        sys.exit(1)

    # Customize the plot
    plt.title('Benchmark Timing Results for Redis Operations')
    plt.ylabel('Time (ns)')
    plt.xlabel('Run Number')
    plt.yscale('log')  # Set y-axis to logarithmic scale
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.legend()
    # Adjust x-axis ticks to show a reasonable number of points for large datasets
    if max_runs > 100:
        step = max_runs // 20  # Show fewer ticks for very large datasets
        plt.xticks(range(0, max_runs + 1, step))
    elif max_runs > 20:
        step = max_runs // 10
        plt.xticks(range(0, max_runs + 1, step))
    else:
        plt.xticks(range(0, max_runs + 1))

    # Determine the output filename by changing the extension to .png
    output_file = os.path.splitext(input_file)[0] + '.png'
    plt.savefig(output_file, dpi=300, bbox_inches='tight')
    plt.close()

    print(f"Graph saved to {output_file}")

def main():
    if len(sys.argv) != 2:
        print("Usage: python generate_graph.py <input_csv_file>")
        sys.exit(1)

    input_file = sys.argv[1]
    if not os.path.exists(input_file):
        print(f"Error: Input file {input_file} does not exist.")
        sys.exit(1)

    generate_graph(input_file)

if __name__ == "__main__":
    main()
