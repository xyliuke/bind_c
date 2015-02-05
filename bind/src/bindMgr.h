//
//  bindMgr.h
//  bind
//
//  Created by liuke on 15/1/28.
//  Copyright (c) 2015年 liuke. All rights reserved.
//

#ifndef __bind__bindMgr__
#define __bind__bindMgr__

#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>

namespace bind {
    class bindMgrImpl;
    
    class BindUnit
    {
    public:
        std::string key;
    };
    
    class BindMgr
    {
    public:
        template<typename T>
        int bind(const std::string& key, const std::string& keypath, std::shared_ptr<T> user, std::function<void(void*)> callback)
        {
            std::weak_ptr<T> weak_user = user;
            std::function<void(void*)> func = callback;
            std::shared_ptr<BindUnit> bind(new BindUnit());
            bind->key = key;
//            return bind_impl(key, keypath, user, callback);
            return 0;
        }
        
        
    private:
        std::shared_ptr<bindMgrImpl> impl_;
        std::shared_ptr<std::vector<BindUnit>> set_;
    };
}

#endif /* defined(__bind__bindMgr__) */
