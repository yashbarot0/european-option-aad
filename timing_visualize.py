import numpy as np
import matplotlib
matplotlib.use('PDF')  # Set the backend to PDF
import matplotlib.pyplot as plt
import subprocess
import re

# Generate data for visualization
S_range = np.linspace(80, 120, 100)

# Lists to store timing results
times_aad = []
times_fd = []

for S in S_range:
    # Run the C++ program with these parameters
    result = subprocess.run(["./european_option", str(S), "100.0", "1.0", "0.05", "0.2"], capture_output=True, text=True)
    
    # Print the full output for debugging
    print(f"Output for S={S}:")
    print(result.stdout)
    
    # Parse the results
    output = result.stdout.split('\n')
    
    # Find timing information
    aad_time = None
    fd_time = None
    
    for line in output:
        if "AAD Calculation Time" in line:
            aad_time = float(line.split(":")[1].strip().split()[0])
        elif "Finite Differences Calculation Time" in line:
            fd_time = float(line.split(":")[1].strip().split()[0])
    
    if aad_time is not None and fd_time is not None:
        times_aad.append(aad_time)
        times_fd.append(fd_time)
    else:
        print(f"Timing information not found for S={S}")
        continue

# Create plot
plt.figure(figsize=(10, 6))

# Plot timing comparison
plt.plot(S_range[:len(times_aad)], times_aad, label='AAD')
plt.plot(S_range[:len(times_fd)], times_fd, '--', label='Finite Differences')
plt.title('Execution Time Comparison for European Option Pricing')
plt.xlabel('Underlying Price (S)')
plt.ylabel('Time (microseconds)')
plt.legend()

# Save the figure to a PDF file
plt.savefig('timing_comparison.pdf')
print("Timing comparison plot saved to timing_comparison.pdf")