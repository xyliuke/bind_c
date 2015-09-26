# bind_c

##bind_c的简介
bind_c库是模拟了Objective-C中的KVO（Key-Value Observing）功能，即对类对象变量或者类静态变量进行修改时，调用事先注册注册好的闭包，来达到通知观察者的上的。

##bind_c使用方法
因为bind_c使用了Boost库中Any类，所以需要导入Boost库。
###类变量的使用方法
这里有两种方式：
####1.继承base_cache类
继承base_cache类后，只需要使用KVO_PROPERTY(TYPE,NAME,COMMENT)宏在.h文件中定义自己的变量即可。其中TYPE为变量类型，NAME为变量的keypath值，COMMENT为注释（可不写）。这个宏生成四个函数，有set_NAME(TYPE)，get_NAME(), init_NAME(TYPE)和__NAME__()，前两个是设置和获取方法，第三个是内部用来初始化变量不会引起kvo的触发，第四个是返回keypath的字符串值。
定义好自己的变量后，需要调用base_cache中的int register_one_property(const std::string& keypath, std::function<void(const boost::any& old, const boost::any& newer)> callback);来注册某个keypath的闭包，支持对相同的keypath注册多个闭包。如果需要注销某个闭包时，只需要调用void remove_register(int tag);其中tag值为register_one_property的返回值。

###2.使用kvo类
模拟base_cache的做法，在你需要的类中加入kvo类型的成员变量，并使用KVO_PROPERTY(TYPE,NAME,COMMENT)定义变量，然后调用kvo类中的上述方法进行注册和更新。具体过程查看base_cache源代码。

###类静态变量的使用方法
####1.继承base_cache类
同样调用静态的base_cache中方法static int register_one_property(std::type_info class_type, const std::string& keypath, std::function<void(const boost::any& old, const boost::any& newer)> callback)，其中class_type为子类的类型，使用typeid()方法来获取。

####2.只使用kvo类
使用宏KVO_STATIC_PROPERTY_DEFINE(TYPE, NAME, COMMENT)和KVO_STATIC_PROPERTY_IMPL(TYPE, NAME, CLASS)分别在h文件和cpp文件中使用。



