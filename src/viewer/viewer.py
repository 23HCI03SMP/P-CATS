import plotly.express as px
import pandas as pd
from flask import Flask, render_template
from flaskwebgui import FlaskUI
import random

ignore_random = True
# read data from file

def plot_data(min_x, min_y, min_z, max_x, max_y, max_z):
    import os
    
    file_path = os.path.join(os.path.dirname(__file__), "positions.csv")
    with open(file_path, "r") as f:
        f.readline() # skip header
        data = f.read().split("\n")
    timestep = [[], [], [], [], []]  # timeStep, x, y, z, alias

    for line in data[:-1]:
        line_str = line.split(",")
        if random.randint(1, 2) == 1 or ignore_random:
            timestep[0].append(int(line_str[0]))
            timestep[1].append(float(line_str[1]))
            timestep[2].append(float(line_str[2]))
            timestep[3].append(float(line_str[3]))
            timestep[4].append(str(line_str[4]))


    df = pd.DataFrame({
        "timeStep": timestep[0],
        "x": timestep[1],
        "y": timestep[2],
        "z": timestep[3],
        "Particle": timestep[4]
    })

    # add domain of grid
    fig = px.scatter_3d(df, x='x', y='y', z='z', color = 'Particle', range_x=[min_x, max_x], range_y=[min_y, max_y], range_z=[min_z, max_z], animation_frame="timeStep", hover_data={
        'timeStep': False,
        'x': False,
        'y': False,
        'z': False,
        'Particle': True
    })

    fig.update_layout(scene=dict(
        aspectmode='cube',
        uirevision = "foo"
    ))
    
    # create directory "templates" if it does not exist
    if not os.path.exists(os.path.join(os.path.dirname(__file__), "templates")):
        os.makedirs(os.path.join(os.path.dirname(__file__), "templates"))
    
    print("Writing plot")
    directory = os.path.join(os.path.dirname(__file__), "templates")
    file_path = os.path.join(directory, "plot.html")
    fig.write_html(file_path, auto_open=False)

#client

app = Flask(__name__)
ui = FlaskUI(app = app, server = 'flask')

@app.route('/')
def index():
    plot_data(0, 0, 0, 10, 10, 10)
    return render_template('plot.html')

ui.run()