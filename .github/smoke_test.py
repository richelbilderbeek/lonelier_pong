"""Check that the WebAssembly game starts in a browser and animates."""
import base64
import io
import sys
import time

from PIL import Image

from playwright.sync_api import sync_playwright

URL = "http://localhost:8000/lonelier_pong.html"

with sync_playwright() as p:
    browser = p.chromium.launch(channel="chrome")
    # The same size as the iframe on the website
    page = browser.new_page(viewport={"width": 600, "height": 100})
    errors = []
    page.on("pageerror", lambda e: errors.append(str(e)))
    page.goto(URL)
    time.sleep(10)
    # Give the game keyboard focus, then move the paddle
    page.mouse.click(300, 50)
    before = page.screenshot()
    page.keyboard.press("ArrowRight")
    time.sleep(0.2)
    page.keyboard.press("ArrowLeft")
    time.sleep(0.2)
    after_left = page.screenshot()
    page.keyboard.press("ArrowRight")
    time.sleep(0.2)
    after = page.screenshot()
    browser.close()

print("Screenshot:", base64.b64encode(after).decode())
if errors:
    sys.exit(f"Page errors: {errors}")
if before == after:
    sys.exit("The game is not running: the page did not change")


def paddle_at_left(png):
    """Is the paddle (a dark vertical line) at the left edge?"""
    img = Image.open(io.BytesIO(png)).convert("L")
    return any(
        all(img.getpixel((x, y)) < 128 for y in range(0, 100, 5))
        for x in range(0, 5)
    )


if not paddle_at_left(after_left):
    print("Screenshot after Left:", base64.b64encode(after_left).decode())
    sys.exit("Pressing Left did not move the paddle to the left")
if paddle_at_left(after):
    sys.exit("Pressing Right did not move the paddle to the right")
