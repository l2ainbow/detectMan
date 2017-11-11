# -*- coding: utf-8 -*-
"""
Created on Sat Nov 11 16:07:14 2017

@author: daichi morita
"""

import cv2
import numpy as np
import time

filename = "color_shirts_data/IMG_5380/IMG_5380_0301.jpg"
height = 100

def detectShirt(im):
    _, contours, _ = cv2.findContours(mask(im), cv2.RETR_TREE, method=cv2.CHAIN_APPROX_SIMPLE)
    rects = []
    for contour in contours:
        approx = cv2.convexHull(contour)
        rect = cv2.boundingRect(approx)
        rects.append(np.array(rect))
        
    rects = list(filter(lambda x: (x[2]/x[3]<2)&(x[2]/x[3]>0.5), rects))
    
    return rects
    
def mask(im):
    hsv = cv2.cvtColor(im, cv2.COLOR_RGB2HSV_FULL)
    
    h = hsv[:, :, 0]
    s = hsv[:, :, 1]
    
    mask = np.zeros(h.shape, dtype=np.uint8)
    mask[(h>10) & (h<100) & (s>50)] = 255
    
    return mask

if __name__=="__main__":
    start = time.clock()
    
    im = cv2.imread(filename)
    h, w = im.shape[:2]
    h = int(h/5)
    w = int(w/5)
    im = cv2.resize(im, (w, h))
    
    rects = detectShirt(im)
    if len(rects)>0:
        rect = max(rects, key=(lambda x: x[2] * x[3]))
        cv2.rectangle(im, tuple(rect[0:2]), tuple(rect[0:2] + rect[2:4]), (0, 0, 255), thickness=2)
        
        x = rect[0] + rect[2]/2 - w/2
        y = rect[1] + rect[3]/2 - h/2
        
        r = np.sqrt(x**2 + y**2)
        theta = np.arctan(y/x) + (1 - np.sign(y)) *  np.pi
        
        print("r: %.2f, theta: %.2f"%(r, theta))
    
#    im = mask(im)
    
    print("detecting time: %.0f[ms]"%((time.clock()-start)*10**3))

    cv2.imshow("shirts", im)
    cv2.waitKey(500000)
    cv2.destroyAllWindows()
    
