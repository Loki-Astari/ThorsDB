#include "RespPackage.h"
// 1

#ifdef COVERAGE_MySQL
/*
 * This code is only compiled into the unit tests for code coverage purposes
 * It is not part of the live code.
 */
#include "RespPackage.tpp"
#include "RespPackageOK.h"
#include "RespPackageEOF.h"
#include "RespPackagePrepare.h"
#include "RespPackageHandShake.h"
#include "RespPackageResultSet.h"
#include "RespPackagePrepareExecute.h"
#include "PrepareStatement.h"

template std::unique_ptr<ThorsAnvil::MySQL::RespPackage> ThorsAnvil::MySQL::downcastUniquePtr<ThorsAnvil::MySQL::RespPackage>(std::unique_ptr<ThorsAnvil::MySQL::RespPackage>&&);
template std::unique_ptr<ThorsAnvil::MySQL::RespPackageOK> ThorsAnvil::MySQL::downcastUniquePtr<ThorsAnvil::MySQL::RespPackageOK>(std::unique_ptr<ThorsAnvil::MySQL::RespPackage>&&);
template std::unique_ptr<ThorsAnvil::MySQL::RespPackageEOF> ThorsAnvil::MySQL::downcastUniquePtr<ThorsAnvil::MySQL::RespPackageEOF>(std::unique_ptr<ThorsAnvil::MySQL::RespPackage>&&);
template std::unique_ptr<ThorsAnvil::MySQL::RespPackagePrepare> ThorsAnvil::MySQL::downcastUniquePtr<ThorsAnvil::MySQL::RespPackagePrepare>(std::unique_ptr<ThorsAnvil::MySQL::RespPackage>&&);
template std::unique_ptr<ThorsAnvil::MySQL::RespPackageHandShake> ThorsAnvil::MySQL::downcastUniquePtr<ThorsAnvil::MySQL::RespPackageHandShake>(std::unique_ptr<ThorsAnvil::MySQL::RespPackage>&&);
template std::unique_ptr<ThorsAnvil::MySQL::RespPackageResultSet> ThorsAnvil::MySQL::downcastUniquePtr<ThorsAnvil::MySQL::RespPackageResultSet>(std::unique_ptr<ThorsAnvil::MySQL::RespPackage>&&);
template std::unique_ptr<ThorsAnvil::MySQL::RespPackagePrepareExecute> ThorsAnvil::MySQL::downcastUniquePtr<ThorsAnvil::MySQL::RespPackagePrepareExecute>(std::unique_ptr<ThorsAnvil::MySQL::RespPackage>&&);

#endif
