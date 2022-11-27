
#include <iostream>
#include <string>
#include "pilgrim/general/scores/cache.h"

using boost::multi_index_container;
using namespace boost::multi_index;
using namespace std;
using namespace PILGRIM;

//=============================================================================
pmCache::pmCache(int maxTaille) {
  this->taille=maxTaille;
}

//=============================================================================
pmCache::~pmCache() {
  cout << "cache détruit";
}

//=============================================================================
long double pmCache::isPresent(unsigned int node, plVariablesConjunction var) {
  line_set::iterator it = this->begin();
  do {
    if (it->node == node && var == it->parents ) {  // voir si on est sur la bonne ligne
      es.relocate(es.begin(), it);  // replace node at the begining of the cache
      return it->score;
    }
    ++it;
  } while (it != this->end());
  return -1;
}
//=============================================================================
void pmCache::insert(unsigned int node, plVariablesConjunction var, long double score) {
  pair<line_set::iterator,bool> p=es.push_front(line(node, var, score));

  if (!p.second) {                     /* duplicate item */
    es.relocate(es.begin(), p.first);  /* put in front */
  } else if(es.size()>this->taille) {  /* keep the length <= taille */
    es.pop_back();
  }
}

//=============================================================================
void pmCache::printIt() {
  int t = 0;
  line_set::iterator it = this->begin();
  do {
    cout << "node:" << it->node << " p:" << it->parents << " sc: " << it->score << endl;
    ++it;
    ++t;
  } while (it != this->end());
  cout << "Taille=" << t << endl;
}

//=============================================================================
pmCache::line_set::iterator pmCache::begin() {
  return es.begin();
}

//=============================================================================
pmCache::line_set::iterator pmCache::end() {
  return es.end();
}
