//
//  main.cpp
//  bind
//
//  Created by liuke on 15/1/28.
//  Copyright (c) 2015年 liuke. All rights reserved.
//

#include <iostream>
#include <glog/logging.h>
#include "bindMgr.h"
#include "./src/bindMgr.cpp"
#include "friend_cache.h"

std::shared_ptr<bind::BindMgr> s_1;

#include <stdlib.h>

//static  __attribute__((constructor(1))) void before()
//{
//    
//    printf("Hello");
//}
//
//static  __attribute__((constructor(2))) void before1()
//{
//    
//    printf(" World!");
//}
//
//static  __attribute__((destructor)) void after()
//{
//    printf(" bey!\n");
//}

int main(int argc, const char * argv[]) {

    friend_cache f;
    f.set_name(9);
    std::function<void (const boost::any &, const boost::any &)> callback = [](const boost::any &old, const boost::any &newer)
    {
        std::cout<<"callback old:" << boost::any_cast<int>(old) << " new:" << boost::any_cast<int>(newer) << std::endl;
    };
    int b1 = f.register_property(f.__name_keypath__(), callback);
    
    std::function<void (const boost::any &, const boost::any &)> callback1 = [](const boost::any &old, const boost::any &newer)
    {
        std::cout<<"callback1 old:" << boost::any_cast<int>(old) << " new:" << boost::any_cast<int>(newer) << std::endl;
    };
    int b2 = f.register_property(f.__name_keypath__(), callback1);
    
    f.set_name(4);
    
    f.remove_register(b2);
    
    f.set_name(8);
    
    f.remove_register(b1);
    
    f.set_name(23);
    
    return 0;
}
