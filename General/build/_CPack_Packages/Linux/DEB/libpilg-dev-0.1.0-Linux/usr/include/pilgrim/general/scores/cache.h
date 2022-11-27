/*=============================================================================
 * Product        : 
 * File           : cache.h
 * Author         : 
 * Creation       : 
 
  *=============================================================================
 *        (c) Copyright 2013, PILGRIM - all rights reserved
 *=============================================================================
 */

#ifndef CACHE_H
#define CACHE_H

#include <pl.h> 

/*#if !defined(NDEBUG)
#define BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING
#define BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
#endif*/

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>

using boost::multi_index_container;
using namespace boost::multi_index;

struct line
{
	int         node;
	plVariablesConjunction parents;
	long double         score;

	line(int node_,plVariablesConjunction parents_,long double score_):node(node_),parents(parents_),score(score_){}
};

struct node{};
struct score{};

namespace PILGRIM{

class pmCache
{

	typedef multi_index_container<
		line,
		indexed_by<
		sequenced<>,
		ordered_non_unique<tag<node>,  BOOST_MULTI_INDEX_MEMBER(line,int,node)>,
		ordered_non_unique<tag<score>, BOOST_MULTI_INDEX_MEMBER(line,long double,score)> >
	> line_set;

	typedef line_set::index<node>::type line_set_by_node;
	typedef line_set::index<score>::type line_set_by_score;

public :

	//typedef typename line_set::iterator iterator;

	//constructor
	pmCache(int maxTaille);

	//destructor
	~pmCache();

	/**
	Test if a score is already computed. Returns score if present, return -1 else.
	*/
	long double isPresent(unsigned int node, plVariablesConjunction var);

	/**
	Insert a value in cache
	*/
	void insert(unsigned int node, plVariablesConjunction var, long double score);
	/**
	* \brief  Run the MI test
	* \details 
	* \param variables : 
	**/
	void printIt();

	line_set::iterator begin();
	line_set::iterator end();

private:
	int taille;
	line_set es;
};
	};
#endif //CACHE_H