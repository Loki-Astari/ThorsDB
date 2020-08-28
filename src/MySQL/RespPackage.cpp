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
#include "RespPackageAuthMoreData.h"
#include "RespPackagePrepareExecute.h"
#include "RespPackageAuthSwitchRequest.h"
#include "PrepareStatement.h"

template std::unique_ptr<ThorsAnvil::DB::MySQL::RespPackage> ThorsAnvil::DB::MySQL::downcastUniquePtr<ThorsAnvil::DB::MySQL::RespPackage>(std::unique_ptr<ThorsAnvil::DB::MySQL::RespPackage>&&);
template std::unique_ptr<ThorsAnvil::DB::MySQL::RespPackageOK> ThorsAnvil::DB::MySQL::downcastUniquePtr<ThorsAnvil::DB::MySQL::RespPackageOK>(std::unique_ptr<ThorsAnvil::DB::MySQL::RespPackage>&&);
template std::unique_ptr<ThorsAnvil::DB::MySQL::RespPackageEOF> ThorsAnvil::DB::MySQL::downcastUniquePtr<ThorsAnvil::DB::MySQL::RespPackageEOF>(std::unique_ptr<ThorsAnvil::DB::MySQL::RespPackage>&&);
template std::unique_ptr<ThorsAnvil::DB::MySQL::RespPackagePrepare> ThorsAnvil::DB::MySQL::downcastUniquePtr<ThorsAnvil::DB::MySQL::RespPackagePrepare>(std::unique_ptr<ThorsAnvil::DB::MySQL::RespPackage>&&);
template std::unique_ptr<ThorsAnvil::DB::MySQL::RespPackageHandShake> ThorsAnvil::DB::MySQL::downcastUniquePtr<ThorsAnvil::DB::MySQL::RespPackageHandShake>(std::unique_ptr<ThorsAnvil::DB::MySQL::RespPackage>&&);
template std::unique_ptr<ThorsAnvil::DB::MySQL::RespPackageResultSet> ThorsAnvil::DB::MySQL::downcastUniquePtr<ThorsAnvil::DB::MySQL::RespPackageResultSet>(std::unique_ptr<ThorsAnvil::DB::MySQL::RespPackage>&&);
template std::unique_ptr<ThorsAnvil::DB::MySQL::RespPackageAuthMoreData> ThorsAnvil::DB::MySQL::downcastUniquePtr<ThorsAnvil::DB::MySQL::RespPackageAuthMoreData>(std::unique_ptr<ThorsAnvil::DB::MySQL::RespPackage>&&);
template std::unique_ptr<ThorsAnvil::DB::MySQL::RespPackagePrepareExecute> ThorsAnvil::DB::MySQL::downcastUniquePtr<ThorsAnvil::DB::MySQL::RespPackagePrepareExecute>(std::unique_ptr<ThorsAnvil::DB::MySQL::RespPackage>&&);
template std::unique_ptr<ThorsAnvil::DB::MySQL::RespPackageAuthSwitchRequest> ThorsAnvil::DB::MySQL::downcastUniquePtr<ThorsAnvil::DB::MySQL::RespPackageAuthSwitchRequest>(std::unique_ptr<ThorsAnvil::DB::MySQL::RespPackage>&&);

#endif
