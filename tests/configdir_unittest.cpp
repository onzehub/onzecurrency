#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include "configdir.hpp"
using namespace Crowd;

BOOST_AUTO_TEST_SUITE(ConfigDirTest)

ConfigDir cd;

BOOST_AUTO_TEST_CASE(Get)
{
    BOOST_CHECK(boost::algorithm::ends_with(cd.GetConfigDir(), ".config/onzehub/"));
}

BOOST_AUTO_TEST_SUITE_END()