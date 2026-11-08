"""Check that the WebAssembly game starts in a browser and animates."""
import base64
import sys
import time

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
    page.keyboard.press("ArrowLeft")
    time.sleep(1)
    after = page.screenshot()
    browser.close()

print("Screenshot:", base64.b64encode(after).decode())
if errors:
    sys.exit(f"Page errors: {errors}")
if before == after:
    sys.exit("The game is not running: the page did not change")
