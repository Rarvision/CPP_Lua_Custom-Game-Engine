#ifndef SINGLETON_HPP
#define SINGLETON_HPP

/// @brief Base class of all classes that require singleton
/// @tparam T Your class name
template<typename T>
class Singleton {
public:
    /// @brief use GetInstance when you want to use the singleton
    /// @return the class object's reference
    static T& GetInstance(){
        static T instance;
        return instance;
    }

protected:
    Singleton(){}
    ~Singleton(){}
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

#endif  // SINGLETON_HPP