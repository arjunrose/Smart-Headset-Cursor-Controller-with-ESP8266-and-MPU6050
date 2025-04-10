import requests
import pyautogui
import time
import keyboard  # For detecting 'q' key press

# Disable PyAutoGUI fail-safe
pyautogui.FAILSAFE = False

esp8266_ip = "192.168.137.102"

# Smoothing factor for exponential moving average
alpha = 0.2
prev_x_speed = 0
prev_y_speed = 0

# Variables to store the reference position
ref_x = 0
ref_y = 0
reset_flag = False  # Track if reset button was pressed

def get_esp_data():
    global reset_flag
    try:
        response = requests.get(f"http://{esp8266_ip}/", timeout=0.2)
        data = response.text.split(',')
        accelX, accelY, sensitivity, button_state = float(data[0]), float(data[1]), int(data[2]), int(data[3])
        
        if button_state == 1:  # If reset button is pressed
            reset_flag = True  # Mark reset state
        return accelX, accelY, sensitivity
    except:
        return 0, 0, 10  # Default values if ESP8266 is not responding

while True:
    if keyboard.is_pressed('q'):
        print("Exiting program...")
        break  # Exit loop when 'q' is pressed

    accelX, accelY, sensitivity = get_esp_data()

    # If reset button was pressed, set current position as reference
    if reset_flag:
        ref_x = accelX
        ref_y = accelY
        reset_flag = False
        print("New reference position set!")

    # Apply filtering to ignore small fluctuations
    accelX -= ref_x
    accelY -= ref_y

    # Scale movement speed with sensitivity
    raw_x_speed = accelX * sensitivity * 2  
    raw_y_speed = -accelY * sensitivity * 2

    # Apply exponential smoothing for smoother movement
    x_speed = alpha * raw_x_speed + (1 - alpha) * prev_x_speed
    y_speed = alpha * raw_y_speed + (1 - alpha) * prev_y_speed

    prev_x_speed, prev_y_speed = x_speed, y_speed  # Store previous values

    # Stop movement if very small (dead zone)
    if abs(x_speed) < 1 and abs(y_speed) < 1:
        x_speed, y_speed = 0, 0

    # Move the cursor smoothly
    pyautogui.moveRel(x_speed, y_speed, duration=0.05)  # Small delay for smooth motion
    time.sleep(0.05)  # Adjust refresh rate for responsiveness
