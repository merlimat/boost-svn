//
//  (C) Copyright 2011 Vicente J. Botet Escriba
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//

/**
 * Duration formatting facet for output.
 */
#ifndef BOOST_CHRONO_IO_DURATION_PUT_HPP
#define BOOST_CHRONO_IO_DURATION_PUT_HPP

#include <boost/chrono/config.hpp>
#include <boost/chrono/io/duration_units.hpp>
#include <boost/assert.hpp>
#include <locale>

namespace boost
{
  namespace chrono
  {

    /**
     * @tparam ChatT a character type
     * @tparam OutputIterator a model of @c OutputIterator
     *
     * The @c duration_put facet provides facilities for formatted output of duration values.
     * The member function of @c duration_put take a duration and translate this into character string representation.
     *
     * FIXME As the facet doesn't have any data, I'm wondering if this should functions
     * shouldn't be at the namespace level or static member functions.
     * This will leverage the user to need to ensure that the facet is imbued on the ios_base.
     */
    template <class CharT, class OutputIterator = std::ostreambuf_iterator<CharT> >
    class duration_put: public std::locale::facet
    {
    public:
      /**
       * Type of character the facet is instantiated on.
       */
      typedef CharT char_type;
      /**
       * Type of iterator used to write in the character buffer.
       */
      typedef OutputIterator iter_type;

      /**
       * Construct a duration_put facet.
       * @param refs
       * @Effects Construct a duration_put facet.
       * If the @c refs argument is @c 0 then destruction of the object is
       * delegated to the @c locale, or locales, containing it. This allows
       * the user to ignore lifetime management issues. On the other had,
       * if @c refs is @c 1 then the object must be explicitly deleted;
       * the @c locale will not do so. In this case, the object can be
       * maintained across the lifetime of multiple locales.
       */
      explicit duration_put(size_t refs = 0) :
        std::locale::facet(refs)
      {
      }

      /**
       *
       * @param s an output stream iterator
       * @param ios a reference to a ios_base
       * @param d the duration
       * @param pattern begin of the formatting patter
       * @param pat_end end of the formatting pattern
       *
       * @Effects Steps through the sequence from @c pattern to @c pat_end,
       * identifying characters that are part of a pattern sequence. Each character
       * that is not part of a pattern sequence is written to @c s immediately, and
       * each pattern sequence, as it is identified, results in a call to
       * @c put_value or @c put_unit;
       * thus, pattern elements and other characters are interleaved in the output
       * in the order in which they appear in the pattern. Pattern sequences are
       * identified by converting each character @c c to a @c char value as if by
       * @c ct.narrow(c,0), where @c ct is a reference to @c ctype<charT> obtained from
       * @c ios.getloc(). The first character of each sequence is equal to @c �%�,
       * followed by a pattern specifier character @c spec, which can be @c 'v' for
       * the duration value or @c 'u' for the duration unit. .
       * For each valid pattern sequence identified, calls
       * <c>put_value(s, ios, d)</c> or <c>put_unit(s, ios, d)</c>.
       *
       * @Returns An iterator pointing immediately after the last character produced.
       */
      template <typename Rep, typename Period>
      iter_type put(iter_type s, std::ios_base& ios, duration<Rep, Period> const& d, const CharT* pattern,
          const CharT* pat_end) const
      {
        if (!std::has_facet<duration_units<CharT> >(ios.getloc())) ios.imbue(
            std::locale(ios.getloc(), new duration_units_default<CharT> ()));

        for (; pattern != pat_end; ++pattern)
        {
          // FIXME: Shouldn't "uvx" be a basic_string<CharT> ?????
          if ( (*pattern != CharT('%')) || ( (pattern + 1) == pat_end) || (!std::strchr("uvx", * (pattern + 1))))
          {
            *s++ = *pattern;
          }
          else
          {
            ++pattern;
            switch (*pattern)
            {
            case 'v':
            {
              put_value(s, ios, d);
              break;
            }
            case 'u':
            {
              put_unit(s, ios, d);
              break;
            }
            case 'x':
            {
              std::basic_string<CharT> pat = std::use_facet<duration_units<CharT> >(ios.getloc()).get_pattern();
              pattern = pat.data();
              pat_end = pattern + pat.size();
              break;
            }
            default:
              BOOST_ASSERT(false && "Boost::Chrono internal error.");
              break;
            }
          }
        }
        return s;
      }

      /**
       *
       * @param s an output stream iterator
       * @param ios a reference to a ios_base
       * @param d the duration
       * @Effects imbue in @c ios the @c duration_units_default facet if not already present.
       * Retrieves Stores the duration pattern from the @c duration_unit facet in let say @c str. Last as if
       * @code
       *   return put(s, ios, d, str.data(), str.data() + str.size());
       * @codeend
       * @Returns An iterator pointing immediately after the last character produced.
       */
      template <typename Rep, typename Period>
      iter_type put(iter_type s, std::ios_base& ios, duration<Rep, Period> const& d) const
      {
        if (!std::has_facet<duration_units<CharT> >(ios.getloc())) ios.imbue(
            std::locale(ios.getloc(), new duration_units_default<CharT> ()));

        std::basic_string<CharT> str = std::use_facet<duration_units<CharT> >(ios.getloc()).get_pattern();
        return put(s, ios, d, str.data(), str.data() + str.size());
      }

      /**
       *
       * @param s an output stream iterator
       * @param ios a reference to a ios_base
       * @param d the duration
       * @Effects Calls do_put_value(s, f, d).
       * @Returns An iterator pointing immediately after the last character produced.
       */
      template <typename Rep, typename Period>
      iter_type put_value(iter_type s, std::ios_base& ios, duration<Rep, Period> const& d) const
      {
        return std::use_facet<std::num_put<CharT, iter_type> >(ios.getloc()).put(s, ios, ' ',
            static_cast<long int> (d.count()));
      }

      /**
       *
       * @param s an output stream iterator
       * @param ios a reference to a ios_base
       * @param d the duration
       * @param pattern
       * @Effects Calls do_put_unit(s, f, d).
       * @Returns An iterator pointing immediately after the last character produced.
       */
      template <typename Rep, typename Period>
      iter_type put_unit(iter_type s, std::ios_base& ios, duration<Rep, Period> const& d) const
      {
        return std::use_facet<duration_units<CharT> >(ios.getloc()).put(s, ios, d);
      }

      /**
       * Unique identifier for this type of facet.
       */
      static std::locale::id id;

      /**
       * @Effects Destroy the facet
       */
      ~duration_put()
      {
      }

    };

    template <class CharT, class OutputIterator>
    std::locale::id duration_put<CharT, OutputIterator>::id;

  } // chrono
} // boost

#endif  // BOOST_CHRONO_CHRONO_IO_HPP
