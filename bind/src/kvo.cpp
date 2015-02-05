//
//  kvo.cpp
//  bind
//
//  Created by liuke on 15/1/29.
//  Copyright (c) 2015年 liuke. All rights reserved.
//

#include "kvo.h"
#include <map>

namespace bind {
    

    class kvo_impl{
        typedef std::vector<std::shared_ptr<observer_unit>> kvo_raw_value;
        typedef std::shared_ptr<kvo_raw_value> kvo_value;
        typedef std::map<std::string, kvo_value> kvo_map;
        std::shared_ptr<kvo_map> observer_;
        static std::map<size_t, kvo_value> static_observer_;
        static int a;
    public:
        kvo_impl() : observer_(new kvo_map)
        {
            
        }
        int register_one_property(const std::string &keypath, std::function<void (const boost::any&, const boost::any&)> callback)
        {
            static int count = 0;
            bool isFind;
            kvo_value vec = findObserverObj(keypath, &isFind);
            if (isFind) {
                std::shared_ptr<observer_unit> ptr(new observer_unit);
                ptr->keypath = keypath;
                ptr->callback = callback;
                ptr->count = count ++;
                vec->push_back(ptr);
                return ptr->count;
            }else{
                std::shared_ptr<observer_unit> ptr(new observer_unit);
                ptr->keypath = keypath;
                ptr->callback = callback;
                ptr->count = count ++;
                
                kvo_value vec(new kvo_raw_value);
                vec->push_back(ptr);
                observer_->insert(std::pair<std::string, kvo_value>(keypath, vec));
                return ptr->count;
            }
        }
        static int register_one_property(size_t class_hash_code, const std::string &keypath, std::function<void (const boost::any &, const boost::any &)> callback)
        {
            bool isFind;
            std::map<size_t, kvo_value>::iterator it = static_observer_.find(class_hash_code);
            kvo_value vec;
            if (it != static_observer_.end()) {
                isFind = true;
                vec = it->second;
            }else{
                isFind = false;
            }
            
            if (isFind) {
                std::shared_ptr<observer_unit> ptr(new observer_unit);
                ptr->keypath = keypath;
                ptr->count = 0;
                ptr->callback = callback;
                ptr->hash_code = class_hash_code;
                vec->push_back(ptr);
                return ptr->count;
            }else{
                std::shared_ptr<observer_unit> ptr(new observer_unit);
                ptr->keypath = keypath;
                ptr->callback = callback;
                ptr->count = 0;
                ptr->hash_code = class_hash_code;
                
                vec.reset(new kvo_raw_value);
                vec->push_back(ptr);
                static_observer_.insert(std::pair<size_t, kvo_value>(class_hash_code, vec));
                return ptr->count;
            }
        }
        
        void remove_register(int tag)
        {
            kvo_map::iterator it = observer_->begin();
            for (; it != observer_->end(); it ++) {
                kvo_value vec = it->second;
                for (int i = 0; i < vec->size(); i ++) {
                    if (vec->at(i)->count == tag) {
                        vec->erase(vec->begin() + i);
                        return;
                    }
                }
            }
        }
        bool is_register(const std::string &keypath)
        {
            bool isFind;
            findObserverObj(keypath, &isFind);
            return isFind;
        }
        
        std::shared_ptr<std::vector<std::shared_ptr<observer_unit>>> findObserverObj(const std::string &keypath, bool *isFind)
        {
            kvo_map::iterator it = observer_->find(keypath);
            if (it != observer_->end()) {
                *isFind = true;
                return it->second;
            }else{
                *isFind = false;
                return std::shared_ptr<kvo_raw_value>();
            }
        }
        
        void update(const std::string& keypath, const boost::any & old_val, const boost::any &new_val)
        {
            bool isFind;
            kvo_value ret = findObserverObj(keypath, &isFind);
            if (isFind) {
                for (int i = 0; i < ret->size(); i ++) {
                    ret->at(i)->callback(old_val, new_val);
                }
            }
        }
        static void update(size_t class_hash_code, const std::string &keypath, const boost::any &old_val, const boost::any &new_val)
        {
            std::map<size_t, kvo_value>::iterator it = static_observer_.find(class_hash_code);
            if (it != static_observer_.end()) {
                kvo_value ret = it->second;
                for (int i = 0; i < ret->size(); i ++) {
                    std::shared_ptr<observer_unit> ob = ret->at(i);
                    if (ob->keypath == keypath) {
                        ret->at(i)->callback(old_val, new_val);
                    }
                }
            }
        }
        
    };
    
    std::map<size_t, kvo_impl::kvo_value> kvo_impl::static_observer_ = std::map<size_t, kvo_impl::kvo_value>();
    
    kvo::kvo() : impl_(new kvo_impl())
    {
    }
    
    int kvo::register_one_property(const std::string &keypath, std::function<void (const boost::any&, const boost::any&)> callback)
    {
        return impl_->register_one_property(keypath, callback);
    }
    
    int kvo::register_one_property(size_t class_hash_code, const std::string &keypath, std::function<void (const boost::any &, const boost::any &)> callback)
    {
        return kvo_impl::register_one_property(class_hash_code, keypath, callback);
    }
    
    void kvo::remove_register(int tag)
    {
        impl_->remove_register(tag);
    }
    
    bool kvo::is_register(const std::string &keypath)
    {
        return impl_->is_register(keypath);
    }
    
    void kvo::update(const std::string& keypath, const boost::any &old_val, const boost::any &new_val)
    {
        impl_->update(keypath, old_val, new_val);
    }
    
    void kvo::update(size_t class_hash_code, const std::string &keypath, const boost::any &old_val, const boost::any &new_val)
    {
        kvo_impl::update(class_hash_code, keypath, old_val, new_val);
    }
    
    kvo::~kvo()
    {
        
    }
}