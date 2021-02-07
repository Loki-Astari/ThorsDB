#ifndef THORSANVIL_DB_MONGO_VIEW_H
#define THORSANVIL_DB_MONGO_VIEW_H

#include "ThorSerialize/SerUtil.h"

namespace ThorsAnvil::DB::Mongo
{

template<typename T, typename = void>
inline constexpr bool isContainer       = false;
template<typename T>
inline constexpr bool isContainer<T, std::void_t<decltype(sizeof(typename Base<T>::value_type))>> = true;


template<typename T>
using ValidContainer    = std::enable_if_t<isContainer<T>,  bool>;
template<typename T>
using ValidSingle       = std::enable_if_t<!isContainer<T>, bool>;


struct View_Base {};
template<typename T>
class ViewSingle;
template<typename T>
class ViewContainer;

template<typename T, bool = isContainer<T>>
struct ViewTypeFinder;


template<typename T>
struct ViewTypeFinder<T, false>
{
    using View = ViewSingle<Base<T>>;
};
template<typename T>
struct ViewTypeFinder<T, true>
{
    using View = ViewContainer<Base<T>>;
};
template<typename T>
using ViewType = typename ViewTypeFinder<T>::View;


template<typename T, ValidSingle<T> = true>
ViewSingle<Base<T>>       make_XView(T& object)       {return ViewSingle<Base<T>>(object);}
template<typename T, ValidContainer<T> = true>
ViewContainer<Base<T>>    make_XView(T& object)       {return ViewContainer<Base<T>>(object);}


template<typename T>
class ViewBackInsertContainer;
struct FilterBackInserter {};
template<typename C>
ViewBackInsertContainer<Base<C>>  operator|(ViewContainer<C> view, FilterBackInserter)  {return ViewBackInsertContainer<Base<C>>(view);}


/*
 * Simple View constructed from iterators or Containers (ie other range).
 */
template<typename Object>
class ViewSingle: public View_Base
{
    Object&  object;
    public:
        using value_type    = Object;
        using iterator      = Object*;
        ViewSingle(Object& object)
            : object(object)
        {}
        ViewSingle(ViewSingle const&)             = default;
        ViewSingle(ViewSingle&&)                  = default;
        ViewSingle& operator=(ViewSingle const&)  = default;
        ViewSingle& operator=(ViewSingle&&)       = default;

        iterator    begin() const {return &object;}
        iterator    end()   const {return begin() + 1;}
        std::size_t size()  const {return 1UL;}
        value_type&     operator[](std::size_t) {return object;}
};

template<typename C>
class ViewContainer: public View_Base
{
    friend class ViewBackInsertContainer<C>;

    C& container;
    public:
        using value_type    = typename C::value_type;
        using iterator      = decltype(std::begin(container));
        ViewContainer(C& container)
            : container(container)
        {}
        ViewContainer(ViewContainer const&)             = default;
        ViewContainer(ViewContainer&&)                  = default;
        ViewContainer& operator=(ViewContainer const&)  = default;
        ViewContainer& operator=(ViewContainer&&)       = default;

        iterator    begin() const {return std::begin(container);}
        iterator    end()   const {return std::end(container);}
        std::size_t size()  const {return std::size(container);}
        value_type&     operator[](std::size_t pos)
        {
            std::cerr << "Insert: " << pos << "\n";
            container.resize(pos + 1);
            return container[pos];
        }
        value_type const& operator[](std::size_t pos) const
        {
            std::cerr << "Read Only: " << pos << "\n";
            return container[pos];
        }
};

template<typename C>
class ViewBackInsertContainer: public View_Base
{
    C&  container;
    public:
        using value_type    = typename C::value_type;
        using iterator      = decltype(std::begin(container));
        ViewBackInsertContainer(ViewContainer<C> view)
            : container(view.container)
        {}
        ViewBackInsertContainer(ViewBackInsertContainer const&)               = default;
        ViewBackInsertContainer(ViewBackInsertContainer&&)                    = default;
        ViewBackInsertContainer& operator=(ViewBackInsertContainer const&)    = default;
        ViewBackInsertContainer& operator=(ViewBackInsertContainer&&)         = default;

        iterator    begin()    const   {return std::begin(container);}
        iterator    end()      const   {return std::end(container);}
        std::size_t size()     const   {return std::size(container);}
        value_type&     operator[](std::size_t pos)
        {
            if (pos <= container.size())
            {
                container.resize(pos + 1);
            }
            return container[pos];
        }
        value_type const& operator[](std::size_t pos) const
        {
            return container[pos];
        }
};


}

namespace ThorsAnvil::Serialize
{


template<typename C>
class MemberEmplacer<ThorsAnvil::DB::Mongo::ViewSingle<C>>
{
    using View = ThorsAnvil::DB::Mongo::ViewSingle<C>;

    View const&   view;
    public:
        using T = typename View::value_type;

        MemberEmplacer(View const& view)
            : view(view)
        {}
        T& get(std::size_t const& pos)
        {
            return view[pos];
        }
};

template<typename C>
class Traits<ThorsAnvil::DB::Mongo::ViewSingle<C>>: public ArrayLike<ThorsAnvil::DB::Mongo::ViewSingle<C>, C>
{
    public:
        using View = ThorsAnvil::DB::Mongo::ViewSingle<C>;
        static constexpr TraitType type = TraitType::Array;
        using MemberExtractor = ContainerMemberExtractorEmplacer<View>;
        static MemberExtractor const& getMembers()
        {
            static constexpr MemberExtractor    memberExtractor;
            return memberExtractor;
        }
};

// ------

template<typename C>
class MemberEmplacer<ThorsAnvil::DB::Mongo::ViewContainer<C>>
{
    using View = ThorsAnvil::DB::Mongo::ViewContainer<C>;

    View const&   view;
    public:
        using T = typename View::value_type;

        MemberEmplacer(View const& view)
            : view(view)
        {}
        T& get(std::size_t const& pos)
        {
            return view[pos];
        }
};

template<typename C>
class Traits<ThorsAnvil::DB::Mongo::ViewContainer<C>>: public ArrayLike<ThorsAnvil::DB::Mongo::ViewContainer<C>, typename C::value_type>
{
    public:
        using View = ThorsAnvil::DB::Mongo::ViewContainer<C>;
        static constexpr TraitType type = TraitType::Array;
        using MemberExtractor = ContainerMemberExtractorEmplacer<View>;
        static MemberExtractor const& getMembers()
        {
            static constexpr MemberExtractor    memberExtractor;
            return memberExtractor;
        }
};

// ------

template<typename C>
class MemberEmplacer<ThorsAnvil::DB::Mongo::ViewBackInsertContainer<C>>
{
    using View = ThorsAnvil::DB::Mongo::ViewBackInsertContainer<C>;

    View const&   view;
    public:
        using T = typename View::value_type;

        MemberEmplacer(View const& view)
            : view(view)
        {}
        T& get(std::size_t const& pos)
        {
            return view[pos];
        }
};

template<typename C>
class Traits<ThorsAnvil::DB::Mongo::ViewBackInsertContainer<C>>: public ArrayLike<ThorsAnvil::DB::Mongo::ViewBackInsertContainer<C>, typename C::value_type>
{
    public:
        using View = ThorsAnvil::DB::Mongo::ViewBackInsertContainer<C>;
        static constexpr TraitType type = TraitType::Array;
        using MemberExtractor = ContainerMemberExtractorEmplacer<View>;
        static MemberExtractor const& getMembers()
        {
            static constexpr MemberExtractor    memberExtractor;
            return memberExtractor;
        }
};

}

#endif
