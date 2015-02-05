//
//  kvo.h
//  bind
//
//  Created by liuke on 15/1/29.
//  Copyright (c) 2015年 liuke. All rights reserved.
//

#ifndef __bind__kvo__
#define __bind__kvo__

#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <boost/any.hpp>


namespace bind {
    
#define KVO_PROPERTY(TYPE, NAME, COMMENT) private:TYPE __##NAME##__; \
    void init_##NAME(TYPE v){__##NAME##__=v;} \
    public:void set_##NAME(TYPE v){kvo_->update_one_property<TYPE>(#NAME, __##NAME##__, v); __##NAME##__ = v;} \
    TYPE get_##NAME(){return __##NAME##__;} \
    static std::string __##NAME##_keypath__(){return #NAME;}
    
    class base_cache;

    class observer_unit
    {
    public:
        std::string keypath;
        std::function<void(const boost::any&, const boost::any&)> callback;
        int count;//唯一标识一个绑定单元
    };
    
    class kvo_impl;
    /**
     *  监听类
     */
    class kvo {
    public:
        kvo();
        ~kvo();
        /**
         *  注册一个数据更新监听
         *
         *  @param Cache   所有cache类的基类
         *  @param keypath
         */
        /**
         *  注册一个数据更新监听
         *
         *  @param keypath 属性名
         *  @param callback 闭包
         *
         *  @return 返回注册项的唯一标识
         */
        int register_one_property(const std::string& keypath, std::function<void(const boost::any& old, const boost::any& newer)> callback);
       
        /**
         *  当一个属性被修改后，调用这个函数，向所有注册的闭包发送消息
         *
         *  @param keypath 属性名
         *  @param old     旧值
         *  @param newer   新值
         *
         *  @return
         */
        template <typename T>
        void update_one_property(const std::string& keypath, T old, T newer)
        {
            boost::any old_any = boost::any::any(old);
            boost::any new_any = boost::any::any(newer);
            update(keypath, old_any, new_any);
        }
        
        /**
         *  判断一个属性名是否被注册过
         *
         *  @param keypath 属性名
         *
         *  @return true 已经注册 false 没有注册
         */
        bool is_register(const std::string& keypath);
        
        /**
         *  删除一个注册项
         *
         *  @param tag 注册项的唯一标识，由register_one_property返回
         */
        void remove_register(int tag);
    private:
        std::shared_ptr<kvo_impl> impl_;
        void update(const std::string&, const boost::any&, const boost::any&);
    };

    /**
     *  所有cache类的基类
     */
    class base_cache
    {
    public:
        virtual const std::string& getKey() = 0;
        base_cache():kvo_(new kvo){}
        int register_property(const std::string& keypath, std::function<void(const boost::any& old, const boost::any& newer)> callback)
        {
            return kvo_->register_one_property(keypath, callback);
        }
        
        void remove_register(int tag)
        {
            kvo_->remove_register(tag);
        }
    protected:
        std::shared_ptr<kvo> kvo_;
    };
    
}
#endif /* defined(__bind__kvo__) */
