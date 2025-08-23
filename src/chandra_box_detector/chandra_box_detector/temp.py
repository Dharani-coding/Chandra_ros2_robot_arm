import cv2
import numpy as np

def detect_and_classify_cup(image_path):
    # Load image
    img = cv2.imread(image_path)
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    blur = cv2.medianBlur(gray, 5)

    # Hough Circle Detection (for rim)
    circles = cv2.HoughCircles(blur, cv2.HOUGH_GRADIENT, dp=1.2, minDist=50,
                               param1=100, param2=30, minRadius=20, maxRadius=200)

    # Cup mask for contour height
    _, thresh = cv2.threshold(blur, 0, 255, cv2.THRESH_BINARY_INV + cv2.THRESH_OTSU)
    kernel = np.ones((5,5), np.uint8)
    mask = cv2.morphologyEx(thresh, cv2.MORPH_CLOSE, kernel, iterations=2)
    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    if not contours or circles is None:
        label = "No Cup Detected"
        cv2.putText(img, label, (30, 40), cv2.FONT_HERSHEY_SIMPLEX, 1.2, (0,0,255), 3)
        return cv2.cvtColor(img, cv2.COLOR_BGR2RGB), label

    # Cup height from largest contour
    c = max(contours, key=cv2.contourArea)
    x, y, w, h = cv2.boundingRect(c)
    cup_height = h
    cv2.rectangle(img, (x,y), (x+w, y+h), (0,255,0), 2)

    # Rim diameter from largest circle
    circles = np.uint16(np.around(circles))
    largest_circle = max(circles[0, :], key=lambda c: c[2])
    cx, cy, r = largest_circle
    rim_diameter = 2 * r
    cv2.circle(img, (cx, cy), r, (255, 0, 0), 3)

    # Classification rule
    if cup_height > rim_diameter * 1.2:
        label = "Normal Cup"
    else:
        label = "Wide Cup"

    cv2.putText(img, label, (30, 40), cv2.FONT_HERSHEY_SIMPLEX, 1.2, (0,0,255), 3)
    return cv2.cvtColor(img, cv2.COLOR_BGR2RGB), label


img, label = detect_and_classify_cup("images/large_cup_image.png")
print(label)
cv2.imshow("Cup Detection", cv2.cvtColor(img, cv2.COLOR_RGB2BGR))
cv2.waitKey(0)  
