#pragma once

// The next defines are used to easy predeclare regular or 
// template classes nested in different deep namespaces.

#define PREDEF_0( name )            class name;
#define PREDEF_1( one, name )       namespace one { class name; }
#define PREDEF_2( one, two, name )  namespace one { namespace two { class name; } }

#define PREDEF_TPL_0( name )            template<class T> class name;
#define PREDEF_TPL_1( one, name )       namespace one { template<class T> class name; }
#define PREDEF_TPL_2( one, two, name )  namespace one { namespace two { template<class T> class name; } }

#define PREDEF( name )      PREDEF_0( name )
#define PREDEF_TPL( name )  PREDEF_TPL_0( name )