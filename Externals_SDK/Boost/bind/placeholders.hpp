#ifndef BOOST_BIND_PLACEHOLDERS_HPP_INCLUDED
#define BOOST_BIND_PLACEHOLDERS_HPP_INCLUDED

#if _MSC_VER >= 1020
#pragma once
#endif

//
//  bind/placeholders.hpp - _N definitions
//
//  Copyright (c) 2002 Peter Dimov and Multi Media Ltd.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//  See http://www.boost.org/libs/bind/bind.html for documentation.
//

#include <boost/bind/arg.hpp>
#include <boost/config.hpp>

namespace
{
#if (defined(BOOST_MSVC) && BOOST_MSVC <= 1300) || (defined(__DECCXX_VER) && __DECCXX_VER <= 60590031) || defined(__MWERKS__)
	  static   boost::arg<1> _1xxx;
	  static   boost::arg<2> _2xxx;
	  static   boost::arg<3> _3xxx;
	  static   boost::arg<4> _4xxx;
	  static   boost::arg<5> _5xxx;
	  static   boost::arg<6> _6xxx;
	  static   boost::arg<7> _7xxx;
	  static   boost::arg<8> _8xxx;
	  static   boost::arg<9> _9xxx;
#else
	/*static*/ boost::arg<1> _1xxx;
    /*static*/ boost::arg<2> _2xxx;
    /*static*/ boost::arg<3> _3xxx;
    /*static*/ boost::arg<4> _4xxx;
    /*static*/ boost::arg<5> _5xxx;
    /*static*/ boost::arg<6> _6xxx;
    /*static*/ boost::arg<7> _7xxx;
    /*static*/ boost::arg<8> _8xxx;
    /*static*/ boost::arg<9> _9xxx;
#endif
} // unnamed namespace

#endif // #ifndef BOOST_BIND_PLACEHOLDERS_HPP_INCLUDED
