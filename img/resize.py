import numpy as np
from pathlib import Path
import cv2
import sys

def resize(img: cv2.Mat, w_out: int, h_out: int) -> cv2.Mat:
    h_in, w_in, c = img.shape
    resized = np.zeros((h_out, w_out, c), dtype=img.dtype)
    x_scale = w_in / w_out
    y_scale = h_in / h_out

    for y in range(h_out):
        for x in range(w_out):
            resized[y, x] = img[int(y * y_scale), int(x * x_scale)]

    return resized

def quantize_rgb565(img_rgb: np.ndarray) -> np.ndarray:
    img = img_rgb.astype(np.float32)

    r = img[..., 0]
    g = img[..., 1]
    b = img[..., 2]

    r5 = np.clip(np.round(r * 31.0 / 255.0), 0, 31).astype(np.uint16)
    g6 = np.clip(np.round(g * 63.0 / 255.0), 0, 63).astype(np.uint16)
    b5 = np.clip(np.round(b * 31.0 / 255.0), 0, 31).astype(np.uint16)

    return (r5 << 11) | (g6 << 5) | b5


if __name__ == '__main__':

    args = sys.argv
    if len(args) < 2:
        print("Usage: python resized.py <image_path>")
        sys.exit(1)
    
    path = Path(args[1])
    filename = path.stem
    img = cv2.imread(path, cv2.IMREAD_COLOR_BGR)
    h_in, w_in, c = img.shape

    w_out, h_out = 240, 135

    resized = resize(img, w_out, h_out)
    
    rgb = cv2.cvtColor(resized, cv2.COLOR_BGR2RGB)
    rgb565 = quantize_rgb565(rgb)


    cv2.imshow("original", img)
    cv2.imshow("resized", resized)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

    

    file = open(f"{filename}.rgb", "wb")
    file.write(rgb565.byteswap().tobytes())
    file.close()
