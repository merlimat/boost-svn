
//  (C) Copyright Edward Diener 2011
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#include "test_mf_has_template.hpp"
#include <boost/mpl/assert.hpp>
#include <boost/tti/mf/mf_has_template.hpp>

int main()
  {
  
  using namespace boost::mpl::placeholders;
  
  // TemplateNotExist does not exist at all
  
  BOOST_MPL_ASSERT((boost::tti::mf_has_template
                      <
                      BOOST_TTI_HAS_TEMPLATE_GEN(TemplateNotExist)<_>,
                      MT_BType<AType>
                      >
                  ));
  
  return 0;
  
  }
