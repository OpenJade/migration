// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

#ifndef dsssl_ns_INCLUDED
#define dsssl_ns_INCLUDED 1

#ifdef DSSSL_NAMESPACE
#ifdef SP_NAMESPACE
namespace SP_NAMESPACE { }
#endif
#ifdef GROVE_NAMESPACE
namespace GROVE_NAMESPACE { }
#endif

namespace DSSSL_NAMESPACE {
#ifdef SP_NAMESPACE
  using namespace SP_NAMESPACE;
#endif
#ifdef GROVE_NAMESPACE
  using namespace GROVE_NAMESPACE;
#endif
}
#endif /* DSSSL_NAMESPACE */

#endif /* not dsssl_ns_INCLUDED */
