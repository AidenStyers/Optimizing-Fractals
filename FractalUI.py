import tkinter as tk
from PIL import Image, ImageTk
import os
from ctypes import *
import numpy as np
from numpy.ctypeslib import ndpointer

# Import fractal generation functions

fractal_generation_functions = CDLL("C:/Users/aiden/Fractals/Release/fractal_generation.dll")

# Define input and output types for functions with array inputs
fractal_generation_functions.coolness.argtypes = [
    ndpointer(dtype=np.float32, flags="C_CONTIGUOUS"),
    c_int
]
fractal_generation_functions.coolness.restype = int


fractal_generation_functions.formattedGreyscaleFractal.argtypes = [
    c_char_p,
    ndpointer(dtype=np.float32, flags="C_CONTIGUOUS"),
    c_int
]
fractal_generation_functions.formattedGreyscaleFractal.restype = int

fractal_generation_functions.formattedFractal.argtypes = [
    c_char_p,
    ndpointer(dtype=np.float32, flags="C_CONTIGUOUS"),
    c_float, 
    c_float, 
    c_float, 
    c_int, 
    c_int
]
fractal_generation_functions.formattedFractal.restype = int


parameters = np.array([-0.59873605, -0.523377  , -0.8273191 , -0.44887334, -0.4417911 ,
       -0.48399195, -0.12021855,  0.07123734, -0.23138577,  0.30694085,
        0.58028346,  0.50386673, -0.78419316,  0.74131984, -0.9554869 ,
        0.00466489, -0.98105997,  0.2888265 ,  0.02735518, -0.36006135,
       -0.3985477 , -0.00113548,  0.22578485, -0.4860962 ,  0.22074234,
        0.3801627 ,  0.67600876,  0.7855595 , -0.3369591 ,  0.8820408 ,
        0.04885696, -0.41388005, -0.15344574, -0.81404763, -0.65192395,
        0.3022214 ,  0.10688563,  0.12383255, -0.52157086, -0.9935714 ,
        0.9551164 , -0.7977333 , -0.64926004,  0.78809327,  0.5223523 ,
        0.8369143 , -0.32329178,  0.8245339 , -0.22968203, -0.01218032,
        0.6385087 , -0.55122054,  0.40428516,  0.54691684], dtype=c_float)

betterParameters = [np.float64(0.306130926411601),
 np.float64(-1.694189262350915),
 np.float64(-0.11008876934572466),
 np.float64(0.03297363955160642),
 np.float64(0.018925388611936697),
 np.float64(-0.3096979497254595)]

parameters = np.array([0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
                       0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
                       0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                       0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                       0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                       0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                       0.0, 0.0, 0.0, 0.0, 0.0, 0.0], dtype=c_float)

# Generate coefficients 
parameters[2] = betterParameters[0]
parameters[3] = betterParameters[1]
parameters[12] = betterParameters[2]
parameters[13] = betterParameters[3]
parameters[18] = betterParameters[4]
parameters[19] = betterParameters[5]

# Function which reloads the image based on global parameters
def reload_image():
    global x_coord, y_coord, radius, parameters
    fractal_generation_functions.formattedFractal("test.png".encode('utf-8'), parameters, x_coord, y_coord, radius, 100, 800)



# Create the main window
root = tk.Tk()
root.title("Fractal UI")
root.geometry("1000x800")

# Create a sidebar frame on the left
sidebar = tk.Frame(root, bg="lightgray", width=200)
sidebar.pack(side=tk.LEFT, fill=tk.BOTH)
sidebar.pack_propagate(False)

# Create a main content frame on the right
content = tk.Frame(root, bg="white")
content.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True)



# Globals for pan/zoom of image
x_offset = 0
y_offset = 0
zoom_factor = 2.0
PAN_STEP = 50
ZOOM_MIN = 0.1
ZOOM_MAX = 10.0

# Globals for actual position and zoom of fractal being displayed
x_coord = 0.0
y_coord = 0.0
radius = 4.0

# Load the base image
image_path = os.path.join(os.path.dirname(__file__), "test.png")
original_image = Image.open(image_path)

# Create a canvas to display the image (allows panning & zooming)
canvas = tk.Canvas(content, bg="white")
canvas.pack(fill=tk.BOTH, expand=True, padx=0, pady=0)

# Status label in the sidebar
status_label = tk.Label(sidebar, text=f"offset: {x_offset},{y_offset}  zoom: {zoom_factor:.2f}", bg="lightgray")
status_label.pack(padx=10, pady=(5, 15))

# Second status label for fractal parameters
status_label2 = tk.Label(sidebar, text=f"Fractal coords: {x_coord:.2f},{y_coord:.2f}  radius: {radius:.2f}", bg="lightgray")
status_label2.pack(padx=10, pady=(5, 15))

displayed_photo = None

def update_status():
    status_label.config(text=f"offset: {x_offset},{y_offset}  zoom: {zoom_factor:.2f}")
    status_label2.config(text=f"Fractal coords: {x_coord:.2f},{y_coord:.2f}  radius: {radius:.2f}")

def update_display():

    global x_coord, y_coord, radius, x_offset, y_offset, zoom_factor

    # Check if image needs to be regenerated
    if (zoom_factor >= 4.0 or zoom_factor <= 1.0):
        # Set fractal parameters to this position and zoom
        x_coord += -x_offset * radius / 1600
        y_coord += y_offset * radius / 1600

        if (zoom_factor >= 4.0):
            radius = radius / zoom_factor
        else:
            radius = radius * 2
        # Reset image parameters
        x_offset = 0
        y_offset = 0
        zoom_factor = 2.0
        # Redraw fractal
        reload_and_refresh()

    # Rescale and redraw the image centered on canvas + offsets.
    global displayed_photo, original_image
    cw = canvas.winfo_width()
    ch = canvas.winfo_height()
    if cw <= 1 or ch <= 1:
        # Canvas not yet sized; try again shortly
        root.after(50, update_display)
        return

    w, h = original_image.size
    new_w = max(1, int(w * zoom_factor))
    new_h = max(1, int(h * zoom_factor))
    resized = original_image.resize((new_w, new_h), Image.LANCZOS)
    displayed_photo = ImageTk.PhotoImage(resized)

    canvas.delete("IMG")
    x = cw // 2 + x_offset
    y = ch // 2 + y_offset
    canvas.create_image(x, y, image=displayed_photo, anchor=tk.CENTER, tags="IMG")
    update_status()

def pan(dx, dy):
    global x_offset, y_offset
    x_offset += dx
    y_offset += dy
    update_display()

def on_key(event):
    key = event.keysym
    if key == "Left":
        pan(PAN_STEP, 0)
    elif key == "Right":
        pan(-PAN_STEP, 0)
    elif key == "Up":
        pan(0, PAN_STEP)
    elif key == "Down":
        pan(0, -PAN_STEP)

def on_mousewheel(event):
    global zoom_factor
    # Windows: event.delta positive = up, negative = down
    factor = 1.1 if event.delta > 0 else (1.0 / 1.1)
    new_zoom = zoom_factor * factor
    zoom_factor = max(ZOOM_MIN, min(ZOOM_MAX, new_zoom))
    update_display()

# Ensure canvas updates on resize
canvas.bind("<Configure>", lambda e: update_display())

# Bind keys and mouse wheel (focus is needed for key events)
root.bind("<Left>", on_key)
root.bind("<Right>", on_key)
root.bind("<Up>", on_key)
root.bind("<Down>", on_key)
canvas.bind_all("<MouseWheel>", on_mousewheel)
root.focus_set()

# Update the reload function to refresh our image object + canvas
def reload_and_refresh():
    reload_image()
    try:
        global original_image
        original_image = Image.open(image_path)
        update_display()
    except Exception as e:
        print("Failed to reload image:", e)


reload_button = tk.Button(sidebar, text="Reload", command=reload_and_refresh)
reload_button.pack(padx=10, pady=10)


# Generate inital fractal
reload_and_refresh()

# Start the main loop
root.mainloop()