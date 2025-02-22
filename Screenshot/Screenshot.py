import pyautogui
from PIL import Image
import pytesseract
import time
import re

screen_width, screen_height = pyautogui.size()

crop_width = int(screen_width * 0.25)
crop_height = int(screen_height * 0.18)

x1 = (screen_width - crop_width) // 2
y1 = (screen_height - crop_height) // 2
x2 = x1 + crop_width
y2 = int(y1 + crop_height / 1.65)

screenshot = pyautogui.screenshot()
cropped_image = screenshot.crop((x1, y1, x2, y2))

text = pytesseract.image_to_string(cropped_image)

codes = re.findall(r'(\d{3})', text)  # Pronalazi samo 3-cifrene brojeve

boje = {
    "crvena": r".r[vy]en[ao]m?",
    "zelena": r"[z2]e[lI|]en[ao]m?",
    "svetlo plava": r"[s$5]vetl[io] pla[vy][ao]m?",
    "zuta": r"[z2]ut[ao]m?",
    "roze": r"r[o0]z[eo]m?"
}

pronadjen_kod = False
izabrani_kod = None

if len(codes) >= 5:
    for boja, regex in boje.items():
        if re.search(regex, text, re.IGNORECASE):
            indeks = list(boje.keys()).index(boja)
            izabrani_kod = codes[indeks]
            pronadjen_kod = True
            print(izabrani_kod) 
            break

if not pronadjen_kod:
    print("None") 
