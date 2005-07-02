#ifndef POSIX_TIME_CONVERSION_HPP___
#define POSIX_TIME_CONVERSION_HPP___

/* Copyright (c) 2002-2005 CrystalClear Software, Inc.
 * Use, modification and distribution is subject to the 
 * Boost Software License, Version 1.0. (See accompanying
 * file LICENSE-1.0 or http://www.boost.org/LICENSE-1.0)
 * Author: Jeff Garland, Bart Garst
 * $Date$
 */

#include "boost/date_time/posix_time/ptime.hpp"
#include "boost/date_time/posix_time/posix_time_duration.hpp"
#include "boost/date_time/filetime_functions.hpp"
#include "boost/date_time/c_time.hpp"
#include "boost/date_time/gregorian/conversion.hpp"

namespace boost {

namespace posix_time {


  //! Function that converts a time_t into a ptime.
  inline
  ptime from_time_t(std::time_t t) 
  {
    ptime start(gregorian::date(1970,1,1));
    return start + seconds(static_cast<long>(t));
  }

  //! Convert a time to a tm structure truncating any fractional seconds 
  inline
  tm to_tm(const boost::posix_time::ptime& t) {
    tm timetm = boost::gregorian::to_tm(t.date());
    boost::posix_time::time_duration td = t.time_of_day();
    timetm.tm_hour = td.hours(); 
    timetm.tm_min = td.minutes(); 
    timetm.tm_sec = td.seconds();
    timetm.tm_isdst = -1; // -1 used when dst info is unknown 
    return timetm;
  }
  //! Convert a time_duration to a tm structure truncating any fractional seconds and zeroing fields for date components 
  inline
  tm to_tm(const boost::posix_time::time_duration& td) {
    tm timetm;
    timetm.tm_year = 0;
    timetm.tm_mon = 0;
    timetm.tm_mday = 0;
    timetm.tm_wday = 0;
    timetm.tm_yday = 0;
    
    timetm.tm_hour = td.hours(); 
    timetm.tm_min = date_time::absolute_value(td.minutes());
    timetm.tm_sec = date_time::absolute_value(td.seconds());
    timetm.tm_isdst = -1; // -1 used when dst info is unknown
    return timetm;
  }

  //! Convert a tm struct to a ptime ignoring is_dst flag
  inline
  ptime ptime_from_tm(const tm& timetm) {
    boost::gregorian::date d = boost::gregorian::date_from_tm(timetm);
    return ptime(d, time_duration(timetm.tm_hour, timetm.tm_min, timetm.tm_sec));
  }


#if defined(BOOST_HAS_FTIME)
  
  //! Function to create a time object from an initialized FILETIME struct.
  /*! Function to create a time object from an initialized FILETIME struct.
   * A FILETIME struct holds 100-nanosecond units (0.0000001). When 
   * built with microsecond resolution the FILETIME's sub second value 
   * will be truncated. Nanosecond resolution has no truncation. 
   *
   * Note ftime is part of the Win32 API, so it is not portable to non-windows
   * platforms.
   */
  template<class time_type>
  inline
  time_type from_ftime(const FILETIME& ft)
  {
    return boost::date_time::time_from_ftime<time_type>(ft);
  }

#endif // BOOST_HAS_FTIME

} } //namespace boost::posix_time




#endif

