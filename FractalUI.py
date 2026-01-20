import tkinter as tk
from PIL import Image, ImageTk
import os
import ctypes
import numpy as np

# Import fractal generation functions

lib = ctypes.CDLL(r"C:\Users\aiden\Fractals\optimized_fractals_dll\build\Release\optimized_fractals.dll")

lib.standard_fractal.argtypes = [
    ctypes.c_float, ctypes.c_float, ctypes.c_float,
    ctypes.c_int, ctypes.c_int, ctypes.c_int, 
    ctypes.POINTER(ctypes.c_int),
    ctypes.POINTER(ctypes.c_ubyte),
    ctypes.POINTER(ctypes.c_float)
]

lib.slow_fractal.argtypes = [
    ctypes.c_float, ctypes.c_float, ctypes.c_float,
    ctypes.c_int, ctypes.c_int, ctypes.c_int,
    ctypes.POINTER(ctypes.c_ubyte),
    ctypes.POINTER(ctypes.c_float)
]

lib.get_last_error.restype = ctypes.c_char_p

width, height = 3200, 3200
buffer = np.zeros(width * height * 3, dtype=np.uint8)

betterParameters = [0.8333823967120799,
 -0.5855587062485642,
 0.03677216069678857,
 0.020355120337153136,
 -1.3515378462881071,
 0.18413599326104096]

parameters = [  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
                0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                0.0, 0.0, 0.0, 0.0, 0.0, 0.0]

# Generate coefficients 
parameters[0] = betterParameters[0]
parameters[1] = betterParameters[1]
parameters[10] = betterParameters[2]
parameters[11] = betterParameters[3]
parameters[16] = betterParameters[4]
parameters[17] = betterParameters[5]


parameters = [  1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
                0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
                0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                0.0, 0.0, 0.0, 0.0, 0.0, 0.0]

coeffArray = ctypes.c_float * 52
parameters_c = coeffArray(*parameters)

color_palette = [
    0, 0, 204,
    255, 255, 255, 
    255, 128, 0,
    64, 64, 64
]

colorCoeffArray = ctypes.c_int * 12
color_palette_c = colorCoeffArray(*color_palette)


# Function which reloads the image based on global parameters
def reload_image():

    print("Image reloaded")
    global x_coord, y_coord, scale, parameters

    err = lib.standard_fractal(
        x_coord, y_coord, scale,
        width, height, 1000,
        color_palette_c,
        buffer.ctypes.data_as(ctypes.POINTER(ctypes.c_ubyte)),
        parameters_c
    )
    if err == -1:
        print(lib.get_last_error())

    img = Image.frombuffer("RGB", (width, height), buffer, "raw", "RGB", 0, 1)
    img.save("test.png")


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
last_resized_zoom = None
last_resized_image = None
x_offset = 0
y_offset = 0
zoom_factor = 2.0
PAN_STEP = 50
ZOOM_MIN = 1.0 # Zoom starts at 2.0, the image can be shrunk or grown by a factor of two before being redrawn
ZOOM_MAX = 4.0

# Globals for actual position and zoom of fractal being displayed
x_coord = 0.0
y_coord = 0.0
scale = 4.0 / width

# Load the base image
reload_image()
image_path = os.path.join(os.path.dirname(__file__), "test.png")
original_image = Image.open(image_path)

# Create a canvas to display the image (allows panning & zooming)
canvas = tk.Canvas(content, bg="white")
canvas.pack(fill=tk.BOTH, expand=True, padx=0, pady=0)

# Status label in the sidebar
status_label = tk.Label(sidebar, text=f"Offset: {x_offset},{y_offset}  Zoom: {zoom_factor:.2f}", bg="lightgray")
status_label.pack(padx=10, pady=(5, 15))

# Second status label for fractal coord
status_label2 = tk.Label(sidebar, text=f"Fractal coords: {x_coord:.2f},{y_coord:.2f}", bg="lightgray")
status_label2.pack(padx=10, pady=(5, 15))

# Third status label for scale
status_label3 = tk.Label(sidebar, text=f"Scale: {scale:.2f}", bg="lightgray")
status_label3.pack(padx=10, pady=(5, 15))

displayed_photo = None
display_update_pending = False

def update_status():
    status_label.config(text=f"offset: {x_offset},{y_offset}  zoom: {zoom_factor:.2f}")
    status_label2.config(text=f"Fractal coords: {x_coord:.2f},{y_coord:.2f}")
    status_label3.config(text=f"Scale: {scale:.2f}", bg="lightgray")

def update_display():
    """Update canvas display without regenerating fractal"""
    global x_coord, y_coord, scale, x_offset, y_offset, zoom_factor
    global displayed_photo, original_image, display_update_pending
    global last_resized_zoom, last_resized_image

    display_update_pending = False
    
    cw = canvas.winfo_width()
    ch = canvas.winfo_height()
    if cw <= 1 or ch <= 1:
        display_update_pending = True
        root.after(50, update_display)
        return

    # Only resize if zoom actually changed
    if last_resized_zoom != zoom_factor:
        w, h = original_image.size
        new_w = max(1, int(w * zoom_factor))
        new_h = max(1, int(h * zoom_factor))
        resized = original_image.resize((new_w, new_h), Image.BILINEAR)
        displayed_photo = ImageTk.PhotoImage(resized)
        last_resized_zoom = zoom_factor
    
    # Always update canvas position (this is fast)
    canvas.delete("IMG")
    cw = canvas.winfo_width()
    ch = canvas.winfo_height()
    x = cw // 2 + x_offset
    y = ch // 2 + y_offset
    canvas.create_image(x, y, image=displayed_photo, anchor=tk.CENTER, tags="IMG")
    update_status()

def schedule_display_update():
    """Schedule display update to avoid redundant updates"""
    global display_update_pending
    if not display_update_pending:
        display_update_pending = True
        root.after(16, update_display)  # ~60fps cap

def pan(dx, dy):
    global x_offset, y_offset, x_coord, y_coord, scale
    x_offset += dx
    y_offset += dy
    
    # Check if we've panned far enough to need a fractal regeneration
    pan_threshold = 100  # regenerate when offset exceeds this
    if abs(x_offset) > pan_threshold or abs(y_offset) > pan_threshold:
        # Apply the pan to fractal coordinates
        x_coord += -x_offset * scale
        y_coord += y_offset * scale
        x_offset = 0
        y_offset = 0
        reload_and_refresh()
    else:
        schedule_display_update()

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
    global zoom_factor, x_coord, x_offset, y_coord, y_offset, scale
    # Windows: event.delta positive = up, negative = down
    factor = 1.1 if event.delta > 0 else (1.0 / 1.1)
    zoom_factor = zoom_factor * factor

    # Check if we've zoomed far enough to need a fractal regeneration
    if zoom_factor >= ZOOM_MAX:
        # Apply the pan to fractal coordinates
        x_coord += -x_offset * scale
        y_coord += y_offset * scale
        x_offset = 0
        y_offset = 0
        scale = 2 * scale / ZOOM_MAX
        zoom = 2.0
        reload_and_refresh()
    elif zoom_factor <= ZOOM_MIN:
        # Apply the pan to fractal coordinates
        x_coord += -x_offset * scale
        y_coord += y_offset * scale
        x_offset = 0
        y_offset = 0
        scale = 2.0 * scale 
        zoom_factor = 2.0
        reload_and_refresh()
    else:
        schedule_display_update()

# Ensure canvas updates on resize
canvas.bind("<Configure>", lambda e: schedule_display_update())

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
        global original_image, zoom_factor, x_offset, y_offset
        original_image = Image.open(image_path)
        zoom_factor = 2.0  # Reset zoom
        x_offset = 0
        y_offset = 0
        schedule_display_update()
    except Exception as e:
        print("Failed to reload image:", e)


reload_button = tk.Button(sidebar, text="Reload", command=reload_and_refresh)
reload_button.pack(padx=10, pady=10)


# Generate initial fractal
reload_and_refresh()

# Start the main loop
root.mainloop()