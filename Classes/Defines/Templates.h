#pragma once
#include <iostream>

namespace pattern
{
    //Singleton define
    template <class T>
    class Singleton
    {
    public:
        static T* getInstance();
        static bool hasInstance();
        static void freeInstance();
        
        Singleton();
        virtual ~Singleton(){}
        
    protected:
        static T* m_instance;
    };
    
    //Singleton implement functions
    template<class T>
    T* Singleton<T>::m_instance = nullptr;
    
    template<class T>
    Singleton<T>::Singleton()
    {
		m_instance = (T*)this;
    }
    
    template<class T>
    T* Singleton<T>::getInstance()
    {
        return m_instance;
    }
    
    template<class T>
    bool Singleton<T>::hasInstance()
    {
        return m_instance != nullptr;
    }
    
    template<class T>
    void Singleton<T>::freeInstance()
    {
        delete m_instance;
    }
    
}

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args) {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
#endif
