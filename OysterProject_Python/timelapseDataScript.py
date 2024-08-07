import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

def extract_data(file_path, column, init, interval):
    # Determine the file type based on the file extension
    if file_path.endswith('.csv'):
        # Read the CSV file into a DataFrame
        df = pd.read_csv(file_path)
    elif file_path.endswith('.tsv') or file_path.endswith('.txt'):
        # Read the TSV file into a DataFrame
        df = pd.read_csv(file_path, sep='\t')
    else:
        raise ValueError("Unsupported file format. Only CSV and TSV files are supported.")
    
    # Check if column is specified as an index (integer) or a name (string)
    if isinstance(column, int):
        if column < 0 or column >= len(df.columns):
            raise ValueError(f"Column index '{column}' is out of range.")
        column_data = df.iloc[:, column]
    elif isinstance(column, str):
        if column not in df.columns:
            raise ValueError(f"Column '{column}' not found in the file.")
        column_data = df[column]
    else:
        raise ValueError("Column must be specified as an integer (index) or a string (name).")
        
    # Convert the column data to numeric, coerce errors to NaN
    column_data = pd.to_numeric(column_data, errors='coerce')
    
    # Extract data every k + 10n
    extracted_data = np.array([column_data.iloc[i] for i in range(init, len(column_data), interval)])
    
    return extracted_data

# Extract voltage data
hallReading_file = 'hallReading_12-07-2024_09-10.csv'
gapeDistance_file = 'timelapse_gape_distance.txt'

# Extract data
voltage_raw = extract_data(hallReading_file, ' Voltage (V)', 0, 10)
gape_distance = extract_data(gapeDistance_file, 1, 1, 1)

# Process voltage data into relative gaping 

voltage_norm = voltage_raw - min(voltage_raw)

voltage_norm_scaled = np.exp(-voltage_norm)

rel_gape_sensor = (voltage_norm_scaled / max(voltage_norm_scaled))

norm_gape_distance = gape_distance - min(gape_distance)
rel_gape_video = norm_gape_distance / max(norm_gape_distance)

# Create time array (int 10 seconds)
sampling_int = 10
total_duration = len(voltage_raw)*10
time = np.arange(0, total_duration, sampling_int)


# Create subplots
fig, axes = plt.subplots(3, 1, figsize=(20, 18))

# Plot gaping distance versus time
axes[0].plot(time, rel_gape_video, marker='.', linestyle='-', color='#1f77b4', label='Gaping Distance (Video)',
             markersize=6, alpha=0.8)
axes[0].set_xlabel('Time (seconds)')
axes[0].set_ylabel('Gaping Distance (mm)')
axes[0].set_title('Gaping Distance vs Time (Video)')
axes[0].grid(True)
axes[0].legend()

# Plot relative gaping versus time
axes[1].plot(time, rel_gape_sensor, marker='.', linestyle='-', color='#ff91af', label='Relative Gaping (Sensor)',
             markersize=6, alpha=0.8)
axes[1].set_xlabel('Time (seconds)')
axes[1].set_ylabel('Relative Gaping')
axes[1].set_title('Relative Gaping vs Time (Sensor)')
axes[1].grid(True)
axes[1].legend()

# Plot overlay of the two previous plots
axes[2].plot(time, rel_gape_video, marker='.', linestyle='-', color='#1f77b4', label='Gaping Distance (Video)',
             markersize=6, alpha=0.8)
axes[2].plot(time, rel_gape_sensor, marker='.', linestyle='-', color='#ff91af', label='Relative Gaping (Sensor)',
             markersize=6, alpha=0.8)
axes[2].set_xlabel('Time (seconds)')
axes[2].set_ylabel('Values')
axes[2].set_title('Overlay: Hall Sensor vs. Video')
axes[2].grid(True)
axes[2].legend()

plt.tight_layout()
plt.show()