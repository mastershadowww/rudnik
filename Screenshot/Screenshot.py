import pyautogui
from PIL import Image
import pytesseract
import time

time.sleep(5)

screen_width, screen_height = pyautogui.size()

crop_width = int(screen_width * 0.25)
crop_height = int(screen_height * 0.18)

x1 = (screen_width - crop_width) // 2
y1 = (screen_height - crop_height) // 2
x2 = x1 + crop_width
y2 = y1 + crop_height/1.7

screenshot = pyautogui.screenshot()

cropped_image = screenshot.crop((x1, y1, x2, y2))

cropped_image.save("screenshot.png")

text = pytesseract.image_to_string(cropped_image)

print(text)