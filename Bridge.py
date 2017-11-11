# -*- coding: utf-8 -*-
"""
Created on Sat Nov 11 21:59:26 2017

@author: daichi morita
"""

import cv2

import objc
from Foundation import NSObject

import logging

logger = logging.getLogger(__name__)
logging.basicConfig(level=logging.INFO)

logger.info("Loaded python bundle")

# Load the protocol from Objective-C
BridgeInterface = objc.protocolNamed("PythonToSwiftExample.BridgeInterface")

class Bridge(NSObject, protocols=[BridgeInterface]):
    @classmethod
    def createInstance(self):
        return Bridge.alloc().init()
    
    def detectMan(self, im):
        h, w = im.shape[:2]
        return w/2, h/2
    
    def detectManFromPath(self, filename):
        im = cv2.imread(filename)
        return self.detectMan(im)
        