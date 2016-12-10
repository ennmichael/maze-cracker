#ifndef _LAZY_H__
#define _LAZY_H__

#include <functional>
#include <boost/optional/optional.hpp>

namespace General_purpose
{
    template <class T>
    class Cached {
    public:
        using Evaluator = std::function<T()>;
        
        explicit Cached(Evaluator) noexcept;
        
        auto notify_change() -> void;
        auto get() const noexcept -> const T&;
        auto get() noexcept -> T&;
        
    private:
        template <class Self>
        static auto& get_impl(Self& s)
        {
            if (!s.value_)
                s.value_ = s.e_();
            
            return *s.value_;
        }
        
        Evaluator e_;
        mutable boost::optional<T> value_; 
        // Deal with non-default constructible Ts
    };
    
    template <class T>
    Cached<T>::Cached(Evaluator e) noexcept
        : e_ { e }
        , value_ { boost::none }
    { }
    
    template <class T>
    auto Cached<T>::notify_change() -> void
    {
        value_ = boost::none;
    }
    
    template <class T>
    auto Cached<T>::get() const noexcept -> const T&
    {
        return get_impl(*this);
    }
    
    template <class T>
    auto Cached<T>::get() noexcept -> T&
    {
        return get_impl(*this);
    }
}

#endif
