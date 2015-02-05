//
//  friend_cache.cpp
//  bind
//
//  Created by liuke on 15/2/3.
//  Copyright (c) 2015年 liuke. All rights reserved.
//

#include "friend_cache.h"

static std::string key;

const std::string& friend_cache::getKey()
{
    key = "d";
    return key;
}
