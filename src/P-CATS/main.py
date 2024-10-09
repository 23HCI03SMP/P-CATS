import tkinter as tk
from tkinter import filedialog
import subprocess
import threading
import os
import json
import time

CURRENT_DIRECTORY = os.path.dirname(os.path.abspath(__file__))
CACHE_FILE = f"{CURRENT_DIRECTORY}/cache.json"

SIZE_X_HEADER = "size_x"
SIZE_Y_HEADER = "size_y"
SIZE_Z_HEADER = "size_z"
DENSITY_HEADER = "density"
TEMPERATURE_HEADER = "temperature"
WORKING_DIRECTORY_HEADER = "working_directory"

working_directory = ""

def update_json(file, key, value):
    with open(file, 'r') as f:
        data = json.load(f)
        data[key] = value

    with open(file, 'w') as f:
        json.dump(data, f)

def to_console(text):
    console_output.config(state=tk.NORMAL)
    console_output.insert(tk.END, text)
    console_output.see(tk.END)
    console_output.config(state=tk.DISABLED)

def clear_console():
    console_output.config(state=tk.NORMAL)
    console_output.delete(1.0, tk.END)
    console_output.config(state=tk.DISABLED)

def load_cache():
    global working_directory
    if os.path.exists(CACHE_FILE):
        with open(CACHE_FILE, 'r') as f:
            data = json.load(f)
            working_directory = data.get(WORKING_DIRECTORY_HEADER)
            if working_directory:
                to_console(f"Loaded cached directory: {working_directory}\n")
            else:
                to_console("No working directory found. Please choose a working directory.\n")

            sizex = data.get(SIZE_X_HEADER)
            sizey = data.get(SIZE_Y_HEADER)
            sizez = data.get(SIZE_Z_HEADER)
            temperature = data.get(TEMPERATURE_HEADER)
            density = data.get(DENSITY_HEADER)

            if sizex and sizey and sizez:
                sizex_entry.insert(0, sizex)
                sizey_entry.insert(0, sizey)
                sizez_entry.insert(0, sizez)
                to_console(f"Loaded cached size parameters (x: {sizex}, y: {sizey}, z: {sizez}).\n")
            else:
                to_console("No cached size parameters found.\n")

            if temperature:
                temperature_entry.insert(0, temperature)
                to_console(f"Loaded cached temperature parameter: {temperature}\n")
            else:
                to_console("No cached temperature parameter found.\n")

            if density:
                density_entry.insert(0, density)
                to_console(f"Loaded cached density parameter: {density}\n")
            else:
                to_console("No cached density parameter found.\n")
    else:
        to_console("No cache file found.\n")

def save_working_directory():
    if os.path.exists(CACHE_FILE):
        update_json(CACHE_FILE, WORKING_DIRECTORY_HEADER, working_directory)
    else:
        with open(CACHE_FILE, 'w') as f:
            json.dump({WORKING_DIRECTORY_HEADER: working_directory}, f)

def save_size_parameters(x, y, z):
    if os.path.exists(CACHE_FILE):
        update_json(CACHE_FILE, SIZE_X_HEADER, x)
        update_json(CACHE_FILE, SIZE_Y_HEADER, y)
        update_json(CACHE_FILE, SIZE_Z_HEADER, z)
    else:
        with open(CACHE_FILE, 'w') as f:
            json.dump({SIZE_X_HEADER: x, SIZE_Y_HEADER: y, SIZE_Z_HEADER: z}, f)

def save_density_parameter(density):
    if os.path.exists(CACHE_FILE):
        update_json(CACHE_FILE, DENSITY_HEADER, density)
    else:
        with open(CACHE_FILE, 'w') as f:
            json.dump({DENSITY_HEADER: density}, f)

def save_temperature_parameter(temperature):
    if os.path.exists(CACHE_FILE):
        update_json(CACHE_FILE, TEMPERATURE_HEADER, temperature)
    else:
        with open(CACHE_FILE, 'w') as f:
            json.dump({TEMPERATURE_HEADER: temperature}, f)

def choose_directory():
    global working_directory
    working_directory = filedialog.askdirectory()
    if working_directory:
        to_console(f"Working directory set to: {working_directory}\n")
        save_working_directory()

        build_button.config(state=tk.NORMAL)

def run_pcats():
    command = [f"{CURRENT_DIRECTORY}/P-CATS.exe", "--output", f"{CURRENT_DIRECTORY}/positions.csv"]

    to_console("Running P-CATS.\n")

    # Size parameters
    sizex = sizex_entry.get()
    sizey = sizey_entry.get()
    sizez = sizez_entry.get()
    # Density parameter
    density = density_entry.get()
    # Temperature parameter
    temperature = temperature_entry.get()

    if not sizex or not sizey or not sizez:
        to_console("Complete size parameters not provided, using default option.\n")
    else:
        save_size_parameters(sizex, sizey, sizez)
        to_console(f"Size parameters saved (x: {sizex}, y: {sizey}, z: {sizez}).\n")
        command.extend(["--sizex", sizex, "--sizey", sizey, "--sizez", sizez])
    
    if not density:
        to_console("Density parameter not provided, using default option.\n")
    else:
        save_density_parameter(density)
        to_console(f"Density parameter saved: {density}\n")
        command.extend(["--density", density])

    if not temperature:
        to_console("Temperature parameter not provided, using default option.\n")
    else:
        save_temperature_parameter(temperature)
        to_console(f"Temperature parameter saved: {temperature}\n")
        command.extend(["--temperature", temperature])

    def run_command():
        process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

        def read_output(pipe, tag):
            for line in iter(pipe.readline, ''):
                to_console(f"{tag}: {line}")
            pipe.close()

        stdout_thread = threading.Thread(target=read_output, args=(process.stdout, "STDOUT"))
        stderr_thread = threading.Thread(target=read_output, args=(process.stderr, "STDERR"))

        stdout_thread.start()
        stderr_thread.start()

        stdout_thread.join()
        stderr_thread.join()

        process.wait()

    threading.Thread(target=run_command).start()

def build_pcats():
    def run_build():
        start_time = time.time()

        process = subprocess.Popen(["g++", 
                                    "-g", 
                                    f"{working_directory}/*.cpp", 
                                    "-o",
                                    f"{CURRENT_DIRECTORY}/P-CATS.exe",
                                    "-lgsl"],
                                    stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

        def read_output(pipe, tag):
            for line in iter(pipe.readline, ''):
                to_console(f"[{tag}] {line}")
            pipe.close()

        stdout_thread = threading.Thread(target=read_output, args=(process.stdout, "STDOUT"))
        stderr_thread = threading.Thread(target=read_output, args=(process.stderr, "STDERR"))

        stdout_thread.start()
        stderr_thread.start()

        stdout_thread.join()
        stderr_thread.join()

        process.wait()

        end_time = time.time()
        elapsed_time = end_time - start_time

        to_console(f"Build completed ({elapsed_time:.2f}s).\n")
        
        run_button.config(state=tk.NORMAL)

    to_console("Building P-CATS.\n")

    threading.Thread(target=run_build).start()

m = tk.Tk(screenName="P-CATS Console")

button_frame = tk.Frame(m)
button_frame.pack(side=tk.LEFT, fill=tk.Y)

# Button to choose working directory
choose_dir_button = tk.Button(button_frame, text="Choose Working Directory", command=choose_directory)
choose_dir_button.pack(fill=tk.X)

# Buttons to build P-CATS
build_button = tk.Button(button_frame, text="Build", width=10, command=build_pcats)
build_button.pack(fill=tk.X)

# Button to run P-CATS
run_button = tk.Button(button_frame, text="Run", width=10, command=run_pcats)
run_button.pack(fill=tk.X)

# Text fields for size parameters
sizex_label = tk.Label(button_frame, text="Base space size (x)")
sizex_label.pack(fill=tk.X)
sizex_entry = tk.Entry(button_frame)
sizex_entry.pack(fill=tk.X)

sizey_label = tk.Label(button_frame, text="Base space size (y)")
sizey_label.pack(fill=tk.X)
sizey_entry = tk.Entry(button_frame)
sizey_entry.pack(fill=tk.X)

sizez_label = tk.Label(button_frame, text="Base space size (z)")
sizez_label.pack(fill=tk.X)
sizez_entry = tk.Entry(button_frame)
sizez_entry.pack(fill=tk.X)

# Text field for density parameter
density_label = tk.Label(button_frame, text="Density (fill in units)")
density_label.pack(fill=tk.X)
density_entry = tk.Entry(button_frame)
density_entry.pack(fill=tk.X)

# Text field for temperature parameter
temperature_label = tk.Label(button_frame, text="Temperature (K)")
temperature_label.pack(fill=tk.X)
temperature_entry = tk.Entry(button_frame)
temperature_entry.pack(fill=tk.X)

# Clear console button, put at bottom of left side
clear_console_button = tk.Button(button_frame, text="Clear Console", command=lambda: clear_console())
clear_console_button.pack(side=tk.BOTTOM, fill=tk.X)

console_output = tk.Text(m, wrap='word', height=20, width=80)
console_output.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
console_output.config(state=tk.DISABLED)

m.title("P-CATS Console")

load_cache()
to_console(f"Current program directory: {CURRENT_DIRECTORY}\n")

# If no working directory, disable build and run buttons
if not working_directory:
    build_button.config(state=tk.DISABLED)
    run_button.config(state=tk.DISABLED)

if working_directory and not os.path.exists(f"{CURRENT_DIRECTORY}/P-CATS.exe"):
    run_button.config(state=tk.DISABLED)

m.mainloop()