import plotly.express as px
import pandas as pd
from flask import Flask, render_template
from flaskwebgui import FlaskUI
import os

# read data from file

def plot_data(max_x, max_y, max_z):
    current_dir = os.path.dirname(os.path.abspath(__file__))
    file_path = os.path.join(current_dir, "positions.csv")
    f = open(file_path, "r")
    f.readline() # skip header
    data = f.read().split("\n")
    f.close()
    timestep = [[], [], [], [], []]  # timeStep, x, y, z, alias

    for line in data[:-1]:
        line_str = line.split(",")
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
    fig = px.scatter_3d(df, x='x', y='y', z='z', color = 'Particle', range_x=[0, max_x], range_y=[0, max_y], range_z=[0, max_z], animation_frame="timeStep", hover_data={
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

    # save the figure as html and open electron app
    current_dir = os.path.dirname(os.path.abspath(__file__))
    file_path = os.path.join(current_dir, "templates/plot.html")
    return fig.write_html(file_path)

#client

plot_data(10, 10, 10)

print("Starting viewer")

app = Flask(__name__)
ui = FlaskUI(app = app, server = 'flask')

@app.route('/')
def index():
    return render_template('plot.html')

ui.run()