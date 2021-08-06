import mouse
import pyautogui
import time
import subprocess
from PIL import Image
import pytesseract
import cv2
import keyboard


def main():
    filename = 'screen.png'
    print('-----------------------------')
    mouse.wait(button=mouse.LEFT, target_types=mouse.DOWN)
    first_position = mouse.get_position()
    mouse.wait(button=mouse.LEFT, target_types=mouse.DOWN)
    second_position = mouse.get_position()
    step_h = (-first_position[0] + second_position[0]) / 5
    step_v = (-first_position[1] + second_position[1]) / 5
    pyautogui.screenshot(filename, region=(first_position[0], first_position[1], second_position[0] - first_position[0],
                                           second_position[1] - first_position[1]))
    cv2.imwrite(filename, cv2.cvtColor(cv2.imread(filename), cv2.COLOR_BGR2GRAY))
    image = Image.open(filename)
    pre_s = ''
    for i in range(0, 5):
        for j in range(0, 5):
            im = image.crop((step_h * (j + 0.25), step_v * (i + 0.15), step_h * (j + 1 - 0.15), step_v * (i + 1 - 0.18)))
            # im.save('{}.png'.format(i * 5 + j))
            pre_s += (pytesseract.image_to_string(im, lang='rus', config='--psm 10 --oem 3 -c tessedit_char_blacklist=().,{}|\/©1234567890[]-+@')[0])
    with open('input.txt', 'w') as out:
        out.write(pre_s.lower())
    print(pre_s.lower())
    subprocess.Popen('szs.exe').wait()
    start_time = time.time()
    with open("resultAuto.txt", 'r') as inp:
        for x in inp.readlines():
            if time.time() - start_time > 115 or keyboard.is_pressed('space'): break
            mouse.release(button=mouse.LEFT)
            mouse.move(first_position[0] + (int(x[1]) + 0.5) * step_h,
                       first_position[1] + (int(x[0]) + 0.5) * step_v, duration=0.05)
            mouse.press(button=mouse.LEFT)
            for i in range(2, len(x) - 1, 2):
                mouse.move((int(x[i + 1]) - int(x[i - 1])) * step_h, (int(x[i]) - int(x[i - 2])) * step_v,
                           absolute=False, duration=0.05)
            mouse.release(button=mouse.LEFT)
            time.sleep(0.3)


if __name__ == '__main__':
    main()
