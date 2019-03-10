#pragma once

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
        
//        Singleton();
        virtual ~Singleton(){}
        
    protected:
        static T* m_instance;
    };
    
    //Singleton implement functions
    template<class T>
    T* Singleton<T>::m_instance = new T;
    
//    template<class T>
//    Singleton<T>::Singleton()
//    {
//        if(m_instance == nullptr)
//            m_instance = new T;
//    }
    
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
