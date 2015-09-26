//
//  main.cpp
//  bind
//
//  Created by liuke on 15/1/28.
//  Copyright (c) 2015年 liuke. All rights reserved.
//

#include <iostream>
#include <glog/logging.h>
#include "friend_cache.h"

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

    
    std::cout << typeid(bind::base_cache).hash_code() << std::endl;
    std::cout << typeid(friend_cache).hash_code() << std::endl;
    
    friend_cache f;
    std::cout << typeid(f).hash_code() << std::endl;
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
    
//    friend_cache::set_xxx(8.0);
    
    std::function<void (const boost::any &, const boost::any &)> callback_static = [](const boost::any &old, const boost::any &newer)
    {
        std::cout<<"callback static old:" << boost::any_cast<double>(old) << " new:" << boost::any_cast<double>(newer) << std::endl;
    };
//    friend_cache
    bind::kvo::register_one_property(typeid(friend_cache).hash_code(), friend_cache::__xxx_keypath__(), callback_static);
    friend_cache::set_xxx(90.3);
    std::function<void (const boost::any &, const boost::any &)> callback_static1 = [](const boost::any &old, const boost::any &newer)
    {
        std::cout<<"callback static1 old:" << boost::any_cast<double>(old) << " new:" << boost::any_cast<double>(newer) << std::endl;
    };
    bind::kvo::register_one_property(typeid(friend_cache).hash_code(), friend_cache::__xxx_keypath__(), callback_static1);
    
    friend_cache::set_xxx(90.333);
    return 0;
}
