//
//  friend_cache.h
//  bind
//
//  Created by liuke on 15/2/3.
//  Copyright (c) 2015年 liuke. All rights reserved.
//

#ifndef __bind__friend_cache__
#define __bind__friend_cache__

#include <stdio.h>
#include "kvo.h"
class friend_cache : public bind::base_cache{
    
    
public:
    
    const std::string& getKey();
    
    KVO_PROPERTY(int, name, 注释)
    KVO_PROPERTY(double, age, 年龄)
};

#endif /* defined(__bind__friend_cache__) */
