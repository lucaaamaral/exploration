#!/usr/bin/env python3

import os
import sys
import csv
import re
from collections import defaultdict
import statistics

def extract_library_name(directory):
    """Extract library name from the directory path or file naming pattern."""
    if 'redis-plus-plus' in directory.lower():
        return 'redis_plus_plus'
    elif 'redis-cpp' in directory.lower():
        return 'redis_cpp'
    return 'unknown_library'

def parse_benchmark_csv(file_path):
    """Parse a benchmark CSV file and return a list of benchmark results."""
    results = []
    try:
        with open(file_path, 'r', newline='') as f:
            lines = f.readlines()
            if not lines:
                print(f"Warning: Empty file {file_path}")
                return results
            
            # Find the header row by looking for expected column names
            header = None
            header_index = 0
            for i, line in enumerate(lines):
                # Split the line to check for potential CSV header
                potential_row = line.strip().split(',')
                if len(potential_row) > 3:  # Expect at least a few columns
                    # Check if common column names are present
                    lower_row = [col.strip().lower().replace(' ', '_') for col in potential_row]
                    if any('name' in col for col in lower_row) and any('real' in col for col in lower_row):
                        header = potential_row
                        header_index = i
                        print(f"Found CSV header at line {i+1} in {file_path}, skipped {i} unrelated lines")
                        break
            
            if header is None:
                print(f"Warning: Could not find CSV header in {file_path}")
                return results
            
            # Identify columns for name, iterations, real_time, cpu_time, etc., with case-insensitive matching
            name_idx = -1
            real_time_idx = -1
            cpu_time_idx = -1
            iterations_idx = -1
            
            for i, col in enumerate(header):
                col_lower = col.strip().lower().replace(' ', '_')
                if 'name' in col_lower:
                    name_idx = i
                elif 'real_time' in col_lower or 'real' in col_lower:
                    real_time_idx = i
                elif 'cpu_time' in col_lower or 'cpu' in col_lower:
                    cpu_time_idx = i
                elif 'iterations' in col_lower or 'iter' in col_lower:
                    iterations_idx = i

            if name_idx == -1 or real_time_idx == -1 or cpu_time_idx == -1 or iterations_idx == -1:
                print(f"Warning: Missing required columns in {file_path}. Found indices - Name: {name_idx}, Real Time: {real_time_idx}, CPU Time: {cpu_time_idx}, Iterations: {iterations_idx}")
                return results
            
            # Process the remaining lines as CSV data
            import csv
            from io import StringIO
            data_lines = lines[header_index:]
            reader = csv.reader(StringIO(''.join(data_lines)))
            next(reader, None)  # Skip the header row we already processed
            
            for row in reader:
                if len(row) <= max(name_idx, real_time_idx, cpu_time_idx, iterations_idx):
                    print(f"Warning: Incomplete row in {file_path}: {row}")
                    continue
                try:
                    name = row[name_idx].strip()
                    real_time = float(row[real_time_idx].strip())
                    cpu_time = float(row[cpu_time_idx].strip())
                    iterations = int(row[iterations_idx].strip())
                    results.append({
                        'name': name,
                        'real_time': real_time,
                        'cpu_time': cpu_time,
                        'iterations': iterations
                    })
                except (ValueError, IndexError) as e:
                    print(f"Warning: Error parsing row in {file_path}: {row}, Error: {e}")
                    continue
    except Exception as e:
        print(f"Warning: Failed to read {file_path}: {e}")
    return results

def calculate_averages(results_dir):
    """Calculate averages for benchmark results in the specified directory."""
    if not os.path.isdir(results_dir):
        print(f"Error: Directory {results_dir} does not exist or is not a directory.")
        return
    
    library_name = extract_library_name(results_dir)
    output_file = os.path.join(results_dir, f"{library_name}_average.csv")
    
    # Collect all CSV files in the directory
    csv_files = [f for f in os.listdir(results_dir) if f.endswith('.csv') and 'average' not in f.lower()]
    if not csv_files:
        print(f"Error: No benchmark result CSV files found in {results_dir}.")
        return
    
    print(f"Processing {len(csv_files)} result files in {results_dir}...")
    
    # Aggregate data by benchmark name
    aggregated_data = defaultdict(list)
    for csv_file in csv_files:
        file_path = os.path.join(results_dir, csv_file)
        results = parse_benchmark_csv(file_path)
        for result in results:
            aggregated_data[result['name']].append(result)
    
    if not aggregated_data:
        print(f"Error: No valid data extracted from CSV files in {results_dir}.")
        return
    
    # Calculate averages
    averages = []
    all_real_times = []
    all_cpu_times = []
    all_iterations = []
    
    for benchmark_name, entries in aggregated_data.items():
        real_times = [entry['real_time'] for entry in entries]
        cpu_times = [entry['cpu_time'] for entry in entries]
        iterations_list = [entry['iterations'] for entry in entries]
        
        avg_real_time = statistics.mean(real_times) if real_times else 0
        avg_cpu_time = statistics.mean(cpu_times) if cpu_times else 0
        avg_iterations = statistics.mean(iterations_list) if iterations_list else 0
        
        averages.append({
            'name': benchmark_name,
            'real_time': avg_real_time,
            'cpu_time': avg_cpu_time,
            'iterations': int(avg_iterations),
            'run_count': len(entries)  # Number of runs contributing to this average
        })
        
        all_real_times.extend(real_times)
        all_cpu_times.extend(cpu_times)
        all_iterations.extend(iterations_list)
    
    # Calculate overall averages
    overall_avg_real_time = statistics.mean(all_real_times) if all_real_times else 0
    overall_avg_cpu_time = statistics.mean(all_cpu_times) if all_cpu_times else 0
    overall_avg_iterations = statistics.mean(all_iterations) if all_iterations else 0
    
    # Add overall average to the list
    averages.append({
        'name': 'OVERALL_AVERAGE',
        'real_time': overall_avg_real_time,
        'cpu_time': overall_avg_cpu_time,
        'iterations': int(overall_avg_iterations),
        'run_count': len(csv_files)  # Number of files processed
    })
    
    # Write averages to output CSV
    with open(output_file, 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(['name', 'real_time', 'cpu_time', 'iterations', 'run_count'])
        for avg in sorted(averages, key=lambda x: x['name']):
            writer.writerow([
                avg['name'],
                avg['real_time'],
                avg['cpu_time'],
                avg['iterations'],
                avg['run_count']
            ])
    
    print(f"Averages calculated and saved to {output_file}")

def main():
    if len(sys.argv) != 2:
        print("Usage: python calculate_averages.py <results_directory>")
        sys.exit(1)
    
    results_dir = sys.argv[1]
    calculate_averages(results_dir)

if __name__ == "__main__":
    main()
