#ifndef THORS_ANVIL_DB_MYSQL_RESP_PACKAGE_TPP
#define THORS_ANVIL_DB_MYSQL_RESP_PACKAGE_TPP

namespace ThorsAnvil::DB::MySQL
{

template<typename To>
std::unique_ptr<To> downcastUniquePtr(std::unique_ptr<RespPackage>&& item)
{
    if (item.get() == nullptr)
    {
        return nullptr;
    }

    std::unique_ptr<To> result;
    // Note the dynamic_cast may fail and throw.
    // If this happens item retains ownership.
    try
    {
        result.reset(&dynamic_cast<To&>(*item));
    }
    catch (std::bad_cast const&)
    {
        throw std::domain_error("ThorsAnvil::DB::Access::downcastUniquePtr: Casting reponse to expected type failed");
    }
    // Now that ownership has been transferred.
    // Release the original pointer.
    item.release();

    return result;
}

}

#endif
