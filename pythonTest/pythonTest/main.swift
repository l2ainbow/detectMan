//
//  main.swift
//  pythonTest
//
//  Created by 外村真吾 on 2017/11/12.
//  Copyright © 2017年 Shingo. All rights reserved.
//

import Foundation
import UIKit
let path = Bundle.main.path(forResource: "Bridge", ofType: "plugin")
guard let pluginbundle = Bundle(path: path!) else {
    fatalError("Could not load python plugin bundle")
}
pluginbundle.load()
//UIApplicationMain()
