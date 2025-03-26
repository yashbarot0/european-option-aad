import numpy as np
import matplotlib
matplotlib.use('PDF')  # Set the backend to PDF
import matplotlib.pyplot as plt
import subprocess

# Generate data for visualization
S_range = np.linspace(80, 120, 100)
epsilon = 0.01

# Lists to store AAD results
prices_aad = []
deltas_aad = []
gammas_aad = []
vegas_aad = []
thetas_aad = []
rhos_aad = []

# Lists to store finite differences results
deltas_fd = []
gammas_fd = []
vegas_fd = []
thetas_fd = []
rhos_fd = []

for S in S_range:
    # Run the C++ program with these parameters
    result = subprocess.run(["./european_option", str(S), "100.0", "1.0", "0.05", "0.2"], capture_output=True, text=True)
    
    # Print the full output for debugging
    print(f"Output for S={S}:")
    print(result.stdout)
    
    # Parse the results
    output = result.stdout.split('\n')
    
    # Check if we have enough lines in the output
    if len(output) < 12:
        print(f"Insufficient output lines for S={S}")
        continue
    
    # AAD results
    try:
        price_aad = float(output[0].split(":")[1].strip())
        delta_aad = float(output[1].split(":")[1].strip())
        gamma_aad = float(output[2].split(":")[1].strip())
        vega_aad = float(output[3].split(":")[1].strip())
        theta_aad = float(output[4].split(":")[1].strip())
        rho_aad = float(output[5].split(":")[1].strip())
    except Exception as e:
        print(f"Error parsing AAD results for S={S}: {e}")
        continue
    
    # Finite differences results
    try:
        delta_fd = float(output[7].split(":")[1].strip())
        gamma_fd = float(output[8].split(":")[1].strip())
        vega_fd = float(output[9].split(":")[1].strip())
        theta_fd = float(output[10].split(":")[1].strip())
        rho_fd = float(output[11].split(":")[1].strip())
    except Exception as e:
        print(f"Error parsing finite differences results for S={S}: {e}")
        continue
    
    # Store results
    prices_aad.append(price_aad)
    deltas_aad.append(delta_aad)
    gammas_aad.append(gamma_aad)
    vegas_aad.append(vega_aad)
    thetas_aad.append(theta_aad)
    rhos_aad.append(rho_aad)
    
    deltas_fd.append(delta_fd)
    gammas_fd.append(gamma_fd)
    vegas_fd.append(vega_fd)
    thetas_fd.append(theta_fd)
    rhos_fd.append(rho_fd)

# Create plots
plt.figure(figsize=(12, 8))

# Delta Comparison
plt.subplot(2, 3, 1)
plt.plot(S_range[:len(deltas_aad)], deltas_aad, label='AAD')
plt.plot(S_range[:len(deltas_fd)], deltas_fd, '--', label='Finite Differences')
plt.title('Delta Comparison')
plt.xlabel('Underlying Price (S)')
plt.ylabel('Delta')
plt.legend()

# Gamma Comparison
plt.subplot(2, 3, 2)
plt.plot(S_range[:len(gammas_aad)], gammas_aad, label='AAD')
plt.plot(S_range[:len(gammas_fd)], gammas_fd, '--', label='Finite Differences')
plt.title('Gamma Comparison')
plt.xlabel('Underlying Price (S)')
plt.ylabel('Gamma')
plt.legend()

# Vega Comparison
plt.subplot(2, 3, 3)
plt.plot(S_range[:len(vegas_aad)], vegas_aad, label='AAD')
plt.plot(S_range[:len(vegas_fd)], vegas_fd, '--', label='Finite Differences')
plt.title('Vega Comparison')
plt.xlabel('Underlying Price (S)')
plt.ylabel('Vega')
plt.legend()

# Theta Comparison
plt.subplot(2, 3, 4)
plt.plot(S_range[:len(thetas_aad)], thetas_aad, label='AAD')
plt.plot(S_range[:len(thetas_fd)], thetas_fd, '--', label='Finite Differences')
plt.title('Theta Comparison')
plt.xlabel('Underlying Price (S)')
plt.ylabel('Theta')
plt.legend()

# Rho Comparison
plt.subplot(2, 3, 5)
plt.plot(S_range[:len(rhos_aad)], rhos_aad, label='AAD')
plt.plot(S_range[:len(rhos_fd)], rhos_fd, '--', label='Finite Differences')
plt.title('Rho Comparison')
plt.xlabel('Underlying Price (S)')
plt.ylabel('Rho')
plt.legend()

# Price Comparison
plt.subplot(2, 3, 6)
plt.plot(S_range[:len(prices_aad)], prices_aad, label='AAD')
plt.title('Option Price')
plt.xlabel('Underlying Price (S)')
plt.ylabel('Price')

plt.tight_layout()

# Save the figure to a PDF file
plt.savefig('option_greeks_comparison.pdf')
print("Comparison plots saved to option_greeks_comparison.pdf")