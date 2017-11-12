//
//  ViewController.swift
//  c++Test
//
//  Created by 外村真吾 on 2017/11/12.
//  Copyright © 2017年 Shingo. All rights reserved.
//

import UIKit

class ViewController: UIViewController {

    override func viewDidLoad() {
        super.viewDidLoad()
        let image = UIImage()
        let r: Double = 0.0
        let theta: Double = 0.0
        let objcpp = ObjCpp()
        let a: String = objcpp.calcPosition(image, distance: r, radian: theta)
        print(a)
        // Do any additional setup after loading the view, typically from a nib.
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }


}

